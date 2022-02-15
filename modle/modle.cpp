#if !defined(IS_DEBUG)
#define A_BINARY_START _binary_awa_exe_start
#define A_BINARY_END _binary_awa_exe_end
#define A_BINARY_OUT "awa.exe"
#define B_BINARY_START _binary_bwb_exe_start
#define B_BINARY_END _binary_bwb_exe_end
#define B_BINARY_OUT "bwb.exe"
#define RELASE_DIR "temp\\"
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h>

using namespace std;

struct BinaryFile
{
    string Out;
    char *End;
    char *Start;
};

void exec(string str)
{
    char *exeName = new char[strlen(str.c_str()) + 1];
    strcpy(exeName, str.c_str());

    STARTUPINFO StartInfo;
    PROCESS_INFORMATION pinfo;
    //对程序的启动信息不作任何设定，全部清0
    memset(&StartInfo, 0, sizeof(STARTUPINFO));
    StartInfo.cb = sizeof(STARTUPINFO); //设定结构的大小

    CreateProcess(
        NULL,                  //启动程序路径名
        exeName,               //参数（当exeName为NULL时，可将命令放入参数前）
        NULL,                  //使用默认进程安全属性
        NULL,                  //使用默认线程安全属性
        TRUE,                  //句柄不继承
        NORMAL_PRIORITY_CLASS, //使用正常优先级
        NULL,                  //使用父进程的环境变量
        ".\\",                 //指定工作目录
        &StartInfo,            //子进程主窗口如何显示
        &pinfo);               //用于存放新进程的返回信息
}

void relase_binary(BinaryFile *binaryFile)
{
    string output = RELASE_DIR;
    output += binaryFile->Out;
    int size = binaryFile->End - binaryFile->Start;
    ofstream fp;
    fp.open(output, ios::binary | ios::out); //打开（指定）输出文件
    fp.write(binaryFile->Start, size);       // write函数各参数意义：待写入数据头指针，写入的数据大小
    fp.close();
    // system(binaryFile->Out.c_str());
    // WinExec(binaryFile->Out.c_str(), SW_NORMAL);
    exec(output);
    // cout << "文件 " << output << " 释放完毕，大小是：" << size << endl;
}

int main()
{
    extern char A_BINARY_START[];
    extern char A_BINARY_END[];

    extern char B_BINARY_START[];
    extern char B_BINARY_END[];

    BinaryFile *A = new BinaryFile();
    BinaryFile *B = new BinaryFile();

    A->Start = A_BINARY_START;
    A->End = A_BINARY_END;
    A->Out = A_BINARY_OUT;

    B->Start = B_BINARY_START;
    B->End = B_BINARY_END;
    B->Out = B_BINARY_OUT;

    system("mkdir " RELASE_DIR "");

    relase_binary(A);
    relase_binary(B);

    // system("pause");
}