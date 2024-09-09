#include <windows.h>
#include "SP_LB2-3.h"
#include <string>

BOOL IsThreadExist(ThreadParams thParams)
{
    BOOL flag = FALSE;
    int threadNum = thParams.Num;

    if (threadNum < 0 || threadNum > 2)
    {
        MessageBox(thParams.hWnd, L"Invalid thread number", L"Error", MB_OK | MB_ICONERROR);
        return TRUE;
    }
    if (hSecThread[threadNum] != NULL || dwSecThreadId[threadNum] != 0)  flag = TRUE;

    return flag;
}

void CreateUserThread(ThreadParams thParams)
{
    if (thParams.Num < 1 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Определение функции для потока
    LPTHREAD_START_ROUTINE lpThreadFunc = RunningLine;
    if (lpThreadFunc == NULL)
    {
        MessageBox(thParams.hWnd, _T("The thread function is not defined"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Создание потока
    hSecThread[thParams.Num] = CreateThread(
        NULL,                     
        0,                        
        lpThreadFunc,             
        &thParams,               
        0,                        
        &dwSecThreadId[thParams.Num] 
    );

    // Проверка успешного создания потока
    if (!IsThreadExist(thParams))
    {
        MessageBox(thParams.hWnd, _T("Failed to create a thread"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Увеличение счетчика потоков
    g_uThCount++;

    // Сообщение об успешном создании потока
    std::wstring msg = L"Thread is " + std::to_wstring(thParams.Num) + L" created.";
    MessageBox(thParams.hWnd, msg.c_str(), L"Info", MB_OK);
}

void DestroyUserThread(ThreadParams thParams, bool quietMode)
{
    // Проверка на допустимый номер потока
    if (thParams.Num < 0 || thParams.Num > 2)
    {
        if (!quietMode)
        {
            MessageBox(thParams.hWnd, L"The thread function is not defined", L"Error", MB_OK | MB_ICONERROR);
        }       
        return;
    }

    // Проверка, существует ли поток
    if (!IsThreadExist(thParams))
    {
        if (!quietMode)
        {
            MessageBox(thParams.hWnd, L"The thread descriptor is not valid", L"Error", MB_OK | MB_ICONERROR);
        }       
        return;
    }

    // Отправка сигнала завершения потока
    if ((TerminateThread(hSecThread[thParams.Num], 0) == 0) && quietMode == FALSE)
    {
        DWORD dwError = GetLastError();
        std::wstring errorMessage = L"Failed to terminate the thread. Error code: " + std::to_wstring(dwError);
        MessageBox(thParams.hWnd, errorMessage.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Ожидание завершения потока
    WaitForSingleObject(hSecThread[thParams.Num], INFINITE);

    // Закрытие дескриптора потока
    CloseHandle(hSecThread[thParams.Num]);
    hSecThread[thParams.Num] = NULL;  // Обнуление дескриптора
    dwSecThreadId[thParams.Num] = 0;  // Обнуление идентификатора потока

    // Уменьшение счетчика потоков
    g_uThCount--;

    // Сообщение об успешном уничтожении потока
    if (quietMode == FALSE)
    {
        std::wstring msg = L"Thread " + std::to_wstring(thParams.Num) + L" Destroyed.";
        MessageBox(thParams.hWnd, msg.c_str(), L"Info", MB_OK);
    }
}

void CreateWaitingThread(ThreadParams thParams)
{

    if (thParams.Num < 1 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Определение функции для потока
    LPTHREAD_START_ROUTINE lpThreadFunc = RunningLine;
    if (lpThreadFunc == NULL)
    {
        MessageBox(thParams.hWnd, _T("The thread function is not defined"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Создание потока
    hSecThread[thParams.Num] = CreateThread(
        NULL,
        0,
        lpThreadFunc,
        &thParams,
        CREATE_SUSPENDED,
        &dwSecThreadId[thParams.Num]
    );

    // Проверка успешного создания потока
    if (!IsThreadExist(thParams))
    {
        MessageBox(thParams.hWnd, _T("Failed to create a thread"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Увеличение счетчика потоков
    g_uThCount++;

    // Сообщение об успешном создании потока
    std::wstring msg = L"Thread is " + std::to_wstring(thParams.Num) + L" created.";
    MessageBox(thParams.hWnd, msg.c_str(), L"Info", MB_OK);

}

void SuspendUserThread(ThreadParams thParams)
{ 

    if (thParams.Num < 0 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    SuspendThread(hSecThread[thParams.Num]);

}

void ContinueThread(ThreadParams thParams) 
{
    if (thParams.Num < 0 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    ResumeThread(hSecThread[thParams.Num]);
}

void IncreasePriorityThread(ThreadParams thParams)
{ 
    if (thParams.Num < 0 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }
    SetThreadPriority(hSecThread[thParams.Num], THREAD_PRIORITY_TIME_CRITICAL);
}

void DecreasePriorityThread(ThreadParams thParams)
{
    if (thParams.Num < 0 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }
    SetThreadPriority(hSecThread[thParams.Num], THREAD_PRIORITY_LOWEST);
}

BOOL GetThreadInfo(ThreadParams thParams, ThreadInfo &thInfo)
{ 
    if (thParams.Num < 0 || thParams.Num > 2)
    {
        MessageBox(thParams.hWnd, _T("Incorrect thread number"), _T("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }
    if (!IsThreadExist(thParams)) return FALSE;

    thInfo.dwThreadId = GetThreadId(hSecThread[thParams.Num]);
    thInfo.hThread = hSecThread[thParams.Num];
    thInfo.dwThreadState = STILL_ACTIVE;
    thInfo.nThreadPriority = GetThreadPriority(hSecThread[thParams.Num]);

    FILETIME creationTime, exitTime, kernelTime, userTime;
    GetThreadTimes(hSecThread[thParams.Num], &creationTime, &exitTime, &kernelTime, &userTime);
    ULARGE_INTEGER ullCreationTime, ullCurrentTime;
    ullCreationTime.LowPart = creationTime.dwLowDateTime;
    ullCreationTime.HighPart = creationTime.dwHighDateTime;
    ullCurrentTime.LowPart = kernelTime.dwLowDateTime + userTime.dwLowDateTime;
    ullCurrentTime.HighPart = kernelTime.dwHighDateTime + userTime.dwHighDateTime;
    thInfo.ullThreadStartTime = ullCreationTime.QuadPart;

    return TRUE;

}
