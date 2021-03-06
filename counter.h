#ifndef COUNTER_H
#define COUNTER_H
#include "common.h"

/*Ethernet header */
struct sniff_ethernet {
  u_char ether_dhost[6];
  u_char ether_shost[6];
  u_short ether_type;
};
/*IP header*/
struct sniff_ip{
  u_char ip_vhl;
  u_char ip_tos;
  u_short ip_len;
  u_short ip_id;
  u_short ip_off;
  u_char ip_ttl;
  u_char ip_p;
  u_short ip_sum;
  struct in_addr ip_src, ip_dst;
};
#define IP_HL(ip) (((ip)->ip_vhl)&0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)
#define SIZE_ETHERNET 14

void init(void);
void listenpck(void);
int packetcheck(const u_char *packet, unsigned long len);
int updateflowbuf(int sig, int len);
void bufstation(void);
void parseflowbuf(void);
void addtraffic(uint64_t *destmb, int *destkb, uint64_t srcmb, int srckb);
#endif
