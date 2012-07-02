;;; H8/300Hモニタ仮想ベクタテーブル
;;; 
;;; 2002.03.27	Takashi SHUDO

	.h8300h
	.section .text
	
	.global	__int01
	.global	__int02
	.global	__int03
	.global	__int04
	.global	__int05
	.global	__int06
	.global	__int07
	.global	__int08
	.global	__int09
	.global	__int10
	.global	__int11
	.global	__int12
	.global	__int13
	.global	__int14
	.global	__int15
	.global	__int16
	.global	__int17
	.global	__int18
	.global	__int19
	.global	__int20
	.global	__int21
	.global	__int22
	.global	__int23
	.global	__int24
	.global	__int25
	.global	__int26
	.global	__int27
	.global	__int28
	.global	__int29
	.global	__int30
	.global	__int31
	.global	__int32
	.global	__int33
	.global	__int34
	.global	__int35
	.global	__int36
	.global	__int37
	.global	__int38
	.global	__int39
	.global	__int40
	.global	__int41
	.global	__int42
	.global	__int43
	.global	__int44
	.global	__int45
	.global	__int46
	.global	__int47
	.global	__int48
	.global	__int49
	.global	__int50
	.global	__int51
	.global	__int52
	.global	__int53
	.global	__int54
	.global	__int55
	.global	__int56
	.global	__int57
	.global	__int58
	.global	__int59
	.global	__int60
	.global	__int61
	.global	__int62
	.global	__int63

	.extern _task_reg
	.extern _vvect
	
; 各割りこみ処理
__int01:
	mov.l	er0,@_task_reg
	mov.l	# 1,er0
	jmp	vvectfunc

__int02:
	mov.l	er0,@_task_reg
	mov.l	# 2,er0
	jmp	vvectfunc

__int03:
	mov.l	er0,@_task_reg
	mov.l	# 3,er0
	jmp	vvectfunc

__int04:
	mov.l	er0,@_task_reg
	mov.l	# 4,er0
	jmp	vvectfunc

__int05:
	mov.l	er0,@_task_reg
	mov.l	# 5,er0
	jmp	vvectfunc

__int06:
	mov.l	er0,@_task_reg
	mov.l	# 6,er0
	jmp	vvectfunc

__int07:
	mov.l	er0,@_task_reg
	mov.l	# 7,er0
	jmp	vvectfunc

__int08:
	mov.l	er0,@_task_reg
	mov.l	# 8,er0
	jmp	vvectfunc

__int09:
	mov.l	er0,@_task_reg
	mov.l	# 9,er0
	jmp	vvectfunc

__int10:
	mov.l	er0,@_task_reg
	mov.l	#10,er0
	jmp	vvectfunc

__int11:
	mov.l	er0,@_task_reg
	mov.l	#11,er0
	jmp	vvectfunc

__int12:
	mov.l	er0,@_task_reg
	mov.l	#12,er0
	jmp	vvectfunc

__int13:
	mov.l	er0,@_task_reg
	mov.l	#13,er0
	jmp	vvectfunc

__int14:
	mov.l	er0,@_task_reg
	mov.l	#14,er0
	jmp	vvectfunc

__int15:
	mov.l	er0,@_task_reg
	mov.l	#15,er0
	jmp	vvectfunc

__int16:
	mov.l	er0,@_task_reg
	mov.l	#16,er0
	jmp	vvectfunc

__int17:
	mov.l	er0,@_task_reg
	mov.l	#17,er0
	jmp	vvectfunc

__int18:
	mov.l	er0,@_task_reg
	mov.l	#18,er0
	jmp	vvectfunc

__int19:
	mov.l	er0,@_task_reg
	mov.l	#19,er0
	jmp	vvectfunc

__int20:
	mov.l	er0,@_task_reg
	mov.l	#20,er0
	jmp	vvectfunc

__int21:
	mov.l	er0,@_task_reg
	mov.l	#21,er0
	jmp	vvectfunc

__int22:
	mov.l	er0,@_task_reg
	mov.l	#22,er0
	jmp	vvectfunc

__int23:
	mov.l	er0,@_task_reg
	mov.l	#23,er0
	jmp	vvectfunc

__int24:
	mov.l	er0,@_task_reg
	mov.l	#24,er0
	jmp	vvectfunc

__int25:
	mov.l	er0,@_task_reg
	mov.l	#25,er0
	jmp	vvectfunc

__int26:
	mov.l	er0,@_task_reg
	mov.l	#26,er0
	jmp	vvectfunc

__int27:
	mov.l	er0,@_task_reg
	mov.l	#27,er0
	jmp	vvectfunc

