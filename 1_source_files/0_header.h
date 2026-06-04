#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
 
#define memory_size ((size_t)1000)  //size of static memory heap.

#define alignment ((size_t)8)   //allocated addresses will be 8-byte aligned.

/*  This is the metadata header block structure used to form a linked list of header blocks.  
 *  Contains the following members:
 *  used    : Flag used to determine whether assigned memory block is free(0) or used(1).
 *  size    : To store size of assigned memory block.
 *  next    : Pointer to next header block node.   
 */ 
typedef struct Header_block
{
    int used;
    size_t size;
    struct Header_block *next;

}Header_block; 


/*  This structure contains the core elements that make up the allocator.
 *  memory  : This stack memory acts as the dynamic heap.
 *  offset  : Position of memory address to be allocated next, relative to the start of heap.
 *  head    : Pointer to first node of header block linked list.
 *  tail    : Pointer to last node of header block linked list.
 */
typedef struct Storage
{
    _Alignas(alignment) char memory[memory_size];
    size_t offset;
    Header_block *head, *tail;
}Storage;


/*  Function used to align sizes.
 *  Input size rounded up to nearest multiple of 8.
 */
static inline size_t size_align(size_t size)
{
    return ( (size+(alignment-1)) & ~(alignment-1) );
}


void* allocate(size_t size);

void deallocate(void *ptr);

void* reuse(Header_block **head, size_t size);

void* split_block(Header_block **node, size_t size);

void coalesce(void);

void malloc_chk(void *ptr);

void alloc_dump(void);

size_t largest_free_block(void);

size_t block_count(void);

void display_alloc_stat(void);

#endif