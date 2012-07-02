;;; H8/300Hモニタユーザプログラム実行
;;; 
;;; 2002.03.27	Takashi SHUDO

	.h8300h
	.section .text
	
	.extern _finish_breakpoints
	.extern _mon
	.extern __mon_rcv
	.extern _task_reg
	.extern _break_reg
	.extern	_stack
	
	.global __exec
	.global __trapa_break
	.global __exec_mon
	.global __sci0_break
	.global __sci1_break
	.global	__trapa2

;;; ユーザプログラム実行
__exec:
	mov.l	@(_task_reg+28),er7	; SP

	mov.b	@(_task_reg+39),r0l
	mov.b	r0l,@-er7		; PC[3]
	mov.b	@(_task_reg+38),r0l
	mov.b	r0l,@-er7		; PC[2]
	mov.b	@(_task_reg+37),r0l
	mov.b	r0l,@-er7		; PC[1]
	mov.b	@(_task_reg+35),r0l
	mov.b	r0l,@-er7		; CCR

	mov.l	@_task_reg,er0
	mov.l	@(_task_reg+4),er1
	mov.l	@(_task_reg+8),er2
	mov.l	@(_task_reg+12),er3
	mov.l	@(_task_reg+16),er4
	mov.l	@(_task_reg+20),er5
	mov.l	@(_task_reg+24),er6
	
	rte
	
;;; TRAPAブレーク時
__trapa_break:
	mov.l	er0,@_task_reg
	mov.l	er1,@(_task_reg+4)
	mov.l	er2,@(_task_reg+8)
	mov.l	er3,@(_task_reg+12)
	mov.l	er4,@(_task_reg+16)
	mov.l	er5,@(_task_reg+20)
	mov.l	er6,@(_task_reg+24)

	mov.l	er7,er0
	add.l	#4,er0			; SP
	mov.l	er0,@(_task_reg+28)	; SP+=4

	mov.b	@er7+,r0l		; CCR
	mov.b	r0l,@(_task_reg+35)

	mov.l	#0,er0
	mov.b	@er7+,r0h		; PC[1]
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	mov.b	@er7+,r0h		; PC[2]
	mov.b	@er7+,r0l		; PC[3]
	sub.l	#2,er0			; PC-=2
	mov.l	er0,@(_task_reg+36) 

__exec_mon:		
	mov.l	#_stack,er7		; Set SP
	andc.b	#0xbf,ccr		; Enable Interrupt
	
	jsr	_finish_breakpoints	; Clear Break Point 
	jmp	_mon			; Start Monitor

;;; SCI1ブレーク時
__sci1_break:
	;; 割り込み時のレジスタを保存
	mov.l	er0,@_break_reg
	mov.l	er1,@(_break_reg+4)
	mov.l	er2,@(_break_reg+8)
	mov.l	er3,@(_break_reg+12)
	mov.l	er4,@(_break_reg+16)
	mov.l	er5,@(_break_reg+20)
	mov.l	er6,@(_break_reg+24)

	mov.l	er7,er0			; 割り込み込み前のSPは
	add.l	#4,er0			; SP+4
	mov.l	er0,@(_break_reg+28)	; 

	mov.l	#0,er0
	mov.b	@er7+,r0l		; CCR
	mov.b	r0l,@(_break_reg+35)	; 

	mov.l	#0,er0
	mov.b	@er7+,r0h		; PC[1]
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	mov.b	@er7+,r0h		; PC[2]
	mov.b	@er7+,r0l		; PC[3]
	mov.l	er0,@(_break_reg+36)

	jsr	__sci1_rcv

	;; レジスタを元に戻す
	mov.b	@(_break_reg+39),r0l
	mov.b	r0l,@-er7		; PC[3]
	mov.b	@(_break_reg+38),r0l
	mov.b	r0l,@-er7		; PC[2]
	mov.b	@(_break_reg+37),r0l
	mov.b	r0l,@-er7		; PC[1]
	mov.b	@(_break_reg+35),r0l
	mov.b	r0l,@-er7		; CCR

	mov.l	@_break_reg,er0
	mov.l	@(_break_reg+4),er1
	mov.l	@(_break_reg+8),er2
	mov.l	@(_break_reg+12),er3
	mov.l	@(_break_reg+16),er4
	mov.l	@(_break_reg+20),er5
	mov.l	@(_break_reg+24),er6
	
	rte

;;; SCI0ブレーク時
__sci0_break:
	;; 割り込み時のレジスタを保存
	mov.l	er0,@_break_reg
	mov.l	er1,@(_break_reg+4)
	mov.l	er2,@(_break_reg+8)
	mov.l	er3,@(_break_reg+12)
	mov.l	er4,@(_break_reg+16)
	mov.l	er5,@(_break_reg+20)
	mov.l	er6,@(_break_reg+24)

	mov.l	er7,er0			; 割り込み込み前のSPは
	add.l	#4,er0			; SP+4
	mov.l	er0,@(_break_reg+28)	; 

	mov.l	#0,er0
	mov.b	@er7+,r0l		; CCR
	mov.b	r0l,@(_break_reg+35)	; 

	mov.l	#0,er0
	mov.b	@er7+,r0h		; PC[1]
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	mov.b	@er7+,r0h		; PC[2]
	mov.b	@er7+,r0l		; PC[3]
	mov.l	er0,@(_break_reg+36)

	jsr	__sci0_rcv

	;; レジスタを元に戻す
	mov.b	@(_break_reg+39),r0l
	mov.b	r0l,@-er7		; PC[3]
	mov.b	@(_break_reg+38),r0l
	mov.b	r0l,@-er7		; PC[2]
	mov.b	@(_break_reg+37),r0l
	mov.b	r0l,@-er7		; PC[1]
	mov.b	@(_break_reg+35),r0l
	mov.b	r0l,@-er7		; CCR

	mov.l	@_break_reg,er0
	mov.l	@(_break_reg+4),er1
	mov.l	@(_break_reg+8),er2
	mov.l	@(_break_reg+12),er3
	mov.l	@(_break_reg+16),er4
	mov.l	@(_break_reg+20),er5
	mov.l	@(_break_reg+24),er6
	
	rte

__trapa2:
	;; ER2の設定値により以下の機能を実行
	;; ER2=0 : sci1_putci
	;; ER2=1 : sci1_getc
	;; ER2=2 : sci0_putc
	;; ER2=3 : sci0_getc
	;; ER2 は TRAPA2 実行前にスタック等に退避すること
	
	shll.l	er2
	shll.l	er2
	add.l	#moniotbl,er2
	mov.l	@er2,er2	; ER2=とび先
	jsr	@er2		; システムコール実行
	
	rte

moniotbl:
	.long	_sci1_putci
	.long	_sci1_getc
	.long	_sci0_putc
	.long	_sci0_getc
	
	.end
	