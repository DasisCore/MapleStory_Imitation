#pragma once
#include "CUI.h"

//#include "CScene.h"
//#include "CObject.h"

class CScene;
class CObject;

typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

typedef void(CScene::* SCENE_MEMFUNC)(void);
typedef void(CObject::* OBJECT_MEMFUNC)(void);

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC m_pFunc;
    DWORD_PTR m_param1;
    DWORD_PTR m_param2;

    SCENE_MEMFUNC m_pSceneFunc; // 호출할 함수 // 함수 포인터
    CScene* m_pSceneInst;       // 호출할 객체


public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

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


    virtual CBtnUI* Clone() override { return new CBtnUI(*this); }

public:
    // 임시로 넣어놓음. 자신의 이름 출력
    virtual void render(HDC _dc) override;
    void Dedicate_workshop_render(HDC _dc);
    
public:
    CBtnUI();
    ~CBtnUI();
};

