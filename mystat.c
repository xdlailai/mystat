#include "common.h"
#include "mystat.h"
#include "counter.h"
#include "pthread.h"
#include "mod.h"
#include "xml_cli.h"
void print_data()
{
  while(1){
    showxml();
    transXml();
    sleep(10);
  }
}
void get_cpu_record()
{
  read_cpu_stats();
  sleep(5);
  while(1){
    read_cpu_stats();
    sleep(5);
  }
}
void get_mem_record()
{
  while(1){
    read_mem_stats();
    sleep(5);
  }
}
void get_load_record()
{
  while(1){
    read_load_stats();
    sleep(5);
  }
}
void get_partion_record()
{
  while(1){
    read_partion_stat();
    sleep(5);
  }
}
void get_traffic_record()
{
  while(1){
    read_traffic_stats();
    sleep(5);
  }
}

void tranXml()
{
  while(1){
    transXml();
    sleep(5);
  }
}

int main(int argc, const char *argv[])
{
  pthread_t pd_listen, pd_cpu, pd_mem, pd_load, pd_partition, pd_traffic, pd_tranxml;
  int error;
  error = pthread_create(&pd_listen, NULL, print_data, NULL);
  if(error!=0)
    printf("error pthread_create");
  error = pthread_create(&pd_cpu, NULL, get_cpu_record, NULL);
  if(error!=0)
    printf("error pthread_cpu");
  error = pthread_create(&pd_mem, NULL, get_mem_record, NULL);
  if(error!=0)
    printf("error pthread_mem");
  error = pthread_create(&pd_load, NULL, get_load_record, NULL);
  if(error!=0)
    printf("error pthread_load");
  error = pthread_create(&pd_partition, NULL, get_partion_record, NULL);
  if(error!=0)
    printf("error pthread_partion");
  error = pthread_create(&pd_traffic, NULL, get_traffic_record, NULL);
  if(error!=0)
    printf("error pthread_traffic");
//  error = pthread_create(&pd_tranxml, NULL, tranXml, NULL);
//  if(error!=0)
//    printf("error transfer xml file");
//  daemonize();
  init();
  listenpck();
  return 0;
}


void daemonize(void)
{
  int i, lfp;
  char str[10];
  if(getppid()==1){
    return;
  }
  i=fork();
  if(i<0){
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if(i>0){
    exit(EXIT_SUCCESS);
  }
  setsid();
  for(i=getdtablesize();i>=0;--i){
    close(i);
  }
  i=open("/dev/null", O_RDWR);
  dup(i);
  dup(i);
  umask(027);
  if(chdir(RUNNING_DIR) < 0){
    perror("chdir(/)");
    snprintf(errorstring, 512, "directory change to / failed.exiting.");
    printe(PT_Error);
    exit(EXIT_FAILURE);
  }
  lfp=open(LOCK_FILE, O_RDWR|O_CREAT, 0640);
  if(lfp<0){
    perror("LOCK_FILE");
    snprintf(errorstring, 512, 'LOCK_FILE failed, exiting');
    printe(PT_Error);
    exit(EXIT_FAILURE);
  }
  if(lockf(lfp, F_TLOCK, 0)<0){
    perror("LOCK_FILE lock");
    snprintf(errorstring, 512, "LOCKFILE lock failed ,exiting.");
    printe(PT_Error);
    exit(EXIT_FAILURE);
  }
  /*the only instance continues*/
  sprintf(str, "%d\n", getpid());
  /*record pid to lockfile*/
  if(write(lfp, str, strlen(str)) < 0){
    perror("write(LOCK_FILE");
    snprintf(errorstring, 512, "writing to lockfile failed, exiting");
    printe(PT_Error);
    exit(EXIT_FAILURE);
  }
  signal(SIGCHLD, SIG_IGN); /*ignore child*/
  signal(SIGTSTP, SIG_IGN); /*Ignore tty signals*/
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGHUP, signal_handler);/*catch hangup signal*/
  signal(SIGTERM, signal_handler);/*catch kill signal*/

}

void log_message(char *filename, char *message)
{
  FILE *logfile;
  logfile=fopen(filename, "a");
  if(!logfile)
    return;
  fprintf(logfile, "%s\n", message);
  fclose(logfile);
}

void signal_handler(int sig)
{
  switch(sig){
    case SIGHUP:
      log_message(LOG_FILE, "hangup signal catched");
      break;
    case SIGTERM:
      log_message(LOG_FILE, "terminate signal catched");
      exit(0);
      break;
  }
}
