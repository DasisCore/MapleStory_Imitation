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
	, m_wStrPath(L"")
	, m_bDrag(false)
	, m_pMainUI(nullptr)
	, m_pTargetMQ(nullptr)
{
	m_lFrame.push_back(tFrame{});
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
	pLoadUI->SetName(L"불러오기");
	pLoadUI->SetScale(Vec2(50.f, 25.f));
	pLoadUI->SetPos(Vec2(0.f, 0.f));
	pLoadUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::LoadTexture);
	pPanelUI->AddChild(pLoadUI);
	
	CBtnUI* pMarqueeOnBtn = new CBtnUI;
	pMarqueeOnBtn->SetName(L"▷");
	pMarqueeOnBtn->SetScale(Vec2(25.f, 25.f));
	pMarqueeOnBtn->SetPos(Vec2(70.f, 0.f));
	pMarqueeOnBtn->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::SetMarqueeState);
	pPanelUI->AddChild(pMarqueeOnBtn);

	CBtnUI* pMarqueeOffBtn = new CBtnUI;
	pMarqueeOffBtn->SetName(L"▶");
	pMarqueeOffBtn->SetScale(Vec2(25.f, 25.f));
	pMarqueeOffBtn->SetPos(Vec2(100.f, 0.f));
	pMarqueeOffBtn->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::SetDefaultState);
	pPanelUI->AddChild(pMarqueeOffBtn);

	//CBtnUI* pSaveBtn = new CBtnUI;
	//pSaveBtn->SetName(L"추출");
	//pSaveBtn->SetScale(Vec2(30.f, 25.f));
	//pSaveBtn->SetPos(Vec2(130.f, 0.f));
	////pSaveBtn->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::AddFrameInfo);
	//pPanelUI->AddChild(pSaveBtn);


	AddObject(pPanelUI, GROUP_TYPE::UI);
	

	// ============================================================
	// 윈도우 창의 해상도 변경을 위한 작업
	// ============================================================
	// 
	// 모니터의 해상도 얻기
	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.7f;
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.7f;

	// workshop 백버퍼를 생성하고 기존 백버퍼와 교체한다.
	CTexture* pTex = CResMgr::GetInst()->CreateTexture(L"Workshop", (UINT)m_iScreenWidth, (UINT)m_iScreenHeight);
	CCore::GetInst()->SetMemTex(pTex);
	
	CCore::GetInst()->ChangeWindowSize(Vec2(m_iScreenWidth, m_iScreenHeight), false);

	// 화면 정 중앙의 값 계산 (씬 변경시, 윈도우를 모니터의 중앙에 위치시키기 위함)
	int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

	RECT rt = { 0, 0, (long)m_iScreenWidth, (long)m_iScreenHeight };
	SetWindowPos(CCore::GetInst()->GetMainHwnd(), nullptr, centerX - (m_iScreenWidth / 2.f), centerY - (m_iScreenHeight / 2.f), rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE);

	// CWorkshop window를 생성
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

	// 컨텐츠 Path를 가져옴
	wstring strTextureFolder = CPathMgr::GetInst()->GetContentPath();
	strTextureFolder += L"Texture";

	ofn.lpstrInitialDir = strTextureFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 읽기가 성공했다면.
	if (GetOpenFileName(&ofn))
	{
		wstring temp(szName);
		if (!CheckImageFormat(temp))
		{
			MessageBox(nullptr, L"bmp, png, jpeg, gif 확장자만 지원합니다.", L"Error Message", MB_OK);
			return;
		}

		wstring strAbsolutePath(szName);
		m_wStrPath = szName;
		
		// 작업은 한번에 하나의 스프라이트만 가능하다.
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

void CScene_Ani_Workshop::AddFrameInfo(CMarquee* _pMarquee)
{
	// 스프라이트의 위치 및 크기 정보
	Vec2 vSpritePos = m_pMainSprite->GetPos();
	Vec2 vSpriteScale = m_pMainSprite->GetScale();
	
	// 스프라이트의 좌상단, 우하단 벡터
	Vec2 vSpriteLT = Vec2(vSpritePos.x - (vSpriteScale.x / 2.f), vSpritePos.y - (vSpriteScale.y / 2.f));
	Vec2 vSpriteRB = Vec2(vSpritePos.x + (vSpriteScale.x / 2.f), vSpritePos.y + (vSpriteScale.y / 2.f));

	// Marquee의 위치와 크기 벡터
	Vec2 vPos = _pMarquee->GetPos();
	Vec2 vScale = _pMarquee->GetScale();

	// Marquee의 좌상단, 우하단 벡터
	Vec2 vMQLT = Vec2(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f));
	Vec2 vMQRB = Vec2(vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));

	// 스프라이트를 기준으로 Marquee의 벡터가 어떻게 위치하는지
	Vec2 vLT = vMQLT - vSpriteLT;
	Vec2 vRB = vMQRB - vSpriteLT;

	// 최소 최대값 보정 -> 좌상단, 우하단이 스프라이트의 밖으로 나갔을 때를 보정
	if (vLT.x < 0) vLT.x = 0.f;
	if (vLT.y < 0) vLT.y = 0.f	;
	if (vRB.x >= vSpriteScale.x) vRB.x = vSpriteScale.x;
	if (vRB.y >= vSpriteScale.y) vRB.y = vSpriteScale.y;

	tFrame tf = {};
	tf.vLT = vLT;
	tf.vSliceSize = vRB - vLT;
		
	m_lFrame.push_back(tf);
	CWorkshopWindow::GetInst()->SetTargetFrm(m_lFrame.size() - 1);
}

