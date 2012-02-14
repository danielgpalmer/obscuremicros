#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS				1
#define MEM_ALIGNMENT		4
#define LWIP_SOCKET         0
#define LWIP_NETCONN	    0
#define LWIP_RAW			1
#define LWIP_HAVE_SLIPIF	1
#define SLIP_BLOCK     		0
#define SLIP_DONTBLOCK 		1
#define SLIP_DEBUG                  LWIP_DBG_OFF

#define TCP_QUEUE_OOSEQ         0
#define TCP_MSS                 512
#define TCP_SND_BUF             2 * TCP_MSS
#define TCP_SND_QUEUELEN        6 * TCP_SND_BUF/TCP_MSS
#define TCP_WND                 512
#define TCP_MAXRTX              12
#define TCP_SYNMAXRTX           4

#define LWIP_ARP                    1
#define LWIP_ICMP                   1
#define LWIP_BROADCAST_PING         1
#define LWIP_STATS_DISPLAY          0
#define MEM_STATS                   0
#define SYS_STATS                   0
#define MEMP_STATS                  0
#define LINK_STATS                  0
#define LWIP_CHKSUM_ALGORITHM       2

#define LWIP_TCP_KEEPALIVE          1

// Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing
//#define TCP_KEEPIDLE_DEFAULT        10000UL // Default KEEPALIVE timer in milliseconds
//#define TCP_KEEPINTVL_DEFAULT       2000UL  // Default Time between KEEPALIVE probes in milliseconds
//#define TCP_KEEPCNT_DEFAULT         9U      // Default Counter for KEEPALIVE probes

#define LWIP_DEBUG                  LWIP_DBG_OFF

#define ETHARP_DEBUG                LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF

#endif /* __LWIPOPTS_H__ */
