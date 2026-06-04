# Memory_Allocator_C_project

A First-fit Dynamic Memory Allocator containing the following:
1. Fixed-size heap
2. Metadata headers
3. Linked list of blocks
4. Allocation
5. Free
6. Reuse of freed blocks
7. Block splitting
8. Coalescing
9. Fragmentation awareness

## Output
Here is a sample output:<br>
<img width="518" height="1012" alt="6_sample_test_case" src="https://github.com/user-attachments/assets/6c26ad92-772b-46cd-9e83-3f2453584ae3" /><br><br>
Here is a sample output showcasing error handling:<br>
<img width="518" height="660" alt="2_double_free" src="https://github.com/user-attachments/assets/623dbdb4-f5d9-415c-b091-f03f6433aea0" /><br>
*More test case samples are present in '2_output_images' folder*

## Concepts used:
### A. Heap Management

### B. Metadata Design & Memory Layout
1. Using data structures to form memory blocks

### C. Pointer Manipulation

### D. Linked Lists

### E. Tradeoff Analysis
1. Metadata storage or address computation for free()
2. Alignment for strict standard portability or Embedded systems 

### F. Allocator Robustness
Handling: 
1. Invalid free
2. Double free
3. Allocation failure

