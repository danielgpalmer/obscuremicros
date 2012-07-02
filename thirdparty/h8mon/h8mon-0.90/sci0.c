/** @file
 * @brief	H8/300H 3052,3067,3068,3069シリアル(SCI0)ドライバ(gdb stub用)
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

#define USETXI	//!< 送信割り込みを使う

#define SCI_ADD		0xffffb0	//!<SCI0のアドレス(3052,3067,3068,3069)

#define	MAX_SCI_RXBUF_SIZE	80	//!< SCIの受信バッファサイズ
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

/**
 * @brief	SCI0を初期化する
 * 8bit, stop bit 1, パリティ無し, 調歩同期
 * 受信割り込み使用
 */
static void sci0_init(void)
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
}

/**
 * @brief	SCI0からの受信データを取得する
 * @param[out]	rd	受信データポインタ
 * @return	!=0:データあり 0:データ無し
 */
unsigned char sci0_getc(char *rd)
{
	return read_fifo(&rfifo, (unsigned char *)rd);
}

#ifdef USEINTRX
/**
 * @brief	SCI0受信割り込みハンドラ
 */
void _sci0_rxi(void) __attribute__((interrupt_handler));

void _sci0_rxi(void)
{
	unsigned char rd;

	if(SCI.SSR.BIT.RDRF) {
		rd = SCI.RDR;		// データ受信
		write_fifo(&rfifo, rd);	// FIFOへ格納
		SCI.SSR.BIT.RDRF = 0;
	}
}
#endif

/**
 * @brief	SCI0受信
 */
void _sci0_rcv(void)
{
	unsigned char rd;

	if(SCI.SSR.BIT.RDRF) {
		rd = SCI.RDR;		// データ受信
		SCI.SSR.BIT.RDRF = 0;
		
		if(rd == 0x03) {	// CTRL-C？
			break_task();
		}
		
		if(write_fifo(&rfifo, rd)) {	// FIFOへ格納
			DPRINTF("SCI0 Receive buffer full.\n");
		}
	}
}

/**
 * @brief	SCI0受信エラー割り込みハンドラ
 */
void _sci0_eri(void) __attribute__((interrupt_handler));

void _sci0_eri(void)
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
 * @brief	SCI0送信データエンプティ割り込みハンドラ
 */
void _sci0_txi(void) __attribute__((interrupt_handler));

void _sci0_txi(void)
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
 * @brief	SCI0送信終了割り込みハンドラ(未使用)
 */
void _sci0_tei(void) __attribute__((interrupt_handler));

void _sci0_tei(void)
{
	SCI.SCR.BIT.TEIE = 0;
}

/**
 * @brief	タスクからのSCI0出力
 * @param[in]	td	送信データ
 */
void sci0_putc(char td)
{
#ifdef USETXI
	while(write_fifo(&tfifo, td))
		;
	
	if(SCI.SCR.BIT.TIE == 0) {
		SCI.SCR.BIT.TIE = 1;
	}
#else
	SCI.TDR = td;

	SCI.SSR.BIT.TDRE = 0;
	
	while(!SCI.SSR.BIT.TEND)
	  ;
#endif
}

const struct st_monio sci0_monio = {
	.init	= sci0_init,
	.getc	= sci0_getc,
	.putc	= sci0_putc
};
