#define WIN32_LEAN_AND_MEAN //exludes rarely-used options in windows.h
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//colors

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)
#define YELLOW RGB(255, 255, 0)
#define ORANGE RGB(255, 150, 0)
#define PURPLE RGB(255, 0, 255)
#define BLACK RGB(0, 0, 0)
#define RANDOM_COLOR RGB(rand() % 256, rand() % 256, rand() % 256)

//globals

int winxsz, winysz, winxloc, winyloc;
int mousex, mousey;
int currentColor;
COLORREF lastRandomColor;
BOOL paintOn = 0;
int brushSize = 1;

//prototypes

void BrushPaintSpot(HDC hdc, int x, int y, COLORREF color, int bSize);
int wtstrlen(LPCWSTR str);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	ps.fErase = TRUE;
	HDC hdc;

	switch(msg)
	{
		case WM_CREATE: //window startup procedures go here
		{
			srand((unsigned)time(NULL));
			return(0);
		}
		case WM_PAINT: //if the window needs painting
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd , &ps);
			return(0);
		}
		case WM_MOVE: //if the window gets moved
		{
			winxloc = LOWORD(lParam);
			winyloc = HIWORD(lParam);
			return(0);
		}
		case WM_SIZE: //if the window gets resized
		{
			winxsz = LOWORD(lParam);
			winysz = HIWORD(lParam);
			return(0);
		}
		case WM_DESTROY: //if the Close or Exit buttons get clicked
		{
			PostQuitMessage(0);
			return(0);
		}
		case WM_COMMAND: //if the user clicks on a menu button
		{
			switch(wParam)
			{
				case 1:
				{
					PostMessage(hwnd, WM_QUIT, NULL, NULL);
					break;
				}
				case 1001:
				{
					currentColor = 1;
					break;
				}
				case 1002:
				{
					currentColor = 2;
					break;
				}
				case 1003:
				{
					currentColor = 3;
					break;
				}
				case 1004:
				{
					currentColor = 4;
					break;
				}
				case 1005:
				{
					currentColor = 5;
					break;
				}
				case 1006:
				{
					currentColor = 6;
					break;
				}
				case 1007:
				{
					currentColor = 7;
					break;
				}
				case 1010:
				{
					currentColor = 10;
					break;
				}
				case 2001:
				{
					brushSize = 1;
					break;
				}
				case 2002:
				{
					brushSize = 2;
					break;
				}
				case 2003:
				{
					brushSize = 3;
					break;
				}
				case 2004:
				{
					brushSize = 4;
					break;
				}
			}
		}
		case WM_MOUSEMOVE: //if the user moves the mouse
		{
			mousex = (int)LOWORD(lParam);
			mousey = (int)HIWORD(lParam);
			wchar_t rgbstring[50];
			if(paintOn)
			{
				hdc = GetDC(hwnd);
				switch(currentColor)
				{
					case 1:
					{
						BrushPaintSpot(hdc, mousex, mousey, RED, brushSize);
						break;
					}
					case 2:
					{
						BrushPaintSpot(hdc, mousex, mousey, GREEN, brushSize);
						break;
					}
					case 3:
					{
						BrushPaintSpot(hdc, mousex, mousey, BLUE, brushSize);
						break;
					}
					case 4:
					{
						BrushPaintSpot(hdc, mousex, mousey, YELLOW, brushSize);
						break;
					}
					case 5:
					{
						BrushPaintSpot(hdc, mousex, mousey, ORANGE, brushSize);
						break;
					}
					case 6:
					{
						BrushPaintSpot(hdc, mousex, mousey, PURPLE, brushSize);
						break;
					}
					case 7:
					{
						BrushPaintSpot(hdc, mousex, mousey, BLACK, brushSize);
						break;
					}
					case 10:
					{
						lastRandomColor = RANDOM_COLOR;
						BrushPaintSpot(hdc, mousex, mousey, lastRandomColor, brushSize);
						wsprintf(rgbstring, L"Magic Pen: Last pixel RGB value %d, %d, %d", (int)GetRValue(lastRandomColor), (int)GetGValue(lastRandomColor), (int)GetBValue(lastRandomColor));
						TextOutW(hdc, 10, winysz - 25, rgbstring, wstrlen(rgbstring));
						break;
					}
				}
				ReleaseDC(hwnd, hdc);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			paintOn = 1;
			break;
		}
		case WM_LBUTTONUP:
		{
			paintOn = 0;
			break;
		}
		default: break;
	}
	return(DefWindowProc(hwnd, msg, wParam, lParam));
	
}// end WinProc()

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	HDC hdc; //handle to GDI device context
	MSG msg;
	HWND hwnd;
	WNDCLASS defaultWindowClass;
	defaultWindowClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	defaultWindowClass.lpfnWndProc = WindowProc;
	defaultWindowClass.cbClsExtra = 0;
	defaultWindowClass.cbWndExtra = 0;
	defaultWindowClass.hInstance = hinstance;
	defaultWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	defaultWindowClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	defaultWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	defaultWindowClass.lpszMenuName = L"MainMenu";
	defaultWindowClass.lpszClassName = L"WindowClassName";

	if(!RegisterClass(&defaultWindowClass)) //register window class
	{
		return(1); //if the class can't be registered
	}

	//create the window
	if(!(hwnd = CreateWindow(L"WindowClassName", //class
		L"MagicPaint", //window title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, //flags
		40,70, //x,y
		800,600, //width, height
		NULL, //handle to parent
		NULL, //handle to menu
		hinstance, //instance
		NULL))) //creation parameters
	{
		return(0); //if the window can't be created
	}
	while(1) //infinite loop
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg); //translate any accelerator keys
			DispatchMessage(&msg); //send message to WindowProc()
		}
		//main game processing goes here
	}
	return(msg.wParam);
} // end WinMain()

void BrushPaintSpot(HDC hdc, int x, int y, COLORREF color, int bSize)
{
	int a, b;
	
	//this nested loop means that the area painted in this function is equal to bSize^2
	for(b = y; b < (y + bSize); b++)
	{
		for(a = x; a < (x + bSize); a++)
		{
			SetPixel(hdc, a, b, color);
		}
	}
} // end BrushPaintSpot()

int wstrlen(LPCWSTR str)
{
	int x = 0;

	while(str[x] != '\0')
	{
		x++;
	}
	return(++x);
} // end wstrlen()