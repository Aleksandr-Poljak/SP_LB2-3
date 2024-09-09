#include "framework.h"
#include "resource.h"
#include "Windows.h"
#include "ThreadsControlFuncs.h"


/// <summary>
/// About
/// </summary>
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


/// <summary>
/// Starts the ticker.
/// </summary>
DWORD WINAPI RunningLine(LPVOID lpParam);

/// <summary>///
/// Menu access control
/// </summary>///
void ControlMenu(HMENU hMenu);

/// <summary>/// 
/// Displays thread information.
/// </summary>
void ShowThreadInfoDialog(HWND hWnd, const ThreadInfo& threadInfo);

//A�������
extern bool g_bStopAnimation; // ���� ������ ��������
extern HANDLE hAnimationThread ; // ���������� ������ ��������

// �������� ������
struct ThreadParamsRectangle
{
    HWND hWnd;
    int x;
    int y;
    int width;
    int height;
    int step;

    ThreadParamsRectangle(HWND window, int startX, int startY, int rectWidth, int rectHeight, int moveStep)
        : hWnd(window), x(startX), y(startY), width(rectWidth), height(rectHeight), step(moveStep)
    {
    }
};
//�������������� ������� ���������
void FillSolidRect(HDC hdc, RECT* rect, COLORREF color);
// ������� ���������
DWORD WINAPI MoveRectangle(LPVOID lpParam);
// ������ ������� � ��������� ������
void StartAnimation(HWND hWnd);
// ��������� ��������
void StopAnimation();

// Sync threads/
extern HANDLE hmtx;
extern  BOOL g_bSyncnEnabled;


