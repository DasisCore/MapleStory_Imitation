#pragma once
class CToolWindow
{
	SINGLE(CToolWindow);

private:
	HWND m_hWndTool;
	HDC m_hDCTool;

	HWND m_hWndName;
	HWND m_hWndReset;
	HWND m_hWndObjType;
	HWND m_hWndDirection;
	HWND m_hWndGrade;

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
	
	HWND m_hWndSaveSceneBtn;
	HWND m_hWndCreateObjBtn;

	HDC m_hDCBack;


public:
	vector<wstring> m_vecAniPath;

public:
	void init();
	void showWindow();
	void update();
	void render();

public:
	HWND GethWndName() { return m_hWndName; }
	HWND GethWndObjType() { return m_hWndObjType; }
	HWND GethWndDirection() { return m_hWndDirection; }
	HWND GethWndGrade() { return m_hWndGrade; }
	HWND GethWndPosX() { return m_hWndPosX; }
	HWND GethWndPosY() { return m_hWndPosY; }
	HWND GethWndScaleX() { return m_hWndScaleX; }
	HWND GethWndScaleY() { return m_hWndScaleY; }
	HWND GethWndColCheckBox() { return m_hWndColCheckBox; }
	HWND GethWndColOffsetX() { return m_hWndColOffsetX; }
	HWND GethWndColOffsetY() { return m_hWndColOffsetY; }
	HWND GethWndColScaleX() { return m_hWndColScaleX; }
	HWND GethWndColScaleY() { return m_hWndColScaleY; }
	HWND GethWndAniCheckBox() { return m_hWndAniCheckBox; }
	HWND GethWndAniLoadBtn() { return m_hWndAniLoadBtn; }
	HWND GethWndAniListView() { return m_hWndAniListView; }
	HWND GethWndGravityCheckBox() { return m_hWndGravityCheckBox; }
	HWND GethWndRigidBodyCheckBox() { return m_hWndRigidBodyCheckBox; }
	

private:
	HWND CraeteComboBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateEdit(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateCheckBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateListView(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);
	HWND CreateButton(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc);


public:
	void LoadAnimation();
	
	// 멤버 변수의 정보를 토대로 오브젝트를 만들어 Scene에 추가.
	void CreateObject();
	void ResetData();

public:
	HWND GetToolhWnd() { return m_hWndTool; }
	void Destroy_window();
	// ToolWindow를 메인 윈도우 옆으로 옮긴다.
	void shiftWindow();		
};

