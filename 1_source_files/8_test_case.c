//Helper functions to showcase allocator functioning and robustness.

#include <stdio.h>
#include <stdint.h>
#include "0_header.h"

extern Storage S;

void full_alloc(void)
{
    printf("\n=============================================================\n");
    printf("TEST : FILL ENTIRE HEAP\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Allocate fixed-size blocks repeatedly until allocator\n");
    printf("runs out of memory and returns NULL.\n\n");

    printf("Operation:\n");
    printf("allocate(10) repeatedly.\n\n");

    size_t count = 0;

    while(allocate(10))
        count++;

    printf("Allocator became full after %zu successful allocations.\n",
           count);

    alloc_dump();

    printf("Expected Result:\n");
    printf("- All blocks should be marked USED.\n");
    printf("- No free space should remain for another allocation.\n");
    printf("- Final allocation attempt should return NULL.\n");

    printf("=============================================================\n\n");
}

void full_free(void)
{
    printf("\n=============================================================\n");
    printf("TEST : FREE ENTIRE HEAP\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Free every allocated block in the heap.\n\n");

    Header_block *header = (Header_block*)S.memory;
    Header_block *data;

    while(header)
    {
        data = (Header_block*)((char*)header + sizeof(Header_block));

        deallocate(data);

        header = header->next;
    }

    printf("\nHeap after all blocks have been released:\n");

    alloc_dump();

    printf("Expected Result:\n");
    printf("- All blocks should be marked FREE.\n");
    printf("- Coalescing should reduce the number of blocks.\n");
    printf("- Allocator should be ready for reuse.\n");

    printf("=============================================================\n\n");
}

void exact_fit_tiny_leftover(void)
{
    printf("\n=============================================================\n");
    printf("TEST : EXACT FIT AND TINY LEFTOVER FRAGMENT\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify allocator behaviour when a free block is reused\n");
    printf("either exactly or with insufficient space to split.\n\n");

    printf("STEP 1 : Allocate 200 bytes\n");

    int *t1 = allocate(200);
    malloc_chk(t1);

    alloc_dump();

    printf("STEP 2 : Free the 200-byte block\n");

    deallocate(t1);

    alloc_dump();

    printf("STEP 3 : Reallocate 200 bytes\n");
    printf("Expected: allocator should reuse the same free block.\n\n");

    t1 = allocate(200);
    malloc_chk(t1);

    alloc_dump();

    printf("STEP 4 : Free the block again\n");

    deallocate(t1);

    printf("STEP 5 : Allocate 192 bytes\n");
    printf("Expected:\n");
    printf("- Allocator should decide whether splitting is worthwhile.\n");
    printf("- Tiny unusable fragments should be avoided.\n\n");

    t1 = allocate(192);
    malloc_chk(t1);

    alloc_dump();

    printf("=============================================================\n\n");
}

void coalesce_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : COALESCING ADJACENT FREE BLOCKS\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify neighbouring free blocks merge into a larger block.\n\n");

    printf("STEP 1 : Allocate five blocks of 50 bytes each\n");

    int *t0 = allocate(50);
    int *t1 = allocate(50);
    int *t2 = allocate(50);
    int *t3 = allocate(50);
    int *t4 = allocate(50);

    malloc_chk(t0);
    malloc_chk(t1);
    malloc_chk(t2);
    malloc_chk(t3);
    malloc_chk(t4);

    alloc_dump();

    printf("STEP 2 : Free middle block (Block 2)\n");
    deallocate(t2);

    printf("STEP 3 : Free neighbouring block on left (Block 1)\n");
    deallocate(t1);

    printf("Expected:\n");
    printf("Block 1 and Block 2 should merge.\n\n");

    printf("STEP 4 : Free neighbouring block on right (Block 3)\n");
    deallocate(t3);

    printf("Expected:\n");
    printf("All three adjacent free blocks should coalesce into\n");
    printf("one larger free block.\n\n");

    alloc_dump();

    printf("Verification:\n");
    printf("There should be a single larger FREE region where\n");
    printf("Blocks 1, 2 and 3 previously existed.\n");

    printf("=============================================================\n\n");
}

