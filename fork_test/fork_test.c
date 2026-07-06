#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(){
	int x=100;
	printf("(pid:%d)\n",(int)getpid());
	int rc=fork();
	if(rc<0){
		fprintf(stderr,"fork error\n");
		exit(1);
	}
	else if(rc==0){
		x=200;
		printf("(pid:%d)\n",(int)getpid());
		printf("%d,%p\n",x,(void*)&x);
	}
	else{
		wait(NULL);
		x=300;
		printf("(pid:%d)\n",(int)getpid());
		printf("%d,%p\n",x,(void*)&x);
	}
	return 0;
}
