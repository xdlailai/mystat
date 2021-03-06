#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>
#include <sys/file.h>
#include <sys/statvfs.h>
#include <fcntl.h>
#include <signal.h>

#define DATABASEDIR  "/home/fly100"
#define XMLBASEDIR "/var/www/dataxml"
#define DEFIFACE "eth0"
#define LOCKTRYLIMIT 5
#define MYSTATVERSION "1.11"
#define XMLVERSION 1
typedef struct {
  char name[32];
  int filled;
  uint64_t rx;
  uint64_t tx;
  time_t date;
} FLOWBUF;

typedef struct {
  time_t date;
  uint64_t rx, tx;
  int rxk, txk;
  int used;
} DAY;

typedef struct {
  time_t month;
  uint64_t rx, tx;
  int rxk, txk;
  int used;
} MONTH;

typedef struct {
  char interface[32];
  char nick[32];
  int active;
  uint64_t totalrx, totaltx, currx, curtx;
  int totalrxk, totaltxk;
  time_t lastupdated, created;
  DAY day[30];
  MONTH month[12];
  DAY top10[10];
  uint64_t btime;
} DATA;

typedef enum PrintType {
  PT_Info = 0,
  PT_Error,
  PT_Config,
  PT_Multiline,
  PT_ShortMultiline
} PrintType;
/*common functions*/
int printe(PrintType type);
int initbuf(void);

/*common variables*/
DATA data;
FLOWBUF flowbuf;
char dev[32];
char ipaddr[20];
char errorstring[512];
int debug;
FILE *dataxml;
#endif
