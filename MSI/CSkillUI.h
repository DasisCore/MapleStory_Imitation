#pragma once
#include "CUI.h"

class CPlayer;

class CSkillUI :
    public CUI
{
private:
    CPlayer* m_pPlayer;
    
    Vec2 m_vDragStart;

    // UI 배경
    Image* m_pBackImage;

    // 캐릭터 타입
    Image* m_pCharType;

    // 화면에 표시되는 실제 게이지 비율
    float m_fDisplayHP;
    float m_fDisplayMP;
    float m_fDisplayEXP;

    // 축복의 씨앗 쿨타임
    float m_fCoolTimeSkill1;
    float m_fCurCoolTimeSkill1;

    // 요새 쿨타임
    float m_fCoolTimeSkill2;
    float m_fCurCoolTimeSkill2;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;

    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    virtual CSkillUI* Clone() override { return new CSkillUI(*this); }

public:
    // 게이지의 값을 표시하는 함수 123 / 1234 (경험치의 경우 표시 방법이 다름)
    void renderGaugeNumber(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);
    void renderExpGauge(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);
    
    // 스킬 이미지 render
    void renderSkillImage(Graphics& _graphics);
    
    // 스킬 쿨타임 render
    void renderSkillCoolTime(Graphics& _graphics);

    // 스킬 쿨타임 설정
    void SetCoolTimeSkill1() { m_fCurCoolTimeSkill1 = m_fCoolTimeSkill1; }
    void SetCoolTimeSkill2() { m_fCurCoolTimeSkill2 = m_fCoolTimeSkill2; }

    float GetCoolTimeSkill1() { return m_fCurCoolTimeSkill1; }
    float GetCoolTimeSkill2() { return m_fCurCoolTimeSkill2; }

    // 레벨 표시
    void DisplayLevel(Graphics& _graphics);

    // 화면에 표시할 게이지 퍼센트 계산 (한번에 바꾸지 않고 게이지가 부드럽게 조정되도록 계산함)
    void GaugePersentUpdate(const int _iMaxGauge, const int _iGauge, float& _fDisplayGauge);


public:
    CSkillUI(wstring _imageRelativePath);
    ~CSkillUI();

};
