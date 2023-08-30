#include "kernel/types.h"
#include "user.h"

void prime_loop(int leftPipe[2]) {
    int num;
    read(leftPipe[0], &num, sizeof num); //读入管道左侧的第一个数字
    if(num == -1) exit(0); // 如果是35，退出
    else printf("prime %d\n", num); // 打印素数

    
    int rightPipe[2];
    pipe(rightPipe);
    int pid = fork(); // 建立子进程
    if(pid == 0) {
        //子进程
        close(leftPipe[0]); // 关闭管道左侧读端
        close(rightPipe[1]); //关闭管道右侧写端
        prime_loop(rightPipe);
    } else {
        //父进程
        //close(leftPipe[1]); （这是一个bug，导致无法输出答案，why?）
        close(rightPipe[0]); // 关闭管道右侧读端，因为用不到
        int temp;
        //开始读入管道中剩余的数，当读完后停止
        while(read(leftPipe[0], &temp, sizeof temp) && temp != -1) {
            //将不是素数倍数的数写入下一管道
            if(temp % num != 0) {
                write(rightPipe[1], &temp, sizeof temp);
            }
        }
        temp = -1;
        write(rightPipe[1], &temp, sizeof temp);
        
        wait(0);
        exit(0);
    }
}

int main(int argc,char* argv[]){
    int init_pipe[2];
    pipe(init_pipe);
    int pid = fork();
    if(pid == 0) {
        //子进程
        close(init_pipe[1]); //关闭管道写端
        prime_loop(init_pipe); // 将管道输入循环函数
        exit(0);
    } else {
        //父进程
        close(init_pipe[0]);
        int i;
        //将2-35写入到管道中
        for(i = 2; i <= 35; i++) {
            write(init_pipe[1], &i, sizeof i);
        }
        i = -1;
        write(init_pipe[1], &i, sizeof i);

        
    }
    wait(0); // 等待子进程结束
    exit(0); //确保进程退出
}