void alignment_chk(void)
{
    printf("\n=============================================================\n");
    printf("TEST : 8-BYTE ALIGNMENT VALIDATION\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Ensure every returned user pointer is aligned to\n");
    printf("an 8-byte boundary.\n\n");

    void *ptr;
    size_t allocations = 0;
    size_t failures = 0;

    for(size_t i = 1; ; i++)
    {
        ptr = allocate(i);

        if(!ptr)
            break;

        allocations++;

        if(((uintptr_t)ptr % 8) != 0)
        {
            failures++;

            printf("ALIGNMENT FAILURE\n");
            printf("Requested Size : %zu\n", i);
            printf("Returned Ptr   : %p\n\n", ptr);
        }
    }

    printf("Total allocations tested : %zu\n", allocations);
    printf("Alignment failures       : %zu\n\n", failures);

    alloc_dump();

    if(failures == 0)
        printf("PASS : All returned addresses were 8-byte aligned.\n");
    else
        printf("FAIL : Misaligned addresses detected.\n");

    printf("=============================================================\n\n");
}

void sample_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : COALESCE + LARGE REUSE ALLOCATION\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify that coalesced free blocks can satisfy a larger\n");
    printf("allocation request.\n\n");

    printf("STEP 1 : Allocate three blocks of 100 bytes each\n");

    int *t0 = allocate(100);
    int *t1 = allocate(100);
    int *t2 = allocate(100);

    malloc_chk(t0);
    malloc_chk(t1);
    malloc_chk(t2);

    alloc_dump();

    printf("STEP 2 : Free Block 1\n");
    deallocate(t1);

    printf("STEP 3 : Free Block 0\n");
    deallocate(t0);

    printf("Expected:\n");
    printf("Block 0 and Block 1 should merge into a larger\n");
    printf("contiguous free region.\n\n");

    alloc_dump();

    printf("STEP 4 : Request allocation of 192 bytes\n");
    printf("Expected:\n");
    printf("Allocator should reuse the coalesced free region\n");
    printf("instead of extending the heap.\n\n");

    t1 = allocate(192);
    malloc_chk(t1);

    alloc_dump();

    printf("Verification:\n");
    printf("Check whether the allocation was serviced from the\n");
    printf("merged free block.\n");

    printf("=============================================================\n\n");
}

void zero_alloc_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : ZERO-SIZE ALLOCATION\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify allocate(0) is rejected and does not alter\n");
    printf("allocator state.\n\n");

    printf("STEP 1 : Allocate 100 bytes\n");

    void *ptr = allocate(100);
    malloc_chk(ptr);

    printf("\nHeap before allocate(0):\n");
    alloc_dump();

    printf("STEP 2 : Request allocation of 0 bytes\n");

    void *zero = allocate(0);

    if(zero == NULL)
        printf("PASS : allocate(0) returned NULL.\n");
    else
        printf("FAIL : allocate(0) returned %p.\n", zero);

    printf("\nHeap after allocate(0):\n");
    alloc_dump();

    printf("Expected Result:\n");
    printf("- allocate(0) returns NULL.\n");
    printf("- No new block is created.\n");
    printf("- Existing heap structure remains unchanged.\n");

    printf("=============================================================\n\n");
}

void double_free_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : DOUBLE FREE DETECTION\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify allocator detects and rejects a double free.\n\n");

    printf("STEP 1 : Allocate 100 bytes\n");

    void *ptr = allocate(100);
    malloc_chk(ptr);

    alloc_dump();

    printf("STEP 2 : Free block normally\n");

    deallocate(ptr);

    alloc_dump();

    printf("STEP 3 : Attempt to free same block again\n");
    printf("Expected:\n");
    printf("Allocator should report \"Double free detected!!\"\n\n");

    deallocate(ptr);

    alloc_dump();

    printf("=============================================================\n\n");
}

void invalid_pointer_stack_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : INVALID POINTER (STACK ADDRESS)\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify allocator rejects pointers outside the heap\n");
    printf("without corrupting metadata.\n\n");

    printf("STEP 1 : Allocate 100 bytes\n");

    void *ptr = allocate(100);
    malloc_chk(ptr);

    printf("\nHeap before invalid free:\n");
    alloc_dump();

    int x = 123;

    printf("STEP 2 : Attempt deallocate(&x)\n");
    printf("Stack address = %p\n\n", (void*)&x);

    deallocate(&x);

    printf("Heap after invalid free:\n");
    alloc_dump();

    printf("Expected Result:\n");
    printf("- Allocator reports invalid address.\n");
    printf("- Existing allocation remains allocated.\n");
    printf("- Heap metadata remains unchanged.\n");

    printf("=============================================================\n\n");
}

