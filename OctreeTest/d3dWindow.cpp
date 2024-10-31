#include "d3dWindow.h"

//--------------------------------------------------------------------------------------
// ���������� ����������
//--------------------------------------------------------------------------------------
HINSTANCE					g_hInstance = NULL;				//���������� ����������
HWND							g_hWnd = NULL;            //���������� ����
int								g_iWindowWidth = 800;     //������ ����
int								g_iWindowHeight = 600;    //������ ����
bool							g_bApplicationState = true;    //��������� ���������� (true - ��������/false - �� ��������)
IDirect3D9				*g_pDirect3D = NULL;				//��������� ��� �������� ���������� ����������
IDirect3DDevice9	*g_pDirect3DDevice = NULL;  //��������� ���������� ����������

LPDIRECT3DVERTEXBUFFER9 pBufferVershin = NULL;
LPDIRECT3DINDEXBUFFER9 pBufferIndex = NULL;

//--------------------------------------------------------------------------------------
// ����� ����� � ���������. ����� �� ��� �������������� � ������ � ���� ���������.
// ����� ������� ���������� ��� ������ ������� ���������.
//--------------------------------------------------------------------------------------
int WINAPI WinD3DMain( )
{  
  g_hInstance = GetModuleHandle(NULL);

  WNDCLASSEX wc;
  wc.cbSize      = sizeof(WNDCLASSEX);        //������ ���������
  wc.style           = CS_HREDRAW|CS_VREDRAW;      //����� ������ ����
  wc.lpfnWndProc    = WndProc;									//������� ��������� ���������
  wc.cbClsExtra    = 0;													//���������� ���������� ������ ��� �������� ����������
  wc.cbWndExtra      = 0;												//���������� ���������� ������ ��� �������� ����������
  wc.hInstance    = g_hInstance;								//���������� ����������
  wc.hIcon           = LoadIcon(NULL,IDI_APPLICATION);  //��������� ����������� ������
  wc.hCursor         = LoadCursor(0,IDC_ARROW);					//��������� ����������� ������
  wc.hbrBackground   = (HBRUSH)GetStockObject(WHITE_BRUSH);//���� ����� ��������� � ����� ����
  wc.lpszMenuName    = 0;                //�� ���������� ����
  wc.lpszClassName   = L"Lesson 1";            //�������� ������
  wc.hIconSm       = LoadIcon(NULL,IDI_APPLICATION);  //��������� ����������� ������

  if(!RegisterClassEx(&wc))                //������������ ����� � Windows
  {
    Shutdown();														//����������� ������
    MessageBox(NULL, L"Can`t register window class", L"Error",MB_OK|MB_ICONERROR); //������� ���������
    return 0;															//��������� ������ ����������
  }

  g_hWnd = CreateWindowEx(              //������� ����
    WS_EX_APPWINDOW|WS_EX_WINDOWEDGE,        //����������� ����� ����
    L"Lesson 1",                    //�������� ������ ����
    L"Lesson 1 - Create Window. Init Direct3D",    //�������� ����
    WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,//����� ����
    0,                      //������� ���� �� ��� �
    0,                      //������� ���� �� ��� �
    g_iWindowWidth,                //������ ����
    g_iWindowHeight,              //������ ����
    NULL,                    //��� ���� ������� ����
    NULL,                    //���� ����
    g_hInstance,                //���������� ����������
    NULL);                    //�������������� �������� �� ����������

  if(g_hWnd == NULL)                //���� �� ������� ����
  {
    Shutdown();
    MessageBox( NULL, L"Can`t create window", L"Error", MB_OK|MB_ICONERROR );//������� ���������
    return 0;                  //��������� ������ ����������
  }

  if(!InitDirect3D(D3DFMT_R5G6B5,D3DFMT_D16))    //���� �� ������ ���������������� Direct3D
  {
    Shutdown();
    MessageBox( NULL, L"Can`t create direct3d", L"Error", MB_OK|MB_ICONERROR );//������� ���������
    return 0;                  //��������� ������ ����������
  }

  ShowWindow(g_hWnd,SW_SHOW);            //���������� ����  
  UpdateWindow(g_hWnd);              //��������� ����
  SetFocus(g_hWnd);                //������������� ����� �� ���� ����
  SetForegroundWindow(g_hWnd);          //������������� ��������� ���� ���� ��������

  MSG msg;
  ZeroMemory(&msg,sizeof(msg));

  while(g_bApplicationState)            //�������� ����������� ���� ��������� ���������
  {
    if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))//�������� ���������
    {
      TranslateMessage(&msg);          //������������ ���������
      DispatchMessage(&msg);          //������������ ���������
    }
    else
		{
			//DrawFrame();              //���� ��������� ���� ������ �����
			InitialObject();
			RenderingDirect3D();
		}
  }

  Shutdown();                    //����������� ������
  return 0;                    //��������� ������ ����������
}

