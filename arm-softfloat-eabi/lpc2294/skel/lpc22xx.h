#ifndef __LPC22xx_H
#define __LPC22xx_H

#include "lpc21xx.h"

/* External Memory Controller (EMC) */
#define BCFG0          (*((volatile uint32_t *) 0xFFE00000))
#define BCFG1          (*((volatile uint32_t *) 0xFFE00004))
#define BCFG2          (*((volatile uint32_t *) 0xFFE00008))
#define BCFG3          (*((volatile uint32_t *) 0xFFE0000C))

#endif  // __LPC22xx_H
