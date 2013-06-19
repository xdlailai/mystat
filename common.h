#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>



#define DATABASEDIR  "/var/lib/mystat"
#define DEFIFACE "eth0"
#endif

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
  DAY day[31];
  MONTH month[12];
  Day top10[10];
  uint64_t btime;
} DATA;
