#include <stdio.h>
#include <conio.h>
#include <windows.h>

HANDLE hSemaphores[3];

#define SAFE_DELETE_HANDLE(a) if(a){CloseHandle(a);}

DWORD WINAPI print(LPVOID parameter) {
    int n = (int)parameter;
    for(int i=0;i<1000;i++) {
        WaitForSingleObject(hSemaphores[n],INFINITE);
        printf("%d", n+1);
        ReleaseSemaphore(hSemaphores[(n+1)%3],1,NULL);
    }
    return 0;
}

int main(void) {
    DWORD print1Id,print2Id,print3Id;
    HANDLE hprint1,hprint2,hprint3;

    hSemaphores[0] = CreateSemaphore(0,1,1,NULL);
    hSemaphores[1] = CreateSemaphore(0,0,1,NULL);
    hSemaphores[2] = CreateSemaphore(0,0,1,NULL);

    if(hSemaphores[0] && hSemaphores[1] && hSemaphores[2]){
        hprint1 = CreateThread(NULL,0,&print,(LPVOID)0,0,&print1Id);
        hprint2 = CreateThread(NULL,0,&print,(LPVOID)1,0,&print2Id);
        hprint3 = CreateThread(NULL,0,&print,(LPVOID)2,0,&print3Id);
    }

    int c=getch();

    SAFE_DELETE_HANDLE(hSemaphores[0]);
    SAFE_DELETE_HANDLE(hSemaphores[1]);
    SAFE_DELETE_HANDLE(hSemaphores[2]);
    SAFE_DELETE_HANDLE(hprint1);
    SAFE_DELETE_HANDLE(hprint2);
    SAFE_DELETE_HANDLE(hprint3);
    return 0;
}

//Same but using critical section without semaphore
/*CRITICAL_SECTION cs;
int turn = 1; 

DWORD WINAPI print(LPVOID parameter) {
    int value = *(int*)parameter;
    for(int i=0;i<100;i++) {
        while(true) {
            EnterCriticalSection(&cs);
            if(turn == value) {
                printf("%d",value);
                turn = (turn % 3) + 1;
                LeaveCriticalSection(&cs);
                break;
            }
            LeaveCriticalSection(&cs);
            Sleep(1); // to reduce busy waiting
        }
    }
    return 0;
}


int main(void) {
    DWORD print1Id,print2Id,print3Id;
    HANDLE hprint1,hprint2,hprint3;

    InitializeCriticalSection(&cs);

    int t1 = 1;
    int t2 = 2;
    int t3 = 3;

    hprint1 = CreateThread(NULL,0,&print,&t1,0,&print1Id);
    hprint2 = CreateThread(NULL,0,&print,&t2,0,&print1Id);
    hprint3 = CreateThread(NULL,0,&print,&t3,0,&print1Id);

    getch();

    WaitForSingleObject(hprint1, INFINITE);//wait for threads to finish
    WaitForSingleObject(hprint2, INFINITE);
    WaitForSingleObject(hprint3, INFINITE);

    CloseHandle(hprint1);
    CloseHandle(hprint2);
    CloseHandle(hprint3);

    DeleteCriticalSection(&cs);

    return 0;
}
*/