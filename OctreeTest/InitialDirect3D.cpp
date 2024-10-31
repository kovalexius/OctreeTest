#include "InitialDirect3D.h"
 
//------------------------------------------------------------------------------
//Глобальные переменные
//------------------------------------------------------------------------------
LPDIRECT3D9 pDirect3D=NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice=NULL; 
LPDIRECT3DVERTEXBUFFER9 pBufferVershin = NULL;
LPDIRECT3DINDEXBUFFER9 pBufferIndex = NULL;
 
int i=0;
 
struct CUSTOMVERTEX
{
    FLOAT X, Y, Z;
    DWORD color;
};
 
 
 
 
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
//------------------------------------------------------------------------------
//функция
//InitialBufferVershin()
//инициализирует буфер вершин
//------------------------------------------------------------------------------
HRESULT InitialObject()
{
    CUSTOMVERTEX Vershin[]=
    {
        {1.0f,      -1.0f,  -1.0f,  0x00000ff}, //A
        {-1.0f,     -1.0f,  0.0f,   0xfff000f}, //B
        {-1.0f,     1.0f,   0.0f,   0x000ffff}, //C
        {1.0f,      1.0f,   0.0f,   0xfff000f}, //D
 
        {-1.0f,     -1.0f,  1.0f,   0xfffffff}, //A2
        {-1.0f,     -1.0f,  -1.0f,  0xfff000f}, //B2
        {-1.0f,     1.0f,   -1.0f,  0x000ffff}, //C2
        {-1.0f,     1.0f,   1.0f,   0xfff000f}, //D2
 
        {1.0f,      -1.0f,  1.0f,   0x00000ff}, //A3
        {-1.0f,     -1.0f,  1.0f,   0xfff000f}, //B3
        {-1.0f,     1.0f,   1.0f,   0x000ffff}, //C3
        {1.0f,      1.0f,   1.0f,   0xfff000f}, //D3
 
        {1.0f,      -1.0f,  -1.0f,  0x00000ff}, //A4
        {1.0f,      -1.0f,  1.0f,   0xfff000f}, //B4
        {1.0f,      1.0f,   1.0f,   0x000ffff}, //C4
        {1.0f,      1.0f,   -1.0f,  0xfff000f}, //D4
 
        {1.0f,      1.0f,   1.0f,   0x00000ff}, //A5
        {-1.0f,     1.0f,   1.0f,   0xfff000f}, //B5
        {-1.0f,     1.0f,   -1.0f,  0x000ffff}, //C5
        {1.0f,      1.0f,   -1.0f,  0xfff000f}, //D5
 
        {1.0f,      -1.0f,  1.0f,   0x00000ff}, //A6
        {-1.0f,     -1.0f,  1.0f,   0xfff000f}, //B6
        {-1.0f,     -1.0f,  -1.0f,  0x000ffff}, //C6
        {1.0f,      -1.0f,  -1.0f,  0xfff000f}, //D6
 
        //X         Y       Z       цвет
    };
 
    const unsigned short Index[]={
        0, 1, 2,    2, 3, 0,
        4, 5, 6,    6, 7, 4,
        8, 9, 10,   10, 11, 8,
        12, 13, 14,     14, 15, 12,
        16, 17, 18,     18, 19, 16,
        20, 21, 22,     22, 23, 20
    };
    
    if (FAILED(pDirect3DDevice->CreateVertexBuffer(36*sizeof(CUSTOMVERTEX),
        0,
        D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT,
        &pBufferVershin,
        NULL)))
        return E_FAIL;
 
    VOID* pBV;
 
    if (FAILED(pBufferVershin->Lock(0,sizeof(Vershin),(VOID**)&pBV,0)))
        return E_FAIL;
 
    memcpy(pBV,Vershin,sizeof(Vershin));    //Копируем
 
    pBufferVershin->Unlock();   // Разблокируем буфер вершин
 
    //Создаем индексный буфер
    pDirect3DDevice->CreateIndexBuffer(6 * sizeof(Index), 0,
        D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pBufferIndex, NULL);
    
    //блокируем
    VOID* pBI;
    pBufferIndex->Lock(0, sizeof(Index), (void**)&pBI, 0);
    
    //Копируем
    memcpy(pBI, Index, sizeof(Index));
 
    //разблокируем
    pBufferIndex->Unlock();
 
    return S_OK;
}
//------------------------------------------------------------------------------
//Функция
//InitialDirect3D()
//инициализация Direct3D
//------------------------------------------------------------------------------
HRESULT InitialDirect3D(HWND hwnd)
{
    if (NULL==(pDirect3D=Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;
    D3DDISPLAYMODE Display;
    if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Display)))
        return E_FAIL;
    D3DPRESENT_PARAMETERS Direct3DParametr;
    ZeroMemory(&Direct3DParametr,sizeof(Direct3DParametr));
    Direct3DParametr.Windowed=TRUE;
    Direct3DParametr.SwapEffect=D3DSWAPEFFECT_DISCARD;
    Direct3DParametr.BackBufferFormat=Direct3DParametr.AutoDepthStencilFormat;
    if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &Direct3DParametr, &pDirect3DDevice)))
        return E_FAIL;
    //отключить отсечение Direct3D
    pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    //отключить освещение Direct3D
    pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
 
    return S_OK;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
