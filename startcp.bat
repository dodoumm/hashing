@echo off
chcp 65001 > nul

REM
g++ LISCH.cpp main.cpp -o maincpp.exe
"./maincpp.exe"