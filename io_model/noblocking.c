#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(){
	char buf[1024];
	fcntl(0,F_SETFL,O_NONBLOCK);
	while(1){
		int n=read(0,buf,1024);
		if(n>0){
			printf("已经读到数据：%s\n",buf);
		 	break;
		}
		else if(n==-1){
			if(errno==EAGAIN){
				printf("仍然没有数据:\n");
				usleep(2000000);
			}
		}
		else{
			perror("read出错");
			break;
		}
	}
	return 0;

}
