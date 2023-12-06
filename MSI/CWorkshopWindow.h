#pragma once

class CWorkshopBtn;

class CWorkshopWindow
{
	SINGLE(CWorkshopWindow);

private:
	HWND m_hWndWorkshop;
	HDC m_WorkshopMainDC;
	HWND m_hWndList;
	
	// workshop���� ui�� ���� �ٷ�� ���� ����Ʈ
	list<CWorkshopBtn*> m_lBtn;

	// ������۸�
	HDC m_memFrameDC;
	HDC m_memControlDC;
	int m_iTargetFrame;

public:
	void init();
	void update();
	void render(HDC _dc);
	void FrameRender(HDC _dc);

// UI ������Ʈ�� update�� render
private:
	void UIinit();
	void UIupdate();
	void UIrender(HDC _dc);


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

