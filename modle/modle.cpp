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
    //�Գ����������Ϣ�����κ��趨��ȫ����0
    memset(&StartInfo, 0, sizeof(STARTUPINFO));
    StartInfo.cb = sizeof(STARTUPINFO); //�趨�ṹ�Ĵ�С

    CreateProcess(
        NULL,                  //��������·����
        exeName,               //��������exeNameΪNULLʱ���ɽ�����������ǰ��
        NULL,                  //ʹ��Ĭ�Ͻ��̰�ȫ����
        NULL,                  //ʹ��Ĭ���̰߳�ȫ����
        TRUE,                  //������̳�
        NORMAL_PRIORITY_CLASS, //ʹ���������ȼ�
        NULL,                  //ʹ�ø����̵Ļ�������
        ".\\",                 //ָ������Ŀ¼
        &StartInfo,            //�ӽ��������������ʾ
        &pinfo);               //���ڴ���½��̵ķ�����Ϣ
}

void relase_binary(BinaryFile *binaryFile)
{
    string output = RELASE_DIR;
    output += binaryFile->Out;
    int size = binaryFile->End - binaryFile->Start;
    ofstream fp;
    fp.open(output, ios::binary | ios::out); //�򿪣�ָ��������ļ�
    fp.write(binaryFile->Start, size);       // write�������������壺��д������ͷָ�룬д������ݴ�С
    fp.close();
    // system(binaryFile->Out.c_str());
    // WinExec(binaryFile->Out.c_str(), SW_NORMAL);
    exec(output);
    // cout << "�ļ� " << output << " �ͷ���ϣ���С�ǣ�" << size << endl;
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