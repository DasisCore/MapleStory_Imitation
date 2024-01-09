#pragma once
#include "CUI.h"

class CPlayer;

class CSkillUI :
    public CUI
{
private:
    CPlayer* m_pPlayer;
    
    Vec2 m_vDragStart;

    // UI ���
    Image* m_pBackImage;

    // ĳ���� Ÿ��
    Image* m_pCharType;

    // ȭ�鿡 ǥ�õǴ� ���� ������ ����
    float m_fDisplayHP;
    float m_fDisplayMP;
    float m_fDisplayEXP;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;


    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    virtual CSkillUI* Clone() override { return new CSkillUI(*this); }

public:
    // �������� ���� ǥ���ϴ� �Լ� 123 / 1234 (����ġ�� ��� ǥ�� ����� �ٸ�)
    void DrawGaugeNumber(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);
    void DrawExpGauge(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight);

    // ���� ǥ��
    void DisplayLevel(Graphics& _graphics);


    // ȭ�鿡 ǥ���� ������ �ۼ�Ʈ ��� (�ѹ��� �ٲ��� �ʰ� �������� �ε巴�� �����ǵ��� �����)
    void GaugePersentUpdate(const int _iMaxGauge, const int _iGauge, float& _fDisplayGauge);


public:
    CSkillUI(wstring _imageRelativePath);
    ~CSkillUI();

};
