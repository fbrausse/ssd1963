
#ifndef ITDB02_H
#define ITDB02_H

/* HSD050IDW1-A 5.0" TFT LCD */
#define HSD050IDW1_A \
	(struct ssd_display){ \
		SSD_LCD_24BIT | SSD_LCD_MODE_TFT, \
		{ 800, 40, 48, 40 }, \
		{ 480, 13,  3, 29 }, \
		0, 33e6, 40e6, \
	}

/* HSD043I9W1-A 4.3" TFT LCD */
/* the datasheet doesn't specify typical values for hold times of the sync
 * signals and includes these times in the back porch times;
 * minimum is specified to 1 for hsync as well as vsync */
#define HSD043I9W1_A \
	(struct ssd_display){ \
		SSD_LCD_24BIT | SSD_LCD_MODE_TFT, \
		{ 480, 5, 8, 40 - 8 }, \
		{ 272, 8, 1,  8 - 1 }, \
		5e6, 9e6, 12e6, \
	}

#define ITDB02_XTAL_FREQ	12000000

#endif
