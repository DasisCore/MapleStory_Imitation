#pragma once
#include "CScene.h"

class CObject;

class CScene_Ani_Workshop :
    public CScene
{
private:
    CObject* m_pMainSprite;

    // Workshop���� ����ϴ� �ػ� (�� �� Ŀ���Ƿ� ���� ������)
    int m_iScreenWidth;
    int m_iScreenHeight;

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

