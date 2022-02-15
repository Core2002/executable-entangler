#include <iostream>
#include <sstream>

using namespace std;

// ���ļ� filename ת���� .o �ļ�
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
    // cout << "��װִ�����" << command << endl;
    // cout << "�õ���out�ļ���" << O << endl;
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
        cout << "ʹ�÷����� " << argv[0] << "���ļ�A_Out�� ���ļ�B_Out�� �������" << endl;
        system("pause");
        return 0;
    }

    //////////////// ���ȣ�ͨ��objcopy����������Դ�ļ�Ϊo�ļ���
    // #Windowsϵͳ�£�����64λ����ʱ��
    // objcopy -I binary -O pe-x86-64 -B i386:x86-64 ��Դ�ļ��� ����������ļ���
    // #Linuxϵͳ�£�����32λ����ʱ��
    // objcopy -I binary -O elf32-i386 -B i386 ��Դ�ļ��� ����������ļ���
    // #Linuxϵͳ�£�����64λ����ʱ��
    // objcopy -I binary -O elf64-x86-64 -B i386:x86-64 ��Դ�ļ��� ����������ļ���

    string A_Out = to_ofile(argv[1]);
    string B_Out = to_ofile(argv[2]);

    string A_O = string(A_Out);
    string B_O = string(B_Out);

    A_Out = replace_all_distinct(A_Out, ".o", "");
    B_Out = replace_all_distinct(B_Out, ".o", "");

    // ���ɲ���
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

    // ����ִ�ж���
    int commands_len = 11;
    string commands[commands_len];
    stringstream ss;

    commands[0] = "echo // ��С�װ׵ġ����Ӿ��������Զ����� > temp.cpp";
    commands[1] = "echo #define IS_DEBUG cai_guai > temp.cpp";
    // �Խ��ļ�A
    ss.str("");
    ss << "echo #define A_BINARY_START " << A_Start << " >> temp.cpp";
    commands[2] = ss.str();
    ss.str("");
    ss << "echo #define A_BINARY_END " << A_End << " >> temp.cpp";
    commands[3] = ss.str();
    ss.str("");
    ss << "echo #define A_BINARY_OUT \"" << A_Out << "\" >> temp.cpp";
    commands[4] = ss.str();
    // �Խ��ļ�B
    ss.str("");
    ss << "echo #define B_BINARY_START " << B_Start << " >> temp.cpp";
    commands[5] = ss.str();
    ss.str("");
    ss << "echo #define B_BINARY_END " << B_End << " >> temp.cpp";
    commands[6] = ss.str();
    ss.str("");
    ss << "echo #define B_BINARY_OUT \"" << B_Out << "\" >> temp.cpp";
    commands[7] = ss.str();
    // �Խ�Դ��
    commands[8] = "type modle\\modle.cpp >> temp.cpp";
    // ����Դ��
    ss.str("");
    ss << "g++ temp.cpp " << A_O << " " << B_O << " -o " << argv[3];
    commands[9] = ss.str();
    // ���Դ��
    ss.str("");
    ss << "del temp.cpp " << A_O << " " << B_O;
    commands[10] = ss.str();
    // ִ��ִ�ж���
    for (int i = 0; i < commands_len; i++)
    {
        system(commands[i].c_str());
        // cout << "��װִ�����" << commands[i] << endl;
    }

    // �������
    cout << "\n\n\n���������\n";
    cout << "A_Start: " << A_Start << "\nB_Start: " << B_Start << endl;
    cout << "A_End: " << A_End << "\nB_End: " << B_End << endl;
    cout << "A_Out: " << A_Out << "\nB_Out: " << B_Out << endl;
    cout << "A_O: " << A_O << "\nB_O: " << B_O << endl;
    return 0;
}
