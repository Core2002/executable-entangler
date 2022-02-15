.\clean
g++ hello\HelloA.cpp -o awa.exe
g++ hello\HelloB.cpp -o bwb.exe
objcopy -I binary -O pe-x86-64 -B i386:x86-64 awa.exe awa.exe.o
objcopy -I binary -O pe-x86-64 -B i386:x86-64 bwb.exe bwb.exe.o

g++ modle\modle.cpp awa.exe.o bwb.exe.o -o build\Test.exe

echo "Test make over. at build\Test.exe"
# g++ main.cpp
# .\a awa.exe bwb.exe test.exe