
#include<stdio.h>
#include<unistd.h>
#define MAX 100
struct Process
{
	int pid;
	int arr_time;
	int run_time;
	int priority;
	int rev_time;
	int cur_level;
};
void FCFS(struct Process arr[],int n){
	for(int i=0;i<n-1;++i)
	{
		for(int j=0;j<n-1-i;++j)
		{
			if(arr[j].arr_time>arr[j+1].arr_time){
				struct Process temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
	int x=0;
	for (int i=0;i<n;++i)
	{
		printf("时间 %d--%d: P%d\n",x,x+arr[i].run_time,arr[i].pid);
		x+=arr[i].run_time;
	}
}
void RR (struct Process arr[],int n)
{
	int slice=2;
	int cur_time=0;
	int finished=0;
	int  entered[MAX]={0};
	struct Process queue[MAX];
	int head=0;
	int tail=0;
	while(finished<n){
		for(int i=0;i<n;++i){
			if(!entered[i]&&arr[i].arr_time<=cur_time)
			{
				queue[tail]=arr[i];
				tail=(tail+1)%MAX;
				entered[i]=1;
			}
		}
		if(head!=tail){
			int cur_pid=queue[head].pid;
			int time=slice<queue[head].rev_time?slice:queue[head].rev_time;
			queue[head].rev_time-=time;
			printf("时间 %d-%d: P%d\n", cur_time, cur_time+time, cur_pid);
                        cur_time+=time;
			if(queue[head].rev_time==0){
				finished++;
				head=(head+1)%MAX;
			}

			else{
				queue[tail]=queue[head];
				tail=(tail+1)%MAX;
				head=(head+1)%MAX;
			}
		}
		else
			cur_time++;
	}
}
void MLFQ(struct Process arr[],int n){
	struct Process q0[MAX],q1[MAX],q2[MAX];
	int head[3]={0,0,0};
	int tail[3]={0,0,0};
	int slice[3]={2,4,8};
        int finished=0;
	int cur_time=0;
	int cur_level=0;
	int  entered[MAX]={0};
        while(finished<n){
                for(int i=0;i<n;++i){
                        if(!entered[i]&&arr[i].arr_time<=cur_time)
                        {
                                q0[tail[0]]=arr[i];
                                tail[0]=(tail[0]+1)%MAX;
                                entered[i]=1;
                        }
                }
		if(head[0]!=tail[0]){
			int cur_pid=q0[head[0]].pid;
			cur_level=0;
			int time=slice[0]<q0[head[0]].rev_time?slice[0]:q0[head[0]].rev_time;
			q0[head[0]].rev_time-=time;
			if(q0[head[0]].rev_time==0){
				finished++;
				head[0]=(head[0]+1)%MAX;
			}
			else{
				cur_level=cur_level<2?cur_level++:2;
				q1[tail[1]]=q0[head[0]];
				tail[1]=(tail[1]+1)%MAX;
				head[0]=(head[0]+1)%MAX;
			}
			printf("时间 %d--%d: P%d (队列%d)\n",cur_time,cur_time+time,cur_pid,0);
			cur_time+=time;
		}
		else if(head[1]!=tail[1]){
			int cur_pid=q1[head[1]].pid;
			cur_level=1;
                        int time=slice[1]<q1[head[1]].rev_time?slice[1]:q1[head[1]].rev_time;
                        q1[head[1]].rev_time-=time;
                        if(q1[head[1]].rev_time==0){
                                finished++;
                                head[1]=(head[1]+1)%MAX;
                        }
                        else{
                                cur_level=cur_level<2?cur_level++:2;
                                q2[tail[2]]=q1[head[1]];
                                tail[2]=(tail[2]+1)%MAX;
                                head[1]=(head[1]+1)%MAX;
                        }
			printf("时间 %d--%d: P%d (队列%d)\n",cur_time,cur_time+time,cur_pid,1);
                        cur_time+=time;

		}
		else if(head[2]!=tail[2]){
			int cur_pid=q2[head[2]].pid;
			cur_level=2;
                        int time=slice[2]<q2[head[2]].rev_time?slice[2]:q2[head[2]].rev_time;
                        q2[head[2]].rev_time-=time;
                        if(q2[head[2]].rev_time==0){
                                finished++;
                                head[2]=(head[2]+1)%MAX;
                        }
                        else{
                                q2[tail[2]]=q2[head[2]];
                                tail[2]=(tail[2]+1)%MAX;
                                head[2]=(head[2]+1)%MAX;
                        }
			printf("时间 %d--%d: P%d (队列%d)\n",cur_time,cur_time+time,cur_pid,2);
                        cur_time+=time;

		}
		else
			cur_time++;
	}
}
int main(){	
	struct Process procs[]=
	{
		{1,0,8,0,8},{2,1,4,0,4},{3,2,9,0,9},{4,3,5,0,5}
	};
	int n=4;
	printf("-------------------------------------------------------\n");
	FCFS(procs,n);
	printf("-------------------------------------------------------\n");
	RR(procs,n);
	printf("-------------------------------------------------------\n");
	MLFQ(procs,n);
	return 0;
}
