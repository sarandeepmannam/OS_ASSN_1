#include <iostream>  
using namespace std;  
//struct Process represents a process and contains attributes of a process
struct Process
{
int pid;//pid of the process
int cpu_burst;//processing time
int period;//period of the process
int count;//To keep track of no of iterations
int nst;//new start time
int ndl;//new dead line
int priority;// priority of the process
int time_left;//processing time left
int count123;//another variable to store no of iterations
int wait_time;//total wait time
};

typedef struct Process process;
// function to sort an array of processes according to their periods
process* arrange(process pro[],int n)
{
  //if(pro[])
  int i, j;
  process key;
    for (i = 1; i < n; i++)
    {
        key = pro[i];
        j = i - 1;
        while (j >= 0 && pro[j].period> key.period)
        {
            pro[j + 1] = pro[j];
            j = j - 1;
        }
        pro[j + 1] = key;
    }
    return pro;
}
// function that gives the highest priority process (process that have smallest period). If no process available it returns a invalid process with pid=-1
process more_priority(process pro[],int n,int time)
{
  process p1;
  p1.pid=-1;
  for(int i=n-1;i>=0;i--)
  {
    if(pro[i].nst<=time &&  pro[i].count>0)
    {
      p1=pro[i];
    }
  }
  return p1;
}
//Function to check whether all the iterations of all processes are completed.
bool check_complete(process p1[],int n)
{
  int k=0;
  for(int i=0;i<n;i++)
  {
    if(p1[i].count>0)
    {
      k=1;
    }
  }
  if(k==0)
  {
    return 1;
  }
  else
  {
  return 0;
  }
}

