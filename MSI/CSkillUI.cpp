#include "global.h"
#include "CKeyMgr.h"
#include "CSkillUI.h"

#include "CComponent.h"
#include "CAnimator.h"
#include "CPathMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"




CSkillUI::CSkillUI(wstring _imageRelativePath)
	: CUI(false)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	wstring backImagePath = contentPath + _imageRelativePath;
	wstring CharTypePath = contentPath + L"Texture\\SkillBar\\charType.png";
	m_pBackImage = Image::FromFile(backImagePath.c_str());
	m_pCharType = Image::FromFile(CharTypePath.c_str());

	float fWidth = m_pBackImage->GetWidth();
	float fHeight = m_pBackImage->GetHeight();

	SetScale(Vec2(fWidth, fHeight));
}

CSkillUI::~CSkillUI()
{
}

void CSkillUI::update()
{
}

void CSkillUI::render(HDC _dc)
{
	CUI::render(_dc);

	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CPlayer* pPlayer = (CPlayer*)pCurScene->GetPlayer();

	const tPlayerInfo tInfo = pPlayer->GetPlayerInfo();

	Graphics graphics(_dc);

	if (m_pBackImage != nullptr)
	{
		Vec2 vPos = GetPos();
		// background 이미지 출력
		graphics.DrawImage(m_pBackImage, vPos.x, vPos.y);

		// 캐릭터 타입 출력
		graphics.DrawImage(m_pCharType, vPos.x + 20.f, vPos.y + 35.f);

		// 캐릭터 레벨 출력
		wstring lvNumberPath = contentPath + L"Texture\\Skillbar\\lvNumber\\";
		wstring strLv = std::to_wstring(tInfo.iLevel);
		for (int i = 0; i < strLv.length(); i++)
		{
			wstring strNo = lvNumberPath + strLv[i] + L".png";
			Image* numberImage = Image::FromFile(strNo.c_str());
			graphics.DrawImage(numberImage, vPos.x + (i * 11.f) + 43.f, vPos.y + 10.f);
			delete numberImage;
		}

		wstring gaugeNumberPath = contentPath + L"Texture\\Skillbar\\gaugeNumber\\";

		float gaugePosX = vPos.x + 141.f;

		// 캐릭터 hp 출력
		SolidBrush hpBrush(Color(255, 34, 102));
		graphics.FillRectangle(&hpBrush, Rect(gaugePosX, vPos.y + 14.f, 189, 11.f));

		// 캐릭터 mp 출력
		SolidBrush mpBrush(Color(0, 187, 238));
		graphics.FillRectangle(&mpBrush, Rect(gaugePosX, vPos.y + 32.f, 189, 11.f));

		// 캐릭터 exp 출력
		SolidBrush expBrush(Color(187, 221, 0));
		graphics.FillRectangle(&expBrush, Rect(gaugePosX, vPos.y + 50.f, 189, 11.f));

		// 게이지 커버 
		wstring gaugeCoverPath = contentPath + L"Texture\\Skillbar\\cover.png";
		Image* coverImage = Image::FromFile(gaugeCoverPath.c_str());
		graphics.DrawImage(coverImage, vPos.x + 141.f, vPos.y + 14.f);
		delete coverImage;

		DrawGaugeNumber(_dc, tInfo.iMaxHP, tInfo.iHP, vPos.y + 14.f);
		DrawGaugeNumber(_dc, tInfo.iMaxMP, tInfo.iMP, vPos.y + 32.f);
		DrawExpGauge(_dc, tInfo.iMaxExp, tInfo.iExp, vPos.y + 50.f);

	}
}

