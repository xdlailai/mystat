#include "common.h"
#include "dbxml.h"

void showxml(void)
{
  dataxml = fopen(XMLBASEDIR, "w");
  if(!dataxml){
    perror("open xmlfile fail\n");
    return;
  }
  int i;
//  fprintf(dataxml, "<?xml version=\"%s\" xmlversion=\"%d\"?>\n", MYSTATVERSION, XMLVERSION);
  fprintf(dataxml, " <interface id=\"%s\">\n", data.interface);
  fprintf(dataxml, "  <id>%s</id>\n", data.interface);
  fprintf(dataxml, "  <nick>%s</nick>\n", data.nick);
  fprintf(dataxml, "  <created>");
  xmldate(&data.created, 1);
  fprintf(dataxml, "</created>\n");
  fprintf(dataxml, "  <updated>");
  xmldate(&data.lastupdated, 2);
  fprintf(dataxml, "</updated>\n");

  fprintf(dataxml, "  <traffic>\n");
  fprintf(dataxml, "   <total><rx>%llu</rx><tx>%llu</tx></total>\n", (data.totalrx*1024)+data.totalrxk, (data.totaltx*1024)+data.totaltxk);
  fprintf(dataxml, "   <days>\n");
  for(i=0;i<=29;i++){
    if(data.day[i].used){
      fprintf(dataxml, "    <day id=\"%d\">", i);
      xmldate(&data.day[i].date, 1);
      fprintf(dataxml, "<rx>%llu</rx><tx>%llu</tx></day>\n", (data.day[i].rx*1024)+data.day[i].rxk, (data.day[i].tx*1024)+data.day[i].txk);
    }
  }
  fprintf(dataxml, "   </days>\n");

  fprintf(dataxml, "   <months>\n");
  for(i=0;i<=11;i++){
    if(data.month[i].used){
      fprintf(dataxml, "    <month id=\"%d\">", i);
      xmldate(&data.month[i].month, 3);
      fprintf(dataxml, "<rx>%llu</rx><tx>%llu</tx></month>\n", (data.month[i].rx*1024)+data.month[i].rxk, (data.month[i].tx*1024)+data.month[i].txk);
    }
  }
  fprintf(dataxml, "   </months>\n");

  fprintf(dataxml, "   <tops>\n");
  for(i=0;i<=9;i++) {
    if(data.top10[i].used){
      fprintf(dataxml, "    <top id=\"%d\">", i);
      xmldate(&data.top10[i].date, 2);
      fprintf(dataxml, "<rx>%llu</rx><tx>%llu</tx></top>\n", (data.top10[i].rx*1024)+data.top10[i].rxk, (data.top10[i].tx*1024)+data.top10[i].txk);
    }
  }
  fprintf(dataxml, "   </tops>\n");

  fprintf(dataxml, "  </traffic>\n");
  fprintf(dataxml, "  <info>\n");
  fprintf(dataxml, "   <cpu>%.2lf</cpu>\n", statcpu.used);
  fprintf(dataxml, "   <mem>%.2lf</mem>\n", statmem.used);
  fprintf(dataxml, "   <load>%.2lf</load>\n", statload.load_avg);
  fprintf(dataxml, "   <mem>%.2lf</mem>\n", statmem.used);
  fprintf(dataxml, "   <download_rate>%.2lf</download_rate>\n", stattraffic.download_rate/1024);
  fprintf(dataxml, "   <upload_rate>%.2lf</upload_rate>\n", stattraffic.upload_rate/1024);
  fprintf(dataxml, "   <partition>\n");
  fprintf(dataxml, "    <dir>%s</dir>\n", statpartion.dir);
  fprintf(dataxml, "    <total>%ld</total>\n", statpartion.total/1024);
  fprintf(dataxml, "    <used>%ld</used>\n", statpartion.used/1024);
  fprintf(dataxml, "    <pct>%.2lf</pct>\n", statpartion.pct);
  fprintf(dataxml, "   </partition>\n");
  fprintf(dataxml, "  </info>\n");
  fprintf(dataxml, " </interface>\n");

  int b = fflush(dataxml);
  if(b != 0){
    perror("fflush failed!\n");
  }
  int a = 1;
  a = fclose(dataxml);
  if(a != 0){
    perror("fclose fail!\n");
  }
}

void xmldate(time_t *date, int type)
{
  struct tm *d;
  char *buffer;
  char *type1 = "<date><year>%Y</year><month>%m</month><day>%d</day></date>";
  char *type2 = "<date><year>%Y</year><month>%m</month><day>%d</day></date><time><hour>%H</hour><minute>%M</minute></time>";
  char *type3 = "<date><year>%Y</year><month>%m</month></date>";

  d = localtime(date);
  if(type == 1){
    buffer = malloc(strlen(type1)+3);
    strftime(buffer, strlen(type1)+3, type1, d);
    fprintf(dataxml, "%s", buffer);
    free(buffer);
  }else if(type == 2){
    buffer = malloc(strlen(type2)+3);
    strftime(buffer, strlen(type2)+3, type2, d);
    fprintf(dataxml, "%s", buffer);
    free(buffer);
  }else if(type == 3){
    buffer = malloc(strlen(type3)+3);
    strftime(buffer, strlen(type3)+3, type3, d);
    fprintf(dataxml, "%s", buffer);
    free(buffer);
  }
}
