#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

/* Size of the stack for the new context */
#define STACK_SIZE 16384

ucontext_t main_context, new_context;

void new_function()
{
    printf("Inside the new context\n");
    printf("Switching back to the ==> main context\n");

	/* Switch back to the main context */
    swapcontext(&new_context, &main_context);
}

int main()
{
	/* Allocate a stack for the new context */
    char new_stack[STACK_SIZE];

    /* Initialize the new context with the new stack */
    getcontext(&new_context);

	/* Set the context to return to main_context when done */
    new_context.uc_link = &main_context;
    new_context.uc_stack.ss_sp = new_stack;
    new_context.uc_stack.ss_size = sizeof(new_stack);
	/* Associate the function with the new context */
    makecontext(&new_context, new_function, 0);

    printf("Switching to the ==> new context\n");
	/* Switch to the new context */
    swapcontext(&main_context, &new_context);

    printf("Back in the main context\n");

    return 0;
}
