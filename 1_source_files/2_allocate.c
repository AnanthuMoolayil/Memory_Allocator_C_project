#include <stdio.h>
#include <stdlib.h>
#include "0_header.h"

//Global variable containing heap and core elements of allocator.
extern Storage S;

/*  Function to allocate memory address.
 *  Input(size_t size) is to be provided in the form of no. of bytes to be allocated.
 *  Returns a valid memory address on successful allocation.
 *  Returns NULL for the following cases:
 *  a. Input: 0
 *  b. Input: NULL
 *  c. When heap is full
 */
void* allocate(size_t size)
{
    //Zero input handling
    if(size==0)
        return NULL;
    
    //Input size alignment
    size=size_align(size);
    
    //NULL return when heap is full
    if( ( (memory_size-S.offset) < size ) || ( (memory_size-S.offset) < sizeof(Header_block) ) )
    {
        void *temp=reuse( &(S.head), size);
        
        return temp; 
    }    
    
    //First memory allocation
    if( !(S.head) )
    {
        //Creating the first node of header block linked list
        S.head=(Header_block*) (S.memory);
        S.head->used=1;
        S.head->size=size;
        S.head->next=NULL;
        S.tail=S.head;

        //Updating offset
        S.offset=sizeof(Header_block)+size;

        //Returning assigned memory
        return &( S.memory[sizeof(Header_block)] );
    }

    
    /*  Checking if any of the already exixting blocks can be reused & assigned.
     *  First-fit method is used for this allocator.
     */
    void *temp=reuse( &(S.head), size);
    if(temp)
        return temp;

    //Creating node of header block linked list    
    Header_block *node=(Header_block*)&(S.memory[S.offset]);
    node->used=1;
    node->size=size;
    node->next=NULL;

    //Updating the linked list
    S.tail->next=node;
    S.tail=node;
    
    void *ret_address=&( S.memory[S.offset+sizeof(Header_block)] );
    
    //Updating offset
    S.offset=S.offset+sizeof(Header_block)+size;
    
    //Offset overflow adjustment
    if(S.offset>memory_size)
        S.offset=memory_size;
    
    return ret_address;
}



/*bump allocator
void* allocate(size_t size)
{
    if( size > (memory_size-offset) )
        return NULL;
    
    if(size==0)
        return NULL;

    size_t address=offset;

    offset=offset+size;

    return &memory[address];
    
}
*/
