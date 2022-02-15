objcopy -I binary -O pe-x86-64 -B i386:x86-64 awa.exe awa.exe.o
objcopy -I binary -O pe-x86-64 -B i386:x86-64 bwb.exe bwb.exe.o

g++ modle\modle.cpp awa.exe.o bwb.exe.o -o build\test.exe