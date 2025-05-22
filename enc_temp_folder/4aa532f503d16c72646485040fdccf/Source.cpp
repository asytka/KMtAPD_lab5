// lab5_variant3_deadlock.cpp
#include <windows.h>
#include <iostream>

/*
 * Лабораторна робота №5 (варіант 3): Взаємне блокування (deadlock)
 * Мета: продемонструвати проблему deadlock між двома процесами через неправильне захоплення ресурсів.
*/

HANDLE hMutex1, hMutex2;

DWORD WINAPI Thread1(LPVOID) {
    std::cout << "[Thread1] Waiting for Mutex1...\n";
    WaitForSingleObject(hMutex1, INFINITE);
    std::cout << "[Thread1] Acquired Mutex1. Waiting for Mutex2...\n";
    //Sleep(1000); 
    WaitForSingleObject(hMutex2, INFINITE);
    std::cout << "[Thread1] Acquired Mutex2.\n";

    std::cout << "[Thread1] Working in critical section...\n";
    //Sleep(1000);

    ReleaseMutex(hMutex2);
    ReleaseMutex(hMutex1);
    std::cout << "[Thread1] Released Mutexes.\n";
    return 0;
}

DWORD WINAPI Thread2(LPVOID) {
    std::cout << "[Thread2] Waiting for Mutex2...\n";
    WaitForSingleObject(hMutex2, INFINITE);
    std::cout << "[Thread2] Acquired Mutex2. Waiting for Mutex1...\n";
    //Sleep(1000); 
    WaitForSingleObject(hMutex1, INFINITE);
    std::cout << "[Thread2] Acquired Mutex1.\n";

    // Критична секція
    std::cout << "[Thread2] Working in critical section...\n";
   // Sleep(1000);

    ReleaseMutex(hMutex1);
    ReleaseMutex(hMutex2);
    std::cout << "[Thread2] Released Mutexes.\n";
    return 0;
}

int main() {
    std::cout << "[Main] Starting deadlock demonstration...\n";

    hMutex1 = CreateMutex(NULL, FALSE, NULL);
    hMutex2 = CreateMutex(NULL, FALSE, NULL);

    if (!hMutex1 || !hMutex2) {
        std::cerr << "Failed to create mutexes.\n";
        return 1;
    }

    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    for (int i = 0; i < 2; ++i) CloseHandle(hThreads[i]);
    CloseHandle(hMutex1);
    CloseHandle(hMutex2);

    std::cout << "[Main] Finished.\n";
    return 0;
}
