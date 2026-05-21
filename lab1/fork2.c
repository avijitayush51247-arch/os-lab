#include<stdio.h>
#include<unistd.h>
int main(void){
pid_t pid = fork();
if (pid ==0){
printf(" I am the child. My PID IS %d, parent is %d\n", getpid(), getppid());}
else if(pid>0){
printf("I am the parent .My PID is %d, child is %D\n",getpid(), pid);
}
else {
perror("fork Failed");
}
return 0;
}
