/** @file
 * @brief	モニタI/Oデバイス
 *
 * @date	2007.03.11
 * @author	Takashi SHUDO
 */

#ifndef _H_monio
#define _H_monio

struct st_monio {
	void (* init)(void);
	unsigned char (* getc)(char *);
	void (* putc)(char);
};

void init_monio(const struct st_monio *monio);
unsigned char mgetc(char *rd);
void mputc(char td);

void init_stubio(const struct st_monio *monio);
unsigned char stubgetc(char *rd);
void stubputc(char td);

// デバッグ用出力
#ifdef DEBUG
void dputc(char td);
#define DPUTC(x)	dputc(x)
#else 
#define DPUTC(x)
#endif

#endif	/* _H_monio	*/
