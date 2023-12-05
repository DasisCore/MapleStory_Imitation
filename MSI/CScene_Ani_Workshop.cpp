#include "global.h"
#include "CScene_Ani_Workshop.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CCore.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

#include "CSprite.h"
#include "CMarquee.h"

#include "CWorkshopWindow.h"

CScene_Ani_Workshop::CScene_Ani_Workshop()
	: m_eState(TOOL_TYPE::DEFAULT)
	, m_ePrevState(TOOL_TYPE::DEFAULT)
	, m_iScreenWidth(0)
	, m_iScreenHeight(0)
	, m_pMainSprite(nullptr)
	, m_bDrag(false)
	, m_pMainUI(nullptr)
	, m_pTargetMQ(nullptr)
{
}

CScene_Ani_Workshop::~CScene_Ani_Workshop()
{
}

void CScene_Ani_Workshop::Enter()
{
	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"Panel UI");
	pPanelUI->SetScale(Vec2(300.f, 100.f));
	pPanelUI->SetPos(Vec2(300.f, 300.f));
	m_pMainUI = pPanelUI;

	CBtnUI* pLoadUI = new CBtnUI;
	pLoadUI->SetName(L"�ҷ�����");
	pLoadUI->SetScale(Vec2(50.f, 25.f));
	pLoadUI->SetPos(Vec2(0.f, 0.f));
	pLoadUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::LoadTexture);
	pPanelUI->AddChild(pLoadUI);
	
	CBtnUI* pMarqueeOnBtn = new CBtnUI;
	pMarqueeOnBtn->SetName(L"��");
	pMarqueeOnBtn->SetScale(Vec2(25.f, 25.f));
	pMarqueeOnBtn->SetPos(Vec2(70.f, 0.f));
	pMarqueeOnBtn->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::SetMarqueeState);
	pPanelUI->AddChild(pMarqueeOnBtn);

	CBtnUI* pMarqueeOffBtn = new CBtnUI;
	pMarqueeOffBtn->SetName(L"��");
	pMarqueeOffBtn->SetScale(Vec2(25.f, 25.f));
	pMarqueeOffBtn->SetPos(Vec2(100.f, 0.f));
	pMarqueeOffBtn->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::SetDefaultState);
	pPanelUI->AddChild(pMarqueeOffBtn);

	AddObject(pPanelUI, GROUP_TYPE::UI);
	

	// ============================================================
	// ������ â�� �ػ� ������ ���� �۾�
	// ============================================================
	// 
	// ������� �ػ� ���
	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.7f;
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.7f;

	// workshop ����۸� �����ϰ� ���� ����ۿ� ��ü�Ѵ�.
	CTexture* pTex = CResMgr::GetInst()->CreateTexture(L"Workshop", (UINT)m_iScreenWidth, (UINT)m_iScreenHeight);
	CCore::GetInst()->SetMemTex(pTex);
	
	CCore::GetInst()->ChangeWindowSize(Vec2(m_iScreenWidth, m_iScreenHeight), false);

	// ȭ�� �� �߾��� �� ��� (�� �����, �����츦 ������� �߾ӿ� ��ġ��Ű�� ����)
	int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

	RECT rt = { 0, 0, (long)m_iScreenWidth, (long)m_iScreenHeight };
	SetWindowPos(CCore::GetInst()->GetMainHwnd(), nullptr, centerX - (m_iScreenWidth / 2.f), centerY - (m_iScreenHeight / 2.f), rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE);

	// CWorkshop window�� ����
	CWorkshopWindow::GetInst()->init();
	CWorkshopWindow::GetInst()->showWindow();
}

void CScene_Ani_Workshop::Exit()
{
	CTexture* pMemTex = CResMgr::GetInst()->FindTexture(L"BackBuffer");
	assert(pMemTex);
	CCore::GetInst()->SetMemTex(pMemTex);

	CWorkshopWindow::GetInst()->Destroy_window();
}

void CScene_Ani_Workshop::LoadTexture()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	//ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png\0";
	ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png\0bmp\0*.bmp\0jpeg\0*.jpeg\0gif\0*.gif\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	// ������ Path�� ������
	wstring strTextureFolder = CPathMgr::GetInst()->GetContentPath();
	strTextureFolder += L"Texture";

	ofn.lpstrInitialDir = strTextureFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ���� �бⰡ �����ߴٸ�.
	if (GetOpenFileName(&ofn))
	{
		wstring temp(szName);
		if (!CheckImageFormat(temp))
		{
			MessageBox(nullptr, L"bmp, png, jpeg, gif Ȯ���ڸ� �����մϴ�.", L"Error Message", MB_OK);
			return;
		}

		wstring strAbsolutePath(szName);
		
		// �۾��� �ѹ��� �ϳ��� ��������Ʈ�� �����ϴ�.
		if (m_pMainSprite != nullptr)
		{
			DeleteObject(m_pMainSprite);
		}

		CSprite* pSprite = new CSprite(strAbsolutePath);
		pSprite->SetPos(Vec2(100.f, 100.f));
		AddObject(pSprite, GROUP_TYPE::SPRITE);

		m_pMainSprite = pSprite;
	}
}

bool CScene_Ani_Workshop::CheckImageFormat(wstring _wStr)
{
	wstring e = L"";

	bool flag = 0;
	int iLen = _wStr.length();
	for (int i = 0; i < iLen; i++)
	{
		if (_wStr[i] == L'.') flag = 1;
		if (flag == 1) e += _wStr[i];
	}

	if (e == L".png" || e == L".bmp" || e == L".gif" || e == L".jpeg") return true;
	return false;
}