void CScene_Ani_Workshop::AdjustFrameInfo(CMarquee* _pMarquee, tFrame& _tFrame)
{
	// 스프라이트의 위치 및 크기 정보
	Vec2 vSpritePos = m_pMainSprite->GetPos();
	Vec2 vSpriteScale = m_pMainSprite->GetScale();

	// 스프라이트의 좌상단, 우하단 벡터
	Vec2 vSpriteLT = Vec2(vSpritePos.x - (vSpriteScale.x / 2.f), vSpritePos.y - (vSpriteScale.y / 2.f));
	Vec2 vSpriteRB = Vec2(vSpritePos.x + (vSpriteScale.x / 2.f), vSpritePos.y + (vSpriteScale.y / 2.f));

	// Marquee의 위치와 크기 벡터
	Vec2 vPos = _pMarquee->GetPos();
	Vec2 vScale = _pMarquee->GetScale();

	// Marquee의 좌상단, 우하단 벡터
	Vec2 vMQLT = Vec2(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f));
	Vec2 vMQRB = Vec2(vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));

	// 스프라이트를 기준으로 Marquee의 벡터가 어떻게 위치하는지
	Vec2 vLT = vMQLT - vSpriteLT;
	Vec2 vRB = vMQRB - vSpriteLT;

	// 최소 최대값 보정 -> 좌상단, 우하단이 스프라이트의 밖으로 나갔을 때를 보정
	if (vLT.x < 0) vLT.x = 0.f;
	if (vLT.y < 0) vLT.y = 0.f;
	if (vRB.x >= vSpriteScale.x) vRB.x = vSpriteScale.x;
	if (vRB.y >= vSpriteScale.y) vRB.y = vSpriteScale.y;

	_tFrame.vLT = vLT;
	_tFrame.vSliceSize = vRB - vLT;
}

tFrame CScene_Ani_Workshop::GetFrameInfo(UINT _i)
{
	list<tFrame>::iterator iter = ++m_lFrame.begin();
	int idx = 1;

	for (; iter != m_lFrame.end(); iter++)
	{
		if (_i == idx) break;
		idx++;
	}

	if (iter == m_lFrame.end()) return (*--(m_lFrame.end()));

	return *iter;
}