void CSkillUI::DrawGaugeNumber(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	wstring gaugeNumberPath = contentPath + L"Texture\\Skillbar\\gaugeNumber\\";

	Vec2 vPos = GetPos();
	float gaugePosX = vPos.x + 141.f;

	Graphics graphics(_dc);
	
	wstring strMaxGauge = std::to_wstring(_iMaxGauge);
	wstring strGauge = std::to_wstring(_iGauge);

	int iLen = strMaxGauge.length() + strGauge.length() + 1;

	// 한글자에 7px 계산
	float gaugeX = gaugePosX + 95.f - (iLen * 7 / 2.f);

	for (int i = 0; i < strGauge.length(); i++)
	{
		wstring strNo = gaugeNumberPath + strGauge[i] + L".png";
		Image* numberImage = Image::FromFile(strNo.c_str());
		graphics.DrawImage(numberImage, gaugeX, _fHeight);
		gaugeX += 7.f;
		delete numberImage;
	}

	wstring strNo = gaugeNumberPath + L"#" + L".png";
	Image* numberImage = Image::FromFile(strNo.c_str());
	graphics.DrawImage(numberImage, gaugeX, _fHeight);
	gaugeX += 8.f;
	delete numberImage;


	for (int i = 0; i < strMaxGauge.length(); i++)
	{
		wstring strNo = gaugeNumberPath + strMaxGauge[i] + L".png";
		Image* numberImage = Image::FromFile(strNo.c_str());
		graphics.DrawImage(numberImage, gaugeX, _fHeight);
		gaugeX += 7.f;
		delete numberImage;
	}
}

void CSkillUI::DrawExpGauge(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	wstring gaugeNumberPath = contentPath + L"Texture\\Skillbar\\gaugeNumber\\";

	Vec2 vPos = GetPos();
	float gaugePosX = vPos.x + 141.f;

	Graphics graphics(_dc);

	wstring strMaxGauge = std::to_wstring(_iMaxGauge);
	wstring strGauge = std::to_wstring(_iGauge);

	int iLen = strGauge.length() + 9;

	// 한글자에 7px 계산
	float gaugeX = gaugePosX + 95.f - (iLen * 7 / 2.f);

	for (int i = 0; i < strGauge.length(); i++)
	{
		wstring strNo = gaugeNumberPath + strGauge[i] + L".png";
		Image* numberImage = Image::FromFile(strNo.c_str());
		graphics.DrawImage(numberImage, gaugeX, _fHeight);
		gaugeX += 7.f;
		delete numberImage;
	}

	wstring strNo = gaugeNumberPath + L"@" + L".png";
	Image* numberImage = Image::FromFile(strNo.c_str());
	graphics.DrawImage(numberImage, gaugeX, _fHeight);
	gaugeX += 8.f;
	delete numberImage;

	strNo = gaugeNumberPath + L"[" + L".png";
	numberImage = Image::FromFile(strNo.c_str());
	graphics.DrawImage(numberImage, gaugeX, _fHeight);
	gaugeX += 7.f;
	delete numberImage;

	wstring strExpPercent = std::to_wstring((float)_iGauge / (float)_iMaxGauge);

	for (int i = 0; i < 5; i++)
	{
		wstring strNo = gaugeNumberPath + strExpPercent[i] + L".png";
		numberImage = Image::FromFile(strNo.c_str());

		float fHeight = _fHeight;
		if (strExpPercent[i] == L'.') fHeight += 6.f;
		graphics.DrawImage(numberImage, gaugeX, fHeight);
		gaugeX += 7.f;
		delete numberImage;
	}

	strNo = gaugeNumberPath + L"%" + L".png";
	numberImage = Image::FromFile(strNo.c_str());
	graphics.DrawImage(numberImage, gaugeX, _fHeight);
	gaugeX += 7.f;
	delete numberImage;

	strNo = gaugeNumberPath + L"]" + L".png";
	numberImage = Image::FromFile(strNo.c_str());
	graphics.DrawImage(numberImage, gaugeX, _fHeight);
	gaugeX += 7.f;
	delete numberImage;
}


void CSkillUI::MouseOn()
{
	CUI::MouseOn();
	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CSkillUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CSkillUI::MouseLbtnUp()
{
}

void CSkillUI::MouseLbtnClicked()
{
}
