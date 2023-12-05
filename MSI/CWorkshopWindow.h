#pragma once

class CWorkshopWindow
{
	SINGLE(CWorkshopWindow);

private:
	HWND m_hWndWorkshop;
	HDC m_WorkshopMainDC;
	HWND m_hWndList;
	UINT m_iTargetFrame;

	// ������۸�
	HDC m_memDC;


public:
	void init();
	void update();
	void render(HDC _dc);

public:
	void AddFrame(UINT _idx);
	void DeleteFrame();

public:
	void showWindow();
	void SetTargetFrm(UINT _i) { m_iTargetFrame = _i; }
	

public:
	HWND GetWorkshophWnd() { return m_hWndWorkshop; }
	HWND GetListhWnd() { return m_hWndList; }
	void Destroy_window();
	// ToolWindow�� ���� ������ ������ �ű��.
	void shiftWindow();
};

