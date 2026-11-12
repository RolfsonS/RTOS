#include <stdint.h>

/* Define the interrupt vector code. */
extern void main(void);
/* Vars defined in linker.ld.
 * Indicate the start and end of bss and data segments, as well as the
 * end of the stack. bss and data are copied to SRAM from flash. */
extern uint32_t _sbss, _ebss, _sdata, _edata, _sidata, _estack;

/* Reset vector.
 *
 * When resetting, clear the .bss and recopy the .data sections from flash to SRAM.
 * __attribute__((naked, noreturn)) means no prologue or epilogue. 
 * This means no GPRS pushed/popped from stack. noreturn indicates that this function does not return.
 *
 */ 
__attribute__((naked, noreturn)) void _reset(void) {
	/* Clear the .bss section in SRAM */
	for(uint32_t* dest_bss = &_sbss; dest_bss < &_ebss; dest_bss++) {
		*dest_bss = 0;
	}
	
	/* Copy .data from flash to sram */
	for(uint32_t* dest_flash = &_sdata, *source_data = &_sidata; dest_flash < &_edata;) {
		*dest_flash++ = *source_data++;
	}
	main();
	for(;;) (void) 0;
}

/* Interrupt and exception vectors. 
 * __attribute__((section(".vectors")))) indicates the section to place the table.
 * */
__attribute((section(".vectors"))) 
void* const vectors[16 + 150] = {
	&_estack,
	_reset,
};

