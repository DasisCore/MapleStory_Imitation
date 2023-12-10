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

	float m_fDuration;
	float m_fCurTime;
	
	// �ִϸ��̼� ��� ����
	bool m_bIsPlay;

	// �ִϸ��̼� �ǰ��� ����
	bool m_bIsRewind;
	

public:
	void init();
	void update();
	void render(HDC _dc);

	void FrameRender(HDC _dc);
	void SetIsRewind(bool _b) { m_bIsRewind = _b; }

public:
	void LoadAnimation();
	void SaveAnimation();
	void Save(const wstring& _strFilePath);
	void Load(const wstring& _strFilePath);


// ===========================================================
// UI ������Ʈ�� update�� render
private:
	void UIinit();
	void UI_update();
	void UI_render(HDC _dc);

public:
	void AddDuration(float _f) 
	{
		m_fDuration += _f;
		m_fDuration = max(0.1f, m_fDuration);
	};



// ===========================================================
public:
	void AddFrame(UINT _idx);
	void DeleteFrame();
	void PlayAnimation();
	void IsPlayFlip() { m_bIsPlay = !m_bIsPlay; }

public:
	void showWindow();
	void SetTargetFrm(UINT _i) { m_iTargetFrame = _i; }
	

// ������ ���� �Լ� (���� ���, �̵�, �ı�)
public:
	HWND GetWorkshophWnd() { return m_hWndWorkshop; }
	HWND GetListhWnd() { return m_hWndList; }
	void Destroy_window();
	// ToolWindow�� ���� ������ ������ �ű��.
	void shiftWindow();
};

