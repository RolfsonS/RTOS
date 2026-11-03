#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

typedef struct tcb_t tcb;
extern void switch_to_task(tcb*);
tcb* current_task = NULL;

/* Task control block. */
typedef struct tcb_t {
	/* https://gcc.gnu.org/wiki/avr-gcc */
	/* Stack pointer is 3 bytes, and 1 byte aligned. */
	uint16_t sp;

	/* Next task to run */
	struct tcb_t* next;
} tcb;

void schedule() {
	switch_to_task(current_task -> next);
}

/* Blink Task 1 */
void blink1() {
	/* DDRx (Port: x) tells which pin in port is set to output or input.
	 * 1 corresponds to output, 0 to input.
	 */
	DDRB |= (1 << PB7);

	/* Loop forever */
	while (1) {
		/* PORTx (Port:x) control the output of that pin. High or low voltage.
		 * 1 is high, to is low. We toggle for on/off LED.
		 */
		PORTB ^= (1 << PB7);
		_delay_ms(10);
		schedule();
	}
	return;
}


/* Blink Task 2 */
void blink2() {
	/* DDRx (Port: x) tells which pin in port is set to output or input.
	 * 1 corresponds to output, 0 to input.
	 */
	DDRB |= (1 << PB6);

	/* Loop forever */
	while (1) {
		/* PORTx (Port:x) control the output of that pin. High or low voltage.
		 * 1 is high, to is low. We toggle for on/off LED.
		 */
		PORTB ^= (1 << PB6);
		_delay_ms(10);
		schedule();
	}
	return;
}	


tcb* init_task(void (*func)(void)) {
	tcb* new_tcb = (tcb*) malloc(sizeof(tcb));

	/* Malloc 256 Bytes. */
	new_tcb -> sp = (uint16_t) malloc(256);
	
	/* Cast this to a pointer... a little easier to work with */
	uint8_t* stack = (uint8_t*) (new_tcb -> sp);
	stack += 255;
	uint16_t pc = (uint16_t) func;
	/* Add the functions location to the top of the stack */
	*stack-- = pc & 0xFF;
	*stack-- = pc >> 8;
	*stack-- = 0x0;
	
	for(int i = 0; i < 18; i++) {
		*stack-- = 0x0;
	} 
		
	new_tcb -> sp = (uint16_t) stack;

	return new_tcb;
}

int main(void) {
	
	tcb* kthread_main = (tcb*) malloc(sizeof(tcb));
	current_task = kthread_main;

	tcb* t1 = init_task(blink1);
	tcb* t2 = init_task(blink2);

	kthread_main -> next = t1;
	t1 -> next = t2;
	t2 -> next = kthread_main;


	/* DDRx (Port: x) tells which pin in port is set to output or input.
	 * 1 corresponds to output, 0 to input.
	 */
	DDRB |= (1 << PB5);

	/* Loop forever */
	while (1) {
		/* PORTx (Port:x) control the output of that pin. High or low voltage.
		 * 1 is high, to is low. We toggle for on/off LED.
		 */
		PORTB ^= (1 << PB5);
		_delay_ms(10);
		schedule();
	}
}

