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

	float m_fDuration;
	float m_fCurTime;
	
	// 애니메이션 재생 여부
	bool m_bIsPlay;

	// 애니메이션 되감기 여부
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
// UI 오브젝트용 update와 render
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
	

// 윈도우 관련 함수 (인자 얻기, 이동, 파괴)
public:
	HWND GetWorkshophWnd() { return m_hWndWorkshop; }
	HWND GetListhWnd() { return m_hWndList; }
	void Destroy_window();
	// ToolWindow를 메인 윈도우 옆으로 옮긴다.
	void shiftWindow();
};

