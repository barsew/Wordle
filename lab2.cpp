// lab2.cpp : Defines the entry point for the application.
//kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

#include "framework.h"
#include "lab2.h"
#include <winuser.h>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <time.h>
#include <algorithm>

#define MAX_LOADSTRING 100
#define MAX_WINDOWS 4
#define MAX_LETTERS_COUNT 50
#define WORD_LEN 5
#define WORDS_IN_FILE 16825 
#define ALPHABET 26

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int width = 645;
const int heigh = 250;
const int widthChild = 335;
const int heighEasy = 420;
const int heighMedium = heighEasy + 122;
const int heighHard = heighMedium + 122;
const int sizeSquare = 55;
const int margines = 6;
float mytime = 0.0;

enum difficulty {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3,
};

HWND child[MAX_WINDOWS];
difficulty diff = EASY;
bool newGame = true;
WCHAR letters[MAX_LETTERS_COUNT];
int curLettersCount = 0;
int maxLetterToDiff = 30;
int currentWordToWrite = 0;
bool enterOn = true;
std::string fileWords[WORDS_IN_FILE];
std::string winningWords[MAX_WINDOWS];
std::string currentWord;
int colorTabKeyboard[MAX_WINDOWS][ALPHABET];   // -1 - bialy, 0 - nietrafiony, 1 - zolty, 2 - zielony
char keyboard[ALPHABET] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'};
bool activeChild[MAX_WINDOWS];
bool activeChild2[MAX_WINDOWS];
int colorTabEasy[30];   // -1 - bialy, 0 - nietrafiony, 1 - zolty, 2 - zielony
int colorTabMedium[2][40];   // -1 - bialy, 0 - nietrafiony, 1 - zolty, 2 - zielony
int colorTabHard[4][50];   // -1 - bialy, 0 - nietrafiony, 1 - zolty, 2 - zielony

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ATOM MyRegisterClassChild(HINSTANCE hInstance);
LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClassSquare(HINSTANCE hInstance);
LRESULT CALLBACK WndProcSquare(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DrawWords(HWND);
void CheckResult();
void Coloring();
void CheckColors(int tab[], int wordNum, difficulty d);
HBRUSH PickBrush(int c);
void ResetWord();
void Win(int c);
HPEN PickPen(int c);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClassChild(hInstance);

    for (int k = 0; k < MAX_WINDOWS; k++)
    {
        for (int i = 0; i < ALPHABET; i++)
        {
            colorTabKeyboard[k][i] = -1;
        }
    }
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(255, 255, 255));//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
    
    WNDCLASSEXW wcex2;

    wcex2.cbSize = sizeof(WNDCLASSEX);

    wcex2.style = CS_HREDRAW | CS_VREDRAW;
    wcex2.lpfnWndProc = WndProcChild;
    wcex2.cbClsExtra = 0;
    wcex2.cbWndExtra = 0;
    wcex2.hInstance = NULL;
    wcex2.hIcon = NULL;
    wcex2.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//(HBRUSH)(COLOR_WINDOW+1);
    wcex2.lpszMenuName = NULL;
    wcex2.lpszClassName = L"Board";
    wcex2.hIconSm = NULL;



    return RegisterClassExW(&wcex2);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  
   hInst = hInstance; // Store instance handle in our global variable

   RECT rc;
   SystemParametersInfo(NULL, NULL, &rc, NULL);
   

   // Pozycjonowanie okienek
   RECT rect;
   GetClientRect(GetDesktopWindow(), &rect);
   rect.left = (rect.right / 2) - (width / 2);
   rect.top = (rect.bottom / 2) - (heigh / 2);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      rect.left - 130, rect.top + 200, width, heigh, nullptr, nullptr, hInstance, nullptr);


   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
   // Make this window 50% alpha
   SetLayeredWindowAttributes(hWnd, 0, (255 * 50) / 100, LWA_ALPHA);
   // Show this window
   // 


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int bufSize = 256;
    TCHAR buf[bufSize];
  

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EASY:
            {
                newGame = true;
                maxLetterToDiff = 30;
                diff = EASY;
				break;
			}
            case IDM_MEDIUM:
            {
                newGame = true;
                maxLetterToDiff = 40;
                diff = MEDIUM;
                break;
            }
            case IDM_HARD:
            {
                newGame = true;
                maxLetterToDiff = 50;
                diff = HARD;
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        srand(time(NULL));
        HANDLE plik = CreateFile(L"Wordle.ini", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
        if (plik == INVALID_HANDLE_VALUE)
        {
            PostQuitMessage(0);
        }
        char buffer[22];
        ReadFile(plik, buffer, 22, 0, NULL);

        CloseHandle(plik);
        if (buffer[20] == '1')
        {
            diff = EASY;
            newGame = true;
        }
        if (buffer[20] == '2')
        {
            diff = MEDIUM;
            newGame = true;
        }
        if (buffer[20] == '4')
        {
            diff = HARD;
            newGame = true;
        }
        std::ifstream plik2;
        plik2.open("Wordle.txt", std::fstream::in);
        int idx = 0;
        while(std::getline(plik2, fileWords[idx++]))
        { }
        plik2.close();
        SetTimer(hWnd, 7, 1, NULL);       
    }
    break;
    case WM_TIMER:
    {
        
        if (newGame == true)
        {
            InvalidateRect(hWnd, NULL, true);
            DrawWords(hWnd);

            HMENU hmenu = GetMenu(hWnd);
            newGame = false;
            curLettersCount = 0;
            currentWordToWrite = 0;
            enterOn = true;
            currentWord = "";
            for (int i = 0; i < MAX_LETTERS_COUNT; i++)
            {
                letters[i] = '\0';
            }
            //-1 do tablicy z kolorami klawiatury
            for (int k = 0; k < MAX_WINDOWS; k++)
            {
                for (int i = 0; i < ALPHABET; i++)
                {
                    colorTabKeyboard[k][i] = -1;
                }
            }
            //-1 do tablic z kolorami plansz
            for (int i = 0; i < 30; i++)
            {
                colorTabEasy[i] = -1;
            }
            for (int w = 0; w < 2; w++)
            {
                for(int i = 0; i < 40; i++)
                {
                    colorTabMedium[w][i] = -1;
                }
            }
            for (int w = 0; w < MAX_WINDOWS; w++)
            {
                for (int i = 0; i < 50; i++)
                {
                    colorTabHard[w][i] = -1;
                }
            }
            // destroying windows for update  difficulty
            for (int i = 0; i < MAX_WINDOWS; i++)
            {
                DestroyWindow(child[i]);
            }
            for (int i = 0; i < MAX_WINDOWS; i++)
            {
                activeChild2[i] = false;
                activeChild[i] = false;
            }
            RECT rect;
            GetClientRect(GetDesktopWindow(), &rect);
            rect.left = (rect.right / 2) - (width / 2);
            rect.top = (rect.bottom / 2) - (heigh / 2);
            if (diff == EASY)
            {            
                maxLetterToDiff = 30;
                child[0] = CreateWindow(L"Board", L"WORDLE-PUZZLE", WS_OVERLAPPED | WS_VISIBLE, rect.left, rect.top - 180, widthChild, heighEasy, hWnd, NULL, NULL, NULL);
                activeChild[0] = true;
                activeChild2[0] = true;
                CheckMenuItem(hmenu, IDM_EASY, MF_CHECKED);
                CheckMenuItem(hmenu, IDM_MEDIUM, MF_UNCHECKED);
                CheckMenuItem(hmenu, IDM_HARD, MF_UNCHECKED);
            }
            if (diff == MEDIUM)
            {
                maxLetterToDiff = 40;
                for (int i = 0; i < 2; i++)
                {
                    child[i] = CreateWindow(L"Board", L"WORDLE-PUZZLE", WS_OVERLAPPED | WS_VISIBLE, 200 + 2 * i * (rect.left - 200 + 30), rect.top - 180, widthChild, heighMedium, hWnd, NULL, NULL, NULL);
                }
                activeChild[0] = true;
                activeChild[1] = true;
                activeChild2[0] = true;
                activeChild2[1] = true;
                CheckMenuItem(hmenu, IDM_MEDIUM, MF_CHECKED);
                CheckMenuItem(hmenu, IDM_HARD, MF_UNCHECKED);
                CheckMenuItem(hmenu, IDM_EASY, MF_UNCHECKED);
            }
            if (diff == HARD)
            {
                maxLetterToDiff = 50;
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        child[i * 2 - j + 1] = CreateWindow(L"Board", L"WORDLE-PUZZLE", WS_OVERLAPPED | WS_VISIBLE, 200 + 2 * i * (rect.left - 200 + 30), rect.top - j * 400, widthChild, heighHard, hWnd, NULL, NULL, NULL);
                    }
                }
                for (int i = 0; i < MAX_WINDOWS; i++)
                {
                    activeChild[i] = true;
                    activeChild2[i] = true;
                }
                CheckMenuItem(hmenu, IDM_HARD, MF_CHECKED);
                CheckMenuItem(hmenu, IDM_MEDIUM, MF_UNCHECKED);
                CheckMenuItem(hmenu, IDM_EASY, MF_UNCHECKED);
            }
        }

    }
    break;
    case WM_PAINT:
    {
        WCHAR keyboard1[] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'};
        WCHAR keyboard2[] = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'};
        WCHAR keyboard3[] = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M'};
        WCHAR keyboard0[] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M' };

        int sum = margines + sizeSquare;

        if (diff == EASY)
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            HDC offDC = CreateCompatibleDC(hdc);
            HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
            HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);

            HPEN pen;
            HPEN oldPen;
            HBRUSH brush, oldBrush;
            HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
            HFONT oldFont = (HFONT)SelectObject(offDC, font);

            Rectangle(offDC, rc.left, rc.top - 1, rc.right, rc.bottom);

            for (int p = 0; p < ALPHABET; p++)
            {
                pen = CreatePen(PS_SOLID, 2, RGB(211, 211, 211));
                oldPen = (HPEN)SelectObject(offDC, pen);
                brush = PickBrush(colorTabKeyboard[0][p]);
                oldBrush = (HBRUSH)SelectObject(offDC, brush);
                SetBkMode(offDC, TRANSPARENT);

               

                if (p < 10)
                {
                    RoundRect(offDC, 10 + sum * p, 10, 10 + sizeSquare + sum * p, 10 + sizeSquare, 10, 10);
                    TextOut(offDC, 30 + sum * p, 26, &keyboard0[0] + p, 1);
                }
                else if(p < 19)
                {
                    RoundRect(offDC, 10 + sizeSquare / 2 + sum * (p - 10), 10 + sum, 10 + sizeSquare / 2 + sizeSquare + sum * (p - 10), 10 + sizeSquare + sum, 10, 10);
                    TextOut(offDC, 30 + sizeSquare / 2 + sum * (p - 10), 26 + sum, &keyboard0[0] + p, 1);
                }
                else
                {
                    RoundRect(offDC, 10 + sizeSquare / 2 + sum + sum * (p - 19), 10 + 2 * sum, 10 + 3 * sizeSquare / 2 + sum + sum * (p - 19), 10 + sizeSquare + 2 * sum, 10, 10);
                    TextOut(offDC, 30 + sizeSquare / 2 + sum + sum * (p - 19), 26 + 2 * sum, &keyboard0[0] + p, 1);
                }
                SelectObject(offDC, oldPen);
                DeleteObject(pen);
                SelectObject(offDC, oldBrush);
                DeleteObject(brush);
               
            }
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

            SelectObject(offDC, oldFont);
            DeleteObject(font);
            SelectObject(offDC, oldPen);
            DeleteObject(pen);
            SelectObject(offDC, oldBrush);
            DeleteObject(brush);

            SelectObject(offDC, offOldBitmap);
            DeleteObject(offBitmap);

            DeleteDC(offDC);

            EndPaint(hWnd, &ps);
        }
        if (diff == MEDIUM)
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            HDC offDC = CreateCompatibleDC(hdc);
            HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
            HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);

            HPEN pen;
            HPEN oldPen;
            HBRUSH brush, oldBrush;
            HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
            HFONT oldFont = (HFONT)SelectObject(offDC, font);
            SetBkMode(offDC, TRANSPARENT);

            Rectangle(offDC, rc.left, rc.top - 1, rc.right, rc.bottom);

            for (int w = 0; w < 2; w++)
            {

                for (int p = 0; p < ALPHABET; p++)
                {
                    pen = PickPen(colorTabKeyboard[w][p]);
                    brush = PickBrush(colorTabKeyboard[w][p]);
                    oldPen = (HPEN)SelectObject(offDC, pen);
                    oldBrush = (HBRUSH)SelectObject(offDC, brush);
                  
                    if (colorTabKeyboard[w][p] == -1)
                    {
                        if (p < 10)
                        {
                            RoundRect(offDC, 10 + sum * p, 10, 10 + sizeSquare + sum * p, 10 + sizeSquare, 10, 10);
                        }
                        else if (p < 19)
                        {
                            RoundRect(offDC, 10 + sizeSquare / 2 + sum * (p - 10), 10 + sum, 10 + sizeSquare / 2 + sizeSquare + sum * (p - 10), 10 + sizeSquare + sum, 10, 10);
                        }
                        else
                        {
                            RoundRect(offDC, 10 + sizeSquare / 2 + sum + sum * (p - 19), 10 + 2 * sum, 10 + 3 * sizeSquare / 2 + sum + sum * (p - 19), 10 + sizeSquare + 2 * sum, 10, 10);
                        }
                    }
                    else
                    {
                        if (p < 10)
                        {
                            RoundRect(offDC, sizeSquare + 10 + (w - 1) * sizeSquare / 2 + sum * p, sizeSquare + 10, 10 + w * sizeSquare / 2 + sum * p, 10, 10, 10);
                        }
                        else if (p < 19)
                        {
                            RoundRect(offDC, 3 * sizeSquare / 2 + 10 + (w - 1) * sizeSquare / 2 + sum * (p - 10), 10 + sum, 10 + sizeSquare / 2 + w * sizeSquare / 2 + sum * (p - 10), 10 + sizeSquare + sum, 10, 10);
                        }
                        else
                        {
                            RoundRect(offDC, 3 * sizeSquare / 2 + 10 + (w - 1) * sizeSquare / 2 + sum * (p - 19) + sum, 10 + 2 * sum, sum + 10 + sizeSquare / 2 + w * sizeSquare / 2 + sum * (p - 19), 10 + sizeSquare + 2 * sum, 10, 10);
                        }
                    }
                    SelectObject(offDC, oldPen);
                    DeleteObject(pen);
                    SelectObject(offDC, oldBrush);
                    DeleteObject(brush);
                }
            }
            for (int p = 0; p < ALPHABET; p++)
            {
                if (p < 10)
                {
                    TextOut(offDC, 30 + sum * p, 26, &keyboard0[0] + p, 1);
                }
                else if (p < 19)
                {
                    TextOut(offDC, 30 + sizeSquare / 2 + sum * (p - 10), 26 + sum, &keyboard0[0] + p, 1);
                }
                else
                {
                    TextOut(offDC, 30 + sizeSquare / 2 + sum + sum * (p - 19), 26 + 2 * sum, &keyboard0[0] + p, 1);
                }

            }

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

            SelectObject(offDC, oldFont);
            DeleteObject(font);
            SelectObject(offDC, oldPen);
            DeleteObject(pen);
            SelectObject(offDC, oldBrush);
            DeleteObject(brush);

            SelectObject(offDC, offOldBitmap);
            DeleteObject(offBitmap);

            DeleteDC(offDC);

            EndPaint(hWnd, &ps);
        }
        if (diff == HARD)
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            HDC offDC = CreateCompatibleDC(hdc);
            HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
            HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);


            HPEN pen;
            HPEN oldPen;
            HBRUSH brush, oldBrush;
            HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
            HFONT oldFont = (HFONT)SelectObject(offDC, font);
            SetBkMode(offDC, TRANSPARENT);

            Rectangle(offDC, rc.left, rc.top - 1, rc.right, rc.bottom);

            for (int w2 = 0; w2 < 2; w2++)
            {
                for (int w = 0; w < 2; w++)
                {

                    for (int p = 0; p < ALPHABET; p++)
                    {
                        pen = PickPen(colorTabKeyboard[2 * w + w2][p]);
                        brush = PickBrush(colorTabKeyboard[2 * w + w2][p]);
                        oldPen = (HPEN)SelectObject(offDC, pen);
                        oldBrush = (HBRUSH)SelectObject(offDC, brush);

                        if (colorTabKeyboard[2 * w + w2][p] == -1)
                        {
                            if (p < 10)
                            {
                                RoundRect(offDC, 10 + sum * p, 10, 10 + sizeSquare + sum * p, 10 + sizeSquare, 10, 10);
                            }
                            else if (p < 19)
                            {
                                RoundRect(offDC, 10 + sizeSquare / 2 + sum * (p - 10), 10 + sum, 10 + sizeSquare / 2 + sizeSquare + sum * (p - 10), 10 + sizeSquare + sum, 10, 10);
                            }
                            else
                            {
                                RoundRect(offDC, 10 + sizeSquare / 2 + sum + sum * (p - 19), 10 + 2 * sum, 10 + 3 * sizeSquare / 2 + sum + sum * (p - 19), 10 + sizeSquare + 2 * sum, 10, 10);
                            }
                        }
                        else
                        {
                            if (p < 10)
                            {
                                RoundRect(offDC, sizeSquare + 10 + (w - 1) * sizeSquare / 2 + sum * p, sizeSquare + 10 + (w2 - 1) * sizeSquare / 2, 10 + w * sizeSquare / 2 + sum * p, 10 + (w2) * sizeSquare / 2, 10, 10);
                            }
                            else if (p < 19)
                            {
                                RoundRect(offDC, 3 * sizeSquare / 2 + 10 + (w - 1) * sizeSquare / 2 + sum * (p - 10), 10 + sum, 10 + sizeSquare / 2 + w * sizeSquare / 2 + sum * (p - 10), 10 + sizeSquare + sum, 10, 10);
                            }
                            else
                            {
                                RoundRect(offDC, 3 * sizeSquare / 2 + 10 + (w - 1) * sizeSquare / 2 + sum * (p - 19) + sum, 10 + 2 * sum, sum + 10 + sizeSquare / 2 + w * sizeSquare / 2 + sum * (p - 19), 10 + sizeSquare + 2 * sum, 10, 10);
                            }
                        }
                        SelectObject(offDC, oldPen);
                        DeleteObject(pen);
                        SelectObject(offDC, oldBrush);
                        DeleteObject(brush);
                    }
                }
            }
            for (int p = 0; p < ALPHABET; p++)
            {
                if (p < 10)
                {
                    TextOut(offDC, 30 + sum * p, 26, &keyboard0[0] + p, 1);
                }
                else if (p < 19)
                {
                    TextOut(offDC, 30 + sizeSquare / 2 + sum * (p - 10), 26 + sum, &keyboard0[0] + p, 1);
                }
                else
                {
                    TextOut(offDC, 30 + sizeSquare / 2 + sum + sum * (p - 19), 26 + 2 * sum, &keyboard0[0] + p, 1);
                }

            }
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

            SelectObject(offDC, oldFont);
            DeleteObject(font);
            SelectObject(offDC, oldPen);
            DeleteObject(pen);
            SelectObject(offDC, oldBrush);
            DeleteObject(brush);

            SelectObject(offDC, offOldBitmap);
            DeleteObject(offBitmap);

            DeleteDC(offDC);

            EndPaint(hWnd, &ps);
        }

	}
	break;
    case WM_ERASEBKGND:
    {
        return 1;
    }
    break;
    case WM_CHAR:   
    {
        
        if (enterOn && curLettersCount > 0 && curLettersCount % 5 == 0 && wParam == VK_RETURN)
        {
            char tmp[WORD_LEN];
            for (int i = 0; i < WORD_LEN; i++)
            {
                tmp[i] = letters[curLettersCount - 5 + i] + ('a' - 'A');
            }

            currentWord.assign(tmp, WORD_LEN);
            currentWordToWrite++;
            enterOn = false;
            CheckResult();
            InvalidateRect(hWnd, NULL, true); 
            for (int i = 0; i < MAX_WINDOWS; i++)
            {
                if (activeChild2[i] == true)
                {
                    InvalidateRect(child[i], NULL, true);
                }
            }
;       }
        if (wParam == VK_BACK && curLettersCount > 0 && curLettersCount > currentWordToWrite * WORD_LEN)
        {
            letters[--curLettersCount] = '\0';
            enterOn = false;
            for (int i = 0; i < MAX_WINDOWS; i++)
            {
                if (activeChild2[i] == true)
                {
                    InvalidateRect(child[i], NULL, true);
                }
            }
        }
        if (curLettersCount < maxLetterToDiff && curLettersCount < (currentWordToWrite + 1) * WORD_LEN)
        {
            if (wParam >= 'a' && wParam <= 'z')
            {
                wParam = wParam - ('a' - 'A');
            }
            if (wParam >= 'A' && wParam <= 'Z')
            {
                letters[curLettersCount++] = wParam;
                enterOn = true;
            }
            for (int i = 0; i < MAX_WINDOWS; i++)
            {
                if (activeChild2[i] == true)
                {
                    InvalidateRect(child[i], NULL, true);
                }
            }
        }
    }
    break;
   /* case WM_SIZE:
    {
        if (wParam == SIZE_RESTORED)
        {
            ShowWindow(child[0], SW_SHOW);
        }
        else if(wParam == SIZE_MINIMIZED)
        {
            ShowWindow(child[0], SW_HIDE);
        }
    }
    break;*/
    case WM_DESTROY:
    {
        // zapis do pliku
        HANDLE plik = CreateFile(L"Wordle.ini", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
        if (plik == INVALID_HANDLE_VALUE)
        {
            PostQuitMessage(0);
        }
        char buffer2[22] = { '[', 'W', 'O', 'R', 'D', 'L', 'E', ']', '\n', 'D', 'I', 'F', 'F', 'I', 'C', 'U', 'L', 'T', 'Y', '='};
        if (diff == EASY)
            buffer2[20] = '1';
        if (diff == MEDIUM)
            buffer2[20] = '2';
        if (diff == HARD)
            buffer2[20] = '4';
        buffer2[21] = '\0';
        WriteFile(plik, buffer2, 22, 0, NULL);
        CloseHandle(plik);

        KillTimer(hWnd, 7);
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int bufSize = 256;
    TCHAR buf[bufSize];

    int p;
    for (p = 0; p < MAX_WINDOWS; p++)
    {
        if (child[p] == hWnd)
            break;
    }
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
       
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {
        //SetTimer(hWnd, 6, 1, NULL);
    }
    break;
    case WM_TIMER:
    {
       
    }
    break;
    case WM_PAINT:
    {
        bool isWin = false;
        int rows = 6;
        if (diff == MEDIUM)
            rows = 8;
        if (diff == HARD)
            rows = 10;
      
        int sum = sizeSquare + margines;
        
		if (diff == EASY)
		{
            
			if (activeChild2[p] == true)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				RECT rc;
				GetClientRect(hWnd, &rc);

				HDC offDC = CreateCompatibleDC(hdc);
				HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
				HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);

				HPEN pen;
				HPEN oldPen;
				HBRUSH brush, oldBrush;
				HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
				HFONT oldFont = (HFONT)SelectObject(offDC, font);

				Rectangle(offDC, rc.left - 10, rc.top - 1, rc.right + 10, rc.bottom + 10);

				SetBkMode(offDC, TRANSPARENT);

				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < WORD_LEN; j++)
					{
						pen = PickPen(colorTabEasy[i * WORD_LEN + j]);
						oldPen = (HPEN)SelectObject(offDC, pen);
						brush = PickBrush(colorTabEasy[i * WORD_LEN + j]);
						oldBrush = (HBRUSH)SelectObject(offDC, brush);

						RoundRect(offDC, 10 + sum * j, 10 + sum * i, 10 + sizeSquare + sum * j, 10 + sizeSquare + sum * i, 10, 10);
						TextOut(offDC, 10 + sum * j + 17, 10 + sum * i + 15, &letters[0] + 5 * i + j, 1);

						SelectObject(offDC, oldPen);
						DeleteObject(pen);
						SelectObject(offDC, oldBrush);
						DeleteObject(brush);
					}
				}

				BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

                               
                if (activeChild[p] == false)
                {
                    isWin = true;
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    SetBkMode(offDC2, TRANSPARENT);
                    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);

                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                    // kod na podstawie https://stackoverflow.com/questions/52762590/draw-a-transparent-line-using-native-win32-gdi?fbclid=IwAR1zRJnFR5283FXIJWda7uvK65m56ktzszst3-uoLEs20f1PsqqFCAwt9zg
                }
                if (isWin == false && currentWordToWrite == 6)
                {
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    SetBkMode(offDC2, TRANSPARENT);

                    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);
                    std::string upWord = winningWords[p];
                    transform(upWord.begin(), upWord.end(), upWord.begin(), ::toupper);

                    // konwersja stringa https://stackoverflow.com/questions/1200188/how-to-convert-stdstring-to-lpcstr
                    std::wstring stemp = std::wstring(upWord.begin(), upWord.end());
                    LPCWSTR sw = stemp.c_str();
                    HFONT font2 = CreateFont(-MulDiv(14, GetDeviceCaps(offDC2, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
                    HFONT oldFont2 = (HFONT)SelectObject(offDC2, font2);
                    SetTextColor(offDC2, RGB(255, 255, 255));

                    DrawText(offDC2, sw, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    SelectObject(offDC2, oldFont2);
                    DeleteObject(font2);
                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                }


                SelectObject(offDC, oldFont);
                DeleteObject(font);
               
				SelectObject(offDC, offOldBitmap);
				DeleteObject(offBitmap);

                DeleteDC(offDC);

                EndPaint(hWnd, &ps);


			}
		}
		if (diff == MEDIUM)
		{
			if (activeChild2[p] == true)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				RECT rc;
				GetClientRect(hWnd, &rc);

				HDC offDC = CreateCompatibleDC(hdc);
				HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
				HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);

				HPEN pen;
				HPEN oldPen;
				HBRUSH brush, oldBrush;
				HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
				HFONT oldFont = (HFONT)SelectObject(offDC, font);

				Rectangle(offDC, rc.left - 10, rc.top - 1, rc.right + 10, rc.bottom + 10);

				SetBkMode(offDC, TRANSPARENT);

				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < WORD_LEN; j++)
					{
						pen = PickPen(colorTabMedium[p][i * WORD_LEN + j]);
						oldPen = (HPEN)SelectObject(offDC, pen);
						brush = PickBrush(colorTabMedium[p][i * WORD_LEN + j]);
						oldBrush = (HBRUSH)SelectObject(offDC, brush);

						RoundRect(offDC, 10 + sum * j, 10 + sum * i, 10 + sizeSquare + sum * j, 10 + sizeSquare + sum * i, 10, 10);
						TextOut(offDC, 10 + sum * j + 17, 10 + sum * i + 15, &letters[0] + 5 * i + j, 1);

						SelectObject(offDC, oldPen);
						DeleteObject(pen);
						SelectObject(offDC, oldBrush);
						DeleteObject(brush);
					}
				}

                BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

                if (activeChild[p] == false)
                {
                    isWin = true;
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);

                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                    // kod na podstawie https://stackoverflow.com/questions/52762590/draw-a-transparent-line-using-native-win32-gdi?fbclid=IwAR1zRJnFR5283FXIJWda7uvK65m56ktzszst3-uoLEs20f1PsqqFCAwt9zg
                }
                if (isWin == false && currentWordToWrite == 8)
                {
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    SetBkMode(offDC2, TRANSPARENT);

                    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);
                    std::string upWord = winningWords[p];
                    transform(upWord.begin(), upWord.end(), upWord.begin(), ::toupper);

                    // konwersja stringa https://stackoverflow.com/questions/1200188/how-to-convert-stdstring-to-lpcstr
                    std::wstring stemp = std::wstring(upWord.begin(), upWord.end());
                    LPCWSTR sw = stemp.c_str();
                    HFONT font2 = CreateFont(-MulDiv(14, GetDeviceCaps(offDC2, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
                    HFONT oldFont2 = (HFONT)SelectObject(offDC2, font2);
                    SetTextColor(offDC2, RGB(255, 255, 255));

                    DrawText(offDC2, sw, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    SelectObject(offDC2, oldFont2);
                    DeleteObject(font2);
                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                }
				SelectObject(offDC, oldFont);
				DeleteObject(font);

				SelectObject(offDC, offOldBitmap);
				DeleteObject(offBitmap);

                DeleteDC(offDC);

				EndPaint(hWnd, &ps);
			}
		}
        if (diff == HARD)
        {
            if (activeChild2[p] == true)
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                RECT rc;
                GetClientRect(hWnd, &rc);

                HDC offDC = CreateCompatibleDC(hdc);
                HBITMAP offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
                HBITMAP offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);

                HPEN pen;
                HPEN oldPen;
                HBRUSH brush, oldBrush;
                HFONT font = CreateFont(-MulDiv(14, GetDeviceCaps(offDC, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
                HFONT oldFont = (HFONT)SelectObject(offDC, font);

                Rectangle(offDC, rc.left - 10, rc.top - 1, rc.right + 10, rc.bottom + 10);

                SetBkMode(offDC, TRANSPARENT);

                for (int i = 0; i < rows; i++)
                {
                    for (int j = 0; j < WORD_LEN; j++)
                    {
                        pen = PickPen(colorTabHard[p][i * WORD_LEN + j]);
                        oldPen = (HPEN)SelectObject(offDC, pen);
                        brush = PickBrush(colorTabHard[p][i * WORD_LEN + j]);
                        oldBrush = (HBRUSH)SelectObject(offDC, brush);

                        RoundRect(offDC, 10 + sum * j, 10 + sum * i, 10 + sizeSquare + sum * j, 10 + sizeSquare + sum * i, 10, 10);
                        TextOut(offDC, 10 + sum * j + 17, 10 + sum * i + 15, &letters[0] + 5 * i + j, 1);

                        SelectObject(offDC, oldPen);
                        DeleteObject(pen);
                        SelectObject(offDC, oldBrush);
                        DeleteObject(brush);
                    }
                }

                BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);

                if (activeChild[p] == false)
                {
                    isWin = true;
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);

                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                    // kod na podstawie https://stackoverflow.com/questions/52762590/draw-a-transparent-line-using-native-win32-gdi?fbclid=IwAR1zRJnFR5283FXIJWda7uvK65m56ktzszst3-uoLEs20f1PsqqFCAwt9zg
                }
                if (isWin == false && currentWordToWrite == 10)
                {
                    activeChild2[p] = false;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    HDC offDC2 = CreateCompatibleDC(hdc);
                    HBITMAP offBitmap2 = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
                    SelectObject(offDC2, offBitmap2);

                    SetBkMode(offDC2, TRANSPARENT);

                    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                    BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 128, 0 };
                    FillRect(offDC2, &rect, hBrush);
                    std::string upWord = winningWords[p];
                    transform(upWord.begin(), upWord.end(), upWord.begin(), ::toupper);

                    // konwersja stringa https://stackoverflow.com/questions/1200188/how-to-convert-stdstring-to-lpcstr
                    std::wstring stemp = std::wstring(upWord.begin(), upWord.end());
                    LPCWSTR sw = stemp.c_str();
                    HFONT font2 = CreateFont(-MulDiv(14, GetDeviceCaps(offDC2, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
                    HFONT oldFont2 = (HFONT)SelectObject(offDC2, font2);
                    SetTextColor(offDC2, RGB(255, 255, 255));

                    DrawText(offDC2, sw, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


                    AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, offDC2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);

                    SelectObject(offDC2, oldFont2);
                    DeleteObject(font2);
                    DeleteObject(hBrush);
                    DeleteObject(offBitmap2);
                    DeleteDC(offDC2);
                }

                SelectObject(offDC, oldFont);
                DeleteObject(font);

                SelectObject(offDC, offOldBitmap);
                DeleteObject(offBitmap);

                DeleteDC(offDC);

                EndPaint(hWnd, &ps);
            }
        }
    }
    break;
    case WM_ERASEBKGND:
    {
        return 1;
    }
    break;
    case WM_NCHITTEST:
    {
        LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
        if (hit == HTCLIENT) hit = HTCAPTION;
        return hit;
        //zrodlo: https://stackoverflow.com/questions/7773771/how-do-i-implement-dragging-a-window-using-its-client-area
    }
    break;
    case WM_DESTROY:
       // KillTimer(hWnd, 6);
        //PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



void DrawWords(HWND hWnd)
{
    int s;
    for (int i = 0; i < MAX_WINDOWS; i++)
    {
        s = rand() % WORDS_IN_FILE;
        winningWords[i] = fileWords[s];
    }

    //std::wstring stemp = std::wstring(winningWords[0].begin(), winningWords[0].end());
    //LPCWSTR sw = stemp.c_str();
    //SetWindowText(hWnd, sw);
}

void CheckResult()
{
    int i;
    if (diff == EASY)
    {
        if (currentWord == winningWords[0])
        {
            Coloring();
            Win(0);
            return;
        }
        //Checking is word in the file
        for (i = 0; i < WORDS_IN_FILE; i++)
        {
            if (currentWord == fileWords[i])
            {
                break;
            }
        }
        if (i >= WORDS_IN_FILE)
        {
            ResetWord();
            return;
        }
        Coloring();
        return;
    }
    if (diff == MEDIUM)
    {
        for (int k = 0; k < 2; k++)
        {
            if (currentWord == winningWords[k])
            {
                Coloring();
                Win(k);
			}
		}

		//Checking is word in the file
		for (i = 0; i < WORDS_IN_FILE; i++)
		{
			if (currentWord == fileWords[i])
			{
				break;
			}
		}
		if (i >= WORDS_IN_FILE)
		{
			ResetWord();
			return;
		}
		Coloring();


        return;
    }
    if (diff == HARD)
    {
        for (int k = 0; k < 4; k++)
        {
            if (currentWord == winningWords[k])
            {
				Coloring();
				Win(k);
			}
		}


		//Checking is word in the file
		for (i = 0; i < WORDS_IN_FILE; i++)
		{
			if (currentWord == fileWords[i])
			{
				break;
			}
		}
		if (i >= WORDS_IN_FILE)
		{
			ResetWord();
			return;
		}
		Coloring();
        return;
    }
}
void Coloring()
{
    int tab[WORD_LEN];

    if (diff == EASY)
    {
        if (activeChild[0] == true)
        {
            CheckColors(tab, 0, EASY);
        }
        return;
    }
    if (diff == MEDIUM)
    {
        for (int i = 0; i < 2; i++)
        {
            if (activeChild[i] == true)
            {
                CheckColors(tab, i, MEDIUM);
            }
		}

		return;
	}
    if (diff == HARD)
    {
        for (int i = 0; i < MAX_WINDOWS; i++)
        {
            if (activeChild[i] == true)
            {
                CheckColors(tab, i, HARD);
            }
        }
		return;
    }
}
void CheckColors(int tab[], int wordNum, difficulty d)
{
    // 2 - dobre miejsce, 1 - zle miejsce, 0 - zla litera
    for (int i = 0; i < WORD_LEN; i++)
    {
        tab[i] = 0;
        for (int j = 0; j < WORD_LEN; j++)
        {
            if (currentWord[i] == winningWords[wordNum][j])
            {
                if (i == j)
                {
                    tab[i] = 2;
                }
                else
                {
                    if (tab[i] < 2)
                    {
                        tab[i] = 1;
                    }
                }
            }
        }
    }
    // tablica do kolorowania klawiatury
    int l = 0;
    for (int i = 0; i < WORD_LEN; i++)
    {
        for (l = 0; l < ALPHABET; l++)
        {
            if (keyboard[l] == currentWord[i])
                break;
        }
        
        if (colorTabKeyboard[wordNum][l] == 2)
        {
            // nie zmieniamy jak juz byl zielony
            continue;
        }

        colorTabKeyboard[wordNum][l] = tab[i];
    }
    // tablice do koloroan plansz:
    if (d == EASY)
    {
        for (int i = 0; i < WORD_LEN; i++)
        {
            colorTabEasy[(currentWordToWrite - 1) * WORD_LEN + i] = tab[i];
        }
    }
    if (d == MEDIUM)
    {
        for (int i = 0; i < WORD_LEN; i++)
        {
            colorTabMedium[wordNum][(currentWordToWrite - 1) * WORD_LEN + i] = tab[i];
        }
    }
    if (d == HARD)
    {
        for (int i = 0; i < WORD_LEN; i++)
        {
            colorTabHard[wordNum][(currentWordToWrite - 1) * WORD_LEN + i] = tab[i];
        }
    }
}
HBRUSH PickBrush(int c)
{
    HBRUSH brush;
    if (c == 0)
    {
        // zla litera
        brush = CreateSolidBrush(RGB(164, 174, 196));
    }
    else if (c == 1)
    {
        // zle miejsce
        brush = CreateSolidBrush(RGB(243, 194, 55));
    }
    else if (c == 2)
    {
        // dobra litera
        brush = CreateSolidBrush(RGB(121, 184, 81));
    }
    else
    {
        // domyslny kolor
        brush = CreateSolidBrush(RGB(251, 252, 255));
    }
    return brush;
}
HPEN PickPen(int c)
{
    HPEN pen;
    if (c == 0)
    {
        // zla litera
        pen = CreatePen(PS_SOLID, 2, RGB(164, 174, 196));
    }
    else if (c == 1)
    {
        // zle miejsce
        pen = CreatePen(PS_SOLID, 2, RGB(243, 194, 55));
    }
    else if (c == 2)
    {
        // dobra litera
        pen = CreatePen(PS_SOLID, 2, RGB(121, 184, 81));
    }
    else
    {
        // domyslny kolor
        pen = CreatePen(PS_SOLID, 2, RGB(211, 211, 211));
    }
    return pen;
}
void ResetWord()
{
    for (int i = 0; i < WORD_LEN; i++)
    {
        letters[curLettersCount - 1 - i] = '\0';
    }
    currentWordToWrite--;
    curLettersCount -= 5;
}
void Win(int c)
{
    KillTimer(child[c], 6);
    activeChild[c] = false;
}
