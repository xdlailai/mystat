#include "common.h"

void read_mem_stats()
{
  FILE *fp;
  char line[LEN_128];
  memset(line, 0, LEN_128);
  if((fp = fopen(MEMINFO, "r")) == NULL){
    return;
  }

  while(fgets(line, 128, fp) != NULL) {
    if(!strncmp(line, "MemTotal:", 9)){
      sscanf(line + 9, "%lu", &statmem.tlmkb);
    }
    else if(!strncmp(line, "MemFree:", 8)){
      sscanf(line + 8, "%lu", &statmem.frmkb);
    }
    else if(!strncmp(line, "Buffers:", 8)){
      sscanf(line + 8, "%lu", &statmem.bufkb);
    }
    else if(!strncmp(line, "Cached:", 7)){
      sscanf(line + 7, "%lu", &statmem.camkb);
    }
    else if(!strncmp(line, "Active:", 7)){
      sscanf(line + 7, "%lu", &statmem.acmkb);
    }
    else if(!strncmp(line, "Inactive:", 9)){
      sscanf(line + 9, "%lu", &statmem.iamkb);
    }
    else if(!strncmp(line, "Slab:", 5)){
      sscanf(line + 5, "%lu", &statmem.slmkb);
    }
    else if(!strncmp(line, "SwapCached:", 11)){
      sscanf(line + 11, "%lu", &statmem.caskb);
    }
    else if(!strncmp(line, "SwapTotal:", 10)){
      sscanf(line + 10, "%lu", &statmem.tlskb);
    }
    else if(!strncmp(line, "SwapFree:", 9)){
      sscanf(line + 9, "%lu", &statmem.frskb);
    }
    else if(!strncmp(line, "Committed_AS:",  13)){
      sscanf(line + 13, "%lu", &statmem.comkb);
    }
  }
  if(fclose(fp) < 0){
    return;
  }
  statmem.used = (statmem.tlmkb - statmem.frmkb -statmem.bufkb - statmem.camkb) * 100.0/statmem.tlmkb;

}

void set_mem_record()
{
  while(1){
  read_mem_stats();
  printf("%lf\n", statmem.used);
  sleep(3);
  }
}

