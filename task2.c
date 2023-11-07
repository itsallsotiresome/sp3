#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>


struct datum {
    pid_t pid;
    struct tm current_timestamp;
    char buffer[256];
};



int main(void) {
    int fd;
    const int buffer_size = 256;
    char buffer[buffer_size];
    struct datum *dtm = NULL;
    time_t current_timestamp;

    fd = shm_open("my_shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    if (ftruncate(fd, sizeof(struct datum)) != 0) {
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    dtm = mmap(NULL, sizeof(struct datum), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (dtm == MAP_FAILED) {
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }

    while (1) {
        printf("You entered:\n");
        fgets(buffer, buffer_size, stdin);
        msync(dtm, sizeof(struct datum), MS_SYNC);
        printf("Pid: %d\n", dtm->pid);
        printf("Time: %s\n", asctime(&(dtm->current_timestamp)));
        printf("String: %s\n", dtm->buffer);
        dtm->pid = getpid();
        time(&current_timestamp);
        dtm->current_timestamp = (*localtime(&current_timestamp));
        strcpy(dtm->buffer, buffer);
    }
}