#include "SP_LB2-3.h"

void ControlMenu(HMENU hMenu)
{

	// Управление меню потока 1
	if (hSecThread[1] == NULL)
	{
		EnableMenuItem(hMenu, ID_THREAD1_CREATE_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_CREATE_WAITING_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_SUSPEND_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_CONTINUE_WORK_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_DESTROY_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_INCREASE_PRIORITY, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_DECREASE_PRIORITY, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREADSINFORMATION_THREAD1, MF_GRAYED);

		
	}
	else
	{
		EnableMenuItem(hMenu, ID_THREAD1_CREATE_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_CREATE_WAITING_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD1_SUSPEND_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_CONTINUE_WORK_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_DESTROY_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_INCREASE_PRIORITY, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD1_DECREASE_PRIORITY, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREADSINFORMATION_THREAD1, MF_ENABLED);
	}


	// Управление меню потока 2
	if (hSecThread[2] == NULL)
	{
		EnableMenuItem(hMenu, ID_THREAD2_CREATE_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_CREATE_WAITING_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_SUSPEND_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_CONTINUE_WORK_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_DESTROY_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_INCREASE_PRIORITY, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_DECREASE_PRIORITY, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREADSINFORMATION_THREAD2, MF_GRAYED);


	}
	else
	{
		EnableMenuItem(hMenu, ID_THREAD2_CREATE_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_CREATE_WAITING_THREAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_THREAD2_SUSPEND_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_CONTINUE_WORK_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_DESTROY_THREAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_INCREASE_PRIORITY, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREAD2_DECREASE_PRIORITY, MF_ENABLED);
		EnableMenuItem(hMenu, ID_THREADSINFORMATION_THREAD2, MF_ENABLED);
	}
}

void ShowThreadInfoDialog(HWND hWnd, const ThreadInfo& threadInfo)
{
	TCHAR szInfo[256];
	wsprintf(szInfo, _T("ID: %lu\n"), threadInfo.dwThreadId);
	wsprintf(szInfo + lstrlen(szInfo), _T("Stream: %p\n"), threadInfo.hThread);
	wsprintf(szInfo + lstrlen(szInfo), _T("State: %s\n"), (threadInfo.dwThreadState == STILL_ACTIVE) ? _T("Active") : _T("Completed"));
	wsprintf(szInfo + lstrlen(szInfo), _T("Relative priority: %d\n"), threadInfo.nThreadPriority);
	FILETIME ftNow;
	GetSystemTimeAsFileTime(&ftNow);
	ULARGE_INTEGER ullCurrentTime;
	ullCurrentTime.LowPart = ftNow.dwLowDateTime;
	ullCurrentTime.HighPart = ftNow.dwHighDateTime;
	ULONGLONG ullElapsedTime = ullCurrentTime.QuadPart - threadInfo.ullThreadStartTime;
	wsprintf(szInfo + lstrlen(szInfo), _T("Rrunning time: %lu milliseconds\n"), ullElapsedTime / 10000);
	MessageBox(hWnd, szInfo, _T("Thread INFO"), MB_OK | MB_ICONINFORMATION);
}

void FillSolidRect(HDC hdc, RECT* rect, COLORREF color)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	FillRect(hdc, rect, hBrush);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}

DWORD WINAPI MoveRectangle(LPVOID lpParam)
{
	ThreadParamsRectangle* params = (ThreadParamsRectangle*)lpParam;
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));  // Зеленый цвет

	RECT rect;
	HDC hdc;
	HBRUSH hOldBrush;

	while (!g_bStopAnimation)
	{
		hdc = GetDC(params->hWnd);
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		RECT eraseRect = { params->x, params->y, params->x + params->width, params->y + params->height };
		FillSolidRect(hdc, &eraseRect, RGB(255, 255, 255));  // Белый цвет фона

		params->x += params->step;

		if (params->x + params->width > 1100 || params->x < 0)
		{
			params->step = -params->step;
		}

		rect.left = params->x;
		rect.top = params->y;
		rect.right = params->x + params->width;
		rect.bottom = params->y + params->height;
		FillRect(hdc, &rect, hBrush);

		SelectObject(hdc, hOldBrush);
		ReleaseDC(params->hWnd, hdc);

		Sleep(50);
	}

	DeleteObject(hBrush);
	return 0;
}

void StartAnimation(HWND hWnd)
{
	if (hAnimationThread == nullptr)
	{
		ThreadParamsRectangle* params = new ThreadParamsRectangle(hWnd, 0, 100, 100, 50, 10);

		hAnimationThread = CreateThread(
			NULL,                         
			0,                             
			MoveRectangle,                 
			params,                         
			0,                             
			NULL                          
		);
	}
}

void StopAnimation()
{
	if (hAnimationThread != nullptr)
	{
		g_bStopAnimation = true;
		WaitForSingleObject(hAnimationThread, INFINITE);
		CloseHandle(hAnimationThread);
		hAnimationThread = nullptr;
		g_bStopAnimation = false;
	}
}