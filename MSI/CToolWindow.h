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
	// ToolWindow�� ���� ������ ������ �ű��.
	void shiftWindow();		
};

