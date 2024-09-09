#include"SP_LB2-3.h"
#include "Windows.h"
#include <string>

const int LINE_HEIGHT = 20; // ������ ������, ������� �� ������ ����������


//Functions for threads
DWORD WINAPI RunningLine(LPVOID lpParam)
{
    // ����������� ���������� ��������� � ���������
    ThreadParams* pParams = (ThreadParams*)lpParam;

    HWND hWnd = pParams->hWnd;
    int threadNum = pParams->Num;
    int xPos = pParams->xPos; // ��������� ������� �� �����������
    int yPos = pParams->yPos; // ��������� ������� �� ���������

    HDC hdc;
    RECT rect;
    std::wstring text = L"Secondary thread created by Polyak A.A. Thread number " + std::to_wstring(threadNum);
    int textLength = text.length();
    SIZE textSize;
    int textWidth;

    // �������� ���������� �������
    GetClientRect(hWnd, &rect);

    // ���������� �����
    HFONT hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
    HDC hdcMem = CreateCompatibleDC(NULL);
    SelectObject(hdcMem, hFont);

    while (true)
    {
        hdc = GetDC(hWnd);

        // �������� �����
        GetTextExtentPoint32(hdc, text.c_str(), textLength, &textSize);
        textWidth = textSize.cx;

        // ������� ������� ����� ����� ����������
        RECT updateRect = { 0, yPos, rect.right, yPos + 20 };
        InvalidateRect(hWnd, &updateRect, TRUE);
        UpdateWindow(hWnd);

        // ��������� ������
        TextOut(hdc, xPos, yPos, text.c_str(), textLength);

        // ��������� ������� ������
        xPos += 5;

        // ���� ����� ����� �� ������� ������, ���������� �������
        if (xPos >= rect.right)
        {
            xPos = -textWidth;
        }

        Sleep(100); // �������� ��� �������� ������� ������

        // �������� �� �������� ����
        if (IsWindow(hWnd) == FALSE)
        {
            break;
        }

        ReleaseDC(hWnd, hdc);
    }

    DeleteDC(hdcMem);

    return 0;
}

