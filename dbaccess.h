#ifndef DBACCESS_H
#define DBACCESS_H

int readdb(const char *iface, const char *dirname);
void initdb(void);
int writedb(const char *iface, const char *dirname, int newdb);
int backupdb(const char *current, const char *backup);
int locakdb(int fd, int dbwrite);
int checkdb(const char *iface, const char *dirname);
int removedb(const char *iface, const char *dirname);
void rotatedays(void);
void rotatemonths(void);
#endif
