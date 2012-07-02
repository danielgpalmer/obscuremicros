/** @file
 * @break	TFTP
 *
 * @date	2011.04.09
 * @author	Takashi SHUDO
 */

#ifndef _H_tftp
#define _H_tftp

// ポート番号
#define TFTP_PORT	69

// オペコード
#define TFTP_OPC_READREQ	1
#define TFTP_OPC_WRITEREQ	2
#define TFTP_OPC_DATA		3
#define TFTP_OPC_ACK		4
#define TFTP_OPC_ERROR		5

// モード
#define TFTP_MODE_ASCII	"netascii"
#define TFTP_MODE_BIN	"octet"

#define TFTP_BLK_SIZE	512

#endif // _H_tftp
