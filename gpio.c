
/* based on example program by Dom and Gert, 01-01-2013,
 * from http://elinux.org/RPi_Low-level_peripherals
 *
 * modified by Franz Brauße, 06.01.2013, for standard compliance and added
 * tear-down function gpio_fini() */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <inttypes.h>

#define BCM2708_PERI_BASE	0x20000000
#define GPIO_BASE		(BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE		(4*1024)
#define BLOCK_SIZE		PAGE_SIZE

/* I/O access */
volatile uint32_t *gpio;

#include "gpio.h"

/*
 * Set up a memory regions to access GPIO
 */
void gpio_init(void)
{
	int mem_fd;
	void *gpio_map;
	int flags = PROT_READ | PROT_WRITE;

	/* open /dev/mem */
	if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		perror("can't open /dev/mem");
		exit(-1);
	}

	/* mmap GPIO */
	gpio_map = mmap(
		NULL,       /* any adddress in our space will do */
		BLOCK_SIZE, /* map length */
		flags,      /* enable reading & writting to mapped memory */
		MAP_SHARED, /* shared with other processes */
		mem_fd,     /* file to map */
		GPIO_BASE   /* offset to GPIO peripheral */
	);

	close(mem_fd); /* no need to keep mem_fd open after mmap */

	if ((long)gpio_map < 0) {
		perror("mmap error");
		exit(-1);
	}

	/* Always use volatile pointer! */
	gpio = gpio_map;
}

void gpio_fini(void)
{
	munmap((void *)gpio, BLOCK_SIZE);
}

void gpio_setup(uint32_t in, uint32_t out)
{
	int i;

	if (in & out) {
		printf("GPIOs must not be both, input and output: %08x\n",
			in & out);
		exit(-1);
	}

	for (i=0; in || out; i++, in >>= 1, out >>= 1) {
		if (in & 1)
			INP_GPIO(i);
		if (out & 1)
			OUT_GPIO(i);
	}
}
