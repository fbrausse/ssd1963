
#ifndef SSD_GPIO_RPI_H
#define SSD_GPIO_RPI_H

#include "gpio.h"

/*
// #define MB()	__asm__ __volatile__("dmb st")
// #define MB()	__sync_synchronize()

#ifndef MB
#define MB()
#endif
*/

#if 1
/* free: 4, 14, 15, 17, 18, 28-31 */

/* 7, 8, 9, 10, 11, 23, 24, 25 */
#define GPIO_MASK	(0x1f << 7 | 0x07 << 23)

#define GPIO_WR8(i) \
	(((uint32_t)(i) & 0x1f) << 7 | ((uint32_t)(i) & 0xe0) << (23 - 5))

/* #RD:  4 */
#define LCD_RD_MASK	(1 <<  4)
/* #WR: 22 */
#define LCD_WR_MASK	(1 << 22)
/* D/#C: 27 */
#define LCD_DC_MASK	(1 << 27)

#else

/* free: 2,3,7,14,15 */

/* 22, 23, 24, 27, 28, 29, 30, 31 */
#define GPIO_WR8(i) \
	(((uint32_t)(i) & 0x07) << 22 | ((uint32_t)(i) & 0xf8) << (27 - 3))
#define GPIO_MASK	GPIO_WR8(0xff)

#define LCD_RD_MASK	(1 <<  4)	/* #RD :  4 */
#define LCD_WR_MASK	(1 << 17)	/* #WR : 17 */
#define LCD_DC_MASK	(1 << 18)	/* D/#C: 18 */

#endif

#define LCD_D8(v)	GPIO_WR8(v)



#define CPU_MHZ		700
#define CPU_CYCLES(ns)	((CPU_MHZ * (ns) + 999) / 1000)

#define T_ACC	32
#define T_DSW	4
#define T_PWLW	12
#define T_PWLR	36

#define MAX(a,b)	((a) > (b) ? (a) : (b))


void lcd_wr8_slow(unsigned char c);
void lcd_cmd_slow(unsigned char c);

static inline void lcd_wr8_fast(unsigned char c)
{
	uint32_t d = LCD_D8(c);
	
	GPIO_CLR = (~d & GPIO_MASK) | LCD_WR_MASK;
	GPIO_SET =   d & GPIO_MASK;
	// nop(MAX(CPU_CYCLES(T_PWLW) - 2, CPU_CYCLES(T_DSW) - 1));
	GPIO_SET = LCD_WR_MASK;
}

static inline void lcd_cmd_fast(unsigned char c)
{
	/* commands aren't frequent, so use the slow access */
	lcd_cmd_slow(c);
}

void ssd_gpio_rpi_init(void);
void ssd_gpio_rpi_fini(void);

#endif
