#ifndef _IO3069_H
#define _IO3069_H

#define P1DDR (*(volatile unsigned char *) 0xFEE000) // Port 1 data direction register
#define P2DDR (*(volatile unsigned char *) 0xFEE001) // Port 2 data direction register
#define P3DDR (*(volatile unsigned char *) 0xFEE002) // Port 3 data direction register
#define P4DDR (*(volatile unsigned char *) 0xFEE003) // Port 4 data direction register
#define P5DDR (*(volatile unsigned char *) 0xFEE004) // Port 5 data direction register
#define P6DDR (*(volatile unsigned char *) 0xFEE005) // Port 6 data direction register
#define P8DDR (*(volatile unsigned char *) 0xFEE007) // Port 8 data direction register
#define P9DDR (*(volatile unsigned char *) 0xFEE008) // Port 9 data direction register
#define PADDR (*(volatile unsigned char *) 0xFEE009) // Port A data direction register
#define PBDDR (*(volatile unsigned char *) 0xFEE00A) // Port B data direction register
#define MDCR (*(volatile unsigned char *) 0xFEE011) // System mode control register
#define SYSCR (*(volatile unsigned char *) 0xFEE012) // System control register
#define BRCR (*(volatile unsigned char *) 0xFEE013) // BUS controller bus release control register
#define ISCR (*(volatile unsigned char *) 0xFEE014) // Interrupt controller IRQ sense control register
#define IER (*(volatile unsigned char *) 0xFEE015) // Interrupt controller IRQ enable register
#define ISR (*(volatile unsigned char *) 0xFEE016) // Interrupt controller IRQ status register
#define IPRA (*(volatile unsigned char *) 0xFEE018) // Interrupt controller interrupt priority register A
#define IPRB (*(volatile unsigned char *) 0xFEE019) // Interrupt controller interrupt priority register B
#define DASTCR (*(volatile unsigned char *) 0xFEE01A) // DAC standby control register
#define DIVCR (*(volatile unsigned char *) 0xFEE01B) // System division control register
#define MSTCRH (*(volatile unsigned char *) 0xFEE01C) // Module standby control register H
#define MSTCRL (*(volatile unsigned char *) 0xFEE01D) // Module standby control register L
#define ADRCR (*(volatile unsigned char *) 0xFEE01E) // BUS controller address control register
#define CSCR (*(volatile unsigned char *) 0xFEE01F) // BUS controller chip select control register
#define ABWCR (*(volatile unsigned char *) 0xFEE020) // BUS controller bus width control register
#define ASTCR (*(volatile unsigned char *) 0xFEE021) // BUS controller access state control register
#define WCRH (*(volatile unsigned char *) 0xFEE022) // BUS controller wait control register H
#define WCRL (*(volatile unsigned char *) 0xFEE023) // BUS controller wait control register L
#define BCR (*(volatile unsigned char *) 0xFEE024) // BUS controller control register
#define DRCRA (*(volatile unsigned char *) 0xFEE026) // DRAM Interface control register A
#define DRCRB (*(volatile unsigned char *) 0xFEE027) // DRAM Interface control register B
#define RTMCSR (*(volatile unsigned char *) 0xFEE028) // DRAM Interface refresh timer control/status register
#define RTCNT (*(volatile unsigned char *) 0xFEE029) // DRAM Interface refresh timer counter register
#define RTCOR (*(volatile unsigned char *) 0xFEE02A) // DRAM Interface refresh time constant register
#define P2PCR (*(volatile unsigned char *) 0xFEE03C) // Port 2 input pull-up MOS control register
#define P4PCR (*(volatile unsigned char *) 0xFEE03E) // Port 4 input pull-up MOS control register
#define P5PCR (*(volatile unsigned char *) 0xFEE03F) // Port 5 input pull-up MOS control register
#define RAMCR (*(volatile unsigned char *) 0xFEE077) // Flash memory RAM control register
#define FCCS (*(volatile unsigned char *) 0xFEE0B0) // Flash control status register
#define FPCS (*(volatile unsigned char *) 0xFEE0B1) // Flash program code select register
#define FECS (*(volatile unsigned char *) 0xFEE0B2) // Flash erase code select register
#define FKEY (*(volatile unsigned char *) 0xFEE0B4) // Flash key code register
#define FMATS (*(volatile unsigned char *) 0xFEE0B5) // Flash MAT select register
#define FVACR (*(volatile unsigned char *) 0xFEE0B7) // Flash vector address code control register
#define FVADRR (*(volatile unsigned char *) 0xFEE0B8) // Flash vector address data register R
#define FVADRE (*(volatile unsigned char *) 0xFEE0B9) // Flash vector address data register E
#define FVADRH (*(volatile unsigned char *) 0xFEE0BA) // Flash vector address data register H
#define FVADRL (*(volatile unsigned char *) 0xFEE0BB) // Flash vector address data register L
#define MAR0AR (*(volatile unsigned char *) 0xFFFF20) // DMAC memory address register AR channel 0A
#define MAR0AE (*(volatile unsigned char *) 0xFFFF21) // DMAC memory address register AE channel 0A
#define MAR0AH (*(volatile unsigned char *) 0xFFFF22) // DMAC memory address register AH channel 0A
#define MAR0AL (*(volatile unsigned char *) 0xFFFF23) // DMAC memory address register AL channel 0A
#define ETCR0AH (*(volatile unsigned char *) 0xFFFF24) // DMAC execute transfer count register AH channel 0A
#define ETCR0AL (*(volatile unsigned char *) 0xFFFF25) // DMAC execute transfer count register AL channel 0A
#define IOAR0A (*(volatile unsigned char *) 0xFFFF26) // DMAC I/O address register channel 0A
#define DTCR0A (*(volatile unsigned char *) 0xFFFF27) // DMAC data transfer control register channel 0A
#define MAR0BR (*(volatile unsigned char *) 0xFFFF28) // DMAC memory address register BR channel 0B
#define MAR0BE (*(volatile unsigned char *) 0xFFFF29) // DMAC memory address register BE channel 0B
#define MAR0BH (*(volatile unsigned char *) 0xFFFF2A) // DMAC memory address register BH channel 0B
#define MAR0BL (*(volatile unsigned char *) 0xFFFF2B) // DMAC memory address register BL channel 0B
#define ETCR0BH (*(volatile unsigned char *) 0xFFFF2C) // DMAC execute transfer count register BH channel 0B
#define ETCR0BL (*(volatile unsigned char *) 0xFFFF2D) // DMAC execute transfer count register BL channel 0B
#define IOAR0B (*(volatile unsigned char *) 0xFFFF2E) // DMAC I/O address register channel 0B
#define DTCR0B (*(volatile unsigned char *) 0xFFFF2F) // DMAC data transfer control register channel 0B
#define MAR1AR (*(volatile unsigned char *) 0xFFFF30) // DMAC memory address register AR channel 1A
#define MAR1AE (*(volatile unsigned char *) 0xFFFF31) // DMAC memory address register AE channel 1A
#define MAR1AH (*(volatile unsigned char *) 0xFFFF32) // DMAC memory address register AH channel 1A
#define MAR1AL (*(volatile unsigned char *) 0xFFFF33) // DMAC memory address register AL channel 1A
#define ETCR1AH (*(volatile unsigned char *) 0xFFFF34) // DMAC execute transfer count register AH channel 1A
#define ETCR1AL (*(volatile unsigned char *) 0xFFFF35) // DMAC execute transfer count register AL channel 1A
#define IOAR1A (*(volatile unsigned char *) 0xFFFF36) // DMAC I/O address register channel 1A
#define DTCR1A (*(volatile unsigned char *) 0xFFFF37) // DMAC data transfer control register channel 1A
#define MAR1BR (*(volatile unsigned char *) 0xFFFF38) // DMAC memory address register BR channel 1B
#define MAR1BE (*(volatile unsigned char *) 0xFFFF39) // DMAC memory address register BE channel 1B
#define MAR1BH (*(volatile unsigned char *) 0xFFFF3A) // DMAC memory address register BH channel 1B
#define MAR1BL (*(volatile unsigned char *) 0xFFFF3B) // DMAC memory address register BL channel 1B
#define ETCR1BH (*(volatile unsigned char *) 0xFFFF3C) // DMAC execute transfer count register BH channel 1B
#define ETCR1BL (*(volatile unsigned char *) 0xFFFF3D) // DMAC execute transfer count register BL channel 1B
#define IOAR1B (*(volatile unsigned char *) 0xFFFF3E) // DMAC I/O address register channel 1B
#define DTCR1B (*(volatile unsigned char *) 0xFFFF3F) // DMAC data transfer control register channel 1B
#define TSTR (*(volatile unsigned char *) 0xFFFF60) // 16-bit timer all channels start register
#define TSNC (*(volatile unsigned char *) 0xFFFF61) // 16-bit timer all channels synchro register
#define TMDR (*(volatile unsigned char *) 0xFFFF62) // 16-bit timer all channels mode register
#define TOLR (*(volatile unsigned char *) 0xFFFF63) // 16-bit timer all channels output level setting register
#define TISRA (*(volatile unsigned char *) 0xFFFF64) // 16-bit timer all channels interrupt status register A
#define TISRB (*(volatile unsigned char *) 0xFFFF65) // 16-bit timer all channels interrupt status register B
#define TISRC (*(volatile unsigned char *) 0xFFFF66) // 16-bit timer all channels interrupt status register C
#define TCR0_16 (*(volatile unsigned char *) 0xFFFF68) // 16-bit timer channel 0 timer control register
#define TIOR0 (*(volatile unsigned char *) 0xFFFF69) // 16-bit timer channel 0 timer I/O control register
#define TCNT0_16 (*(volatile unsigned int *) 0xFFFF6A) // 16-bit timer channel 0 timer counter register
#define TCNT0_16H (*(volatile unsigned char *) 0xFFFF6A) // 16-bit timer channel 0 timer counter register[high]
#define TCNT0_16L (*(volatile unsigned char *) 0xFFFF6B) // 16-bit timer channel 0 timer counter register[low]
#define GRA0 (*(volatile unsigned int *) 0xFFFF6C) // 16-bit timer channel 0 general register A
#define GRA0H (*(volatile unsigned char *) 0xFFFF6C) // 16-bit timer channel 0 general register A[high]
#define GRA0L (*(volatile unsigned char *) 0xFFFF6D) // 16-bit timer channel 0 general register A[low]
#define GRB0 (*(volatile unsigned int *) 0xFFFF6E) // 16-bit timer channel 0 general register B
#define GRB0H (*(volatile unsigned char *) 0xFFFF6E) // 16-bit timer channel 0 general register B[high]
#define GRB0L (*(volatile unsigned char *) 0xFFFF6F) // 16-bit timer channel 0 general register B[low]
#define TCR1_16 (*(volatile unsigned char *) 0xFFFF70) // 16-bit timer channel 1 timer control register
#define TIOR1 (*(volatile unsigned char *) 0xFFFF71) // 16-bit timer channel 1 timer I/O control register
#define TCNT1_16 (*(volatile unsigned int *) 0xFFFF72) // 16-bit timer channel 1 timer counter register
#define TCNT1_16H (*(volatile unsigned char *) 0xFFFF72) // 16-bit timer channel 1 timer counter register[high]
#define TCNT1_16L (*(volatile unsigned char *) 0xFFFF73) // 16-bit timer channel 1 timer counter register[low]
#define GRA1 (*(volatile unsigned int *) 0xFFFF74) // 16-bit timer channel 1 general register A
#define GRA1H (*(volatile unsigned char *) 0xFFFF74) // 16-bit timer channel 1 general register A[high]
#define GRA1L (*(volatile unsigned char *) 0xFFFF75) // 16-bit timer channel 1 general register A[low]
#define GRB1 (*(volatile unsigned int *) 0xFFFF76) // 16-bit timer channel 1 general register B
#define GRB1H (*(volatile unsigned char *) 0xFFFF76) // 16-bit timer channel 1 general register B[high]
#define GRB1L (*(volatile unsigned char *) 0xFFFF77) // 16-bit timer channel 1 general register B[low]
#define TCR2_16 (*(volatile unsigned char *) 0xFFFF78) // 16-bit timer channel 2 timer control register
#define TIOR2 (*(volatile unsigned char *) 0xFFFF79) // 16-bit timer channel 2 timer I/O control register
#define TCNT2_16 (*(volatile unsigned int *) 0xFFFF7A) // 16-bit timer channel 2 timer counter register
#define TCNT2_16H (*(volatile unsigned char *) 0xFFFF7A) // 16-bit timer channel 2 timer counter register[high]
#define TCNT2_16L (*(volatile unsigned char *) 0xFFFF7B) // 16-bit timer channel 2 timer counter register[low]
#define GRA2 (*(volatile unsigned int *) 0xFFFF7C) // 16-bit timer channel 2 general register A
#define GRA2H (*(volatile unsigned char *) 0xFFFF7C) // 16-bit timer channel 2 general register A[high]
#define GRA2L (*(volatile unsigned char *) 0xFFFF7D) // 16-bit timer channel 2 general register A[low]
#define GRB2 (*(volatile unsigned int *) 0xFFFF7E) // 16-bit timer channel 2 general register B
#define GRB2H (*(volatile unsigned char *) 0xFFFF7E) // 16-bit timer channel 2 general register B[high]
#define GRB2L (*(volatile unsigned char *) 0xFFFF7F) // 16-bit timer channel 2 general register B[low]
#define TCR0_8 (*(volatile unsigned char *) 0xFFFF80) // 8-bit timer channel 0 control register
#define TCR1_8 (*(volatile unsigned char *) 0xFFFF81) // 8-bit timer channel 1 control register
#define TCSR0 (*(volatile unsigned char *) 0xFFFF82) // 8-bit timer channel 0 control/status register
#define TCSR1 (*(volatile unsigned char *) 0xFFFF83) // 8-bit timer channel 1 control/status register
#define TCORA0 (*(volatile unsigned char *) 0xFFFF84) // 8-bit timer channel 0 time constant register A
#define TCORA1 (*(volatile unsigned char *) 0xFFFF85) // 8-bit timer channel 1 time constant register A
#define TCORB0 (*(volatile unsigned char *) 0xFFFF86) // 8-bit timer channel 0 time constant register B
#define TCORB1 (*(volatile unsigned char *) 0xFFFF87) // 8-bit timer channel 1 time constant register B
#define TCNT0_8 (*(volatile unsigned char *) 0xFFFF88) // 8-bit timer channel 0 timer counter register
#define TCNT1_8 (*(volatile unsigned char *) 0xFFFF89) // 8-bit timer channel 1 timer counter register
#define WDT (*(volatile unsigned int *) 0xFFFF8C) // Watchdog Timer control/status register + counter
#define TCSRR (*(volatile unsigned char *) 0xFFFF8C) // Watchdog Timer control/status register + counter[high]
#define WDTH (*(volatile unsigned char *) 0xFFFF8C) // Watchdog Timer control/status register + counter[high]
#define TCNTR (*(volatile unsigned char *) 0xFFFF8D) // Watchdog Timer control/status register + counter[low]
#define WDTL (*(volatile unsigned char *) 0xFFFF8D) // Watchdog Timer control/status register + counter[low]
#define RSTCSR (*(volatile unsigned int *) 0xFFFF8E) // Watchdog Timer reset control/status register
#define RSTCSRH (*(volatile unsigned char *) 0xFFFF8E) // Watchdog Timer reset control/status register[high]
#define RSTCSRL (*(volatile unsigned char *) 0xFFFF8F) // Watchdog Timer reset control/status register[low]
#define TCR2_8 (*(volatile unsigned char *) 0xFFFF90) // 8-bit timer channel 2 control register
#define TCR3_8 (*(volatile unsigned char *) 0xFFFF91) // 8-bit timer channel 3 control register
#define TCSR2 (*(volatile unsigned char *) 0xFFFF92) // 8-bit timer channel 2 control/status register
#define TCSR3 (*(volatile unsigned char *) 0xFFFF93) // 8-bit timer channel 3 control/status register
#define TCORA2 (*(volatile unsigned char *) 0xFFFF94) // 8-bit timer channel 2 time constant register A
#define TCORA3 (*(volatile unsigned char *) 0xFFFF95) // 8-bit timer channel 3 time constant register A
#define TCORB2 (*(volatile unsigned char *) 0xFFFF96) // 8-bit timer channel 2 time constant register B
#define TCORB3 (*(volatile unsigned char *) 0xFFFF97) // 8-bit timer channel 3 time constant register B
#define TCNT2_8 (*(volatile unsigned char *) 0xFFFF98) // 8-bit timer channel 2 timer counter register
#define TCNT3_8 (*(volatile unsigned char *) 0xFFFF99) // 8-bit timer channel 3 timer counter register
#define DADR0 (*(volatile unsigned char *) 0xFFFF9C) // DAC data register 0
#define DADR1 (*(volatile unsigned char *) 0xFFFF9D) // DAC data register 1
#define DACR (*(volatile unsigned char *) 0xFFFF9E) // DAC control register
#define TPMR (*(volatile unsigned char *) 0xFFFFA0) // TPC output mode register
#define TPCR (*(volatile unsigned char *) 0xFFFFA1) // TPC output control register
#define NDERB (*(volatile unsigned char *) 0xFFFFA2) // TPC next data enable register B
#define NDERA (*(volatile unsigned char *) 0xFFFFA3) // TPC next data enable register B
#define NDRB1 (*(volatile unsigned char *) 0xFFFFA4) // TPC next data register B
#define NDRA1 (*(volatile unsigned char *) 0xFFFFA5) // TPC next data register A
#define NDRB2 (*(volatile unsigned char *) 0xFFFFA6) // TPC next data register B
#define NDRA2 (*(volatile unsigned char *) 0xFFFFA7) // TPC next data register A
#define SMR0 (*(volatile unsigned char *) 0xFFFFB0) // SCI serial mode register channel 0
#define BRR0 (*(volatile unsigned char *) 0xFFFFB1) // SCI bit rate register channel 0
#define SCR0 (*(volatile unsigned char *) 0xFFFFB2) // SCI serial control register channel 0
#define TDR0 (*(volatile unsigned char *) 0xFFFFB3) // SCI transmit data register channel 0
#define SSR0 (*(volatile unsigned char *) 0xFFFFB4) // SCI serial status register channel 0
#define RDR0 (*(volatile unsigned char *) 0xFFFFB5) // SCI receive data register channel 0
#define SCMR0 (*(volatile unsigned char *) 0xFFFFB6) // SCI smart card mode register channel 0
#define SMR1 (*(volatile unsigned char *) 0xFFFFB8) // SCI serial mode register channel 1
#define BRR1 (*(volatile unsigned char *) 0xFFFFB9) // SCI bit rate register channel 1
#define SCR1 (*(volatile unsigned char *) 0xFFFFBA) // SCI serial control register channel 1
#define TDR1 (*(volatile unsigned char *) 0xFFFFBB) // SCI transmit data register channel 1
#define SSR1 (*(volatile unsigned char *) 0xFFFFBC) // SCI serial status register channel 1
#define RDR1 (*(volatile unsigned char *) 0xFFFFBD) // SCI receive data register channel 1
#define SCMR1 (*(volatile unsigned char *) 0xFFFFBE) // SCI smart card mode register channel 1
#define SMR2 (*(volatile unsigned char *) 0xFFFFC0) // SCI serial mode register channel 2
#define BRR2 (*(volatile unsigned char *) 0xFFFFC1) // SCI bit rate register channel 2
#define SCR2 (*(volatile unsigned char *) 0xFFFFC2) // SCI serial control register channel 2
#define TDR2 (*(volatile unsigned char *) 0xFFFFC3) // SCI transmit data register channel 2
#define SSR2 (*(volatile unsigned char *) 0xFFFFC4) // SCI serial status register channel 2
#define RDR2 (*(volatile unsigned char *) 0xFFFFC5) // SCI receive data register channel 2
#define SCMR2 (*(volatile unsigned char *) 0xFFFFC6) // SCI smart card mode register channel 2
#define P1DR (*(volatile unsigned char *) 0xFFFFD0) // Port 1 data register
#define P2DR (*(volatile unsigned char *) 0xFFFFD1) // Port 2 data register
#define P3DR (*(volatile unsigned char *) 0xFFFFD2) // Port 3 data register
#define P4DR (*(volatile unsigned char *) 0xFFFFD3) // Port 4 data register
#define P5DR (*(volatile unsigned char *) 0xFFFFD4) // Port 5 data register
#define P6DR (*(volatile unsigned char *) 0xFFFFD5) // Port 6 data register
#define P7DR (*(volatile unsigned char *) 0xFFFFD6) // Port 7 data register
#define P8DR (*(volatile unsigned char *) 0xFFFFD7) // Port 8 data register
#define P9DR (*(volatile unsigned char *) 0xFFFFD8) // Port 9 data register
#define PADR (*(volatile unsigned char *) 0xFFFFD9) // Port A data register
#define PBDR (*(volatile unsigned char *) 0xFFFFDA) // Port B data register
#define ADDRAH (*(volatile unsigned char *) 0xFFFFE0) // ADC data register AH
#define ADDRAL (*(volatile unsigned char *) 0xFFFFE1) // ADC data register AL
#define ADDRBH (*(volatile unsigned char *) 0xFFFFE2) // ADC data register BH
#define ADDRBL (*(volatile unsigned char *) 0xFFFFE3) // ADC data register BL
#define ADDRCH (*(volatile unsigned char *) 0xFFFFE4) // ADC data register CH
#define ADDRCL (*(volatile unsigned char *) 0xFFFFE5) // ADC data register CL
#define ADDRDH (*(volatile unsigned char *) 0xFFFFE6) // ADC data register DH
#define ADDRDL (*(volatile unsigned char *) 0xFFFFE7) // ADC data register DL
#define ADCSR (*(volatile unsigned char *) 0xFFFFE8) // ADC control/status register
#define ADCR (*(volatile unsigned char *) 0xFFFFE9) // ADC control register

#endif

