#pragma once
#include "CScene.h"

class CObject;

class CScene_Ani_Workshop :
    public CScene
{
private:
    CObject* m_pMainSprite;

    // Workshop에서 사용하는 해상도 (좀 더 커지므로 따로 보관함)
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
    // 이미지를 불러올 때, 확장자를 확인하는 함수.
    bool CheckImageFormat(wstring _wStr);


public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();
};

