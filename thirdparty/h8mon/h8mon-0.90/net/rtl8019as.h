/** @file
 * @brief	RTL8019ASドライバ
 *
 * AKI-H8 3069 LAN ボード対応
 *
 * @date	2011.01.11
 * @authoer	Takashi SHUDO
 */

#ifndef _H_rtl8019as
#define  _H_rtl8019as

int rtl8019as_init(void);
int rtl8019as_open(void);
int rtl8019as_close(void);
long rtl8019as_read(unsigned char *data, long size);
long rtl8019as_write(unsigned char *data, long size);
int rtl8019as_ioctl(long com, long arg);

#endif // _H_rtl8019as
