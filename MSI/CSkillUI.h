#pragma once
#include "CUI.h"

class CSkillUI :
    public CUI
{
private:
    Vec2 m_vDragStart;

    // UI 배경
    Image* m_pBackImage;

    // 캐릭터 타입
    Image* m_pCharType;


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;


    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    virtual CSkillUI* Clone() override { return new CSkillUI(*this); }

public:
    void DrawGaugeNumber(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);
    void DrawExpGauge(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);

public:
    CSkillUI(wstring _imageRelativePath);
    ~CSkillUI();

};
