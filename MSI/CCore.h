#pragma once

class CTexture;

class CCore
{
	SINGLE(CCore);

private:
	HWND m_hWnd;			// 메인 윈도우 핸들
	POINT m_ptResolution;	// 메인 윈도우 해상도
	HDC m_hDC;				// 메인 윈도우 DC
	HINSTANCE m_hInstance;	// 메인 hInstance

	// 더블 버퍼링을 위한 멤버 변수
	CTexture* m_pMemTex;

	HBRUSH m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN m_arrPen[(UINT)PEN_TYPE::END];

	// 메뉴
	HMENU m_hMenu;	// Tool_Scene에서만 사용

	// GDI+ 
	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput m_gdiplusStartupInput;
 
public:
	int init(HWND _hWnd, POINT _ptResolution, HINSTANCE _hInstance);
	void progress();

private:
	void CreateBrushPen();
	void Clear();

public:
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool bMenu);

public:

	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	HDC GetMainDC() { return m_hDC; }
	HINSTANCE GetMainhInstance() { return m_hInstance; }
	CTexture* GetMemTex() { return m_pMemTex; }
	void SetMemTex(CTexture* _pTex) { m_pMemTex = _pTex; }

	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

public:
	void render();
};

