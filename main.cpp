#include <iostream>

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

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        cout << "ʹ�÷����� " << argv[0] << "���ļ�A�� ���ļ�B�� �������" << endl;
        return 0;
    }

    //////////////// ���ȣ�ͨ��objcopy����������Դ�ļ�Ϊo�ļ���
    // #Windowsϵͳ�£�����64λ����ʱ��
    // objcopy -I binary -O pe-x86-64 -B i386:x86-64 ��Դ�ļ��� ����������ļ���
    // #Linuxϵͳ�£�����32λ����ʱ��
    // objcopy -I binary -O elf32-i386 -B i386 ��Դ�ļ��� ����������ļ���
    // #Linuxϵͳ�£�����64λ����ʱ��
    // objcopy -I binary -O elf64-x86-64 -B i386:x86-64 ��Դ�ļ��� ����������ļ���

    string A = to_ofile(argv[1]);
    string B = to_ofile(argv[2]);

    string AA(A);
    string BB(B);



    // ��ʼ����Դ�ļ�
    system("");

    return 0;
}
