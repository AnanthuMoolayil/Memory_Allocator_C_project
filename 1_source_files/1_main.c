#include <stdio.h>
#include "0_header.h"
#include "8_test_case.h"

Storage S={0};

int main(void)
{  
    sample_test();

    display_alloc_stat();

}

void malloc_chk(void *ptr)
{
    if(!ptr)
    {
        printf("Memory allocation failed!!\n");
        return;
    }
}

void alloc_dump(void)
{
    Header_block *temp = S.head;
    int i = 0;

    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("Heap State\n");
    printf("-------------------------------------------------------------\n");
    printf("%-8s %-18s %-10s %-8s\n",
           "Block", "Header Address", "Size", "Used");
    printf("-------------------------------------------------------------\n");

    while(temp)
    {
        printf("%-8d %-18p %-10zu %-8s\n",
               i,
               (void*)temp,
               temp->size,
               temp->used ? "YES" : "NO");

        i++;
        temp = temp->next;
    }

    printf("-------------------------------------------------------------\n");
    printf("Current allocator offset = %zu bytes\n", S.offset);
    printf("-------------------------------------------------------------\n\n");
}



/*
void alloc_dump(void)
{
    Header_block *temp=S.head;
    int i=0;

    while(temp)
    {
        printf("Block %3d : Header Address: %p and block size: %5zu and used=%5d\n", i, (void* )temp, temp->size, temp->used );
        i++;
        temp=temp->next;
    }
    printf("\n");
}
*/



/*
int *t1=allocate(100);
    malloc_chk(t1);
    printf("S.offset=%zu\n\n", S.offset);

    int *t2=allocate(100);
    malloc_chk(t2);
    printf("S.offset=%zu\n\n", S.offset);

    int *t3=allocate(100);
    malloc_chk(t3);
    printf("S.offset=%zu\n\n", S.offset);

    alloc_dump();

    deallocate(t1);
    deallocate(t2);

    t1=allocate(150);
    malloc_chk(t1);
    printf("S.offset=%zu\n\n", S.offset);

    alloc_dump();

*/


/*
    printf("Memory size=%d bytes", memory_size);
    printf("S.offset=%zu\n\n", S.offset);
    
    int *t1=allocate(sizeof(int));
    malloc_chk(t1);
    *t1=333;
    printf("%d\n", *t1);
    printf("S.offset=%zu\n\n", S.offset);

    char *t2=allocate(strlen("hello")+1);
    malloc_chk(t2);
    strcpy(t2, "hello");
    printf("%s\n", t2);
    printf("S.offset=%zu\n\n", S.offset);

    int *t3=allocate(5000*sizeof(int));
    malloc_chk(t3);
    printf("S.offset=%zu\n\n", S.offset);

    unsigned char *t4;
    for(int i=0; i<20; i++)
    {
        t4=allocate(sizeof(char));
        malloc_chk(t4);
        if(t4)
        {
            *t4=i;
            printf("%d\n", *t4);
        }
        printf("S.offset=%zu\n\n", S.offset);
    }
    
*/