#include "common.h"

void read_cpu_stats()
{
  FILE *fp;
  char line[LEN_4096];
  memset(line, 0, LEN_4096);
  if((fp = fopen(STAT, "r")) == NULL){
    return;
  }
  while(fgets(line, LEN_4096, fp)!=NULL){
    if(!strncmp(line, "cpu ", 4)){
      sscanf(line + 5, "%llu %llu %llu %llu %llu %llu %llu %llu %llu",
          &statcpu.cpu_user,
          &statcpu.cpu_nice,
          &statcpu.cpu_sys,
          &statcpu.cpu_idle,
          &statcpu.cpu_iowait,
          &statcpu.cpu_hardirq,
          &statcpu.cpu_softirq,
          &statcpu.cpu_steal,
          &statcpu.cpu_guest);
    }
  }
  if(fclose(fp) < 0){
    return;
  }
  unsigned long long total_last = statcpu.cpu_user_last + statcpu.cpu_nice_last +statcpu.cpu_sys_last + statcpu.cpu_idle_last + statcpu.cpu_iowait_last + statcpu.cpu_hardirq_last + statcpu.cpu_softirq_last;
  unsigned long long total_now = statcpu.cpu_user + statcpu.cpu_nice + statcpu.cpu_sys + statcpu.cpu_idle + statcpu.cpu_iowait + statcpu.cpu_hardirq + statcpu.cpu_softirq;
//  if(statcpu.cpu_idle >= statcpu.cpu_)
  if(total_now <= total_last)
    return;
  if(statcpu.cpu_idle >= statcpu.cpu_idle_last){
    statcpu.used = 100.0 - (double)(statcpu.cpu_idle - statcpu.cpu_idle_last) * 100.0 /(total_now-total_last);
  }
//  printf("%lf \n",  statcpu.used);
  statcpu.cpu_user_last = statcpu.cpu_user;
  statcpu.cpu_nice_last = statcpu.cpu_nice;
  statcpu.cpu_sys_last = statcpu.cpu_sys;
  statcpu.cpu_idle_last = statcpu.cpu_idle;
  statcpu.cpu_iowait_last = statcpu.cpu_iowait;
  statcpu.cpu_hardirq_last = statcpu.cpu_hardirq;
  statcpu.cpu_softirq_last = statcpu.cpu_softirq;
  statcpu.cpu_steal_last = statcpu.cpu_steal;
  statcpu.cpu_guest_last = statcpu.cpu_guest;
}

/*void set_cpu_record()
{
  read_cpu_stats();
  sleep(5);
  while(1){
    read_cpu_stats();
    sleep(5);
//    printf("%lf\n", statcpu.cpu_nice);
  }
}*/
