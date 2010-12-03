/*
	ipc.h	WJ103

	This file is a rip-off of the standard includes:
	/usr/include/sys/types.h
	/usr/include/sys/ipc.h
	/usr/include/sys/shm.h
	/usr/include/bits/shm.h
	etc.

	If things do not work, check for differences between those files
	and this one. I'm sorry, I could not get it done in any other way...
*/

#include <sys/types.h>

#define IPC_CREAT   01000			/* Create key if key does not exist. */
#define IPC_EXCL    02000			/* Fail if key exists.  */
#define IPC_NOWAIT  04000			/* Return error on wait.  */

/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#define IPC_RMID    0				/* Remove identifier.  */
#define IPC_SET     1				/* Set `ipc_perm' options.  */
#define IPC_STAT    2				/* Get `ipc_perm' options.  */
#define IPC_INFO    3				/* See ipcs.  */

/* Special key values.  */
#define IPC_PRIVATE ((key_t)0)		/* Private key.  */


/* Permission flag for shmget.  */
#define SHM_R		0400		/* or S_IRUGO from <linux/stat.h> */
#define SHM_W		0200		/* or S_IWUGO from <linux/stat.h> */

/* Flags for `shmat'.  */
#define SHM_RDONLY	010000		/* attach read-only else read-write */
#define SHM_RND		020000		/* round attach address to SHMLBA */
#define SHM_REMAP	040000		/* take-over region on attach */

/* Commands for `shmctl'.  */
#define SHM_LOCK	11		/* lock segment (root only) */
#define SHM_UNLOCK	12		/* unlock segment (root only) */

/* ipcs ctl commands */
#define SHM_STAT 	13
#define SHM_INFO 	14

/* shm_mode upper byte flags */
#define SHM_DEST	01000	/* segment will be destroyed on last detach */
#define SHM_LOCKED	02000   /* segment will not be swapped */


/* Permission struct */
struct ipc_perm {
	key_t __key;					/* Key.  */
	unsigned short int uid;			/* Owner's user ID.  */
	unsigned short int gid;			/* Owner's group ID.  */
	unsigned short int cuid;		/* Creator's user ID.  */
	unsigned short int cgid;		/* Creator's group ID.  */
	unsigned short int mode;		/* Read/write permission.  */
	unsigned short int __seq;		/* Sequence number.  */
};

/* Data structure describing a set of semaphores.  */
struct shmid_ds
  {
    struct ipc_perm shm_perm;		/* operation permission struct */
    int shm_segsz;			/* size of segment in bytes */
	time_t shm_atime;			/* time of last shmat() */
    time_t shm_dtime;			/* time of last shmdt() */
    time_t shm_ctime;			/* time of last change by shmctl() */
    pid_t shm_cpid;				/* pid of creator */
    pid_t shm_lpid;				/* pid of last shmop */
    unsigned short int shm_nattch;	/* number of current attaches */
    unsigned short int __shm_npages;	/* size of segment (pages) */
    unsigned long int *__shm_pages;	/* array of ptrs to frames -> SHMMAX */
    void *__attaches;				/* descriptors for attaches */
  };

struct shminfo {
    int shmmax;
    int shmmin;
    int shmmni;
    int shmseg;
    int shmall;
};

struct shm_info {
    int used_ids;
    unsigned long int shm_tot;	/* total allocated shm */
    unsigned long int shm_rss;	/* total resident shm */
    unsigned long int shm_swp;	/* total swapped shm */
    unsigned long int swap_attempts;
    unsigned long int swap_successes;
};



/* Flags for `semop'.  */
#define SEM_UNDO	0x1000		/* undo the operation on exit */

/* Commands for `semctl'.  */
#define GETPID		11		/* get sempid */
#define GETVAL		12		/* get semval */
#define GETALL		13		/* get all semval's */
#define GETNCNT		14		/* get semncnt */
#define GETZCNT		15		/* get semzcnt */
#define SETVAL		16		/* set semval */
#define SETALL		17		/* set all semval's */


/* Data structure describing a set of semaphores.  */
struct semid_ds
{
  struct ipc_perm sem_perm;		/* operation permission struct */
  time_t sem_otime;			/* last semop() time */
  time_t sem_ctime;			/* last time changed by semctl() */
  struct sem *__sembase;		/* ptr to first semaphore in array */
  struct sem_queue *__sem_pending;	/* pending operations */
  struct sem_queue *__sem_pending_last; /* last pending operation */
  struct sem_undo *__undo;		/* ondo requests on this array */
  unsigned short int sem_nsems;		/* number of semaphores in set */
};

/*
	According to X/Open you have to define the union semun yourself.
	What the crap is that all about anyway??? It's probably just because
	of its name. Well, here it is. I define the semun myself. Ha!

		--Walter
*/
union semun {
     int val;				/* value for SETVAL */
     struct semid_ds *buf;		/* buffer for IPC_STAT & IPC_SET */
     unsigned short int *array;		/* array for GETALL & SETALL */
     struct seminfo *__buf;		/* buffer for IPC_INFO */
};

/* ipcs ctl cmds */
#define SEM_STAT 18
#define SEM_INFO 19

struct  seminfo
{
  int semmap;
  int semmni;
  int semmns;
  int semmnu;
  int semmsl;
  int semopm;
  int semume;
  int semusz;
  int semvmx;
  int semaem;
};


/*
	Note: use the helper function mkmsgbuf() to create a message buffer
	This structure is merely used for typecasting a larger object
*/
struct msgbuf
  {
    long int mtype;		/* type of received/sent message */
    char mtext[1];		/* text of the message */
  };


/* Define options for message queue functions.  */
#define MSG_NOERROR	010000	/* no error if message is too big */
#define MSG_EXCEPT	020000	/* recv any msg except of specified type */


/* Structure of record for one message inside the kernel.
   The type `struct msg' is opaque.  */
struct msqid_ds
{
  struct ipc_perm msg_perm;	/* structure describing operation permission */
  struct msg *__msg_first;	/* pointer to first message on queue */
  struct msg *__msg_last;	/* pointer to last message on queue */
  time_t msg_stime;		/* time of last msgsnd command */
  time_t msg_rtime;		/* time of last msgrcv command */
  time_t msg_ctime;		/* time of last change */
  void *__wwait;	/* ??? */
  void *__rwait;	/* ??? */
  unsigned short int __msg_cbytes;/* current number of bytes on queue */
  unsigned short int msg_qnum;	/* number of messages currently on queue */
  unsigned short int msg_qbytes;/* max number of bytes allowed on queue */
  pid_t msg_lspid;	/* pid of last msgsnd() */
  pid_t msg_lrpid;	/* pid of last msgrcv() */
};

#define msg_cbytes	__msg_cbytes

/* ipcs ctl commands */
#define MSG_STAT 11
#define MSG_INFO 12

/* buffer for msgctl calls IPC_INFO, MSG_INFO */
struct msginfo {
    int msgpool;
    int msgmap;
    int msgmax;
    int msgmnb;
    int msgmni;
    int msgssz;
    int msgtql;
    unsigned short int msgseg;
};

/* EOB */
