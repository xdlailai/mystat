#ifndef DEFINE_H
#define DEFINE_H

#define MAX_MOD_BUM 32
#define U_BIT 3
#define U_64 unsigned long long
#define LEN_32 32
#define LEN_64 64
#define LEN_128 128
#define LEN_256 256
#define LEN_512 512
#define LEN_1024 1024
#define LEN_4096 4096


#define STAT "/proc/stat"
#define VMSTAT "/proc/vmstat"
#define MEMINFO "/proc/meminfo"
#define LOADAVG "/proc/loadavg"
#define NET_DEV "/proc/net/dev"
#define NET_SNMP "/proc/net/snmp"
#define TCP "/proc/net/tcp"
#define NETSTAT "/proc/net/netstat"

enum {
  HIDE_BIT,
  DETAIL_BIT,
  SUMMARY_BIT,
  SPEC_BIT
};

enum {
  STATS_NULL,
  STATS_SUB,
  STATS_SUB_INTER
};

#endif
