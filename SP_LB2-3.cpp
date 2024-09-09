#include "framework.h"
#include "SP_LB2-3.h"

#define MAX_LOADSTRING 256

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


DWORD g_uThCount = 0;                           //Количество потоков
HANDLE hSecThread[3] = { nullptr, nullptr, nullptr }; // Дескрипторы потоков
DWORD dwSecThreadId[3] = { 0, 0, 0 };                  //Идентификаторы потоков

// Анимация прямоугольника в отдельном потоке.
bool g_bStopAnimation = false;
HANDLE hAnimationThread = nullptr;




ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    wcscpy_s(szTitle, TEXT("Poliak_thread_lb"));
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLB22, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLB22));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLB22));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLB22);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   HMENU hMenu = GetMenu(hWnd);
   ControlMenu(hMenu);
   hSecThread[0] = GetCurrentThread();
   dwSecThreadId[0] = GetThreadId(hSecThread[0]);
   g_uThCount++;

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            HMENU hMenu = GetMenu(hWnd);
            switch (wmId)
            {
                // Поток 1
            case ID_THREAD1_CREATE_THREAD:
            {
                ThreadParams threadParams1(hWnd, 1, 150, 100);
                CreateUserThread(threadParams1); // Создаем поток 1
                ControlMenu(hMenu);
                break;
            }          
            case ID_THREAD1_CREATE_WAITING_THREAD:
            {
                ThreadParams threadParams1(hWnd, 1, 150, 100);
                CreateWaitingThread(threadParams1); // Создаем ожидающий поток 1
                ControlMenu(hMenu);
                break;
            }
                
            case ID_THREAD1_SUSPEND_THREAD:
            {
                ThreadParams threadParams1(hWnd, 1);
                SuspendUserThread(threadParams1); // Приостанавливаем поток 1
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD1_CONTINUE_WORK_THREAD:
            {
                ThreadParams threadParams1(hWnd, 1);
                ContinueThread(threadParams1); // Возобновляем работу потока 1
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD1_DESTROY_THREAD:
            {
                ThreadParams threadParams1(hWnd, 1);
                DestroyUserThread(threadParams1, FALSE); // Завершаем поток 1
                InvalidateRect(hWnd, NULL, TRUE);     
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD1_INCREASE_PRIORITY:
            {
                ThreadParams threadParams1(hWnd, 1);
                IncreasePriorityThread(threadParams1); // Увеличиваем приоритет потока 1
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD1_DECREASE_PRIORITY:
            {
                ThreadParams threadParams1(hWnd, 1);
                DecreasePriorityThread(threadParams1); // Уменьшаем приоритет потока 1
                ControlMenu(hMenu);
                break;
            }
                
                // Поток 2
            case ID_THREAD2_CREATE_THREAD:
            {
                ThreadParams threadParams2(hWnd, 2, 150, 140);
                CreateUserThread(threadParams2); // Создаем поток 2
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_CREATE_WAITING_THREAD:
            {
                ThreadParams threadParams2(hWnd, 2, 150, 140);
                CreateWaitingThread(threadParams2); // Создаем ожидающий поток 2
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_SUSPEND_THREAD:
            {
                ThreadParams threadParams2(hWnd, 2); 
                SuspendUserThread(threadParams2); // Приостанавливаем поток 2
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_CONTINUE_WORK_THREAD:
            {
                ThreadParams threadParams2(hWnd, 2); 
                ContinueThread(threadParams2); // Возобновляем работу потока 2
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_DESTROY_THREAD:
            {
                ThreadParams threadParams2(hWnd, 2); 
                DestroyUserThread(threadParams2, FALSE); // Завершаем поток 2
                InvalidateRect(hWnd, NULL, TRUE);
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_INCREASE_PRIORITY:
            {
                ThreadParams threadParams2(hWnd, 2); 
                IncreasePriorityThread(threadParams2); // Увеличиваем приоритет потока 2
                ControlMenu(hMenu);
                break;
            }
            case ID_THREAD2_DECREASE_PRIORITY:
            {
                ThreadParams threadParams2(hWnd, 2);
                DecreasePriorityThread(threadParams2); // Уменьшаем приоритет потока 2
                ControlMenu(hMenu);
                break;
            }

                // Информация о потоках
            case ID_THREADSINFORMATION_PRIMARY_THREAD:
            {
                // Информация о основном потоке
                ThreadParams threadParams0(hWnd, 0);
                ThreadInfo thInfo0;
                if (GetThreadInfo(threadParams0, thInfo0)) 
                {
                    ShowThreadInfoDialog(hWnd, thInfo0);
                }                    
                break;
            }
            case ID_THREADSINFORMATION_THREAD1:
            {
                // Информация о потоке 1
                ThreadParams threadParams1(hWnd, 1);
                ThreadInfo thInfo1;
                if (GetThreadInfo(threadParams1, thInfo1))
                {
                    ShowThreadInfoDialog(hWnd, thInfo1);
                }
                break;
            }
            case ID_THREADSINFORMATION_THREAD2:
            {
                // Информация о потоке 2
                ThreadParams threadParams2(hWnd, 2);
                ThreadInfo thInfo2;
                if (GetThreadInfo(threadParams2, thInfo2))
                {
                    ShowThreadInfoDialog(hWnd, thInfo2);
                }
                break;
            }

            case ID_ANIMATION_START:
            {
                StartAnimation(hWnd); 
                break;
            }

            case ID_ANIMATION_DESTROY:
            {
                StopAnimation();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);         
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        ThreadParams threadParams0(hWnd, 0);
        ThreadParams threadParams1(hWnd, 1);
        ThreadParams threadParams2(hWnd, 2);

        DestroyUserThread(threadParams1, TRUE);
        DestroyUserThread(threadParams1, TRUE);
        DestroyUserThread(threadParams2, TRUE);
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

