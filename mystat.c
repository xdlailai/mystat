#include "common.h"
#include "mystat.h"
#include "counter.h"

int main(int argc, const char *argv[])
{
  daemonize();
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
  if(logfile)
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
