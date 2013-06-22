#include "common.h"
#include "dbaccess.h"

int readdb(const char *iface, const char *dirname)
{
  FILE *db;
  char file[512], backup[512];
  int newdb=0;
  snprintf(file, 512, "%s/%s", dirname, iface);
  snprintf(backup, 512, "%s/%s", dirname, iface);
  if((db = fopen(file,"r")) != NULL) {

    if(!lockdb(fileno(db), 0)) {
      return -1;
    }

    if(fread(&data,sizeof(DATA),1,db) == 0) {
       if(debug){
        printf("db: Database read failed for file \" %s\".\n",file);
       }
    }else {
      if(debug){
        printf("db: Database loaded for interface \" %s\"...\n", data.interface);
      }
    }

    fclose(db);

    if(strcmp(data.interface,iface)){
      snprintf(errorstring, 512, "warning;\n the previous interface for this file was \"%s\".",data.interface);
      printe(PT_Multiline);
      snprintf(errorstring, 512, "it has now been replaces with \"%s\".", iface);
    }

  }else {
    snprintf(errorstring, 512, "Unable to read database \"%s\".",file);
    printe(PT_Error);
    newdb=1;
    initdb();
    strncpy(data.interface, iface, 32);
    strncpy(data.nick, data.interface, 32);

  }
  return newdb;
}

void initdb(void)
{
  int i;
  time_t current;
  struct tm *d;
  current = time(NULL);
  d = localtime(&current);
  data.active=1;
  data.totalrx=0;
  data.totaltx=0;
  data.currx=0;
  data.curtx=0;
  data.totalrxk=0;
  data.totaltxk=0;
  data.lastupdated=current;
  data.created=current;

  for(i=0;i<=29;i++){
    data.day[i].rx=0;
    data.day[i].tx=0;
    data.day[i].rxk=0;
    data.day[i].txk=0;
    data.day[i].date=0;
    data.day[i].used=0;
  }

  for(i=0;i<=11;i++){
    data.month[i].rx=0;
    data.month[i].tx=0;
    data.month[i].rxk=0;
    data.month[i].txk=0;
    data.month[i].month=0;
    data.month[i].used=0;
  }

  for(i=0;i<=9;i++){
    data.top10[i].rx=0;
    data.top10[i].tx=0;
    data.top10[i].rxk=0;
    data.top10[i].txk=0;
    data.top10[i].date=0;
    data.top10[i].used=0;
  }
  data.day[0].used=data.month[0].used=1;
  data.day[0].date=current;
  data.month[0].month=current;
}

int writedb(const char *iface, const char *dirname, int newdb)
{
  FILE *db;
  char file[512], backup[512];
  snprintf(file, 512, "%s/%s", dirname, iface);
  snprintf(backup, 512, "%s/.%s", dirname, iface);
  /*backup the old data if it is a new database*/
  if(!newdb && !backupdb(file, backup)) {
    snprintf(errorstring, 512, "Unable create database backup \"%s\".", backup);
    printe(PT_Error);
    return 0;
  }

  if((db=fopen(file, "w"))!=NULL) {
    if(!lockdb(fileno(db), 1)) {
      return 0;
    }
    data.lastupdated = time(NULL);
    if(fwrite(&data,sizeof(DATA),1,db)==0){
      snprintf(errorstring, 512, "Unable to write database \"%s\".", file);
      printe(PT_Error);
      return 0;
    } else {
      if(debug){
        printf("db: database \"%s\" saved.\n", file);
      }
      fclose(db);
      if(newdb){
        snprintf(errorstring, 512, "->a new database has been created.");
        printe(PT_Info);
      }
    }
  } else{
    snprintf(errorstring, 512, "Unable to write database \"%s\".", file);
    printe(PT_Error);
    return 0;
  }
  return 1;
}

