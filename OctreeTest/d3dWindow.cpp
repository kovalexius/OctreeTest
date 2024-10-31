#include "d3dWindow.h"

//--------------------------------------------------------------------------------------
// Глобальные переменные
//--------------------------------------------------------------------------------------
HINSTANCE					g_hInstance = NULL;				//Дескриптор приложения
HWND							g_hWnd = NULL;            //Дескриптор окна
int								g_iWindowWidth = 800;     //Ширина окна
int								g_iWindowHeight = 600;    //Высота окна
bool							g_bApplicationState = true;    //Состояние приложения (true - работает/false - не работает)
IDirect3D9				*g_pDirect3D = NULL;				//Интерфейс для создания устройства рендеринга
IDirect3DDevice9	*g_pDirect3DDevice = NULL;  //Интерфейс устройства рендеринга

LPDIRECT3DVERTEXBUFFER9 pBufferVershin = NULL;
LPDIRECT3DINDEXBUFFER9 pBufferIndex = NULL;

//--------------------------------------------------------------------------------------
// Точка входа в программу. Здесь мы все инициализируем и входим в цикл сообщений.
// Время простоя используем для вызова функции рисования.
//--------------------------------------------------------------------------------------
int WINAPI WinD3DMain( )
{  
  g_hInstance = GetModuleHandle(NULL);

  WNDCLASSEX wc;
  wc.cbSize      = sizeof(WNDCLASSEX);        //Размер структуры
  wc.style           = CS_HREDRAW|CS_VREDRAW;      //Стили класса окна
  wc.lpfnWndProc    = WndProc;									//Функция обработки сообщений
  wc.cbClsExtra    = 0;													//Количество выделяемой памяти при создании приложения
  wc.cbWndExtra      = 0;												//Количество выделяемой памяти при создании приложения
  wc.hInstance    = g_hInstance;								//Дескриптор приложения
  wc.hIcon           = LoadIcon(NULL,IDI_APPLICATION);  //Загружаем стандартную иконку
  wc.hCursor         = LoadCursor(0,IDC_ARROW);					//Загружаем стандартный курсор
  wc.hbrBackground   = (HBRUSH)GetStockObject(WHITE_BRUSH);//Окно будет закрашено в белый цвет
  wc.lpszMenuName    = 0;                //Не используем меню
  wc.lpszClassName   = L"Lesson 1";            //Названия класса
  wc.hIconSm       = LoadIcon(NULL,IDI_APPLICATION);  //Загружаем стандартную иконку

  if(!RegisterClassEx(&wc))                //Регистрируем класс в Windows
  {
    Shutdown();														//Освобождаем память
    MessageBox(NULL, L"Can`t register window class", L"Error",MB_OK|MB_ICONERROR); //Выводим сообщение
    return 0;															//Завершаем работу приложения
  }

  g_hWnd = CreateWindowEx(              //Создаем окно
    WS_EX_APPWINDOW|WS_EX_WINDOWEDGE,        //Расширенный стиль окна
    L"Lesson 1",                    //Названия класса окна
    L"Lesson 1 - Create Window. Init Direct3D",    //Названия окна
    WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,//Стиль окна
    0,                      //Позиция окна по оси Х
    0,                      //Позиция окна по оси У
    g_iWindowWidth,                //Ширина окна
    g_iWindowHeight,              //Высота окна
    NULL,                    //Это наше главное окно
    NULL,                    //Нету меню
    g_hInstance,                //Дескриптор приложения
    NULL);                    //Дополнительный настроек не используем

  if(g_hWnd == NULL)                //Если не создали окно
  {
    Shutdown();
    MessageBox( NULL, L"Can`t create window", L"Error", MB_OK|MB_ICONERROR );//Выводим сообщение
    return 0;                  //Завершаем работу приложения
  }

  if(!InitDirect3D(D3DFMT_R5G6B5,D3DFMT_D16))    //Если не смогли инициализировать Direct3D
  {
    Shutdown();
    MessageBox( NULL, L"Can`t create direct3d", L"Error", MB_OK|MB_ICONERROR );//Выводим сообщение
    return 0;                  //Завершаем работу приложения
  }

  ShowWindow(g_hWnd,SW_SHOW);            //Отображаем окно  
  UpdateWindow(g_hWnd);              //Обновляем окно
  SetFocus(g_hWnd);                //Устанавливаем фокус на наше окно
  SetForegroundWindow(g_hWnd);          //Устанавливаем приоритет окна выше среднего

  MSG msg;
  ZeroMemory(&msg,sizeof(msg));

  while(g_bApplicationState)            //Начинаем бесконечный цикл обработки сообщений
  {
    if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))//Получаем сообщения
    {
      TranslateMessage(&msg);          //Обрабатываем сообщения
      DispatchMessage(&msg);          //Обрабатываем сообщения
    }
    else
		{
			//DrawFrame();              //Если сообщений нету рисуем кадры
			InitialObject();
			RenderingDirect3D();
		}
  }

  Shutdown();                    //Освобождаем память
  return 0;                    //Завершаем работу приложения
}