void CScene_Ani_Workshop::temp_render(HDC _dc)
{
	Graphics graphics(_dc);

	Vec2 vMousePos = MOUSE_POS;

	Font font(L"Arial", 8);
	SolidBrush brush(Gdiplus::Color(255, 0, 0, 0)); // 검은색 브러시

	wstring mousePos = L"『" + std::to_wstring(vMousePos.x) + L" / " + std::to_wstring(vMousePos.y) + L"』";
	graphics.DrawString(L"마우스 위치", -1, &font, PointF(1512.f, 85.f), &brush);
	graphics.DrawString(mousePos.c_str(), -1, &font, PointF(1510.f, 100.f), &brush);

	int idx = 1;

	for (auto iter = m_lFrame.begin(); iter != m_lFrame.end(); iter++)
	{
		mousePos = L"Frame " + std::to_wstring(idx) + L" 정보";
		graphics.DrawString(mousePos.c_str(), -1, &font, PointF(1512.f, 130.f + 80.f * (idx - 1)), &brush);

		Vec2 vLT = iter->vLT;
		Vec2 vSliceSize = iter->vSliceSize;

		mousePos = L"『 LT : " + std::to_wstring(vLT.x) + L" / " + std::to_wstring(vLT.y) + L"』";
		graphics.DrawString(mousePos.c_str(), -1, &font, PointF(1510.f, 145.f + 80.f * (idx - 1)), &brush);

		mousePos = L"『 vSliceSize : " + std::to_wstring(vSliceSize.x) + L" / " + std::to_wstring(vSliceSize.y) + L"』";
		graphics.DrawString(mousePos.c_str(), -1, &font, PointF(1510.f, 165.f + 80.f * (idx - 1)), &brush);
		
		idx++;
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

void CScene_Ani_Workshop::reset()
{
	DeleteObject(m_pMainSprite);

	for (auto iter = m_lMarquee.begin(); iter != m_lMarquee.end(); iter++)
	{
		DeleteObject(*iter);
	}

	m_bResetFlag = true;
}



void CScene_Ani_Workshop::update()
{
	CScene::update();
	
	CWorkshopWindow::GetInst()->update();

	if (m_pMainUI->IsLbtnDown() && m_eState == TOOL_TYPE::MARQUEE)
	{
		m_eState = TOOL_TYPE::DEFAULT;
		m_bDrag = false;
	}
	
	if (KEY_TAP(KEY::F2))
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

	finalupdateMarquee();

	if (m_bResetFlag)
	{
		m_lMarquee.clear();
		m_lMarquee.resize(0);

		m_lFrame.clear();
		m_lFrame.resize(0);
		m_lFrame.push_back(tFrame{});

		m_pMainSprite = nullptr;
		
		m_bResetFlag = false;
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

	temp_render(_dc);
}

/// =======================================================================
/// Marquee 관련 기능 
/// =======================================================================

void CScene_Ani_Workshop::update_MQ()
{
	if (KEY_TAP(KEY::LBTN))
	{
		ResetMarquee();
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
	Pen pen(Color(0, 255, 0), 2);
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

	// 이전의 marquee들의 타겟팅을 제거해준다.
	ResetMarquee();

	CMarquee* pMarquee = new CMarquee;
	pMarquee->SetName(L"Marquee" + std::to_wstring(m_lMarquee.size() + 1));
	pMarquee->SetScale(Vec2(rectWidth, rectHeight));
	pMarquee->SetPos(vCenter);
	pMarquee->SetTarget(true);
	AddMarquee(pMarquee);

	CWorkshopWindow::GetInst()->AddFrame(UINT(m_lMarquee.size()));

	AddFrameInfo(pMarquee);
	AddObject(pMarquee, GROUP_TYPE::MARQUEE);
}

void CScene_Ani_Workshop::SearchMarquee()
{
	if (m_eState == TOOL_TYPE::DEFAULT && KEY_TAP(KEY::LBTN))
	{
		bool flag = 0;

		list<CMarquee*>::iterator iter = m_lMarquee.begin();

		int idx = 0;
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
				CWorkshopWindow::GetInst()->SetTargetFrm(idx + 1);
			}
			idx++;
		}
	}
}

void CScene_Ani_Workshop::DeleteMarquee()
{
	// 현재 타겟팅 된 Marquee를 지워야 한다.
	int idx = 1;
	int target = 0;
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
			target = idx;
		}
	}

	list<tFrame>::iterator eraseIter = m_lFrame.begin()++;
	for (int i = 0; i < target; i++) eraseIter++;
	m_lFrame.erase(eraseIter);

	CWorkshopWindow::GetInst()->SetTargetFrm(m_lFrame.size() - 2);
	CWorkshopWindow::GetInst()->DeleteFrame();
}

void CScene_Ani_Workshop::ResetMarquee()
{
	list<CMarquee*>::iterator iter = m_lMarquee.begin();

	for (; iter != m_lMarquee.end(); iter++)
	{
		(*iter)->SetTarget(false);
	}
}



void CScene_Ani_Workshop::SetTargetMarquee(UINT _i)
{
	ResetMarquee();

	int idx = 0;
	list<CMarquee*>::iterator iter = m_lMarquee.begin();

	for (; iter != m_lMarquee.end(); iter++)
	{
		(*iter)->SetTarget(false);
		if(idx == _i) (*iter)->SetTarget(true);
		idx++;
	}
}

CMarquee* CScene_Ani_Workshop::GetMarquee(int _i)
{
	int idx = 0;
	list<CMarquee*>::iterator iter = m_lMarquee.begin();
	for (; iter != m_lMarquee.end(); iter++)
	{
		if (idx == _i) return *iter;
		idx++;
	}
	return nullptr;
}

void CScene_Ani_Workshop::finalupdateMarquee()
{
	list<CMarquee*>::iterator MarqueeIter = m_lMarquee.begin();
	list<tFrame>::iterator FrameIter = ++m_lFrame.begin();

	for (; MarqueeIter != m_lMarquee.end(); MarqueeIter++)
	{
		AdjustFrameInfo(*MarqueeIter, *FrameIter);
		FrameIter++;
	}
}


