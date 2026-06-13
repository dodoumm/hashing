@echo off
chcp 65001 > nul

:loop
g++ LISCH.cpp main.cpp -o maincpp.exe
"./maincpp.exe"
goto loop