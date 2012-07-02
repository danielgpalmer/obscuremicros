;;; H8/300Hモニタユーザプログラム実行
;;; 
;;; 2002.03.27 Takashi SHUDO

	.h8300h
	.section .text
	
	.extern _FinishBreakpoints
	.extern _mon
	
	.global __exec
	.global __trapa_break
	.global _taskReg
	.global	_stack
	
;;; ユーザプログラム実行
__exec:
	mov.l	@(_taskReg+28),sp	; SP

	mov.b	@(_taskReg+39),r0l
	mov.b	r0l,@-sp		; PC[3]
	mov.b	@(_taskReg+38),r0l
	mov.b	r0l,@-sp		; PC[2]
	mov.b	@(_taskReg+37),r0l
	mov.b	r0l,@-sp		; PC[1]
	mov.b	@(_taskReg+35),r0l
	mov.b	r0l,@-sp		; CCR

	mov.l	@_taskReg,er0
	mov.l	@(_taskReg+4),er1
	mov.l	@(_taskReg+8),er2
	mov.l	@(_taskReg+12),er3
	mov.l	@(_taskReg+16),er4
	mov.l	@(_taskReg+20),er5
	mov.l	@(_taskReg+24),er6
	
	rte
	
;;; ブレーク時
__trapa_break:
	mov.l	er0,@_taskReg
	mov.l	er1,@(_taskReg+4)
	mov.l	er2,@(_taskReg+8)
	mov.l	er3,@(_taskReg+12)
	mov.l	er4,@(_taskReg+16)
	mov.l	er5,@(_taskReg+20)
	mov.l	er6,@(_taskReg+24)

	mov.l	sp,er0
	add.l	#4,er0			; SP
	mov.l	er0,@(_taskReg+28)	; SP+=4

	mov.b	@sp+,r0l		; CCR
	mov.b	r0l,@(_taskReg+35)

	mov.b	@sp+,r0h		; PC[1]
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	shll.l	er0
	mov.b	@sp+,r0h		; PC[2]
	mov.b	@sp+,r0l		; PC[3]
	sub.l	#2,er0			; PC-=2
	mov.l	er0,@(_taskReg+36) 

	mov.l	#_stack,er7		; Set SP
	andc.b	#0x3f,ccr		; Enable Interrupt
	
	jsr	_FinishBreakpoints	; Clear Break Point 
	jmp	_mon			; Start Monitor

	.end
	