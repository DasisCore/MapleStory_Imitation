#pragma once
#include "CScene.h"

enum class TOOL_TYPE
{
    DEFAULT,
    MARQUEE,
    
    END,
};

struct tFrame
{
    Vec2 vLT;
    Vec2 vSliceSize;
};

class CSprite;
class CMarquee;
class CUI;

class CScene_Ani_Workshop :
    public CScene
{
private:
    TOOL_TYPE m_eState;
    TOOL_TYPE m_ePrevState;

    CSprite* m_pMainSprite;
    
    list<CMarquee*> m_lMarquee;
    
    CMarquee* m_pTargetMQ;

    CUI* m_pMainUI;

    // Workshop에서 사용하는 해상도 (좀 더 커지므로 따로 보관함)
    int m_iScreenWidth;
    int m_iScreenHeight;

    // 드래그 시작 지점
    Vec2 m_vDragStart;
    Vec2 m_vDragEnd;

    // 현재 드래그 중인지 확인
    bool m_bDrag;

    // 프레임 자르기 임시 자료구조
    list<tFrame> m_lFrame;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void LoadTexture();
    void SetMarqueeState() { m_eState = TOOL_TYPE::MARQUEE; };
    void SetDefaultState() { m_eState = TOOL_TYPE::DEFAULT; };

public:
    // 프레임 관련 기능
    void AddFrameInfo(CMarquee* _pMarquee);
    void temp_render(HDC _dc);


private:
    // 이미지를 불러올 때, 확장자를 확인하는 함수.
    bool CheckImageFormat(wstring _wStr);




// marquee 관련 함수들
private:
    void DrawMQRect(HDC _dc);
    void update_MQ();
    void CreateMQObj();
    void AddMarquee(CMarquee* _pMarquee) { m_lMarquee.push_back(_pMarquee); };
    void SearchMarquee();
    void DeleteMarquee();
    // 모든 Marquee의 타겟팅을 해제한다.
    void ResetMarquee();



public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();
};

