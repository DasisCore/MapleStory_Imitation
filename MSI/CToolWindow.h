#pragma once
class CToolWindow
{
	SINGLE(CToolWindow);

	HWND m_hWndTool;

	HWND m_hWndObjectCombo;
	HWND m_hWndEdit;


public:
	void init();
	void showWindow();

public:
	HWND GetObjectCombo() { return m_hWndObjectCombo; }
	HWND GetEdit() { return m_hWndEdit; }

private:
	HWND CraeteComboBox(wstring _strName, Vec2 _vPos, Vec2 _vScale);
	HWND CreateEdit(wstring _strName, Vec2 _vPos, Vec2 _vScale);



public:
	HWND GetToolhWnd() { return m_hWndTool; }
	void Destroy_window();
	// ToolWindow를 메인 윈도우 옆으로 옮긴다.
	void shiftWindow();		
};

