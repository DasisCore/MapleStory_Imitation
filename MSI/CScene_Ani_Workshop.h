#pragma once
#include "CScene.h"

enum class TOOL_TYPE
{
    DEFAULT,
    MARQUEE,
    
    END,
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
    wstring m_wStrPath;
    
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

    bool m_bResetFlag;

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
    void AdjustFrameInfo(CMarquee* _pMarquee, tFrame& _tFrame);
    const list<tFrame> GetFrameList() { return m_lFrame; }
    list<CMarquee*> GetMarqueeList() { return m_lMarquee; }
    tFrame GetFrameInfo(UINT _i);
    CSprite* GetSprite() { return m_pMainSprite; }
    wstring GetAbsolutePath() { return m_wStrPath; }
    void SetAbsolutePath(wstring _strPath) { m_wStrPath = _strPath; }
    void temp_render(HDC _dc);
    void reset();


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
    void finalupdateMarquee();

public:
    void SetTargetMarquee(UINT _i);
    // _i번째 Marquee를 준다. (0번 인덱스 포함)
    CMarquee* GetMarquee(int _i);
    bool isMarquee() { return m_eState == TOOL_TYPE::MARQUEE ? true : false; }


public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();

    friend class CWorkshopWindow;
};