int main()  
{ 
  FILE* fp;
  FILE* fp1;
  FILE* fp2;
  process p;
  char name[20];
  cout<<"Enter the name of the input file:"<<endl;
  scanf("%s",name);
  fp=fopen(name,"r");
  if(fp==NULL)
  {
    cout<<"File opening was unsuccessful\n";
    return EXIT_SUCCESS;
  }
  if(fp!=NULL)
  {
    cout<<"File "<<name<<" successfully opened\n";
  }
  int n;
  fscanf(fp,"%d",&n);
  int total=0;
  process pro[n];
  int count1=0;//no of process successfully completed
int count2=0;// no of process teminated
  for(int i=0;i<n;i++)
  {
    fscanf(fp,"%d %d %d %d",&pro[i].pid,&pro[i].cpu_burst,&pro[i].period,&pro[i].count);
    total=total+pro[i].count;
  }
  fclose(fp);
  fp1=fopen("RMS-log.txt","w");
  arrange(pro,n);
   for(int i=0;i<n;i++)
   {
    fprintf(fp1,"process %d processesingtime:%d; deadline:%d; period:%d joined the system at time 0\n",pro[i].pid,pro[i].cpu_burst,pro[i].period,pro[i].period);
    pro[i].nst=0;
    pro[i].ndl=pro[i].period;
    pro[i].priority=i+1;
    pro[i].time_left=pro[i].cpu_burst;
    pro[i].count123=pro[i].count;
    pro[i].wait_time=0;
   }
 int time=0;
 int t;
 fprintf(fp1,"process %d starts execution at time %d\n",pro[0].pid,time);
 p=pro[0];
 while(check_complete(pro,n)!=1)
 {
  t=p.priority;
  for(int i=0;i<n;i++)
  {
    if(time>pro[i].nst && i!=t-1 && pro[i].count>0)
    {
      pro[i].wait_time++;
    }
  }
  for(int i=0;i<n;i++)
  {
    if(time>pro[i].ndl && i!=t-1 && pro[i].count>0)
    {
      fprintf(fp1,"process %d missed the deadline at time %d\n",pro[i].pid,time-1);
      count2++;
      pro[i].count--;
      pro[i].time_left=pro[i].cpu_burst;
      pro[i].nst=pro[i].nst+pro[i].period;
      pro[i].ndl=pro[i].ndl+pro[i].period;
    }
  }
  
  if(time+p.time_left>p.ndl)
  {
   fprintf(fp1,"process %d is terminated at time %d\n",p.pid,--time);
   count2++;
   pro[t-1].count--;
   pro[t-1].time_left=pro[t-1].cpu_burst;
   p.time_left=p.cpu_burst;
   pro[t-1].nst=pro[t-1].nst+pro[t-1].period;
   p.nst=p.nst+p.period;
   pro[t-1].ndl=pro[t-1].ndl+pro[t-1].period;
   p.ndl=p.ndl+p.period;
   process p1=more_priority(pro,n,time);
   if(p1.pid==-1)
   {
     if(more_priority(pro,n,time+1).pid!=-1)
     {
      fprintf(fp1,"cpu is idle until time %d\n",time);
     }
      time++;
     continue;
   }
   else 
   {
    if(p1.time_left==p1.cpu_burst)
    {
      p=p1;
      int k=p.priority;
      fprintf(fp1,"processs %d starts at time %d\n",p.pid,time);
      pro[k-1].time_left--;
      p.time_left--;
    }
    else
    {
      p=p1;
      int k=p.priority;
      fprintf(fp1,"processs %d resumes at time %d\n",p.pid,time);
      pro[k-1].time_left--;
      p.time_left--;
    }
   }
  }
  else
  {
   if(pro[t-1].time_left==0)
   {
    fprintf(fp1,"process %d finshes execution at time %d\n",pro[t-1].pid,time);
    count1++;
    if(pro[t-1].count>0)
    {
     pro[t-1].count--;
     pro[t-1].time_left=pro[t-1].cpu_burst;
     p.time_left=p.cpu_burst;
     pro[t-1].nst=pro[t-1].nst+pro[t-1].period;
     p.nst=p.nst+p.period;
     pro[t-1].ndl=pro[t-1].ndl+pro[t-1].period;
     p.ndl=p.ndl+p.period;
    }
   }
   process p1=more_priority(pro,n,time);
   if(p1.pid==-1)
   {
     if(more_priority(pro,n,time+1).pid!=-1)
     {
      fprintf(fp1,"cpu is idle until time %d\n",time);
     }
      time++;
     continue;
   }
   else if(p1.pid!=p.pid)
   {
    if(p.time_left==p.cpu_burst )
    {
      p=p1;
      int k=p.priority;
     if(p.time_left==p.cpu_burst)
     {
      pro[k-1].time_left--;
      p.time_left--;
      fprintf(fp1,"process %d starts execution at time %d\n",p.pid,time);
     }
     else
     {
     pro[k-1].time_left--;
      p.time_left--;
     fprintf(fp1,"process %d resumes execution at time %d\n" ,p.pid,time);
     }
    }
    else
    {
     fprintf(fp1,"process %d is preempted by process %d at time %d,remaining processing time %d\n",p.pid,p1.pid,time,p.time_left);
     p=p1;
     int k=p.priority;
     if(p.time_left==p.cpu_burst)
     {
      p=p1;
      fprintf(fp1,"process %d starts execution at time %d\n",p.pid,time);
      pro[k-1].time_left--;
      p.time_left--;
     }
     else
     {
      pro[k-1].time_left--;
      p.time_left--;
     fprintf(fp1,"process %d resumes execution at time %d\n",p.pid,time);
     }
    }
   }
   else
   {
    if((p.time_left==p.cpu_burst && more_priority(pro,n,time-1).pid==-1 && time>0 )|| (p.time_left==p.cpu_burst && more_priority(pro,n,time-1).pid==more_priority(pro,n,time-1).pid &&  more_priority(pro,n,time-1).pid!=-1))
    {
      fprintf(fp1,"process %d starts execution at time %d\n",p.pid,time);
    }
    pro[t-1].time_left--;
    p.time_left--;
   }
  }
 time++;
 }
fclose(fp1);
fp2=fopen("RMS-stats.txt","w");
fprintf(fp2,"total number of processes:%d\n",total);
fprintf(fp2,"no of processes that are successfully completed are %d\n",count1);
fprintf(fp2,"no of processes that are terminated are %d\n",count2);

float avg;
float avg1[n];
float total_time=0;
for(int i=0;i<n;i++)
{ avg1[i]=float(pro[i].wait_time)/float(pro[i].count123);
  fprintf(fp2,"average wait time of process %d is %f\n",pro[i].pid,avg1[i]);
}
for(int i=0;i<n;i++)
  {
    total_time=total_time+pro[i].wait_time;
  }
avg=total_time/float(total);
fprintf(fp2,"Total average wait is %f\n",avg);
fclose(fp2);
  return 0;  
}  