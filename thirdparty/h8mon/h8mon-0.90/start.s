;
;	H8/306xF Initialize stack pointer & boot main()
;

	.h8300h
	.section .text

	.extern __init
	.extern _init_sect
	.extern _main
	.extern _end
	.extern	_stack

	.global	_start

_start:
	# ��˥��Υ����å���0x55�����Ƥ���(�ǥХå���)
	mov.l	#_end,er0
	mov.b	#0x55,r1l

loop:
	cmp.l	#_stack,er0
	beq	init_sp
	mov.b	r1l,@er0
	add.l	#1,er0
	bra	loop

init_sp:
	mov.l	#_stack,er7		; ��˥���SP

	jsr	__init			; �桼�����˥���饤��
	
	jsr	_init_sect		; �������������

	jmp	_main			; Goto main()

	.end