VOID Matrix()
{
    
    D3DXMATRIX MatrixWorld;         // мировая матрица
    D3DXMATRIX MatrixView;          // матрица вида
    D3DXMATRIX MatrixProjection;    // матрица поэкции
    // MatrixWorld
    UINT Time       = timeGetTime() % 50000;
    FLOAT Angle     = (Time * (2.0f * D3DX_PI) / 5000.0f)*1;
    
 
        D3DXMatrixRotationX(&MatrixWorld, Angle);
 
    pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);
    // MatrixView
    D3DXMatrixLookAtLH(&MatrixView, &D3DXVECTOR3(0.0f, 0.0f, -8.0f),
                        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    pDirect3DDevice->SetTransform(D3DTS_VIEW, &MatrixView);
    // MatrixProjection
    D3DXMatrixPerspectiveFovLH(&MatrixProjection, D3DX_PI/4,
                                1.0f, 1.0f, 100.0f);
    pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);
}
//------------------------------------------------------------------------------
//Функция
//RenderingDirect3D()
//рисование
//------------------------------------------------------------------------------
VOID RenderingDirect3D()
{
    pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_XRGB(255, 255, 50), 1.0f, 0);
 
    pDirect3DDevice->BeginScene();      //Начало сцены
    
    //Здесь происходит прорисовка сцены
    Matrix();
    pDirect3DDevice->SetStreamSource(0,pBufferVershin,0,sizeof(CUSTOMVERTEX));
    pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
    pDirect3DDevice->SetIndices(pBufferIndex);
 
    pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 360, 12, 36);
 
 
 
    pDirect3DDevice->EndScene();        //Конец сцены
 
        pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
//Функция 
//DeleteDirect3D()
//освобождение захваченых ресурсов
//------------------------------------------------------------------------------
VOID DeleteDirect3D()
{
    if (pBufferIndex!=NULL)
        pBufferIndex->Release();
    if (pBufferVershin!=NULL)
        pBufferVershin->Release();
    if (pDirect3DDevice!=NULL)
        pDirect3DDevice->Release();
    if (pDirect3D!=NULL)
        pDirect3D->Release();
}
//------------------------------------------------------------------------------
//Функция 
//MainWinProc();
//Здесь происходит обработка сообщений
//------------------------------------------------------------------------------
LRESULT CALLBACK MainWinProc(HWND hwnd,     //дескриптор окна
                             UINT msg,      //идентификатор сообщений
                             WPARAM wparam, //дополнительная информация
                             LPARAM lparam)//дополнительная информация
{
    switch(msg)
    {
    case WM_DESTROY:
        {
            DeleteDirect3D();
            PostQuitMessage(0);
            return(0);
        }
    }
    return (DefWindowProc(hwnd,msg,wparam,lparam));
}
//------------------------------------------------------------------------------
//функция
//WinMain();
//Входная точка приложения
//------------------------------------------------------------------------------
int WINAPI WinMainD3D( /*HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR LpCmdLine,
                   int nCmdShow*/ )
{
	HINSTANCE hinstance = GetModuleHandle(NULL);

    WNDCLASSEX windowsclass;    //создаем класс
    HWND hwnd;                  //создаем дескриптор окна
    MSG msg;                    //идентификатор сообщений
    //определим класс окна WNDCLASSEX
    //static TCHAR szWindowClass[] = _T("WindowClass");
    windowsclass.cbSize         =sizeof(WNDCLASSEX);
    windowsclass.style          =CS_VREDRAW|CS_HREDRAW|CS_OWNDC|CS_DBLCLKS;
    windowsclass.lpfnWndProc    =MainWinProc;
    windowsclass.cbClsExtra     =0;
    windowsclass.cbWndExtra     =0;
    windowsclass.hInstance      =0;
    windowsclass.hIcon          =LoadIcon( 0, IDI_APPLICATION );
    windowsclass.hCursor        =LoadCursor(NULL,IDC_ARROW);
    windowsclass.hbrBackground  =(HBRUSH)GetStockObject(BLACK_BRUSH);
    windowsclass.lpszMenuName   =NULL;
    windowsclass.lpszClassName  =(LPCWSTR)("WINDOWSCLASS");
    windowsclass.hIconSm        =LoadIcon(0,IDI_APPLICATION);
    //зарегестрируем класс
    if(!RegisterClassEx(&windowsclass))
        return (0);
    //можно создавать окно
    hwnd=CreateWindow((LPCWSTR)("WINDOWSCLASS"),
            (LPCWSTR) (" "),
            WS_MAXIMIZE,
            0,0,                                            //Верхняя левая точкка
            600,400,                                        //Ширина и высота
            NULL, 
            NULL, 
            hinstance,
            NULL);
 
    if(!hwnd)                           
        return(0);
 
    if (SUCCEEDED(InitialDirect3D(hwnd)))
    {   
        InitialObject();
        ShowWindow(hwnd, SW_SHOWDEFAULT);   //нарисуем окно
        UpdateWindow(hwnd);                 //обновим окно
        ZeroMemory(&msg,sizeof(msg));
 
        RenderingDirect3D();
        while(msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
                RenderingDirect3D();
 
        }
        DeleteDirect3D();
    }
    return (msg.wParam);
}