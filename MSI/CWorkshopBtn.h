#pragma once

#include "CScene.h"

typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

typedef void(CScene::* SCENE_MEMFUNC)(void);
typedef void(CObject::* OBJECT_MEMFUNC)(void);

class CWorkshopBtn
{
private:
	wstring m_strName;
	Vec2 m_vPos;
	Vec2 m_vScale;

	bool m_bMouseOn;
	bool m_bLbtnDown;

	BTN_FUNC m_pFunc;
	DWORD_PTR m_param1;
	DWORD_PTR m_param2;

	SCENE_MEMFUNC m_pSceneFunc; // 호출할 함수 // 함수 포인터
	CScene* m_pSceneInst;       // 호출할 객체

	// 임시
	Color color;
	

public:
	void SetName(wstring _strName) { m_strName = _strName; }
	wstring GetName() { return m_strName; }
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }
	
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	void render(HDC _dc);
	void update();

	void SetClickCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
	{
		m_pFunc = _pFunc;
		m_param1 = _param1;
		m_param2 = _param2;
	}

	void SetClickCallBack(CScene* _pScene, SCENE_MEMFUNC _pFunc)
	{
		m_pSceneInst = _pScene;
		m_pSceneFunc = _pFunc;
	}


public:
	void MouseOn();
	void MouseLbtnDown();
	void MouseLbtnUp();
	void MouseLbtnClicked();


public:
	CWorkshopBtn();
	~CWorkshopBtn();
};

