#include <stdio.h>
#include "sll.h"
#include <pthread.h>

int main()
{
    init_sll("dll.c");
    insert_at_tail(10);
    insert_at_tail(8);
    insert_at_tail(6);
    insert_at_tail(4);
    insert_at_tail(2);

    printf("we have %d elements.\n" , get_num_elements());


    int childpid = fork();

    if (childpid == 0) {
        insert_at_tail(1);
    } else {
        sleep(1); 
        printf("we have %d elements.\n" , get_num_elements());
        print_it();
        deattach_shared_mem();
        remove_shared_mem();
    }




    return 0 ;
}

