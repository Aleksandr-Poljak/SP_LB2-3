#include"SP_LB2-3.h"
#include "Windows.h"
#include <string>

const int LINE_HEIGHT = 20; // Высота строки, которую вы хотите отобразить


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
    std::wstring text = L"Вторичный поток создан Polyak A.A. Номер потока " + std::to_wstring(threadNum);
    int textLength = text.length();
    SIZE textSize;
    int textWidth;

    // Получаем клиентскую область
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

        // Два полных прохода строки
        for (int i = 0; i < 2; i++)
        {
            xPos = -textWidth;  // Сбрасываем начальную позицию на левый край (за пределы окна)

            while (xPos < rect.right)  // Пока текст не выйдет за правый край окна
            {
                // Очищаем только область текущей строки перед новой отрисовкой
                RECT updateRect = { 0, yPos, rect.right, yPos + LINE_HEIGHT };
                InvalidateRect(hWnd, &updateRect, TRUE);
                UpdateWindow(hWnd);

                // Рисуем строку
                TextOut(hdc, xPos, yPos, text.c_str(), textLength);

                xPos += 5;  // Шаг движения строки
                Sleep(50);  // Задержка для плавного движения строки
            }
        }

        // Если синхронизация включена, освобождаем мьютекс после двух полных проходов
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
