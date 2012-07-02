/** @file
 * @brief	H8/300H 3052,3067,3068,3069シリアル(SCI1)ドライバ
 *
 * @date	2008.05.01
 * @author	Takashi SHUDO
 */
/*
   BBR設定値
  
  クロック	25	22	20	16	15	(MHz)
  115200bps	6	5	-	-	3
  57600bps	13	11	10	-	7
  38400bbs	19	17	15	12	11
  19200bps	40	35	32	25	23
  9600pbs	80	71	64	51	48
*/

#include "monio.h"
#include "tprintf.h"
#include "fifo.h"
#include "task.h"

#define USETXI	//!< 送信割り込みを使う場合定義する

#define SCI_ADD		0xffffb8	//!<SCI1のアドレス(3052,3067,3068,3069)
#define DBGSCI_ADD	0xffffb0	//!<SCI0のアドレス(3052,3067,3068,3069)

#define	MAX_SCI_RXBUF_SIZE	136	//!< SCIの受信バッファサイズ
static struct fifo rfifo;		//!< 受信fifo
static unsigned char rbuf[MAX_SCI_RXBUF_SIZE];	//!< 受信バッファ

#ifdef USETXI
#define	MAX_SCI_TXBUF_SIZE	16	//!< SCIの送信バッファサイズ
static struct fifo tfifo;		//!< 送信fifo
static unsigned char tbuf[MAX_SCI_TXBUF_SIZE];	//!< 送信バッファ
#endif

/**
 * SCIレジスタ構造体
 */
struct st_sci {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CA  :1;
			unsigned char CHR :1;
			unsigned char PE  :1;
			unsigned char OE  :1;
			unsigned char STOP:1;
			unsigned char MP  :1;
			unsigned char CKS :2;
		} BIT;
	} SMR;
	unsigned char	BRR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE :1;
			unsigned char RIE :1;
			unsigned char TE  :1;
			unsigned char RE  :1;
			unsigned char MPIE:1;
			unsigned char TEIE:1;
			unsigned char CKE :2;
		} BIT;
	} SCR;
	unsigned char	TDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TDRE:1;
			unsigned char RDRF:1;
			unsigned char ORER:1;
			unsigned char FER :1;
			unsigned char PER :1;
			unsigned char TEND:1;
			unsigned char MPB :1;
			unsigned char MPBT:1;
		} BIT;
	} SSR;
	unsigned char RDR;
};

#define SCI	(*(volatile struct st_sci *)SCI_ADD)	//!< SCIレジスタ
#ifdef DEBUG
#define DBGSCI	(*(volatile struct st_sci *)DBGSCI_ADD)
#endif

/**
 * @brief	SCIを初期化する
 * 8bit, stop bit 1, パリティ無し, 調歩同期
 * 受信割り込み使用
 */
static void sci1_init(void)
{
	int i;

	extern const unsigned char brr_val;	// BRR値
 
	init_fifo(&rfifo, rbuf, MAX_SCI_RXBUF_SIZE);
#ifdef USETXI
	init_fifo(&tfifo, tbuf, MAX_SCI_TXBUF_SIZE);
#endif

	SCI.SCR.BYTE	= 0x00;	// まずクリア
	SCI.SMR.BYTE	= 0x00;	// 調歩同期,8ビット,パリティ無し,ストップ1,
	SCI.BRR		= brr_val;
	for(i=0; i<255; i++)	// 1ビット時間以上ウェイト
		;
	SCI.SCR.BYTE	= 0x70;	// 送信受信許可、受信割り込み許可
	SCI.SSR.BIT.ORER= 0;
	SCI.SSR.BYTE	= 0x80;	// エラーフラグクリア

#ifdef DEBUG
	DBGSCI.SMR.BYTE	= 0x00;	// 調歩同期,8ビット,パリティ無し,ストップ1,
	DBGSCI.BRR	= brr_val;
	DBGSCI.SCR.BYTE	= 0x30;	// 送信受信許可
#endif
}

/**
 * @brief	SCIからの受信データを取得する
 * @param[out]	rd	受信データポインタ
 * @return	!=0:データあり 0:データ無し
 */
unsigned char sci1_getc(char *rd)
{
	return read_fifo(&rfifo, (unsigned char *)rd);
}

