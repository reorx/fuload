/*
	shmcpy.c	WJ103

	You want to be able to access the shared memory from python
*/

#include "shmcpy.h"

#include <stdlib.h>
#include <string.h>


void shmcpy_to(void *addr, char *str, int len) {
	memcpy(addr, str, len);
}

char *shmcpy_from(void *addr, int len) {
char *str;

	if ((str = (char *)malloc(len+1)) != NULL) {
		memcpy(str, addr, len);
		str[len] = 0;
	}
	return str;
}

/* EOB */
