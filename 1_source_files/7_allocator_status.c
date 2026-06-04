//Helper functions to display allocator statistics.

#include <stdio.h>
#include "0_header.h"

extern Storage S;

size_t largest_free_block(void)
{
    Header_block *node=S.head;
    size_t size=0;

    while(node)
    {
        if( (node->used==0) && (size < node->size) )
            size=node->size;

        node=node->next;
    }
    return size;
}

size_t block_count(void)
{
    Header_block *node=S.head;
    size_t count=0;

    while(node)
    {
        count++;
        node=node->next;
    }
    return count;
}

void display_alloc_stat(void)
{
    double utilization =
        ((double)S.offset / memory_size) * 100.0;

    printf("\n");
    printf("=============================================================\n");
    printf("ALLOCATOR STATISTICS\n");
    printf("=============================================================\n");

    printf("%-30s : %-10zu bytes\n",
           "Total heap size",
           memory_size);

    printf("%-30s : %-10zu bytes\n",
           "Total used memory",
           S.offset);

    printf("%-30s : %-10zu bytes\n",
           "Total free memory",
           memory_size - S.offset);

    printf("%-30s : %-10.2f %%\n",
           "Heap utilization",
           utilization);

    printf("%-30s : %-10zu bytes\n",
           "Metadata per block",
           sizeof(Header_block));

    printf("%-30s : %-10zu bytes\n",
           "Largest free block",
           largest_free_block());

    printf("%-30s : %-10zu\n",
           "Total block count",
           block_count());

    printf("=============================================================\n\n");
}


/*
void display_alloc_stat(void)
{
    printf("*******Allocator Status*******\n\n");
    printf("Total memory: %zu bytes\n", memory_size);
    printf("Total free memory: %zu bytes\n", memory_size-S.offset);
    printf("Total used memory: %zu bytes\n", S.offset);
    printf("Metadata overhead: %zu bytes\n", sizeof(Header_block));
    printf("Largest free block: %zu bytes\n", largest_free_block());
    printf("Block count: %zu \n\n", block_count());
    printf("******************************\n");
}
*/