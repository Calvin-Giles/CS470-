#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 1024
#define NUM_FRAMES 3


int main (void) {
    
    const int number_pages = 4;
    int page_table[] = {5, 2, 9, 1};

    printf("Paging Config\n: ");
    printf("PAGE_SIZE = %d bytes \n", PAGE_SIZE);
    printf("NUM_FRAME = %d (physcial memory = %d bytes) \n", NUM_FRAMES, NUM_FRAMES*PAGE_SIZE);

    printf("Page Table:\n");

    for (int i = 0; i < number_pages; i++) {
        printf(" page %d -> frame %d\n", i, page_table[i]);
    }

    printf("\n");
    int n;
    printf("Enter number of logical addresses (N): ");
    if (scanf("%d", &n)!=1 || n<0){
        fprintf(stderr, "Invalid N.\n");
        return EXIT_FAILURE;
    }
    printf("Enter %d logcial addresses, one per line: \n", n);

    for (int i = 0; i<n; i++) {
        long logical;
        if (scanf("%ld", &logical)!=1){
            fprintf(stderr, "Invalid logcial address input.\n");
            return EXIT_FAILURE;
        }

        if (logical<0) {
            printf(" [#%d] logical = %ld -> INVALID LOGICAL ADDRESS\n", i, logical);
            continue;
        }

        long page = logical / PAGE_SIZE;
        long offset = logical % PAGE_SIZE;

        //address out of range for this process
        if (page >= number_pages) {
            printf(" [#%d] logical = %ld -> INVALID LOGICAL ADDRESS\n", i, logical);
            continue;
        }

        int frame = page_table[page];

        if (frame < 0) {
            printf(" [#%d] Logical = %lg -> Page %ld INVALID\n", i, logical, page);
            continue;
        }

        long physical = (long)frame * PAGE_SIZE + offset;
        printf(" [#%d] logical = %ld -> physical = %ld (frame %d, offset %ld)\n", 
                i, logical, physical, frame, offset);


    }

    return EXIT_SUCCESS;
}