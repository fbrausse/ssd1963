
#ifndef SSD_GPIO_H
#define SSD_GPIO_H

#include "ssd-gpio-rpi.h"

#ifndef SSD_FAST
# define SSD_WR_CMD(x)	lcd_cmd_slow(x)
# define SSD_WR_DATA(x)	lcd_wr8_slow(x)
#else
# define SSD_WR_CMD(x)	lcd_cmd_fast(x)
# define SSD_WR_DATA(x)	lcd_wr8_fast(x)
#endif

#endif