#ifdef NOTUSE_CTRLC_BREAK
/**
 * @brief	SCI受信割り込みハンドラ
 */
void _sci1_rxi(void) __attribute__((interrupt_handler));

void _sci1_rxi(void)
{
	unsigned char rd;

	if(SCI.SSR.BIT.RDRF) {
		rd = SCI.RDR;		// データ受信
		SCI.SSR.BIT.RDRF = 0;
		if(write_fifo(&rfifo, rd)) {	// FIFOへ格納
			DPRINTF("SCI1 Receive buffer full.\n");
		}
	}
}
#endif

/**
 * @brief	SCI1受信
 */
void _sci1_rcv(void)
{
	unsigned char rd;

	if(SCI.SSR.BIT.RDRF) {
		rd = SCI.RDR;		// データ受信
		SCI.SSR.BIT.RDRF = 0;

#if 0
		DBGSCI.TDR = rd;
		
		DBGSCI.SSR.BIT.TDRE = 0;
#endif
		if(rd == 0x03) {	// CTRL-C？
			break_task();
		}
		
		if(write_fifo(&rfifo, rd)) {	// FIFOへ格納
			DPRINTF("SCI1 Receive buffer full.\n");
		}
	}
}

/**
 * @brief	SCI1受信エラー割り込みハンドラ
 */
void _sci1_eri(void) __attribute__((interrupt_handler));

void _sci1_eri(void)
{
	if(SCI.SSR.BIT.FER) {	// フレーミングエラー
		SCI.SSR.BIT.FER = 0;
	}

	if(SCI.SSR.BIT.PER) {	// パリティエラー
		SCI.SSR.BIT.PER = 0;
	}

	if(SCI.SSR.BIT.ORER) {	// オーバーランエラー
		SCI.SSR.BIT.ORER = 0;	// オーバーランエラー
	}
}

/**
 * @brief	SCI1送信データエンプティ割り込みハンドラ
 */
void _sci1_txi(void) __attribute__((interrupt_handler));

void _sci1_txi(void)
{
#ifdef USETXI
	unsigned char td;
	
	if(read_fifo(&tfifo, &td)) {
		SCI.TDR = td;
	
		SCI.SSR.BIT.TDRE = 0;
	} else {
		SCI.SCR.BIT.TIE = 0;
	}
#endif
}

/**
 * @brief	SCI送信終了割り込みハンドラ(未使用)
 */
void _sci1_tei(void) __attribute__((interrupt_handler));

void _sci1_tei(void)
{
	SCI.SCR.BIT.TEIE = 0;
}

/**
 * @brief	タスクからのSCI1出力
 * @param[in]	td	送信データ
 */
void sci1_putc(char td)
{
#ifdef USETXI
	while(write_fifo(&tfifo, td))
		;
	
	if(SCI.SCR.BIT.TIE == 0) {
		SCI.SCR.BIT.TIE = 1;
	}
#else
	while(!SCI.SSR.BIT.TEND)
	  ;
	
	SCI.TDR = td;

	SCI.SSR.BIT.TDRE = 0;
#endif
}

/**
 * @brief	割り込みハンドラからのSCI1出力
 * @param[in]	td	送信データ
 */
void sci1_putci(char td)
{
//#ifdef USETXI
#if 0
	if(write_fifo(&tfifo, td)) {
		unsigned char data;
		while(!SCI.SSR.BIT.TEND)
			;
		read_fifo(&tfifo, &data);
		SCI.TDR = data;
		SCI.SSR.BIT.TDRE = 0;
		write_fifo(&tfifo, td);
	}
	
	if(SCI.SCR.BIT.TIE == 0) {
		SCI.SCR.BIT.TIE = 1;
	}
#else
	while(!SCI.SSR.BIT.TEND)
	  ;
	
	SCI.TDR = td;

	SCI.SSR.BIT.TDRE = 0;
#endif
}

/**/

#ifdef DEBUG
void dputc(char td)
{
	DBGSCI.TDR = td;

	while(!DBGSCI.SSR.BIT.TDRE)
	  ;

	DBGSCI.SSR.BIT.TDRE = 0;
	
	while(!DBGSCI.SSR.BIT.TEND)
	  ;
}
#endif

const struct st_monio sci1_monio = {
	.init	= sci1_init,
	.getc	= sci1_getc,
	.putc	= sci1_putc
};
