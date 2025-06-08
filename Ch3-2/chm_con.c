#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const int SIZE = 4096;
	const char *name = "OS";

	int shm_fd;
	void *ptr;
    int i;

    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1){
        printf("shared memory failed\n");
        exit(-1);
    }

	/* now map the shared memory segment in the address space of the process */
	ptr = (char *)mmap(0,SIZE, PROT_READ , MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

    printf("%s\n", (char*)ptr);

    if(shm_unlink(name) == -1){
        printf("Error removing %s\n", name);
        exit(-1);
    }

	return 0;
}
