#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<malloc.h>

int main(){
	printf("heap top: %p\n",sbrk(0));
	int *p1=malloc(16);
	printf("heap top now: %p\n",sbrk(0));
	int *p2=malloc(1024*1024);
	printf("heap top 1MB: %p\n",sbrk(0));
	mallopt(M_MMAP_THRESHOLD,1024);
	int *p3=malloc(2*1024);
	printf("p3: %p\n",p3);
	free(p1);
	free(p2);
	free(p3);
	return 0;
}

/*
rr@rr-VMware-Virtual-Platform:~/os_lab/malloc_test$ 
  strace ./malloc_demo 2>&1 | grep -E 'brk|mmap'
brk(NULL)                               = 0x5e4ae976c000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x74c1d944f000
mmap(NULL, 71343, PROT_READ, MAP_PRIVATE, 3, 0) = 0x74c1d943d000
mmap(NULL, 2170256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x74c1d9200000
mmap(0x74c1d9228000, 1605632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x74c1d9228000
mmap(0x74c1d93b0000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b0000) = 0x74c1d93b0000
mmap(0x74c1d93ff000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1fe000) = 0x74c1d93ff000
mmap(0x74c1d9405000, 52624, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x74c1d9405000
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x74c1d943a000
brk(NULL)                               = 0x5e4ae976c000
brk(0x5e4ae978d000)                     = 0x5e4ae978d000
mmap(NULL, 1052672, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x74c1d90ff000

*/
