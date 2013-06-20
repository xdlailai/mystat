#include "counter.h"

void init()
{
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

void listenpck()
{
  pcap_t* descr;
  const u_char *packet;
  struct pcap_pkthdr hdr;
  char errbuf[PCAP_ERRBUF_SIZE];
  descr = pcap_open_live(dev,BUFSIZ,0,0,errbuf);
  if(descr == NULL)
  {
    printf("pcap_open_live(): %s\n",errbuf);
    exit(1);
  }
  else
    printf("start counting!");
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
  u_int size_ip;
  ethernet = (struct sniff_ethernet*)(packet);
  ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
  if(IP_V(ip) != 0x04)
    {
      printf("this is not a ipv4 packet\n");
      return 1;
    }
  char myip_src[16];
  char myip_dst[16];
  strncpy(myip_src, inet_ntoa(ip->ip_src), 16);
  strncpy(myip_dst, inet_ntoa(ip->ip_dst), 16);
  int lenth =ntohs(ip->ip_len);
  if(strcmp(myip_src, ipaddr) == 0)
  {
    printf("this is tx\n");
    printf("%d\n", lenth);
    printf("%s, %s\n",myip_src, inet_ntoa(ip->ip_dst));
  }
  if(strcmp(myip_dst, ipaddr) == 0)
  {
    printf("this is rx\n");
    printf("%d\n", lenth);
    printf("%s, %s\n",inet_ntoa(ip->ip_src), myip_dst);
  }

  return 1;

}
