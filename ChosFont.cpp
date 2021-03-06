// ChosFont.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ChosFont.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
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

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHOSFONT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHOSFONT));

    MSG msg;

    // メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHOSFONT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHOSFONT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CHOOSEFONT cf;
	static int cyChar;
	static LOGFONT lf;
	static TCHAR szText[] = TEXT("\x41\x42\x43\x44\x45 ")
		TEXT("\x61\x62\x63\x64\x65 ")
		TEXT("\xC0\xC1\xC2\xC3\xC4\xC5 ")
		TEXT("\xE0\xE1\xE2\xE3\xE4\xE5 ")
#ifdef UNICODE
		TEXT("\x0390\x0391\x0392\x0393\x0394\x0395 ")
		TEXT("\x03B0\x03B1\x03B2\x03B3\x03B4\x03B5 ")
		TEXT("\x0410\x0411\x0412\x0413\x0414\x0415 ")
		TEXT("\x0430\x0431\x0432\x0433\x0434\x0435 ")
		TEXT("\x5000\x5001\x5002\x5003\x5004")
#endif
		;
	HDC hdc;
	int y;
	PAINTSTRUCT ps;
	TCHAR szBuffer[64];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		// Get text height
		cyChar = HIWORD(GetDialogBaseUnits());
		// Initialize the LOGFONT structure
		GetObject(GetStockObject(SYSTEM_FONT), sizeof(lf), &lf);
		// Initialize the CHOOSEFONT structure
		cf.lStructSize = sizeof(CHOOSEFONT);
		cf.hwndOwner = hwnd;
		cf.hDC = NULL;
		cf.lpLogFont = &lf;
		cf.iPointSize = 0;
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		cf.rgbColors = 0;
		cf.lCustData = 0;
		cf.lpfnHook = NULL;
		cf.lpTemplateName = NULL;
		cf.hInstance = NULL;
		cf.lpszStyle = NULL;
		cf.nFontType = 0;
		cf.nSizeMin = 0;
		cf.nSizeMax = 0;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FONT:
			if (ChooseFont(&cf))
				InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// Display sample text using selected font
		SelectObject(hdc, CreateFontIndirect(&lf));
		GetTextMetrics(hdc, &tm);
		SetTextColor(hdc, cf.rgbColors);
		TextOut(hdc, 0, y = tm.tmExternalLeading, szText, lstrlen(szText));
		
		// Display LOGFONT structure fields using system font
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		SetTextColor(hdc, 0);
		TextOut(hdc, 0, y += tm.tmHeight, szBuffer, wsprintf(szBuffer, TEXT("lfHeight = %i"), lf.lfHeight));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfWidth = %i"), lf.lfWidth));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfEscapement = %i"), lf.lfEscapement));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfOrientation = %i"), lf.lfOrientation));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfWeight = %i"), lf.lfWeight));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfItalic = %i"), lf.lfItalic));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfUnderline = %i"), lf.lfUnderline));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfStrikeOut = %i"), lf.lfStrikeOut));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfCharSet = %i"), lf.lfCharSet));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfOutPrecision = %i"), lf.lfOutPrecision));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfClipPrecision = %i"), lf.lfClipPrecision));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfQuality = %i"), lf.lfQuality));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfPitchAndFamily = 0x%02X"), lf.lfPitchAndFamily));
		TextOut(hdc, 0, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("lfFaceName = %s"), lf.lfFaceName));

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
