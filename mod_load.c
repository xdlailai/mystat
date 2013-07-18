#include "common.h"

void read_load_stats()
{
  int load_tmp[3];
  FILE *fp;
  if((fp = fopen(LOADAVG, "r")) == NULL ){
    return;
  }
  if(fscanf(fp, "%d.%d %d.%d %d.%d %ld/%d",
          &load_tmp[0], &statload.load_avg_1,
          &load_tmp[1], &statload.load_avg_5,
          &load_tmp[2], &statload.load_avg_15,
          &statload.nr_running,
          &statload.nr_threads) != 8){
    fclose(fp);
    return;
  }
  statload.load_avg_1 += load_tmp[0] * 100;
  statload.load_avg_5 += load_tmp[1] * 100;
  statload.load_avg_15 += load_tmp[2] * 100;
  statload.load_avg = statload.load_avg_1 / 100.0;

}

