#pragma once

class CWorkshopBtn;

class CWorkshopWindow
{
	SINGLE(CWorkshopWindow);

private:
	HWND m_hWndWorkshop;
	HDC m_WorkshopMainDC;
	HWND m_hWndList;
	
	// workshop에서 ui를 따로 다루기 위한 리스트
	list<CWorkshopBtn*> m_lBtn;

	// 더블버퍼링
	HDC m_memFrameDC;
	HDC m_memControlDC;
	int m_iTargetFrame;

public:
	void init();
	void update();
	void render(HDC _dc);
	void FrameRender(HDC _dc);

// UI 오브젝트용 update와 render
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
	// ToolWindow를 메인 윈도우 옆으로 옮긴다.
	void shiftWindow();
};

