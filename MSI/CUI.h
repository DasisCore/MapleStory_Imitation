#pragma once
#include "CObject.h"


class CUI :
    public CObject
{
private:
    CUI* m_pParentUI;
    vector<CUI*> m_vecChildUI;

    Vec2 m_vFinalPos;

    bool m_bCamAffected;    // UI가 카메라의 영향을 받는지
    bool m_bMouseOn;        // UI 위에 마우스가 있는지
    bool m_bLbtnDown;       // UI에 왼쪽 버튼이 눌린적이 있는지


public:
    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }


public:
    virtual void finalupdate() override;
    virtual void update() override;
    virtual void render(HDC _dc) override;
    
    
    void MouseOnCheck();
    
    virtual CUI* Clone() = 0;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();


private:
    void update_child();
    void render_child(HDC _dc);
    void finalupdate_child();


public:
    CUI(bool _bCamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

    friend class CUIMgr;
};

