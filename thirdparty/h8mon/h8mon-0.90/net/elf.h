/** @file
 * @brief	ELFローダ
 *
 * @date	2008.02.19
 * @author	Takashi SHUDO
 */

#ifndef _H_elf
#define _H_elf

int load_elf(unsigned long *entry, unsigned char *elfbin);

#endif // _H_elf
