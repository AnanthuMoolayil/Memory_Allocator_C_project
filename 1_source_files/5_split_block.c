#include <stdio.h>
#include "0_header.h"

extern Storage S;

//Refer NOTE in function body regarding potential integer overflow.

/*  Block splitting:
 *  1. If suitable block is found to be sufficiently big, block is split into two.
 *  2. One split block is as per the requested memory size and to be returned.
 *  3. Other split block is formed of leftover bytes and is a free block.
 *  In case block is not split, heap address returned without any size modification.
 */
void* split_block(Header_block **node, size_t size)
{
    /*  NOTE:
     *  In if condition, we use: (*node)->size-size. 
     *  It is guaranteed by reuse function that '(*node)->size' will always be greater than or equal to 'size'.
     *  Thus, integer overflow is avoided.
     */

    //Check if block splitting is possible
    if( ( (*node)->size-size ) > sizeof(Header_block) )
    {   
        //Free block creation after splitting
        Header_block *temp1=(Header_block*)(  ( (char*)(*node) ) + (int)sizeof(Header_block) + size );
        temp1->used=0;
        temp1->size=(*node)->size-size-(int)sizeof(Header_block);

        //If the last block is split
        if(S.tail==(*node))
        {
            //Node associated with free block appended at the end of the list.
            (*node)->next=temp1;
            S.tail=temp1;
            temp1->next=NULL;
        }
        else
        {
            //Node associated with free block appended in between existing nodes of list.
            Header_block *temp2=(*node)->next;
            (*node)->next=temp1;
            temp1->next=temp2;
        }
        //Update block usage status and size.   
        (*node)->used=1;
        (*node)->size=size;
    }
    //If not split, only update block usage status.
    (*node)->used=1;

    return ( ( (char*)(*node) ) + (int)sizeof(Header_block) );
}