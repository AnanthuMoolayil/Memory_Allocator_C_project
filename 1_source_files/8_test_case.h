#ifndef TEST_H
#define TEST_H

#include <stdio.h>

//Helper functions to showcase allocator functioning.

void full_alloc(void);

void full_free(void);

void exact_fit_tiny_leftover(void);

void coalesce_test(void);

void alignment_chk(void);

void sample_test(void);



//Helper functions to showcase allocator robustness.

void zero_alloc_test(void);

void double_free_test(void);

void invalid_pointer_stack_test(void);

void interior_pointer_test(void);

void null_free_test(void);

#endif