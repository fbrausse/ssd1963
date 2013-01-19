
#include "ssd-gpio-rpi.h"

#define WAIT1	nop(30)
#define WAIT2	nop(60)

void lcd_wr8_slow(unsigned char c)
{
	GPIO_CLR = LCD_WR_MASK;
	WAIT1;
	GPIO_CLR = ~LCD_D8(c) & GPIO_MASK;
	WAIT1;
	GPIO_SET = LCD_D8(c) & GPIO_MASK;
	WAIT2;
	GPIO_SET = LCD_WR_MASK;
	WAIT2;
}

void lcd_cmd_slow(unsigned char c)
{
	GPIO_CLR = LCD_DC_MASK;
	WAIT1;
	GPIO_CLR = LCD_WR_MASK;
	WAIT1;
	GPIO_CLR = (~LCD_D8(c) & GPIO_MASK);
	GPIO_SET = (LCD_D8(c) & GPIO_MASK);
	WAIT1;
	GPIO_SET = LCD_WR_MASK;
	WAIT1;
	GPIO_SET = LCD_DC_MASK;
	WAIT2;
}

void ssd_gpio_rpi_init(void)
{
	gpio_pull(GPIO_PULL_OFF, GPIO_MASK | LCD_WR_MASK | LCD_DC_MASK | LCD_RD_MASK);
	GPIO_CLR = GPIO_MASK;
	WAIT1;
	GPIO_SET = LCD_WR_MASK | LCD_DC_MASK | LCD_RD_MASK;
	WAIT1;
	gpio_setup(0, GPIO_MASK | LCD_WR_MASK | LCD_DC_MASK | LCD_RD_MASK);
}

void ssd_gpio_rpi_fini(void)
{
	GPIO_CLR = GPIO_MASK;
	// GPIO_SET = LCD_WR_MASK | LCD_DC_MASK | LCD_RD_MASK;
	gpio_setup(GPIO_MASK | LCD_WR_MASK | LCD_DC_MASK | LCD_RD_MASK, 0);
}
