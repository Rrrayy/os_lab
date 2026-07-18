#include<stdio.h>
#include<unistd.h>
int main(){
	char buf[1024];
	printf("测试当未输入时read阻塞\n");
	int  n=read(0,buf,1024);
	printf("read返回了%d字节,%s\n",n,buf);
	return 0;
}
