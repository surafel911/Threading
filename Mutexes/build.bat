@echo off
cd Mutexes/
@echo on

gcc -g -m64 -std=c99 -o main main.c -pthread

@echo off
cd ../
@echo on

