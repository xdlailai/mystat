#include "mysar.h"

/*structure for cpu infomation*/
struct stats_cpu {
  unsigned long long cpu_user;
  unsigned long long cpu_nice;
  unsigned long long cpu_sys;
  unsigned long long cpu_idle;
  unsigned long long cpu_iowait;
  unsigned long long cpu_steal;
  unsigned long long cpu_hardirq;
  unsigned long long cpu_softirq;
  unsigned long long cpu_guest;
};

#define STATS_CPU_SIZE (sizeof(struct stats_cpu))

static char *cpu_usage = " --cpu    CPU share (user, system, interrupt, nice, & idle)";
static void read_cpu_stats(struct module *mod)
{
  FILE *fp;
  char line[LEN_4096];
  char buf[LEN_4096];
  struct stats_cpu st_cpu;

  memset(buf, 0, LEN_4096);
  memset(&st_cpu, 0, sizeof(struct stats_cpu));

  if((fp = fopen(STAT, "r")) == NULL){
    return;
  }
  while(fgets(line, LEN_4096, fp)!= NULL){
    if(!strncmp(line, "cpu ", 4)){
      sscanf(line + 5, "%llu %llu %llu %llu %llu %llu %llu %llu %llu",
          &st_cpu.cpu_user,
          &st_cpu.cpu_nice,
          &st_cpu.cpu_sys,
          &st_cpu.cpu_idle,
          &st_cpu.cpu_iowait,
          &st_cpu.cpu_hardirq,
          &st_cpu.cpu_softirq,
          &st_cpu.cpu_steal,
          &st_cpu.cpu_guest);
    }
  }

  int pos = sprintf(buf, "%llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu",
        st_cpu.cpu_user,
        st_cpu.cpu_sys,
        st_cpu.cpu_iowait,
        st_cpu.cpu_hardirq,
        st_cpu.cpu_softirq,
        st_cpu.cpu_idle,
        st_cpu.cpu_nice,
        st_cpu.cpu_steal,
        st_cpu.cpu_guest);
  buf[pos] = '\0';

  set_mod_record(mod, buf);
  if(fclose(fp) < 0){
    return;
  }
}

static void set_cpu_record(struct module *mod, double st_array[], U_64 pre_array[], U_64 cur_array[], int inter)
{
  int i, j;
  U_64 pre_total, cur_total;
  pre_total = cur_total =0;

  for(i = 0; i < mod->n_col; i++){
    if(cur_array[i] < pre_array[i]){
      for(j = 0; j < 9; j++)
        st_array[j] = -1;
      return;
    }
    pre_total += pre_array[i];
    cur_total += cur_array[i];
  }

  if(cur_total <= pre_total)
    return;
  for(i = 0; i < 9; i++){
    if((i != 5)&&(cur_array[i] >= pre_array[i]))
      st_array[i] = (cur_array[i] - pre_array[i]) * 100.0 / (cur_total - pre_total);
  }
  st_array[5] = st_array[0] + st_array[1] + st_array[3] +st_array[4] + st_array[6];

}

static struct mod_info cpu_info[] = {
  {"  user", DETAIL_BIT, 0, STATS_NULL},
  {"   sys", DETAIL_BIT, 0, STATS_NULL},
  {"  wait", DETAIL_BIT, 0, STATS_NULL},
  {"  hirq", DETAIL_BIT, 0, STATS_NULL},
  {"  sirq", DETAIL_BIT, 0, STATS_NULL},
  {"  util", SUMMARY_BIT, 0, STATS_NULL},
  {"  nice", HIDE_BIT, 0, STATS_NULL},
  {"  steal", HIDE_BIT, 0, STATS_NULL},
  {"  guest", HIDE_BIT, 0, STATS_NULL},
};

void mod__register(struct module *mod)
{
  register_mod_fileds(mod, "--cpu", cpu_usage, cpu_info, 9, read_cpu_stats, set_cpu_record);
}