void Matrix()
{
    D3DXMATRIX MatrixWorld;         // мировая матрица
    D3DXMATRIX MatrixView;          // матрица вида
    D3DXMATRIX MatrixProjection;    // матрица проекции
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
    
    if ( FAILED ( g_pDirect3DDevice->CreateVertexBuffer( 36*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pBufferVershin, NULL ) ) )
        return E_FAIL;
 
    VOID* pBV;
 
    if (FAILED(pBufferVershin->Lock(0,sizeof(Vershin),(VOID**)&pBV,0)))
        return E_FAIL;
 
    memcpy(pBV,Vershin,sizeof(Vershin));    //Копируем
 
    pBufferVershin->Unlock();   // Разблокируем буфер вершин
 
    //Создаем индексный буфер
    g_pDirect3DDevice->CreateIndexBuffer( 6 * sizeof(Index), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pBufferIndex, NULL );
    
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
//RenderingDirect3D()
//рисование
//------------------------------------------------------------------------------
void RenderingDirect3D()
{
    g_pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(255, 255, 50), 1.0f, 0);
    
		g_pDirect3DDevice->BeginScene();      //Начало сцены
    
    //Здесь происходит прорисовка сцены
    //Matrix();
    g_pDirect3DDevice->SetStreamSource( 0,pBufferVershin,0,sizeof(CUSTOMVERTEX) );
    g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
    g_pDirect3DDevice->SetIndices( pBufferIndex );
    g_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 360, 12, 36 );
    g_pDirect3DDevice->EndScene();        //Конец сцены
 
		g_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
}

long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
  switch(iMsg)
  {
    case WM_DESTROY:              //Если получаем сообщение о разрушении окна
    {
      g_bApplicationState = false;      //Устанавливаем состояния приложения в false (это значит что цикл обработки сообщений остановиться)
      return 0;                //Говорим виндовс что мы это сообщение обработали
    }
  }

  return DefWindowProc(hWnd,iMsg,wParam,lParam);  //Если нету для нас нужных сообщений, пусть это обрабатывает виндовс
}

bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat)
{
  if((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//Создаем интерфейс Direct3D
    return false;                //Иначе возвращяем false

  D3DPRESENT_PARAMETERS PresParam;        //Структура с помощью которой передаем информацию устройству рендеринга при его создании
  ZeroMemory(&PresParam,sizeof(PresParam));    //Обнуляем

  HRESULT hr = NULL;                //Создаем переменную для записи в неё результатов работы функций

  D3DDISPLAYMODE DisplayMode;            //Структура для получения информации о режиме отображения в виндовс
  hr = g_pDirect3D->GetAdapterDisplayMode(    //Получаем режим отображения
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    &DisplayMode);                //Записываем режим отображения в DisplayMode
  
  if(FAILED(hr))                  //Если не получилось
    return false;                //Возвращаем false

  PresParam.hDeviceWindow = g_hWnd;        //Дескриптор окна
  PresParam.Windowed = true;            //Оконный режим?
  PresParam.BackBufferWidth = g_iWindowWidth;    //Ширина заднего буфера
  PresParam.BackBufferHeight = g_iWindowHeight;  //Высота заднего буфера
  PresParam.BackBufferCount = 1;          //Количество задних буферов
  PresParam.EnableAutoDepthStencil = true;    //Используем буфер глубины и стенцил буфер
  PresParam.AutoDepthStencilFormat = DepthFormat;  //Формат буфера глубины
  PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;    //Режим смены кадров
  PresParam.BackBufferFormat = DisplayMode.Format;//Устанавливаем формат пикселя определеный в виндовс

  hr = g_pDirect3D->CreateDevice(        //Создаем устройство рендеринга
   D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
   D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
   g_hWnd,                    //Дескриптор окна
   D3DCREATE_HARDWARE_VERTEXPROCESSING,    //Обрабатываем вершинны видеокартой
   &PresParam,                  //Отдаем параметры устройства
   &g_pDirect3DDevice);            //Создаем устройство рендеринга
  
  if(SUCCEEDED(hr))                //Если получилось 
    return true;                //Возвращяем true

  hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
    g_hWnd,                    //Дескриптор окна
    D3DCREATE_MIXED_VERTEXPROCESSING,      //Обрабатываем вершинны смешано (видеокартой и процессором)
    &PresParam,                  //Отдаем параметры устройства
    &g_pDirect3DDevice);            //Создаем устройство рендеринга

  if(SUCCEEDED(hr))                //Если получилось
    return true;                //Возвращяем true

  hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
    g_hWnd,                    //Дескриптор окна
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //Обрабатываем вершинны процесором
    &PresParam,                  //Отдаем параметры устройства
    &g_pDirect3DDevice);            //Создаем устройство рендеринга

  if(SUCCEEDED(hr))                //Если получилось
    return true;                //Возвращяем true

  return false;                  //Возвращяем false
}

void DrawFrame()
{
  HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//Проверяем потерял ли Direct3DDevice устройство
  
  if(hr == D3DERR_DEVICELOST)            //Если да то
    return;                   //Выходи из функции  
  
  g_pDirect3DDevice->Clear(            //Очищаем задний буфер
    0L,																	//Размер буфера, 0 - весь буфер
    NULL,																//Область которую будем очищать, NULL - весь буфер
    D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //Чистим задний буфер и буфер глубины
    D3DCOLOR_XRGB(0,0,0),								//Цвет в который очищаем задний буфер, в нашем случае черный
    1.0f,															//Очищаем буфер глубины, заполнив его единицами
    0L);															//Этот параметрт игнорируеться так как не выстовлен соответсвующий флаг

  g_pDirect3DDevice->BeginScene();        //Начало сцены

  g_pDirect3DDevice->EndScene();          //Конец сцены
  g_pDirect3DDevice->Present(NULL,NULL,NULL,NULL);//Отображаем весь задний буфер
}

void Shutdown()
{
  if(g_pDirect3DDevice != NULL)          //Если мы еще не освободили интерфейс рендеринга
  {
    g_pDirect3DDevice->Release();        //То освобождаем его
    g_pDirect3DDevice = NULL;          //И устанавливаем в ноль
  }

  if(g_pDirect3D != NULL)              //Если мы еще не освободили интерфейс d3d
  {
    g_pDirect3D->Release();            //То освобождаем его
    g_pDirect3D = NULL;              //И устанавливаем в ноль
  }

  if(!DestroyWindow(g_hWnd))            //Если не получилось разрушить окно
    g_hWnd = NULL;                //Устанавливаем дескриптор окна в ноль

  if( !UnregisterClass( L"Lesson 1", g_hInstance ) )  //Если не получилось удалить наше зарегестрированое окно
    g_hInstance = NULL;              //Устанавливаем дескриптор приложения в ноль
}