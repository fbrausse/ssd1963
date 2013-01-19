
#ifndef GPIO_H
#define GPIO_H

#include <inttypes.h>

/* I/O access */
extern volatile uint32_t *gpio;

/* GPIO setup macros. */
#define INP_GPIO(g) (gpio[(g)/10] &= ~(7 << ((g)%10)*3))
#define OUT_GPIO(g) \
	(gpio[(g)/10] = (gpio[(g)/10] & ~(7 << ((g)%10)*3)) \
	              | (1 << ((g)%10)*3))
#define SET_GPIO_ALT(g,a) \
	(gpio[(g)/10] = (gpio[(g)/10] & ~(7 << ((g)%10)*3)) \
	              | ((a) <= 3 ? (a)+4 : (a) == 4 ? 3 : 2) << ((g)%10)*3)

/* sets bits which are 1, ignores bits which are 0 */
#define GPIO_SET		gpio[7]
/* clears bits which are 1, ignores bits which are 0 */
#define GPIO_CLR		gpio[10]
/* returns bits (at least) for input GPIOs */
#define GPIO_VAL		gpio[13]
#define GPIO_DETECT		gpio[16]
#define GPIO_DETECT_RISING	gpio[19]
#define GPIO_DETECT_FALLING	gpio[22]
#define GPIO_DETECT_HIGH	gpio[25]
#define GPIO_DETECT_LOW		gpio[28]
#define GPIO_ASYNC_RISING	gpio[31]
#define GPIO_ASYNC_FALLING	gpio[34]
#define GPIO_PULL_CONTROL	gpio[37]
#define GPIO_PULL		gpio[38]

#define GPIO_PULL_OFF		0
#define GPIO_PULL_DOWN		1
#define GPIO_PULL_UP		2

#define NOP		__asm__ __volatile__("nop")

static inline void nop(unsigned n)
{
	while (n--)
		NOP;
}

static inline void gpio_pull(int control, uint32_t pins)
{
	GPIO_PULL_CONTROL = control;
	nop(150);
	GPIO_PULL = pins;
	nop(150);
	GPIO_PULL_CONTROL = 0;
	GPIO_PULL = pins;
}

/* initializes the gpio pointer and sets those GPIOs whose bits are set to 1 in
 * in and out to input respectively output mode */
void gpio_init(void);
void gpio_fini(void);
void gpio_setup(uint32_t in, uint32_t out);

static inline void gpio_write_hl(uint32_t val, uint32_t mask)
{
	GPIO_SET =  val & mask;
	GPIO_CLR = ~val & mask;
}

static inline void gpio_write_lh(uint32_t val, uint32_t mask)
{
	GPIO_CLR = ~val & mask;
	GPIO_SET =  val & mask;
}

#endif
