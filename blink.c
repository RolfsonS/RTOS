#include <stdint.h>
#include <stdbool.h>

#define BIT(x) (1UL <<(x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255u)
#define PINBANK(pin) (pin >> 8)

typedef struct gpio_t {
	volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} gpio;

typedef struct rcc_t{
	volatile uint32_t CR, HSICFGR, CRRCR, CSICFGR, CFGR, RES1, D1CFGR, D2CFGR,
		D3CFGR, RES2, PLLCKSELR, PLLCFGR, PLL1DIVR, PLL1FRACR, PLL2DIVR,
		PLL2FRACR, PLL3DIVR, PLL3FRACR, RES3, D1CCIPR, D2CCIP1R, D2CCIP2R,
		D3CCIPR, RES4, CIER, CIFR, CICR, RES5, BDCR, CSR, RES6, AHB3RSTR,
		AHB1RSTR, AHB2RSTR, AHB4RSTR, APB3RSTR, APB1LRSTR, APB1HRSTR, APB2RSTR,
		APB4RSTR, GCR, RES7, D3AMR, RES8[9], RSR, AHB3ENR, AHB1ENR, AHB2ENR,
		AHB4ENR, APB3ENR, APB1LENR, APB1HENR, APB2ENR, APB4ENR, RES9, AHB3LPENR,
		AHB1LPENR, AHB2LPENR, AHB4LPENR, APB3LPENR, APB1LLPENR, APB1HLPENR,
		APB2LPENR, APB4LPENR;
} rcc;

#define GPIO(bank) ((gpio*) (0x58020000 + 0x400 * (bank)))
#define RCC	   ((rcc*)  (0x58024400))

enum {
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_AF,
	GPIO_MODE_ANALOG,
};

/* Set the mode of a GPIO pin. 
 * We first reset the previous mode, then set again. */
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
	gpio* curr_gpio = GPIO(PINBANK(pin));
	uint8_t pin_num = PINNO(pin);
	curr_gpio -> MODER &= ~(3U << (pin_num * 2));
	curr_gpio -> MODER |= (mode & 3U) << (pin_num * 2);
}

static inline void gpio_write(uint16_t pin, bool val) {
	gpio* curr_gpio = GPIO(PINBANK(pin));
	curr_gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}

static inline void spin(volatile uint32_t count) {
  	while (count--) (void) 0;
}

int main(void) {
  	uint16_t led = PIN('B', 14);
  	RCC->AHB4ENR |= BIT(PINBANK(led));
  	gpio_set_mode(led, GPIO_MODE_OUTPUT);
  	
	for (;;) {
    		gpio_write(led, true);
    		spin(4000000);
    		gpio_write(led, false);
    		spin(4000000);
 	}

  	return 0;
}
