#include "kernel/types.h"
#include "user.h"
#include "kernel/param.h"

int main(int argc,char* argv[]){
    char* argvTemp[MAXARG];
    int i;
    // 将第一行的1到最后一个参数保存到argvTemp中
      // 便于后续调用exec
    for(i = 1; i < argc; i++) {
        argvTemp[i - 1] = argv[i];
    }

    while(1) {
        int index = 0;
        char sentence[512];
        while(1) {
            char letter;
            read(0, &letter, sizeof letter); // 读取每一个字符
            // 向sentence中存入每一个字符
              // 当读到换行符时，意味着这一行已经读完
                // 为sentence补上结尾符'\0'
            if(letter == '\n') {
                sentence[index] = '\0';
                break;
            } else {
                sentence[index++] = letter;
            }
        }
        if(index == 0) break;
        else {
            index = 0; //将index归零
            argvTemp[argc - 1] = sentence; // 向argvTemp中输入读到的一行 
            argvTemp[argc] = 0; // 将argvTemp的最后一项置为0
            // 子线程执行exec命令
                // 父线程等待子线程执行结束
            int pid = fork();
            if(pid == 0) {
                //子线程
                exec(argv[1], argvTemp);
            } else {
                //父线程
                wait(0);
            }
        }
    }
    exit(0); //确保进程退出
}