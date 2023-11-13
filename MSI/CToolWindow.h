#pragma once
class CToolWindow
{
	SINGLE(CToolWindow);

	HWND m_hWndTool;

public:
	void init();
	void showWindow();

public:
	HWND GetToolhWnd() { return m_hWndTool; }
	void Destroy_window();
	void shiftWindow();		// ToolWindow를 메인 윈도우 옆으로 옮긴다.
};

