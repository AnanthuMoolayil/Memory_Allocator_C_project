#include <stdio.h>
#include "0_header.h"

/*  Function to check & assign already existing blocks for allocation request.
 *  First suitable block is assigned.
 *  Block splitting occurs if suitable block is found to be sufficiently big.
 *  Returns heap address on successful allocation, else returns NULL.
 */
void* reuse(Header_block **head, size_t size)
{
    Header_block *temp=*head;
    
    while(temp)
    {
        //Check if block in consideration is used or not and if it is of suitable size.
        if( ( temp->used == 0 ) && (temp->size >= size) )
            break;
        
        temp=temp->next;
    }
    //When no existing blocks are suitable.
    if(!temp)
        return NULL;

    //Perform block splitting(if applicable) before returning reusable address.
    return split_block(&temp, size);
}