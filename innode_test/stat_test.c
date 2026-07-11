#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
int main(){
	struct stat s;
	stat("a.txt",&s);
	printf("inode: %lu\n", s.st_ino);
        printf("size: %ld\n", s.st_size);
        printf("hard links: %lu\n", s.st_nlink);
        return 0;
}
