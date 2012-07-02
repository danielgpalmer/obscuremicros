/** @file
 * @brief	ネットワーク文字列処理
 *
 * @date	2011.04.23
 * @author	Takashi SHUDO
 */

#include "ip.h"
#include "netstr.h"

int str2ipaddr(char *str, struct IP_ADDR *ipaddr)
{
	int i;
	char *sp = str;
	int val;

	for(i=0; i<IPADDR_LEN; i++) {
		if((*sp < '0') || (*sp > '9')) {
			return -1;
		}
		
		val = 0;
		while(*sp != 0) {
			if(i < (IPADDR_LEN - 1)) {
				if(*sp == '.') break;
			}
			
			if((*sp < '0') || (*sp > '9')) {
				return -1;
			}

			val *= 10;
			val += (*sp - '0');
			if(val > 255) {
				return -1;
			}
			sp ++;
		}
		
		ipaddr->addr[i] = val;
		sp ++;
	}
	
	return 0;
}
