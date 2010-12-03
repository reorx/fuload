//
//	ipc.i	WJ103
//

%module ipc
%{
#include "ipc.h"
%}

// Produce constants and helper functions for structures and unions
%include "ipc.h"

key_t ftok(char *, char);

int shmget(key_t, int, int);
void *shmat(int, void *, int);
int shmdt(void *);
int shmctl(int, int, struct shmid_ds *);

int semget(key_t, int, int);
int semctl(int, int, int, union semun);
int semop(int, struct sembuf *, unsigned int);

int msgget(key_t, int);
int msgctl(int, int, struct msqid_ds *);
int msgsnd(int, struct msgbuf *, int, int);
int msgrcv(int, struct msgbuf *, int, long, int);

%include "shmcpy.h"
%include "mkmsgbuf.h"

%inline
%{
%}

// EOB
