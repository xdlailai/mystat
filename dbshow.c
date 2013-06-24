#include "dbshow.h"
#include "common.h"

void showdb(int qmode)
{
  if(data.totalrx+data.totaltx==0 && data.totalrxk+data.totaltxk==0){
    printf("%s: not enough data available yet.\n", data.interface);
  }else{
    switch(qmode) {
      case 0:
//        showsummary();
        break;
      case 1:
  //      showdays();
        break;
      case 2:
    //    showmonths();
        break;
      case 3:
      //  showtop();
        break;
      case 4:
        dumpdb();
        break;
      case 5:
     //   showshort();
        break;
      default:
        printf("Error: not such query mode:%d\n", qmode);
        break;

    }
  }
}

void dumpdb(void)
{
  int i;
  printf("active;%d\n", data.active);
  printf("interface;%s\n",data.interface);
  printf("created;%u\n", (unsigned int)data.created);
  printf("updated;%u\n", (unsigned int)data.lastupdated);
  printf("totalrx;%llu\n", data.totalrx);
  printf("totaltx;%llu\n", data.totaltx);
  printf("totalrxk;%d\n", data.totalrxk);
  printf("totaltxk;%d\n", data.totaltxk);

  for(i=0;i<=29;i++){
    printf("d;%d;%u;%llu;%llu;%d;%d;%d\n", i, (unsigned int)data.day[i].date, data.day[i].rx, data.day[i].tx, data.day[i].rxk, data.day[i].txk, data.day[i].used);
  }

  for(i=0;i<=11;i++){
    printf("m;%d;%u;%llu;%llu;%d;%d;%d", i, (unsigned int)data.month[i].month, data.month[i].rx, data.month[i].tx, data.month[i].rxk, data.month[i].txk, data.month[i].used);
  }

  for(i=0;i<=9;i++){
    printf("t;%d;%u;%llu;%llu;%d;%d;%d\n", i, (unsigned int)data.top10[i].date, data.top10[i].rx, data.top10[i].tx, data.top10[i].rxk, data.top10[i].txk, data.top10[i].used);
  }
}
