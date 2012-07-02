/** @file
 * @brief	h8monコンフィグレーション
 *
 * @date	2002.03.26
 * @author	Takashi SHUDO
 */

/**
 * コマンド
 * commands.c
 */
#define USE_NET	// ネットワーク(TFTP)対応

/**
 * スタック
 * vect.c
 */
#define SIZEOF_MONSTACK		400	//!< モニタのスタックサイズ

/**
 * コマンドライン
 * lineedit.c
 */
#define	MAX_COM_HIS		8	//!< コマンドヒストリの数
#define	MAX_LINE_COLUMS		40	//!< 1行の最大文字数

/**
 * ブレークポイント数
 * task.c
 */
#define MAX_BREAKPOINT_NUM	8	//!< 設定可能な最大ブレークポイント数

/**
 * アプリケーション自動起動
 */
#define APP_ADDRESS	0xd000		//!< アプリケーションの先頭アドレス

/**
 * stubをSCI0で使う
 */
//#define STUB_IS_SCI0

/**
 * SCI1 割り込みを仮想ベクタで使用しない
 */
//#define FIX_SCI1_INTERRUPT
