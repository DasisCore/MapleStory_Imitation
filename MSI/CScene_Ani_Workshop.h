#pragma once
#include "CScene.h"

class CScene_Ani_Workshop :
    public CScene
{
private:
    Image* m_pImage;


    // Workshop���� ����ϴ� �ػ� (�� �� Ŀ���Ƿ� ���� ������)
    int m_iScreenWidth;
    int m_iScreenHeight;


    // �ҷ��� �̹����� ����
    float m_fRatio;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void LoadTexture();

private:
    // �̹����� �ҷ��� ��, Ȯ���ڸ� Ȯ���ϴ� �Լ�.
    bool CheckImageFormat(wstring _wStr);


public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();
};

