#include"SP_LB2-3.h"
#include "Windows.h"
#include <string>

const int LINE_HEIGHT = 20; // Высота строки, которую вы хотите отобразить


//Functions for threads
DWORD WINAPI RunningLine(LPVOID lpParam)
{
    // Преобразуем переданные параметры в структуру
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

    // Получаем клиентскую область
    GetClientRect(hWnd, &rect);

    // Определяем шрифт
    HFONT hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdcMem, hFont);

    while (true)
    {
        hdc = GetDC(hWnd);

        // Измеряем текст
        GetTextExtentPoint32(hdc, text.c_str(), textLength, &textSize);
        textWidth = textSize.cx;

        // Очистка области перед новой отрисовкой
        RECT updateRect = { 0, yPos, rect.right, yPos + 20 };
        InvalidateRect(hWnd, &updateRect, TRUE);
        UpdateWindow(hWnd);

        // Отрисовка текста
        TextOut(hdc, xPos, yPos, text.c_str(), textLength);

        // Обновляем позицию текста
        xPos += 5;

        // Если текст вышел за пределы экрана, сбрасываем позицию
        if (xPos >= rect.right)
        {
            xPos = -textWidth;
        }

        Sleep(100); // Задержка для скорости бегущей строки

        // Проверка на закрытие окна
        if (IsWindow(hWnd) == FALSE)
        {
            break;
        }

        ReleaseDC(hWnd, hdc);
    }

    DeleteDC(hdcMem);

    return 0;
}

