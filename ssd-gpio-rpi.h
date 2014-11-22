
#ifndef SSD_GPIO_RPI_H
#define SSD_GPIO_RPI_H

#include "gpio.h"

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

#define LCD_D8(v)	GPIO_WR8(v)

void lcd_wr8_slow(unsigned char c);
void lcd_cmd_slow(unsigned char c);

static inline void lcd_wr8_fast(unsigned char c)
{
	uint32_t d = LCD_D8(c);

	GPIO_CLR = (~d & GPIO_MASK) | LCD_WR_MASK;
	GPIO_SET =   d & GPIO_MASK;
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
