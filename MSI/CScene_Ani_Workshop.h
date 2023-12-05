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

    // Workshop���� ����ϴ� �ػ� (�� �� Ŀ���Ƿ� ���� ������)
    int m_iScreenWidth;
    int m_iScreenHeight;

    // �巡�� ���� ����
    Vec2 m_vDragStart;
    Vec2 m_vDragEnd;

    // ���� �巡�� ������ Ȯ��
    bool m_bDrag;

    // ������ �ڸ��� �ӽ� �ڷᱸ��
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
    // ������ ���� ���
    void AddFrameInfo(CMarquee* _pMarquee);
    void temp_render(HDC _dc);


private:
    // �̹����� �ҷ��� ��, Ȯ���ڸ� Ȯ���ϴ� �Լ�.
    bool CheckImageFormat(wstring _wStr);




// marquee ���� �Լ���
private:
    void DrawMQRect(HDC _dc);
    void update_MQ();
    void CreateMQObj();
    void AddMarquee(CMarquee* _pMarquee) { m_lMarquee.push_back(_pMarquee); };
    void SearchMarquee();
    void DeleteMarquee();
    // ��� Marquee�� Ÿ������ �����Ѵ�.
    void ResetMarquee();



public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();
};

