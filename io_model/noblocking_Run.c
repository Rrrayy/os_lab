 #include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(){
        char buf[1024];
	int count=0;
        fcntl(0,F_SETFL,O_NONBLOCK);
        while(1){
                int n=read(0,buf,1024);
                if(n>0){
                        printf("已经读到数据：%s\n",buf);
                        break;
                }
		count++;
		if(count%100000==0){
			printf("已经轮询了%d次，CPU拉满！\n",count);
		}
        }
        return 0;

}


