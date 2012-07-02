/** @file
 * @brief	H8/300Hモニタ
 * 
 * @author Takashi SHUDO
 */

#ifndef _H_mon
#define _H_mon

// モニタの動作モード
#define MMODE_MON	0	//!< モニタモード
#define MMODE_GDB	1	//!< GDB stubモード

void set_monmode(unsigned char mode);
void print_version(void);
void mon(void);

#endif // _H_mon
