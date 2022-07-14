del *.exe *.o

g++ -static main.cpp -o main -Llib/dll-release-x86 -llibcurl -Iinclude

pause
