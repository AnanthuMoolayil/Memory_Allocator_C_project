#include <stdint.h>
#include "0_header.h"

extern Storage S;

//To reclaim allocated memory
void deallocate(void *ptr)
{
    //NULL input handling
    if(!ptr)
        return; 

    //When input pointer points to an address not included in memory heap range.    
    if( ( (uintptr_t)ptr < (uintptr_t)S.memory ) || ( (uintptr_t)ptr >= (uintptr_t)( ((char*)S.memory)+(S.offset) ) ) )
    {
        printf("Input memory address is invalid!!\n");
        return;
    }    

    Header_block *node=S.head;
    Header_block *input = (Header_block*)( (char*)ptr-sizeof(Header_block) );
    while(node)
    {   
        //Check whether input pointer points to valid header block within memory heap.
        if( (uintptr_t)node == (uintptr_t)input )
        {
            //Flag condition used to determine double free.
            if(input->used == 0)
            {
                printf("Double free detected!!\n");
                return;
            }
            //Freeing memory block
            else
            {
                input->used=0;
                coalesce(); //Check and merge contiguous free blocks can be merged.
                return;
            }  
        }
        node=node->next;        
    }
    printf("Input memory address is invalid!!\n");
    return;
}