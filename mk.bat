del *.exe *.o

@rem g++ main.cpp -Iinclude -Llib/dll-release-x86 -lcurl -lws2_32 -lwinmm

g++ main.cpp -o main -Llib/dll-release-x86 -llibcurl -Iinclude

pause