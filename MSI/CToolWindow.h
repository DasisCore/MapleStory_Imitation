#pragma once
class CToolWindow
{
	SINGLE(CToolWindow);

private:
	HWND m_hWndTool;
	HDC m_hDCTool;

	HWND m_hWndName;
	HWND m_hWndObjType;
	HWND m_hWndDirection;
	HWND m_hWndState;

	HWND m_hWndPosX;
	HWND m_hWndPosY;
	HWND m_hWndScaleX;
	HWND m_hWndScaleY;

	HWND m_hWndColCheckBox;
	HWND m_hWndColOffsetX;
	HWND m_hWndColOffsetY;
	HWND m_hWndColScaleX;
	HWND m_hWndColScaleY;
	 
	HWND m_hWndAniCheckBox;
	HWND m_hWndAniLoadBtn;
	HWND m_hWndAniListView;

	HWND m_hWndGravityCheckBox;
	HWND m_hWndRigidBodyCheckBox;
	
	HWND m_hWndCreateObjBtn;

	HWND m_hWndObjectCombo;
	HWND m_hWndEdit;

	HDC m_hDCBack;


public:
	void init();
	void showWindow();
	void update();
	void render();

public:
	HWND GetObjectCombo() { return m_hWndObjectCombo; }
	HWND GetEdit() { return m_hWndEdit; }

private:
	HWND CraeteComboBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateEdit(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateCheckBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateListView(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateButton(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);

public:
	HWND GetToolhWnd() { return m_hWndTool; }
	void Destroy_window();
	// ToolWindow를 메인 윈도우 옆으로 옮긴다.
	void shiftWindow();		
};

