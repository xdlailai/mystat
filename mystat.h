#ifndef MYSTAT_H
#define MYSTAT_H

#define RUNNING_DIR "/tmp"
#define LOCK_FILE "mystat.lock"
#define LOG_FILE "mystat.log"

void log_message(char *filename, char *message);
void signal_handler(int sig);
void daemonize(void);

#endif
