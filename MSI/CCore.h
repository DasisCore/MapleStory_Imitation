#pragma once

class CCore
{
	SINGLE(CCore);

private:
	HWND m_hWnd;			// 메인 윈도우 핸들
	POINT m_ptResolution;	// 메인 윈도우 해상도
	HDC m_hDC;				// 메인 윈도우 DC

	// 더블 버퍼링을 위한 멤버 변수
	HDC m_memDC;
	HBITMAP m_hBit;

	HBRUSH m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN m_arrPen[(UINT)PEN_TYPE::END];

 
public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();

private:
	void CreateBrushPen();

public:
	void ChangeWindowSize(Vec2 _vResolution, bool bMenu);

	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	HDC GetMainDC() { return m_hDC; }


	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

public:
	void render();
};