void Matrix()
{
    D3DXMATRIX MatrixWorld;         // ������� �������
    D3DXMATRIX MatrixView;          // ������� ����
    D3DXMATRIX MatrixProjection;    // ������� ��������
    // MatrixWorld
    UINT Time       = timeGetTime() % 50000;
    FLOAT Angle     = (Time * (2.0f * D3DX_PI) / 5000.0f)*1;
    
    D3DXMatrixRotationX(&MatrixWorld, Angle);
 
    g_pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);
    // MatrixView
    D3DXMatrixLookAtLH( &MatrixView, &D3DXVECTOR3(0.0f, 0.0f, -8.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
    g_pDirect3DDevice->SetTransform( D3DTS_VIEW, &MatrixView );
    // MatrixProjection
    D3DXMatrixPerspectiveFovLH( &MatrixProjection, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    g_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);
}

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
//------------------------------------------------------------------------------
//�������
//InitialBufferVershin()
//�������������� ����� ������
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
 
        //X         Y       Z       ����
    };
 
    const unsigned short Index[]={
        0, 1, 2,    2, 3, 0,
        4, 5, 6,    6, 7, 4,
        8, 9, 10,   10, 11, 8,
        12, 13, 14,     14, 15, 12,
        16, 17, 18,     18, 19, 16,
        20, 21, 22,     22, 23, 20
    };
    
    if ( FAILED ( g_pDirect3DDevice->CreateVertexBuffer( 36*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pBufferVershin, NULL ) ) )
        return E_FAIL;
 
    VOID* pBV;
 
    if (FAILED(pBufferVershin->Lock(0,sizeof(Vershin),(VOID**)&pBV,0)))
        return E_FAIL;
 
    memcpy(pBV,Vershin,sizeof(Vershin));    //��������
 
    pBufferVershin->Unlock();   // ������������ ����� ������
 
    //������� ��������� �����
    g_pDirect3DDevice->CreateIndexBuffer( 6 * sizeof(Index), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pBufferIndex, NULL );
    
    //���������
    VOID* pBI;
    pBufferIndex->Lock(0, sizeof(Index), (void**)&pBI, 0);
    
    //��������
    memcpy(pBI, Index, sizeof(Index));
 
    //������������
    pBufferIndex->Unlock();
 
    return S_OK;
}

//------------------------------------------------------------------------------
//�������
//RenderingDirect3D()
//���������
//------------------------------------------------------------------------------
void RenderingDirect3D()
{
    g_pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(255, 255, 50), 1.0f, 0);
    
		g_pDirect3DDevice->BeginScene();      //������ �����
    
    //����� ���������� ���������� �����
    //Matrix();
    g_pDirect3DDevice->SetStreamSource( 0,pBufferVershin,0,sizeof(CUSTOMVERTEX) );
    g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
    g_pDirect3DDevice->SetIndices( pBufferIndex );
    g_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 360, 12, 36 );
    g_pDirect3DDevice->EndScene();        //����� �����
 
		g_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
}

long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
  switch(iMsg)
  {
    case WM_DESTROY:              //���� �������� ��������� � ���������� ����
    {
      g_bApplicationState = false;      //������������� ��������� ���������� � false (��� ������ ��� ���� ��������� ��������� ������������)
      return 0;                //������� ������� ��� �� ��� ��������� ����������
    }
  }

  return DefWindowProc(hWnd,iMsg,wParam,lParam);  //���� ���� ��� ��� ������ ���������, ����� ��� ������������ �������
}

bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat)
{
  if((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//������� ��������� Direct3D
    return false;                //����� ���������� false

  D3DPRESENT_PARAMETERS PresParam;        //��������� � ������� ������� �������� ���������� ���������� ���������� ��� ��� ��������
  ZeroMemory(&PresParam,sizeof(PresParam));    //��������

  HRESULT hr = NULL;                //������� ���������� ��� ������ � �� ����������� ������ �������

  D3DDISPLAYMODE DisplayMode;            //��������� ��� ��������� ���������� � ������ ����������� � �������
  hr = g_pDirect3D->GetAdapterDisplayMode(    //�������� ����� �����������
    D3DADAPTER_DEFAULT,              //���������� ��������� ����������
    &DisplayMode);                //���������� ����� ����������� � DisplayMode
  
  if(FAILED(hr))                  //���� �� ����������
    return false;                //���������� false

  PresParam.hDeviceWindow = g_hWnd;        //���������� ����
  PresParam.Windowed = true;            //������� �����?
  PresParam.BackBufferWidth = g_iWindowWidth;    //������ ������� ������
  PresParam.BackBufferHeight = g_iWindowHeight;  //������ ������� ������
  PresParam.BackBufferCount = 1;          //���������� ������ �������
  PresParam.EnableAutoDepthStencil = true;    //���������� ����� ������� � ������� �����
  PresParam.AutoDepthStencilFormat = DepthFormat;  //������ ������ �������
  PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;    //����� ����� ������
  PresParam.BackBufferFormat = DisplayMode.Format;//������������� ������ ������� ����������� � �������

  hr = g_pDirect3D->CreateDevice(        //������� ���������� ����������
   D3DADAPTER_DEFAULT,              //���������� ��������� ����������
   D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
   g_hWnd,                    //���������� ����
   D3DCREATE_HARDWARE_VERTEXPROCESSING,    //������������ �������� �����������
   &PresParam,                  //������ ��������� ����������
   &g_pDirect3DDevice);            //������� ���������� ����������
  
  if(SUCCEEDED(hr))                //���� ���������� 
    return true;                //���������� true

  hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
    D3DADAPTER_DEFAULT,              //���������� ��������� ����������
    D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
    g_hWnd,                    //���������� ����
    D3DCREATE_MIXED_VERTEXPROCESSING,      //������������ �������� ������� (����������� � �����������)
    &PresParam,                  //������ ��������� ����������
    &g_pDirect3DDevice);            //������� ���������� ����������

  if(SUCCEEDED(hr))                //���� ����������
    return true;                //���������� true

  hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
    D3DADAPTER_DEFAULT,              //���������� ��������� ����������
    D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
    g_hWnd,                    //���������� ����
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //������������ �������� ����������
    &PresParam,                  //������ ��������� ����������
    &g_pDirect3DDevice);            //������� ���������� ����������

  if(SUCCEEDED(hr))                //���� ����������
    return true;                //���������� true

  return false;                  //���������� false
}

void DrawFrame()
{
  HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//��������� ������� �� Direct3DDevice ����������
  
  if(hr == D3DERR_DEVICELOST)            //���� �� ��
    return;                   //������ �� �������  
  
  g_pDirect3DDevice->Clear(            //������� ������ �����
    0L,																	//������ ������, 0 - ���� �����
    NULL,																//������� ������� ����� �������, NULL - ���� �����
    D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //������ ������ ����� � ����� �������
    D3DCOLOR_XRGB(0,0,0),								//���� � ������� ������� ������ �����, � ����� ������ ������
    1.0f,															//������� ����� �������, �������� ��� ���������
    0L);															//���� ��������� ������������� ��� ��� �� ��������� �������������� ����

  g_pDirect3DDevice->BeginScene();        //������ �����

  g_pDirect3DDevice->EndScene();          //����� �����
  g_pDirect3DDevice->Present(NULL,NULL,NULL,NULL);//���������� ���� ������ �����
}

void Shutdown()
{
  if(g_pDirect3DDevice != NULL)          //���� �� ��� �� ���������� ��������� ����������
  {
    g_pDirect3DDevice->Release();        //�� ����������� ���
    g_pDirect3DDevice = NULL;          //� ������������� � ����
  }

  if(g_pDirect3D != NULL)              //���� �� ��� �� ���������� ��������� d3d
  {
    g_pDirect3D->Release();            //�� ����������� ���
    g_pDirect3D = NULL;              //� ������������� � ����
  }

  if(!DestroyWindow(g_hWnd))            //���� �� ���������� ��������� ����
    g_hWnd = NULL;                //������������� ���������� ���� � ����

  if( !UnregisterClass( L"Lesson 1", g_hInstance ) )  //���� �� ���������� ������� ���� ����������������� ����
    g_hInstance = NULL;              //������������� ���������� ���������� � ����
}