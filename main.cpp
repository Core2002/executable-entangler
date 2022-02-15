#include <iostream>
#include <sstream>

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

string &replace_all_distinct(string &str, const string &old_value, const string &new_value)
{
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
    {
        if ((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
    return str;
}

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        cout << "使用方法： " << argv[0] << "【文件A_Out】 【文件B_Out】 【输出】" << endl;
        system("pause");
        return 0;
    }

    //////////////// 首先，通过objcopy命令链接资源文件为o文件：
    // #Windows系统下（编译64位程序时）
    // objcopy -I binary -O pe-x86-64 -B i386:x86-64 资源文件名 输出的链接文件名
    // #Linux系统下（编译32位程序时）
    // objcopy -I binary -O elf32-i386 -B i386 资源文件名 输出的链接文件名
    // #Linux系统下（编译64位程序时）
    // objcopy -I binary -O elf64-x86-64 -B i386:x86-64 资源文件名 输出的链接文件名

    string A_Out = to_ofile(argv[1]);
    string B_Out = to_ofile(argv[2]);

    string A_O = string(A_Out);
    string B_O = string(B_Out);

    A_Out = replace_all_distinct(A_Out, ".o", "");
    B_Out = replace_all_distinct(B_Out, ".o", "");

    // 生成参数
    string A_Start, B_Start, A_End, B_End;
    A_Start = "_binary_";
    B_Start = "_binary_";
    A_Start += string(A_Out);
    B_Start += string(B_Out);

    A_Start = replace_all_distinct(A_Start, ".", "_");
    A_Start += "_start";
    A_End = string(A_Start);
    A_End = replace_all_distinct(A_End, "_start", "_end");

    B_Start = replace_all_distinct(B_Start, ".", "_");
    B_Start += "_start";
    B_End = string(B_Start);
    B_End = replace_all_distinct(B_End, "_start", "_end");

    // 生成执行队列
    int commands_len = 11;
    string commands[commands_len];
    stringstream ss;

    commands[0] = "echo // 由小白白的【量子纠缠器】自动生成 > temp.cpp";
    commands[1] = "echo #define IS_DEBUG cai_guai > temp.cpp";
    // 对接文件A
    ss.str("");
    ss << "echo #define A_BINARY_START " << A_Start << " >> temp.cpp";
    commands[2] = ss.str();
    ss.str("");
    ss << "echo #define A_BINARY_END " << A_End << " >> temp.cpp";
    commands[3] = ss.str();
    ss.str("");
    ss << "echo #define A_BINARY_OUT \"" << A_Out << "\" >> temp.cpp";
    commands[4] = ss.str();
    // 对接文件B
    ss.str("");
    ss << "echo #define B_BINARY_START " << B_Start << " >> temp.cpp";
    commands[5] = ss.str();
    ss.str("");
    ss << "echo #define B_BINARY_END " << B_End << " >> temp.cpp";
    commands[6] = ss.str();
    ss.str("");
    ss << "echo #define B_BINARY_OUT \"" << B_Out << "\" >> temp.cpp";
    commands[7] = ss.str();
    // 对接源码
    commands[8] = "type modle\\modle.cpp >> temp.cpp";
    // 编译源码
    ss.str("");
    ss << "g++ temp.cpp " << A_O << " " << B_O << " -o " << argv[3];
    commands[9] = ss.str();
    // 清除源码
    ss.str("");
    ss << "del temp.cpp " << A_O << " " << B_O;
    commands[10] = ss.str();
    // 执行执行队列
    for (int i = 0; i < commands_len; i++)
    {
        system(commands[i].c_str());
        // cout << "假装执行命令：" << commands[i] << endl;
    }

    // 调试输出
    cout << "\n\n\n调试输出：\n";
    cout << "A_Start: " << A_Start << "\nB_Start: " << B_Start << endl;
    cout << "A_End: " << A_End << "\nB_End: " << B_End << endl;
    cout << "A_Out: " << A_Out << "\nB_Out: " << B_Out << endl;
    cout << "A_O: " << A_O << "\nB_O: " << B_O << endl;
    return 0;
}