void CScene_Ani_Workshop::update()
{
	CWorkshopWindow::GetInst()->update();

	CScene::update();

	if (m_pMainUI->IsLbtnDown() && m_eState == TOOL_TYPE::MARQUEE)
	{
		m_eState = TOOL_TYPE::DEFAULT;
		m_bDrag = false;
	}
	
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	if (m_pMainSprite != nullptr)
	{
		if (KEY_TAP(KEY::SPACE))
		{
			m_ePrevState = m_eState;
			m_eState = TOOL_TYPE::DEFAULT;
		}

		if (KEY_HOLD(KEY::SPACE))
		{
			m_pMainSprite->SetTarget(true);
		}

		if (KEY_AWAY(KEY::SPACE))
		{
			m_pMainSprite->SetTarget(false);
			m_eState = m_ePrevState;
		}
	}

	if (m_eState == TOOL_TYPE::DEFAULT)
	{
		SearchMarquee();

		if (m_pTargetMQ && (KEY_TAP(KEY::BACKSPACE) || KEY_TAP(KEY::DEL)))
		{
			DeleteMarquee();
		}
	}

	if (m_eState == TOOL_TYPE::MARQUEE)
	{
		update_MQ();
	}
}

void CScene_Ani_Workshop::render(HDC _dc)
{
	CScene::render(_dc);
	CWorkshopWindow::GetInst()->render(_dc);

	Vec2 vPos = Vec2(250.f, 13.f);

	Graphics graphics(_dc);
	Font font(L"Arial", 8);
	SolidBrush brush(Color(255, 0, 0, 0));

	switch (m_eState)
	{
	case TOOL_TYPE::DEFAULT:
		graphics.DrawString(L"Default", -1, &font, PointF(vPos.x, vPos.y), &brush);
		break;
	case TOOL_TYPE::MARQUEE:
		graphics.DrawString(L"Marquee", -1, &font, PointF(vPos.x, vPos.y), &brush);
		if (m_bDrag) DrawMQRect(_dc);
		break;
	case TOOL_TYPE::END:
		break;
	}
}

/// =======================================================================
/// Marquee ���� ��� 
/// =======================================================================

void CScene_Ani_Workshop::update_MQ()
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_vDragStart = MOUSE_POS;
		m_bDrag = true;
	}

	if (KEY_AWAY(KEY::LBTN))
	{
		m_vDragEnd = MOUSE_POS;
		m_bDrag = false;
		CreateMQObj();
	}
}

void CScene_Ani_Workshop::DrawMQRect(HDC _dc)
{
	Vec2 vCurPos = MOUSE_POS;

	Graphics graphics(_dc);
	Pen pen(Color(0, 255, 0), 3);
	pen.SetDashStyle(DashStyleDash);

	float rectWidth = abs(vCurPos.x - m_vDragStart.x);
	float rectHeight = abs(vCurPos.y - m_vDragStart.y);

	graphics.DrawRectangle(&pen, min(vCurPos.x, m_vDragStart.x), min(vCurPos.y, m_vDragStart.y), rectWidth, rectHeight);
}

void CScene_Ani_Workshop::CreateMQObj()
{
	Vec2 vCenter = (m_vDragStart + m_vDragEnd) / 2.f;
	vCenter = CCamera::GetInst()->GetRealPos(vCenter);

	float rectWidth = abs(m_vDragEnd.x - m_vDragStart.x);
	float rectHeight = abs(m_vDragEnd.y - m_vDragStart.y);

	if (rectWidth < 10.f || rectHeight < 10.f) return;

	// ������ marquee���� Ÿ������ �������ش�.
	
	list<CMarquee*>::iterator iter = m_lMarquee.begin();
	for (; iter != m_lMarquee.end(); iter++)
	{
		CMarquee* pMarquee = *iter;
		pMarquee->SetTarget(false);
	}

	CMarquee* pMarquee = new CMarquee;
	pMarquee->SetName(L"Marquee" + std::to_wstring(m_lMarquee.size() + 1));
	pMarquee->SetScale(Vec2(rectWidth, rectHeight));
	pMarquee->SetPos(vCenter);
	pMarquee->SetTarget(true);
	AddMarquee(pMarquee);

	CWorkshopWindow::GetInst()->AddFrame(UINT(m_lMarquee.size()));

	AddObject(pMarquee, GROUP_TYPE::MARQUEE);
}

void CScene_Ani_Workshop::SearchMarquee()
{
	if (m_eState == TOOL_TYPE::DEFAULT && KEY_TAP(KEY::LBTN))
	{
		bool flag = 0;

		list<CMarquee*>::iterator iter = m_lMarquee.begin();

		for(; iter != m_lMarquee.end(); iter++)
		{
			CMarquee* pMarquee = *iter;

			if (flag)
			{
				pMarquee->SetTarget(false);
				continue;
			}

			pMarquee->SetTarget(false);
			m_pTargetMQ = nullptr;
			
			if (CKeyMgr::GetInst()->IsMouseInObj(pMarquee))
			{
				pMarquee->SetTarget(true);
				m_pTargetMQ = pMarquee;
				flag = 1;
			}
		}
	}
}

void CScene_Ani_Workshop::DeleteMarquee()
{
	// ���� Ÿ���� �� Marquee�� ������ �Ѵ�.
	int idx = 1;
	list<CMarquee*>::iterator iter = m_lMarquee.begin();
	for (; iter != m_lMarquee.end();)
	{
		if (*iter != m_pTargetMQ)
		{
			(*iter)->SetName(L"Marquee" + std::to_wstring(idx));
			idx++;
			iter++;
		}
		else
		{
			DeleteObject(*iter);
			iter = m_lMarquee.erase(iter);
		}
	}
	CWorkshopWindow::GetInst()->DeleteFrame();
}


