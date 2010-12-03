//
// ipc.i WJ103
//

%module ipc
%{
#include "ipc.h"
%}

// Produce constants and helper functions for structures and unions
%include "ipc.h"

int ftok(char *, char);

int shmget(int, int, int);
void *shmat(int, void *, int);
int shmdt(void *);
int shmctl(int, int, struct shmid_ds *);

int semget(int, int, int);
int semctl(int, int, int, union semun);
int semop(int, struct sembuf *, unsigned int);

int msgget(int, int);
int msgctl(int, int, struct msqid_ds *);
int msgsnd(int, struct msgbuf *, int, int);
int msgrcv(int, struct msgbuf *, int, long, int);

%inline
%{
%}

