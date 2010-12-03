/*
	mkmsgbuf.c	WJ103
*/

#include "mkmsgbuf.h"

#include <stdlib.h>
#include <string.h>

struct msgbuf *mkmsgbuf(int mtype, char *msg) {
struct msgbuf *mb;

	if (mtype < 0)
		return NULL;

	if (msg == NULL)
		return NULL;

	if ((mb = (struct msgbuf *)malloc(sizeof(struct msgbuf)+strlen(msg))) == NULL)
		return NULL;

	mb->mtype = mtype;
	strcpy(mb->mtext, msg);
	return mb;
}

/* EOB */
