	.h8300h
	.section .text
	.global	_start
	.extern _iram_data
	.extern _iram_image_start
	.extern _iram_image_end
	.extern _iram_bss_start
        .extern _iram_bss_end
	.extern _bss_start
	.extern _bss_end

	
_start:
	nop
	mov.l	#__stack,er7	; Set SP

	;;push er0,er1 on the stack.. why is this important?
	mov.l	er0,@-er7
	mov.l	er1,@-er7

        ;; relocate internal ram sections 
        mov.l   #_iram_image_start,er0
        mov.l   #_iram_image_end,er1
        mov.l   #_iram_data,er2
        bra     loop11

;; move iram sections from dram in sram
loop1:
        mov.l	@er2,er3
	mov.l   er3,@er0
	add	#4,er2
        add     #4,er0
loop11:
        cmp.l   er0,er1
        bne      loop1
	nop

;; .bss section initialize
	mov.l	#_bss_start,er0
	mov.l	#_bss_end,er1
	mov.l	#0,er2
	bra	loop21
loop2:
	mov.l	er2,@er0
	add	#4,er0
loop21:
	cmp.l	er0,er1
	bne	loop2

	;; pop er1, er0
	mov.l	@er7+,er1
	mov.l	@er7+,er0

;; iram bss section initialize
        mov.l   #_iram_bss_start,er0
        mov.l   #_iram_bss_end,er1
        mov.l   #0,er2
        bra     loop31
loop3:
        mov.l   er2,@er0
        add     #4,er0
loop31:
        cmp.l   er0,er1
        bne     loop3

        ;; pop er1, er0
        mov.l   @er7+,er1
        mov.l   @er7+,er0


	
	jsr	_main		; Goto main()



;	rts
	trapa #3


	.end
