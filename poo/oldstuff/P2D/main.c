/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2018 ​by​ Zach Farmer
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ is permitted under the Zach Literally Could Not Care Less If You 
 * Paid Him To License and GNU GPL.
 *
 * ​Zach Farmer ​is not liable for any misuse of this material.
​ ​*
*****************************************************************************/
/**
​ ​*​ ​@file​ ​main.c
​ ​*​ ​@brief​ ​Shared Memory IPC Example
​ ​*
​ ​*​ ​This​ ​is an example of how shared memory (mmap) can be used for IPC
 * (Inter-process communication)
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 3 2018
​ ​*​ ​@version​ ​69
​ ​*
​ ​*/
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

struct poo;
struct node
{
   node* prev;
   int val;
}

void add(int val)

void fillshm(int shmid) {
   node *p;

   if ( (p = shmat (shmid, NULL, 0)) < 0 )
      {
      perror("shmat");
      exit(1);
      }

   printf("writing to shared memory\n");
   strcpy(p->a, "my shared memory");
   p->i = 123;
   p->j = 456;
}


void printshm(int shmid)
{
   somestruct *p;
   if ( (p = shmat (shmid, NULL, 0)) < 0 )
      {
      perror("shmat");
      exit(1);
      }

   printf( "%s, %d, %d\n", p->a, p->i, p->j );
}

int main( int argc, char *argv[] ) {

   int shmid;

   // see if the memory exists and print it if so
   if ( (shmid = shmget (1234, 0, 0)) >= 0 )
      printshm( shmid );
   else
      {
      // didn't exist, so create it
      if ( (shmid = shmget (1234, sizeof( somestruct ), IPC_CREAT | 0600)) < 0 )
         {
         perror("shmget");
         exit(1);
         }

      printf( "shmid = %d\n", shmid );

      fillshm(shmid);
      printf( "Run another instance of this app to read the memory... (press a key): " );
      getchar();

      // delete it
      if ( shmctl (shmid, IPC_RMID, NULL) < 0 )
         {
         perror("semctl");
         exit(1);
         }
      }

   return 0;
}