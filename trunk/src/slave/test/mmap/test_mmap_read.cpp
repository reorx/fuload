#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <error.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
using namespace std;
unsigned long get_file_size(const char *filename)
{
    struct stat buf;
    if(stat(filename, &buf)<0)
    {
        return 0;
    }
    return (unsigned long)buf.st_size;
}


int map_read()
{
    char file_name[] = {"hello.txt"};
    int length = get_file_size(file_name);

    int fd = open(file_name, O_RDWR | O_CREAT, 0644);
    if(fd < 0)  return -1; 
    char *buf = (char *) mmap(0, length, PROT_READ, MAP_SHARED, fd, 0); 
    if(buf == NULL)
    {   
        close(fd);
        return -1; 
    }   
    close(fd);
    printf("%s\n",buf);
}
int main(int argc, const char *argv[])
{
    map_read();
    return 0;
}
