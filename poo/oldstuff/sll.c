#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "sll.h"
#define MAX_SHM_ID_NUM 100

int shm_id_arr[MAX_SHM_ID_NUM];
int current_index = -1 ;

node * my_head = NULL;
int * num_of_elements = NULL ;

char initialized_yet = 0;
key_t* key_first = 0;
key_t* key_current;

int get_num_elements()
{
    return (* num_of_elements);
}

int init_sll(char* sll_name)
{
	/* make the key: */
    if ((key_first = ftok(sll_name, 'R')) == -1) {
        perror("ftok error");
        return -1;
    }

    initialized_yet = 1;

	num_of_elements = (int *)my_malloc(sizeof(int));
    (* num_of_elements) = 0 ;

    return 0;
}

void *  my_malloc(int size)
{
	if(initialized_yet!=1)
	{
		perror("sll not yet initialized");
		return NULL;
	}

	void * ptr = NULL;
    key_current = key_first ++;
    int shm_id;
    if ((shm_id = shmget(key_current, size , IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");printf("errno= %d EINVAL=%d \n ", errno , EINVAL);
       return NULL;
    }

    if ((ptr = shmat(shm_id, NULL, 0)) == (void *) - 1) {
        perror("shmat error");
        //exit(1);
        return NULL;

    }
    current_index ++ ;
    shm_id_arr[current_index] = shm_id ;
    return ptr;

}

void insert_at_tail(int num)
{
    if(my_head == NULL)
    {
        my_head = my_malloc(sizeof(node));
        my_head->val = num;
        my_head->next = NULL;
        my_head->prev = NULL;

    }else
    {
        node * tmp = my_head;
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = my_malloc(sizeof(node));
        tmp->next->val = num;
        tmp->next->next = NULL;
        tmp->next->prev = tmp;
    }
    (* num_of_elements) ++;
}


//void destroy_sll()
/* deAttach the shared memory without removing. */
void deattach_shared_mem()
{

   if (shmdt(num_of_elements) < 0) { /*  deAttach  num_of_elements */
       perror("shmdt error num_of_elements\n");

   }

   if (shmdt(my_head) < 0) {
       perror("shmdt error my_head\n");
   }


   //how to deattach all pointers in list?
}
void remove_shared_mem()
{
    int i;
    for(i = 0 ; i < current_index ; i ++)
    {
        if (shmctl(shm_id_arr[i], IPC_RMID, NULL) < 0) { /* remove the shared memory segment. */
            perror("shmctl error.\n");

        }
    }

}


int my_free_detach(void* ptr, unsigned int num)
{
	if (shmdt(ptr) < 0) {
       perror("shmdt error my_head\n");
  	}
    if (shmctl(shm_id_arr[num], IPC_RMID, NULL) < 0) { //remove
       perror("shmctl error.\n");
    }
    return 0;
}

void print_it()
{
    node * tmp = my_head;
    while(tmp != NULL)
    {
        printf("%d\n" , tmp->val);
        tmp = tmp->next;
    }
}