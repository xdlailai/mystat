#include "common.h"

void read_traffic_stats()
{
  FILE *fp;
  char *p = NULL;
  char line[LEN_4096] = {0};

  if((fp = fopen(NET_DEV, "r")) == NULL){
    return;
  }

  while(fgets(line, LEN_4096, fp) != NULL){
    if(strstr(line, "eth") || strstr(line, "em")){
      p = strchr(line, ':');
      sscanf(p + 1, "%llu %*u %*u %*u %*u %*u %*u %*u "
              "%llu %*u %*u %*u %*u %*u %*u %*u",
              &stattraffic.download_now, &stattraffic.upload_now);
      stattraffic.download_rate = (double)(stattraffic.download_now - stattraffic.download_ago)/5;
      stattraffic.upload_rate = (double)(stattraffic.upload_now - stattraffic.upload_ago)/5;
      stattraffic.download_ago = stattraffic.download_now;
      stattraffic.upload_ago  = stattraffic.upload_now;
//      printf("%llu %llu %lf %lf\n", stattraffic.download_now, stattraffic.upload_now, stattraffic.download_rate, stattraffic.upload_rate);
    }

  }
  fclose(fp);
}
