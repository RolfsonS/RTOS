/* Switching to the next task. */
.global switch_to_task

switch_to_task: 
	/* Push all Callee saved GPRs onto the stack. */
	push r2
	push r3 
	push r4
	push r5
	push r6 
	push r7
	push r8
	push r9 
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29

	/* Save parameter in Y-register */
	movw r28,r24 

	/* Save the stack frame from the currently running task. */
	in r24, 0x3D
	in r25, 0x3E

	/* Stores the mem address of current tcb* */
	lds r30, current_task
	lds r31, current_task + 1
	st Z, r24
	std Z + 1, r25
	
	/* Store next task as current. */
	sts current_task, r28
	sts current_task + 1, r29

	/* Change the stack pointers value */
	ldd r24, Y + 0
	ldd r25, Y + 1
	in r0, 0x3F
	cli
	out 0x3E, r25
	out 0x3D, r24
	out 0x3F, r0

	/* Restore all the required GPRs */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2 

	ret
