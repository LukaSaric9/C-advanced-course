#include <stdio.h>
#include <windows.h>
#include <conio.h>

CRITICAL_SECTION cs; //only 1 thread can access shared variables!

DWORD WINAPI ThreadFunction(LPVOID parameter) { 
    bool* flag = (bool*)parameter;
    while(true){
        bool tempFlag;

        EnterCriticalSection(&cs);
        tempFlag = *flag;
        LeaveCriticalSection(&cs);

        if(tempFlag) {
            break;
        }

        printf(".");
        Sleep(10);
    }
    printf("Thread finished!\n");
    return 0;
}

int main(int argc, char* argv[]) {
    HANDLE hWorker_Thread; //variable that handles the thread!
    bool finishSignal = false;

    InitializeCriticalSection(&cs);

    hWorker_Thread = CreateThread(0,0,&ThreadFunction,&finishSignal,0,0);
    getch(); //waiting for input that signals the thread to stop!

    EnterCriticalSection(&cs);
    finishSignal = true;
    LeaveCriticalSection(&cs);

    getch(); // signal to end the program

    CloseHandle(hWorker_Thread);
    DeleteCriticalSection(&cs); //free the resources

    return 0;
}

