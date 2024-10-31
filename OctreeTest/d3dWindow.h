#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


struct CUSTOMVERTEX
{
    FLOAT X, Y, Z;
    DWORD color;
};
//--------------------------------------------------------------------------------------
// Предварительные объявления функций
//--------------------------------------------------------------------------------------
int WINAPI WinD3DMain( ); //Точка старта приложения
long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);//Обработчик сообщений
bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat);    //Инициализация Direct3D
void DrawFrame();                          //Рисуем кадр
void Shutdown();                          //Освобождаем память
void RenderingDirect3D();
HRESULT InitialObject();
