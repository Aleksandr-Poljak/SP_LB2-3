#include"SP_LB2-3.h"
#include "Windows.h"
#include <string>

const int LINE_HEIGHT = 20;


//Functions for threads
DWORD WINAPI RunningLine(LPVOID lpParam)
{
    ThreadParams* pParams = (ThreadParams*)lpParam;

    HWND hWnd = pParams->hWnd;
    int threadNum = pParams->Num;
    int xPos = pParams->xPos; // Начальная позиция по горизонтали
    int yPos = pParams->yPos; // Начальная позиция по вертикали

    HDC hdc;
    RECT rect;
    std::wstring text = L"Secondary thread created by Polyak A.A. Thread number " + std::to_wstring(threadNum);
    int textLength = text.length();
    SIZE textSize;
    int textWidth;

    GetClientRect(hWnd, &rect);

    HFONT hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdcMem, hFont);

    while (true)  // Бесконечный цикл для постоянного движения строки
    {
        hdc = GetDC(hWnd);
        GetTextExtentPoint32(hdc, text.c_str(), textLength, &textSize);
        textWidth = textSize.cx;

        // Если синхронизация включена, ждем мьютекса
        if (g_bSyncnEnabled)
        {
            WaitForSingleObject(hmtx, INFINITE);
        }

        for (int i = 0; i < 2; i++)
        {
            xPos = -textWidth;  

            while (xPos < rect.right)  
            {
                // Очищаем только область текущей строки перед новой отрисовкой
                RECT updateRect = { 0, yPos, rect.right, yPos + LINE_HEIGHT };
                InvalidateRect(hWnd, &updateRect, TRUE);
                UpdateWindow(hWnd);

                TextOut(hdc, xPos, yPos, text.c_str(), textLength);
                xPos += 5;  
                Sleep(20);  // Задержка , регулировка скрости строки.
            }
        }

        if (g_bSyncnEnabled)
        {
            ReleaseMutex(hmtx);
        }

        // Проверка на закрытие окна
        if (!IsWindow(hWnd))
        {
            break;
        }

        ReleaseDC(hWnd, hdc);
    }

    DeleteDC(hdcMem);
    return 0;
}
