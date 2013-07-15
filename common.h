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

#define STAT "/proc/stat"
#define MEMINFO "/proc/meminfo"
#define LOADAVG "/proc/loadavg"
#define NET_DEV "/proc/net/dev"

#define LEN_4096 4096
#define LEN_128 128


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

typedef struct{
  unsigned long long cpu_user;
  unsigned long long cpu_nice;
  unsigned long long cpu_sys;
  unsigned long long cpu_idle;
  unsigned long long cpu_iowait;
  unsigned long long cpu_hardirq;
  unsigned long long cpu_softirq;
  unsigned long long cpu_steal;
  unsigned long long cpu_guest;
  unsigned long long cpu_user_last;
  unsigned long long cpu_nice_last;
  unsigned long long cpu_sys_last;
  unsigned long long cpu_idle_last;
  unsigned long long cpu_iowait_last;
  unsigned long long cpu_hardirq_last;
  unsigned long long cpu_softirq_last;
  unsigned long long cpu_steal_last;
  unsigned long long cpu_guest_last;
  double used;
} STAT_CPU;

typedef struct {
  unsigned long tlmkb;
  unsigned long frmkb;
  unsigned long bufkb;
  unsigned long camkb;
  unsigned long acmkb;
  unsigned long iamkb;
  unsigned long slmkb;
  unsigned long caskb;
  unsigned long tlskb;
  unsigned long frskb;
  unsigned long comkb;
  double used;
} STAT_MEM;

typedef struct{
  int load_avg_1;
  int load_avg_5;
  int load_avg_15;
  double nr_running;
  int nr_threads;
  double load_avg;
} STAT_LOAD;
typedef struct {
  char dir[32];
  long total;
  long used;
  double pct;
} STAT_PARTION;
typedef struct {
  unsigned long long download_now;
  unsigned long long upload_now;
  unsigned long long download_ago;
  unsigned long long upload_ago;
  double download_rate;
  double upload_rate;
} STAT_TRAFFIC;

/*common functions*/
int printe(PrintType type);
int initbuf(void);

/*common variables*/
STAT_CPU statcpu;
STAT_MEM statmem;
STAT_LOAD statload;
STAT_PARTION statpartion;
STAT_TRAFFIC stattraffic;

DATA data;
FLOWBUF flowbuf;
char dev[32];
char ipaddr[20];
char errorstring[512];
int debug;
FILE *dataxml;
#endif
