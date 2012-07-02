/** @file
 * @brief	H8/3068F,3069F BSC設定
 *
 * HM5117800(2M bytes DRAM,CS2,8bit接続)
 *
 * メモリアドレス:0x400000-0x5fffff
 *
 * _init()では以下の処理を行ってください
 *
 * 1. H8/3664(ノーマルモード)以外はSYSCRのUEビットを0にする<br>
 * 2. SCI1(モニタで使用するSCI)の割込みプライオリティビットを1にする<br>
 * 3. 外部メモリを使用する場合はポート、バスコントローラの初期化を行う<br>
 *
 * @date		2002.04.08
 * @author	Takashi Shudo
 */

#include "config.h"

// コンフィグレーション
const char cpu_name[] = "3069F";	//!< CPU名
const unsigned char brr_val = 6;	//!< SCIビットレート(115200BPS 25MHz)
//const unsigned char brr_val = 10;	//!< SCIビットレート(57600BPS 20MHz)
//const unsigned char vbr_val = 5;	//!< SCIビットレート(115200BPS 22MHz)
const unsigned long init_pc_addr = 0x00400000;	//!< ユーザプログラムのPC初期値
const unsigned long init_sp_addr = 0x00ffff20;	//!< ユーザプログラムのSP初期値

/**
 * システムコントロールレジスタ
 */
union un_syscr {
	unsigned char BYTE;
	struct {
		unsigned char SSBY :1;
		unsigned char STS  :3;
		unsigned char UE   :1;
		unsigned char NMIEG:1;
		unsigned char SSOE :1;
		unsigned char RAME :1;
	} BIT;
};

#define SYSCR (*(volatile union un_syscr *)0xfee012)	//!< SYSCRのアドレス

#define SCI1IPR				0xfee019	//!<SCI1のIPRアドレス
#define SCI1IPRBIT			0x04		//!<SCI1のIPRビット位置
#define SCI0IPR				0xfee019	//!<SCI0のIPRアドレス
#define SCI0IPRBIT			0x08		//!<SCI0のIPRビット位置

#define P1DDR (*(volatile unsigned char *)0xfee000)
#define P2DDR (*(volatile unsigned char *)0xfee001)
#define P6DDR (*(volatile unsigned char *)0xfee005)
#define P8DDR (*(volatile unsigned char *)0xfee007)
#define PBDDR (*(volatile unsigned char *)0xfee00a)
#define PBDR  (*(volatile unsigned char *)0xffffda)

#define ADRCR (*(volatile unsigned char *)0xfee01e)
#define CSCR  (*(volatile unsigned char *)0xfee01f)
#define ABWCR (*(volatile unsigned char *)0xfee020)
#define ASTCR (*(volatile unsigned char *)0xfee021)
#define WCRH  (*(volatile unsigned char *)0xfee022)
#define WCRL  (*(volatile unsigned char *)0xfee023)
#define BCR   (*(volatile unsigned char *)0xfee024)

#define DRCRA (*(volatile unsigned char *)0xfee026)
#define DRCRB (*(volatile unsigned char *)0xfee027)
#define RTMCSR (*(volatile unsigned char *)0xfee028)
#define RTCNT (*(volatile unsigned char *)0xfee029)
#define RTCOR (*(volatile unsigned char *)0xfee02a)

/**
 * @brief	CPUを初期化する
 */
void _init(void)
{
	volatile unsigned long i;
	
	*(unsigned char *)SCI1IPR |= SCI1IPRBIT;// SCI1割り込みプライオリティ=1
	*(unsigned char *)SCI0IPR |= SCI0IPRBIT;// SCI0割り込みプライオリティ=1

	SYSCR.BIT.UE = 0;	// UIビットは割り込みマスクとして使用
	
	// ポート設定
	P1DDR = 0xff;	// A0-A7	出力設定
	P2DDR = 0x07;	// A8-A10	出力設定
	P6DDR = 0xb0;	// ~HWR,~WE	出力設定

//	P8DDR = 0xe4;	// CS2		出力設定
	P8DDR = 0xec;	// CS1,2	出力設定(CS1:RTL8019AS,CS2:DRAM)

	PBDDR = 0x80;	// PB7		出力設定
	PBDR  = 0x80;	// PB7		0:電源Off,1:電源On

	// BSC設定
	ABWCR = 0xff;	// CS0-CS7:8bitバス

//	ASTCR = 0xfb;	// CS0-CS7:3, CS2:2ステートアクセス
	ASTCR = 0xff;	// CS0-CS7:3ステートアクセス

	BCR   = 0xc6;	// 

	// DRAM
	// MSM5117800 : 2048回/32ms = 64KHz
//	RTCOR = 5;	//
	RTCOR = 3;	// 195.3125KHz/3 = 65.1KHz
//	RTMCSR= 0x37;	// 25MHz/φ2048 = 12.207KHz
	RTMCSR= 0x27;	// 25MHz/φ128 = 195.3125KHz
	DRCRB = 0x98;	// カラムアドレス10bit,PB4-5
	
//	DRCRA = 0x3c;	// CS2  :DRAM空間,高速ページモード,RASダウン
	DRCRA = 0x78;	// エリア2,3:CS2  :DRAM空間,高速ページモード
//	DRCRA = 0x38;	// エリア2,3:CS2,3:DRAM空間,高速ページモード
//	DRCRA = 0x30;	// CS2  :DRAM空間,
	
	for(i = 0; i<400000; i++);
}

#define P8DR	(*(volatile unsigned char *)0xffffd7)	//!< P8DR

/**
 * @brief	アプリケーションが存在するかチェックする
 * @return	0:アプリケーション無し !=0:アプリケーション有り
 */
int check_application(void)
{
	if(*(unsigned long *)APP_ADDRESS != 0xffffffffL) {
		if((P8DR & 0x10) == 0) {	/* P84の状態 0:
						 * 押しているボタンあり */
			return 0;
		} else {
			return 1;
		}
	} else {
		return 0;
	}
}
