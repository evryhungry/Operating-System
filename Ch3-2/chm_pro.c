#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(){
    const int SIZE = 4096;
    const char *name = "OS";
    const char *message0 = "Studing ";
    const char *message1 = "Operating System  ";
    const char *message2 = "Is Fun! \n";

    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    ftruncate(shm_fd, SIZE);

    ptr = (char*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED){
        printf("Map failed\n");
        return -1;
    }

    sprintf(ptr,"%s",message0);
	ptr += strlen(message0);
	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);
	sprintf(ptr,"%s",message2);
	ptr += strlen(message2);

    return 0;
}