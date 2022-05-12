#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>
#include<stdbool.h>
#include<sys/sem.h>
int a[4];
int i=0;
bool k;
bool k1=false;
bool flag;
int input_num;
int mas[]={0,1,2,3,4,5,6,7,8,9,10,11,12};
int zna[]={0,0,0,0,0,0,0,0,0,0,0,0,0};
struct ii
{
	int a,b;
};
struct ii tempst;
void handler (int signo)
{	
	if(signo == SIGUSR1) zna[input_num]=1;	
	if(signo==SIGUSR2) zna[input_num]=0;
	if(signo==SIGALRM)
	{
		k1=true;
		a[0]=tempst.a;
		a[1]=tempst.b;
	}
}
void* func(void* lll)
{
	int col=mas[i];
	int ret;
	if(signal(SIGUSR2,handler)==SIG_ERR) signal(SIGUSR2,SIG_IGN);
	if(signal(SIGUSR1,handler)==SIG_ERR) signal(SIGUSR1,SIG_IGN);
	if(signal(SIGALRM,handler)==SIG_ERR) signal(SIGALRM,SIG_IGN);
	
	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL)) perror("state...");
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL))perror("type...");
	printf("%d)",col);
	printf("thread creat\n");
	for(int i=0;;i++)
	{
		ualarm(500,0);
		for(int T=0;;T++)
		{
			if(k1==true)
			{
				k1=false;
				break;
			}
			if(T%2==0){
			tempst.a=0;
			tempst.b=0;
			}
			else{
			tempst.a=1;
			tempst.b=1;
			}
			a[2]=tempst.a;
			a[3]=tempst.b;
		}
		
		if(zna[col]==1 && i%1000==0)
		{
			printf("stat.| %d %d %d %d |",a[0],a[1],a[2],a[3]);
			printf("TID %d        %d\n",pthread_self(),col);
		}
	}
	
}

int main()
{
	pthread_t pth[12];
	char a;
	int ret;
	struct timespec pec;
	pec.tv_sec=0;
	pec.tv_nsec=5000;
	for(;;)
	{
		a=getchar();
		if(a=='+' && i<12)
		{
			pthread_create(&pth[i],NULL,func,&mas[i]);
			nanosleep(&pec,NULL);
			i++;
		}
		else if(a=='-' && i>0)
		{
			i--;
			if(pthread_cancel(pth[i])) perror("pthread_cancel...");
		}
		else if(a=='k'&& i>0)
		{
			for(int j=0;j<i;j++)
			{
				if(pthread_cancel(pth[j])) perror("pthread_cancel...");
			}
			i=0;
		}
		else if(a=='g' && i>0)
		{
			input_num=0;
			for(int j=0;j<i;j++)
			{
				if(pthread_kill(pth[j],SIGUSR1)) perror("pthread_kill ...");
				nanosleep(&pec,NULL);
				input_num++;
			}
		}
		else if(a=='s' && i>0)
		{
			input_num=0;
			for(int j=0;j<i;j++)
			{
				if(pthread_kill(pth[j],SIGUSR2)) perror("pthread_kill ...");
				nanosleep(&pec,NULL);
				input_num++;
			}
		}
		else if(a=='G')
		{
			printf("\ninput number pthread\n");
			scanf("%d",&input_num);
			if(pthread_kill(pth[input_num],SIGUSR1)) perror("pthread_kill ...");
		}
		else if(a=='S')
		{
			printf("\ninput number pthread\n");
			scanf("%d",&input_num);
			if(pthread_kill(pth[input_num],SIGUSR2)) perror("pthread_kill ...");
		}
		
		else if(a=='q') return 0;
	}
	return 0;
}