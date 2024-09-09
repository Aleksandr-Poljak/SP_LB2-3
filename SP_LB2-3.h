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

//Aнимация
extern bool g_bStopAnimation; // Флаг работы анимации
extern HANDLE hAnimationThread ; // Дескриптор потока анимации

// Парметры фигуры
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
//Вспомгательная функция отрисовки
void FillSolidRect(HDC hdc, RECT* rect, COLORREF color);
// Функция рисования
DWORD WINAPI MoveRectangle(LPVOID lpParam);
// Запуск анимции в отдельном потоке
void StartAnimation(HWND hWnd);
// Остановка анимации
void StopAnimation();

// Sync threads/
extern HANDLE hmtx;
extern  BOOL g_bSyncnEnabled;


