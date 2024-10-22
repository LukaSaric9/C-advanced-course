/*
Implement a program that converts lowercase letters to uppercase letters. Create three program threads:
-Input program thread,
-Processing program thread, and
-Output program thread.
The input program thread accepts an input character and stores it in the input circular buffer with a maximum size of RING_SIZE.
The processing program thread retrieves characters from the input buffer and converts them to uppercase letters.
The converted character is then placed in the output circular buffer.
The output program thread retrieves characters from the output buffer and prints them to the standard output.*/

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h> 

#define RING_SIZE 10
#define SLEEPING_TIME 100
#define DELETE_THREAD_HANDLE(a) if(a){CloseHandle(a);}

HANDLE Empty;
HANDLE Full;
HANDLE Finished;
HANDLE Converted;
HANDLE OutputFull;

struct RingBuffer {
    unsigned int tail;
    unsigned int head;
    unsigned char data[RING_SIZE];
};

RingBuffer inputBuffer;
RingBuffer outputBuffer;
CRITICAL_SECTION inputCs,outputCs;

void inputCharacter(RingBuffer *buffer,const char c) {
    buffer->data[buffer->tail] = c;
    buffer->tail = (buffer->tail+1) % RING_SIZE; //tail moved forward by 1 space in the buffer
}

char outputCharacter(RingBuffer *buffer) {
    char c = buffer->data[buffer->head];
    buffer->head = (buffer->head+1) % RING_SIZE;
    return c;
}

DWORD WINAPI inputThread(LPVOID parameter) {
    char c;
    const int semaphore_num = 2;  
    HANDLE semaphores[semaphore_num] = {Finished,Empty};

    while(WaitForMultipleObjects(semaphore_num,semaphores,FALSE,INFINITE) == WAIT_OBJECT_0 + 1) { //The loop continues while Empty is signaled
        c = getch();

       if (c == '1') {
            ReleaseSemaphore(Finished, 2, NULL); 
            break;
        }

        EnterCriticalSection(&inputCs);
        inputCharacter(&inputBuffer,c);
        LeaveCriticalSection(&inputCs);

        ReleaseSemaphore(Full,1,NULL);
    }
    return 0;
}

DWORD WINAPI conversionThread(LPVOID parameter) {
    char c;
    const int semaphore_num = 2;  
    HANDLE semaphores[semaphore_num] = {Finished, Full};

    while (WaitForMultipleObjects(semaphore_num, semaphores, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {  
        EnterCriticalSection(&inputCs);
        c = outputCharacter(&inputBuffer);
        LeaveCriticalSection(&inputCs);

        c = toupper(c);  // Convert to uppercase

        EnterCriticalSection(&outputCs);
        inputCharacter(&outputBuffer, c);  
        LeaveCriticalSection(&outputCs);

        ReleaseSemaphore(OutputFull, 1, NULL);  
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI outputThread(LPVOID parameter) {
    char c;
    const int semaphore_num = 2;  
    HANDLE semaphores[semaphore_num] = {Finished, OutputFull};

    while (WaitForMultipleObjects(semaphore_num, semaphores, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {  
        EnterCriticalSection(&outputCs);
        c = outputCharacter(&outputBuffer);
        LeaveCriticalSection(&outputCs);

        printf("Output: %c\n", c); 

        ReleaseSemaphore(Empty, 1, NULL);  
        Sleep(100);
    }
    return 0;
}

int main(void) {
    DWORD thread1ID,thread2ID,thread3ID;
    HANDLE hThread1,hThread2,hThread3;

    Empty = CreateSemaphore(0,RING_SIZE,RING_SIZE,NULL);
    Full = CreateSemaphore(0,0,RING_SIZE,NULL);
    Finished = CreateSemaphore(0,0,2,NULL);
    OutputFull = CreateSemaphore(0, 0, RING_SIZE, NULL);

    if(Empty && Full && Finished && OutputFull) {
        InitializeCriticalSection(&inputCs);
        InitializeCriticalSection(&outputCs);

        hThread1 = CreateThread(NULL,0,&inputThread,(LPVOID)0,0,&thread1ID);
        hThread2 = CreateThread(NULL,0,&conversionThread,(LPVOID)0,0,&thread2ID);
        hThread3 = CreateThread(NULL,0,&outputThread,(LPVOID)0,0,&thread3ID);

        if(!hThread1 || !hThread2 || !hThread3) {
            ReleaseSemaphore(Finished,2,NULL);
        }

        if(hThread1) WaitForSingleObject(hThread1,INFINITE);
        ReleaseSemaphore(Finished, 1, NULL);
        if (hThread2) WaitForSingleObject(hThread2, INFINITE);
        if (hThread3) WaitForSingleObject(hThread3, INFINITE);
    }   

    DELETE_THREAD_HANDLE(hThread1);
    DELETE_THREAD_HANDLE(hThread2);
    DELETE_THREAD_HANDLE(hThread3);
    DELETE_THREAD_HANDLE(Empty);
    DELETE_THREAD_HANDLE(Full);
    DELETE_THREAD_HANDLE(Finished);
    DELETE_THREAD_HANDLE(OutputFull);

    DeleteCriticalSection(&inputCs);
    DeleteCriticalSection(&outputCs);
    printf("\nTerminating the program!\n");

    return 0;
}