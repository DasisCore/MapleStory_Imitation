#pragma once

#include "CScene.h"

class CScene_Start :
    public CScene
{
private:
    Vec2 m_vForcePos;
    float m_fForceRadius;
    float m_fCurRadius;
    float m_fForce;
    bool m_bUseForce;

    // �ӽ� gdi+ ��ü
    Image* m_pTempImage;
    // �ӽ� ���� ����
    int m_itempNumber;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void CreateForce();

public:
    CScene_Start();
    ~CScene_Start();
};

