#pragma once

class CCore
{
	SINGLE(CCore);

private:
	HWND m_hWnd;			// ���� ������ �ڵ�
	POINT m_ptResolution;	// ���� ������ �ػ�
	HDC m_hDC;				// ���� ������ DC

	// ���� ���۸��� ���� ��� ����
	HDC m_memDC;
	HBITMAP m_hBit;

 
public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();
	
	void ChangeWindowSize(Vec2 _vResolution, bool bMenu);

	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	HDC GetMainDC() { return m_hDC; }

public:
	void render();
};

