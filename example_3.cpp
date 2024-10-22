#include <stdio.h>
#include <conio.h>
#include <Windows.h>

HANDLE hSemaphore;

int counter;

DWORD WINAPI ThreadFunction(LPVOID parameter) {
    while(true) {
        WaitForSingleObject(hSemaphore,INFINITE);
        counter++;
        printf("\nCounter = %d",counter);
    }
}

int main(void) {
    DWORD threadID;
    HANDLE hThread;

    hSemaphore = CreateSemaphore(0,0,1,NULL);
    hThread = CreateThread(NULL,0,&ThreadFunction,NULL,0,&threadID);

    while(true) {
        if(kbhit()) {
            if(getch() == 'q') break;
            ReleaseSemaphore(hSemaphore,1,NULL);
        }
        Sleep(100); //so that the processor wont be preoccupied with checking if the keybord is pressed!
    }

    CloseHandle(hThread);
    CloseHandle(hSemaphore);
    return 0;
}