__int28:
	mov.l	er0,@_task_reg
	mov.l	#28,er0
	jmp	vvectfunc

__int29:
	mov.l	er0,@_task_reg
	mov.l	#29,er0
	jmp	vvectfunc

__int30:
	mov.l	er0,@_task_reg
	mov.l	#30,er0
	jmp	vvectfunc

__int31:
	mov.l	er0,@_task_reg
	mov.l	#31,er0
	jmp	vvectfunc

__int32:
	mov.l	er0,@_task_reg
	mov.l	#32,er0
	jmp	vvectfunc

__int33:
	mov.l	er0,@_task_reg
	mov.l	#33,er0
	jmp	vvectfunc

__int34:
	mov.l	er0,@_task_reg
	mov.l	#34,er0
	jmp	vvectfunc

__int35:
	mov.l	er0,@_task_reg
	mov.l	#35,er0
	jmp	vvectfunc

__int36:
	mov.l	er0,@_task_reg
	mov.l	#36,er0
	jmp	vvectfunc

__int37:
	mov.l	er0,@_task_reg
	mov.l	#37,er0
	jmp	vvectfunc

__int38:
	mov.l	er0,@_task_reg
	mov.l	#38,er0
	jmp	vvectfunc

__int39:
	mov.l	er0,@_task_reg
	mov.l	#39,er0
	jmp	vvectfunc

__int40:
	mov.l	er0,@_task_reg
	mov.l	#40,er0
	jmp	vvectfunc

__int41:
	mov.l	er0,@_task_reg
	mov.l	#41,er0
	jmp	vvectfunc

__int42:
	mov.l	er0,@_task_reg
	mov.l	#42,er0
	jmp	vvectfunc

__int43:
	mov.l	er0,@_task_reg
	mov.l	#43,er0
	jmp	vvectfunc

__int44:
	mov.l	er0,@_task_reg
	mov.l	#44,er0
	jmp	vvectfunc

__int45:
	mov.l	er0,@_task_reg
	mov.l	#45,er0
	jmp	vvectfunc

__int46:
	mov.l	er0,@_task_reg
	mov.l	#46,er0
	jmp	vvectfunc

__int47:
	mov.l	er0,@_task_reg
	mov.l	#47,er0
	jmp	vvectfunc

__int48:
	mov.l	er0,@_task_reg
	mov.l	#48,er0
	jmp	vvectfunc

__int49:
	mov.l	er0,@_task_reg
	mov.l	#49,er0
	jmp	vvectfunc

__int50:
	mov.l	er0,@_task_reg
	mov.l	#50,er0
	jmp	vvectfunc

__int51:
	mov.l	er0,@_task_reg
	mov.l	#51,er0
	jmp	vvectfunc

__int52:
	mov.l	er0,@_task_reg
	mov.l	#52,er0
	jmp	vvectfunc

__int53:
	mov.l	er0,@_task_reg
	mov.l	#53,er0
	jmp	vvectfunc

__int54:
	mov.l	er0,@_task_reg
	mov.l	#54,er0
	jmp	vvectfunc

__int55:
	mov.l	er0,@_task_reg
	mov.l	#55,er0
	jmp	vvectfunc

__int56:
	mov.l	er0,@_task_reg
	mov.l	#56,er0
	jmp	vvectfunc

__int57:
	mov.l	er0,@_task_reg
	mov.l	#57,er0
	jmp	vvectfunc

__int58:
	mov.l	er0,@_task_reg
	mov.l	#58,er0
	jmp	vvectfunc

__int59:
	mov.l	er0,@_task_reg
	mov.l	#59,er0
	jmp	vvectfunc

__int60:
	mov.l	er0,@_task_reg
	mov.l	#60,er0
	jmp	vvectfunc

__int61:
	mov.l	er0,@_task_reg
	mov.l	#61,er0
	jmp	vvectfunc

__int62:
	mov.l	er0,@_task_reg
	mov.l	#62,er0
	jmp	vvectfunc

__int63:
	mov.l	er0,@_task_reg
	mov.l	#63,er0
	jmp	vvectfunc

; 仮想ベクタ判定
vvectfunc:
	shll.l	er0			; ベクタ番号からテーブルを求める
	shll.l	er0	
	add.l	#_vvect,er0		
	mov.l	@er0,er0		; ER0=とび先

	mov.l	er0,@-sp		; スタックにジャンプ先 
	mov.l	@_task_reg,er0		; ER0を戻す

	rts				; ユーザ割りこみ処理へ
	
	.end
	