#pragma once

class CTexture;

class CCore
{
	SINGLE(CCore);

private:
	HWND m_hWnd;			// ���� ������ �ڵ�
	POINT m_ptResolution;	// ���� ������ �ػ�
	HDC m_hDC;				// ���� ������ DC
	HINSTANCE m_hInstance;	// ���� hInstance

	// ���� ���۸��� ���� ��� ����
	CTexture* m_pMemTex;

	HBRUSH m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN m_arrPen[(UINT)PEN_TYPE::END];

	// �޴�
	HMENU m_hMenu;	// Tool_Scene������ ���

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

