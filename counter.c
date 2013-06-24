#include "counter.h"
#include "dbaccess.h"
#include "dbshow.h"
#include "dbxml.h"

void init(void)
{
  initbuf();
//  initdb();
  int nsocket;
  struct ifreq struReqip;
  strncpy(dev, "eth0", 32);
  //strncpy(data.interface, dev,  strlen(dev));
   //获取IP
  nsocket = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&struReqip, 0, sizeof(struReqip));
  strncpy(struReqip.ifr_name, dev, sizeof(dev));
  ioctl(nsocket,SIOCGIFADDR,&struReqip);
  strncpy(ipaddr, inet_ntoa(((struct sockaddr_in*)&(struReqip.ifr_addr))->sin_addr), strlen(inet_ntoa(((struct sockaddr_in*)&(struReqip.ifr_addr))->sin_addr)));
//  printf("ip:%s \n", ipaddr);

}

void listenpck(void)
{
  pcap_t* descr;
  const u_char *packet;
  struct pcap_pkthdr hdr;
  char errbuf[PCAP_ERRBUF_SIZE];
  descr = pcap_open_live(dev,BUFSIZ,0,0,errbuf);
  if(descr == NULL)
  {
    //printf("pcap_open_live(): %s\n",errbuf);
    exit(1);
  }
  else
    //printf("start counting!");
  for(;;)
  {
    packet = pcap_next(descr,&hdr);
    packetcheck(packet, hdr.len);
  }
}

int packetcheck(const u_char* packet, unsigned long len)
{
  const struct sniff_ethernet *ethernet;
  const struct sniff_ip *ip;
  char src;
  int xsignal = 0; /*流量方向标志，0 非有效包 1 上传流量 2 下载流量*/
  u_int size_ip;
  ethernet = (struct sniff_ethernet*)(packet);
  ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
  if(IP_V(ip) != 0x04)
    {
     // printf("this is not a ipv4 packet\n");
      return 1;
    }
  char myip_src[16];
  char myip_dst[16];
  strncpy(myip_src, inet_ntoa(ip->ip_src), 16);
  strncpy(myip_dst, inet_ntoa(ip->ip_dst), 16);
  int lenth =ntohs(ip->ip_len);
  if(strcmp(myip_src, ipaddr) == 0)
  {
    xsignal = 1;
    //printf("this is tx\n");
    //printf("%d\n", lenth);
   // printf("%s, %s\n",myip_src, inet_ntoa(ip->ip_dst));
  }
  if(strcmp(myip_dst, ipaddr) == 0)
  {
    xsignal = 2;
    //printf("this is rx\n");
    //printf("%d\n", lenth);
    //printf("%s, %s\n",inet_ntoa(ip->ip_src), myip_dst);
  }
  updateflowbuf(xsignal, lenth);
  if(flowbuf.filled == 1){
//    printf("flowbuf.rx %d\n", flowbuf.rx);
//    printf("flowbuf.tx %d\n", flowbuf.tx);
    int newdb = readdb("eth0", "/home/fly100");
    parseflowbuf();
    initbuf();
    writedb("eth0", "/home/fly100", newdb);
    //showdb(4);
    showxml();
  }
  return 1;

}

int updateflowbuf(int sig, int len)
{
  if(sig == 0){
    //printf("this packet invalid\n");
  }else if(sig == 1){
    flowbuf.tx += len;
   // printf("tx packet added");
  }else if(sig == 2){
    flowbuf.rx += len;
  //  printf("rx packet added");
  }
  bufstation();
//  time_t current;
//  current = time(NULL);
//  flowbuf.date = current;
/*暂时先不用时间，以后看*/
}

void bufstation(void)
{
  if((flowbuf.rx >= 102400) || (flowbuf.tx >= 102400) )
    flowbuf.filled = 1;
}

void parseflowbuf(void)
{
  uint64_t rxchange = 0, txchange = 0; /*rx change in MB*/
  uint64_t rxkchange = 0, txkchange = 0; /*changes in the KB counters*/
  time_t current;
  struct tm *d;
  int day, month, year;
  current = time(NULL);
  uint64_t buf_rx = flowbuf.rx;
  uint64_t buf_tx = flowbuf.tx;
 // printf(" buf_rx, buf_tx %llu, %llu\n", buf_rx, buf_tx);
  txchange = buf_tx/1048576; /*1024/1024*/
  txkchange = (buf_tx/1024)%1024;
 // printf("txchange, txkchange %llu %llu\n", txchange, txkchange);
  rxchange = buf_rx/1048576;
  rxkchange = (buf_rx/1024)%1024;
 // printf("rxchange, rxkchange %llu %llu\n", rxchange, rxkchange);
  addtraffic(&data.totalrx, &data.totalrxk, rxchange, rxkchange);
  addtraffic(&data.totaltx, &data.totaltxk, txchange, txkchange);
  /*update days and months*/
  addtraffic(&data.day[0].rx, &data.day[0].rxk, rxchange, rxkchange);
  addtraffic(&data.day[0].tx, &data.day[0].txk, txchange, txkchange);
  addtraffic(&data.month[0].rx, &data.month[0].rxk, rxchange,rxkchange);
  addtraffic(&data.month[0].tx, &data.month[0].txk, txchange,txkchange);

 d = localtime(&current);
  day = d->tm_mday;
  month = d->tm_mon;
  year = d->tm_year;

  /*rotate days in database if needed */
  d=localtime(&data.day[0].date);
  if((d->tm_mday!=day) || (d->tm_mon!=month) || (d->tm_year!=year)){
    //printf("day rotated\n");
    if( (data.day[0].rx==0) && (data.day[0].tx==0) && (data.day[0].rxk==0) && (data.day[0].txk==0))
      data.day[0].date=current;
    else
      rotatedays();
  }

  /*rotate months in data if needed */
  d=localtime(&data.month[0].month);
  if(d->tm_mon != month)
    rotatemonths();

  /*test*/
  /*printf(" data download %llu M, %dK\n", data.totalrx, data.totalrxk);
  printf(" data upload %llu M, %dK\n", data.totaltx, data.totaltxk);
  printf(" day download %llu M, %dK\n", data.day[0].rx, data.day[0].rxk);
  printf(" day upload %llu M, %dK\n", data.day[0].tx, data.day[0].txk);
  printf(" month download %llu M, %dK\n", data.month[0].rx, data.month[0].rxk);
  printf(" month upload %llu M, %dK\n", data.month[0].tx, data.month[0].txk);
  */
}


void addtraffic(uint64_t *destmb, int *destkb, uint64_t srcmb, int srckb)
{
  *destmb = *destmb + srcmb;
  *destkb = *destkb + srckb;
  if(*destkb >= 1024){
    *destmb += *destkb/1024;
    *destkb -= (*destkb/1024)*1024;
  }
}
