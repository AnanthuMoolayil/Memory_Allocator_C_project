#include <stdio.h>
#include "0_header.h"

extern Storage S;

/*  Deallocation may give rise to contiguous free blocks. 
 *  This function merges such free blocks to form a single whole free block.
 */
void coalesce(void)
{
    Header_block *temp=S.head;

    while( temp!=NULL && temp->next!=NULL )
    {
        //Check if adjacent blocks are free.
        if( (temp->used==0) && (temp->next->used==0) )
        {
            //Update size of combined free block.
            temp->size = temp->size + temp->next->size + sizeof(Header_block);
            
            //Update header block linked list 
            //If combined free block is at the end.
            if(temp->next==S.tail)
            {
                S.tail=temp;
                temp->next=NULL;
                break;
            }
            //If combined free block happens to be somewhere within the list.
            else
            {
                temp->next = temp->next->next;
                continue;
            }
            
        }
        temp=temp->next;
    }

}