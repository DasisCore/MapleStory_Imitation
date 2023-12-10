#pragma once

//#include "CScene.h"

typedef void(*BTN_FUNCC) (DWORD_PTR, DWORD_PTR, DWORD_PTR);

class CWorkshopBtn
{
private:
	wstring m_strName;
	Vec2 m_vPos;
	Vec2 m_vScale;

	bool m_bMouseOn;
	bool m_bLbtnDown;

	BTN_FUNCC m_pFunc;
	DWORD_PTR m_param1;
	DWORD_PTR m_param2;
	DWORD_PTR m_param3;

	// юс╫ц
	Color color;
	

public:
	void SetName(wstring _strName) { m_strName = _strName; }
	wstring GetName() { return m_strName; }
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }
	
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	bool GetMouseOn() { return m_bMouseOn; }

	virtual void render(HDC _dc);
	virtual void update();

	void SetClickCallBack(BTN_FUNCC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2, DWORD_PTR _param3)
	{
		m_pFunc = _pFunc;
		m_param1 = _param1;
		m_param2 = _param2;
		m_param3 = _param3;
	}


public:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	void MouseLbtnClicked();

public:
	CWorkshopBtn();
	~CWorkshopBtn();
};