int backupdb(const char *current, const char *backup)
{
  FILE *bf;
  int c, b, bytes;
  char buffer[512];

  if((c = open(current, O_RDONLY)) == -1){
    return 0;
  }
  /*to, fopen() in order to get file mode bits corretly */
  if((bf = fopen(backup, "w")) == NULL) {
    close(c);
    return 0;
  }
  b = fileno(bf);

  while((bytes = (int)read(c, buffer, sizeof(buffer))) > 0) {
    if(write(b, buffer, bytes) < 0){
    close(c);
    fclose(bf);
    return 0;
    }
  }
  close(c);
  fclose(bf);
  return 1;
}

int lockdb(int fd, int dbwrite)
{
  int operation, locktry=1;
  if(dbwrite){
    operation = LOCK_EX|LOCK_NB;
  } else{
    operation = LOCK_SH|LOCK_NB;
  }

  while(flock(fd, operation) != 0){
    if(debug){
    printf("db: database access locked (%d, %d)\n", dbwrite, locktry);
    }

    if(locktry >= LOCKTRYLIMIT){
      if(dbwrite){
        snprintf(errorstring, 512, "Locking database file for write failed for %d tries:\n%s (%d)", locktry,strerror(error), errno);
      }else {
        snprintf(errorstring, 512, "Locking databse file for read failed for %d tries:\n%s (%d)", locktry, strerror(errno), errno);
      }
      printe(PT_Error);
      return 0;
    }
    if(errno == EWOULDBLOCK){
      sleep(1);
    }else{
      if(dbwrite){
        snprintf(errorstring, 512, "Locking database file for write failed:\n%s (%d)", strerror(errno),errno);
      }else{
        snprintf(errorstring, 512, "Locking database file for write failed:\n%s (%d)", strerror(errno), errno);
      }
      printe(PT_Error);
      return 0;
    }
    locktry++;
  }
  return 1;
}

int checkdb(const char *iface, const char *dirname)
{
  char file[512];
  struct statvfs buf;
  snprintf(file, 512, "%s/%s", dirname, iface);

  if(statvfs(file, &buf) == 0){
    return 1;
  }else {
    return 0;
  }
}

int removedb(const char *iface, const char *dirname)
{
  char file[512];

  snprintf(file, 512, "%s/.%s", dirname, iface);
  unlink(file);

  snprintf(file, 512, "%s/%s", dirname, iface);
  if(unlink(file)!=0){
   perror("unlink");
   return 0;
  }
  return 1;
}

void rotatedays(void)
{
  int i, j;
  time_t current;
  struct tm *d;

  for(i=29;i>=1;i--){
    data.day[i].rx=data.day[i-1].rx;
    data.day[i].tx=data.day[i-1].tx;
    data.day[i].rxk=data.day[i-1].rxk;
    data.day[i].txk=data.day[i-1].txk;
    data.day[i].date=data.day[i-1].date;
    data.day[i].used=data.day[i-1].used;

  }
  current=time(NULL);
  data.day[0].rx=0;
  data.day[0].tx=0;
  data.day[0].rxk=0;
  data.day[0].txk=0;
  data.day[0].date=current;

  if(debug){
    d=localtime(&data.day[0].date);
    printf("db: days rotated. Current date:%d.%d.%d, d->tm_mday, d->tm_mon+1, d->tm_year+1900");
  }
}


void rotatemonths(void)
{
  int i;
  time_t current;
  struct tm *d;

  for(i=11;i>=1;i--){
    data.month[i].rx=data.month[i-1].rx;
    data.month[i].tx=data.month[i-1].tx;
    data.month[i].rxk=data.month[i-1].rxk;
    data.month[i].txk=data.month[i-1].txk;
    data.month[i].month=data.month[i-1].month;
    data.month[i].used=data.month[i-1].used;
  }

  current=time(NULL);
  data.month[0].rx=0;
  data.month[0].tx=0;
  data.month[0].rxk=0;
  data.month[0].txk=0;
  data.month[0].month=current;

  if(debug){
    d=localtime(&data.month[0].month);
    printf("db: Months rotated. Current month: \"%d\".\n", d->tm_mon+1);
  }
}



