#ifndef MYSAR_H
#define MYSAR_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framework.h"

struct statistic {
  int total_mod_num;
  time_t cur_time;
};

extern struct module mods[MAX_MOD_BUM];
extern struct statistic statis;

#endif