void interior_pointer_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : INTERIOR POINTER FREE\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify allocator rejects pointers that lie inside\n");
    printf("an allocation but are not the original base address.\n\n");

    printf("STEP 1 : Allocate 100 bytes\n");

    char *ptr = allocate(100);
    malloc_chk(ptr);

    printf("\nHeap before invalid free:\n");
    alloc_dump();

    printf("Returned pointer : %p\n", (void*)ptr);
    printf("Interior pointer : %p\n\n", (void*)(ptr + 20));

    printf("STEP 2 : Attempt deallocate(ptr + 20)\n");

    deallocate(ptr + 20);

    printf("\nHeap after invalid free:\n");
    alloc_dump();

    printf("STEP 3 : Free the correct pointer\n");

    deallocate(ptr);

    printf("\nHeap after freeing valid pointer:\n");
    alloc_dump();

    printf("Expected Result:\n");
    printf("- Interior pointer should be rejected.\n");
    printf("- Allocator should report invalid address.\n");
    printf("- Heap state should remain unchanged after the invalid free.\n");
    printf("- Original allocation should still be valid and freeable.\n");

    printf("=============================================================\n\n");
}

void null_free_test(void)
{
    printf("\n=============================================================\n");
    printf("TEST : NULL POINTER FREE\n");
    printf("=============================================================\n");

    printf("Objective:\n");
    printf("Verify deallocate(NULL) performs no operation and does\n");
    printf("not affect existing allocations.\n\n");

    printf("STEP 1 : Allocate 100 bytes\n");

    void *ptr = allocate(100);
    malloc_chk(ptr);

    alloc_dump();

    printf("STEP 2 : Call deallocate(NULL)\n");

    deallocate(NULL);

    printf("\nSTEP 3 : Verify allocated block still exists\n");

    alloc_dump();

    printf("STEP 4 : Free the valid allocation\n");

    deallocate(ptr);

    alloc_dump();

    printf("Expected Result:\n");
    printf("- deallocate(NULL) should produce no output.\n");
    printf("- Existing allocations should remain unaffected.\n");
    printf("- Valid allocation should still be freeable afterwards.\n");

    printf("=============================================================\n\n");
}

/*
void full_alloc(void)
{
    printf("***Filling memory heap completely***\n\n");
    printf("Memory is continuously allocated until full.\n");
    printf("allocate(10) till full\n");

    while(allocate(10));

    alloc_dump();

    printf("************************************\n\n");
}

void full_free(void)
{
    printf("***Clearing memory heap***\n\n");
    
    Header_block *header=(Header_block*)S.memory;
    
    Header_block *data;

    while(header)
    {
        data=(Header_block*)((char*)header+sizeof(Header_block));
        
        deallocate(data);

        header=header->next;
    }
    alloc_dump();

    printf("**************************\n\n");
}

void exact_fit_tiny_leftover(void)
{
    printf("***Allocating 200 bytes***\n");
    int *t1=allocate(200);
    malloc_chk(t1);
    

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    printf("***Freeing 200 bytes***\n");
    deallocate(t1);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    printf("***Reallocating 200 bytes***\n");
    t1=allocate(200);
    malloc_chk(t1);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    deallocate(t1);

    printf("***Allocating 192 bytes***\n");
    t1=allocate(192);
    malloc_chk(t1);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

}

void coalesce_test(void)
{
    printf("***Allocating 50 bytes 5 times***\n");
    int *t0=allocate(50);
    malloc_chk(t0);

    int *t1=allocate(50);
    malloc_chk(t1);

    int *t2=allocate(50);
    malloc_chk(t2);

    int *t3=allocate(50);
    malloc_chk(t3);

    int *t4=allocate(50);
    malloc_chk(t4);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    printf("***Freeing Block 2, then block 1 and then block 3***\n");
    deallocate(t2);

    deallocate(t1);

    deallocate(t3);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);
    
}

void alignment_chk(void)
{
    printf("***Memory address 8 byte alignment check***\n");
    
    void *ptr;

    for(size_t i=1; ; i++)
    {
        ptr=allocate(i);

        if(!ptr)
            break;

        if( (uintptr_t)ptr%8 != 0 )
            printf("Misaligned address: %p allocated!\n", ptr);
    }
    alloc_dump();
}

void gp_test(void)
{
    printf("***Allocating 100 bytes 3 times***\n");
    int *t0=allocate(100);
    malloc_chk(t0);

    int *t1=allocate(100);
    malloc_chk(t1);

    int *t2=allocate(100);
    malloc_chk(t2);


    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    printf("***Freeing block 1 and then block 0 ***\n");
    deallocate(t1);

    deallocate(t0);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);

    printf("***Allocating 180 bytes***\n");
    t1=allocate(192);
    malloc_chk(t1);

    alloc_dump();
    printf("Offset=%zu\n", S.offset);


}
    */