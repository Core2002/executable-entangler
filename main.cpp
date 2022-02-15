#include <iostream>

using namespace std;

// 把文件 filename 转换成 .o 文件
string to_ofile(string filename)
{
    string command = "objcopy -I binary -O pe-x86-64 -B i386:x86-64 ";
    command += filename;
    command += " ";
    command += filename;
    command += ".o";
    string O = filename;
    O += ".o";
    system(command.c_str());
    // cout << "假装执行命令：" << command << endl;
    // cout << "得到的out文件是" << O << endl;
    return O;
}

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        cout << "使用方法： " << argv[0] << "【文件A】 【文件B】 【输出】" << endl;
        return 0;
    }

    //////////////// 首先，通过objcopy命令链接资源文件为o文件：
    // #Windows系统下（编译64位程序时）
    // objcopy -I binary -O pe-x86-64 -B i386:x86-64 资源文件名 输出的链接文件名
    // #Linux系统下（编译32位程序时）
    // objcopy -I binary -O elf32-i386 -B i386 资源文件名 输出的链接文件名
    // #Linux系统下（编译64位程序时）
    // objcopy -I binary -O elf64-x86-64 -B i386:x86-64 资源文件名 输出的链接文件名

    string A = to_ofile(argv[1]);
    string B = to_ofile(argv[2]);

    string AA(A);
    string BB(B);



    // 开始生成源文件
    system("");

    return 0;
}
