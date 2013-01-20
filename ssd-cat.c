
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#define SSD_FAST
#include "ssd-gpio.h"	/* definitions for LCD_WR_CMD, LCD_WR_DATA */
#include "ssd1963.h"	/* commands/init functions for SSD1963 controller */
#include "itdb02.h"	/* LCD definitions */

/* use the 4.3" display from ITead Studio */
#define LCD_PANEL	HSD043I9W1_A
#define LCD_PANEL_W	(LCD_PANEL.hori.visible)
#define LCD_PANEL_H	(LCD_PANEL.vert.visible)

#define USAGE		"usage: %s [-i] X Y W H\n"

#define BUF_SZ		4096

static unsigned char buf[BUF_SZ];

static unsigned do_init = 0;	/* also perform initialization of the display */
static unsigned x, y, w, h;	/* define rectangle to overwrite */

static void parse_options(int argc, char **argv);

int main(int argc, char **argv)
{
	int r = EXIT_SUCCESS;

	parse_options(argc, argv);

	gpio_init();
	ssd_gpio_rpi_init();

	if (do_init &&
	    ssd_init(ITDB02_XTAL_FREQ, 40, 5, 1,
	             &LCD_PANEL, 0, 0, SSD_DATA_8) != SSD_ERR_NONE)
		r = EXIT_FAILURE;

	if (r == EXIT_SUCCESS) {
		size_t i, n;
		/* vertical interval to draw to */
		SSD_SET_PAGE_ADDRESS(y, y + h - 1);
		/* horizontal interval to draw to */
		SSD_SET_COLUMN_ADDRESS(x, x + w - 1);
		/* writing commences... */
		SSD_WRITE_MEMORY_START();
		while (1) {
			n = fread(buf, 1, BUF_SZ, stdin);
			for (i=0; i<n; i++)
				SSD_WR_DATA(buf[i]);
			if (n < BUF_SZ)
				break;
		}
		if (ferror(stdin)) {
			fprintf(stderr, "error %d reading data: %s\n",
				ferror(stdin), strerror(ferror(stdin)));
			r = EXIT_FAILURE;
		}
	}
	/* reset the bus-lines to zero (NOP), so releasing the GPIOs below
	 * doesn't trigger any meaningful commands */
	SSD_NOP();

	ssd_gpio_rpi_fini();
	gpio_fini();

	return r;
}

static void parse_options(int argc, char **argv)
{
	int argi = 1, i;

	if (argi < argc && !strcmp("-i", argv[argi])) {
		do_init = 1;
		argi++;
	}

	for (i=0; i<4 && argi<argc; i++, argi++) {
		char *endptr;
		unsigned long v;
		errno = 0;
		v = strtoul(argv[argi], &endptr, 0);
		if (*endptr || errno || v > UINT_MAX) {
			fprintf(stderr, "invalid value for %c: %s\n",
				"XYWH"[i], argv[argi]);
			exit(EXIT_FAILURE);
		}
		*(unsigned *[]){ &x, &y, &w, &h }[i] = v;
	}
	if (i < 4 || argi < argc) {
		fprintf(stderr, USAGE, argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!(w > 0 && h > 0)) {
		fprintf(stderr, "error: W and H must be greater than zero\n");
		exit(EXIT_FAILURE);
	}
	if (x + w < x || x + w > LCD_PANEL_W) {
		fprintf(stderr, "error: X + W > panel width (%d)\n",
			LCD_PANEL_W);
		exit(EXIT_FAILURE);
	}
	if (y + h < y || y + h > LCD_PANEL_H) {
		fprintf(stderr, "error: Y + H > panel height (%d)\n",
			LCD_PANEL_H);
		exit(EXIT_FAILURE);
	}
}
