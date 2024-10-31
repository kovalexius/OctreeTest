#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


struct CUSTOMVERTEX
{
    FLOAT X, Y, Z;
    DWORD color;
};
//--------------------------------------------------------------------------------------
// ��������������� ���������� �������
//--------------------------------------------------------------------------------------
int WINAPI WinD3DMain( ); //����� ������ ����������
long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);//���������� ���������
bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat);    //������������� Direct3D
void DrawFrame();                          //������ ����
void Shutdown();                          //����������� ������
void RenderingDirect3D();
HRESULT InitialObject();
