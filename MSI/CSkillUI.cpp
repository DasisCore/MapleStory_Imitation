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
	, m_pPlayer(nullptr)
	, m_pBackImage(nullptr)
	, m_pCharType(nullptr)
	, m_fDisplayHP(1.f)
	, m_fCoolTimeSkill1(4.f)
	, m_fCoolTimeSkill2(4.f)
	, m_fCurCoolTimeSkill1(0.f)
	, m_fCurCoolTimeSkill2(0.f)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	wstring backImagePath = contentPath + _imageRelativePath;
	wstring CharTypePath = contentPath + L"Texture\\SkillBar\\charType.png";
	m_pBackImage = Image::FromFile(backImagePath.c_str());
	m_pCharType = Image::FromFile(CharTypePath.c_str());

	float fWidth = m_pBackImage->GetWidth();
	float fHeight = m_pBackImage->GetHeight();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	m_pPlayer = (CPlayer*)pCurScene->GetPlayer();

	SetScale(Vec2(fWidth, fHeight));
}

CSkillUI::~CSkillUI()
{
}

void CSkillUI::update()
{
	const tPlayerInfo tInfo = m_pPlayer->GetPlayerInfo();

	// hp persent update
	GaugePersentUpdate(tInfo.iMaxHP, tInfo.iHP, m_fDisplayHP);

	// mp persent update
	GaugePersentUpdate(tInfo.iMaxMP, tInfo.iMP, m_fDisplayMP);
	
	// exp persent update
	GaugePersentUpdate(tInfo.iMaxExp, tInfo.iExp, m_fDisplayEXP);
}

void CSkillUI::render(HDC _dc)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());

	const tPlayerInfo tInfo = m_pPlayer->GetPlayerInfo();

	Graphics graphics(_dc);

	if (m_pBackImage != nullptr)
	{
		Vec2 vPos = GetPos();
		// background 이미지 출력
		graphics.DrawImage(m_pBackImage, vPos.x, vPos.y);

		// 캐릭터 타입 출력
		graphics.DrawImage(m_pCharType, vPos.x + 20.f, vPos.y + 35.f);

		// 캐릭터 레벨 출력
		DisplayLevel(graphics);

		float gaugePosX = vPos.x + 141.f;

		// 캐릭터 hp 출력
		SolidBrush hpBrush(Color(255, 34, 102));
		graphics.FillRectangle(&hpBrush, Rect(gaugePosX, vPos.y + 14.f, 189 * m_fDisplayHP, 11.f));

		// 캐릭터 mp 출력
		SolidBrush mpBrush(Color(0, 187, 238));
		graphics.FillRectangle(&mpBrush, Rect(gaugePosX, vPos.y + 32.f, 189 * m_fDisplayMP, 11.f));

		// 캐릭터 exp 출력
		SolidBrush expBrush(Color(187, 221, 0));
		graphics.FillRectangle(&expBrush, Rect(gaugePosX, vPos.y + 50.f, 189 * m_fDisplayEXP, 11.f));

		// 게이지 커버 
		wstring gaugeCoverPath = contentPath + L"Texture\\Skillbar\\cover.png";
		Image* coverImage = Image::FromFile(gaugeCoverPath.c_str());
		graphics.DrawImage(coverImage, vPos.x + 141.f, vPos.y + 14.f);
		delete coverImage;

		renderGaugeNumber(_dc, tInfo.iMaxHP, tInfo.iHP, vPos.y + 14.f);
		renderGaugeNumber(_dc, tInfo.iMaxMP, tInfo.iMP, vPos.y + 32.f);
		renderExpGauge(_dc, tInfo.iMaxExp, tInfo.iExp, vPos.y + 50.f);

		renderSkillImage(graphics);

		renderSkillCoolTime(graphics);
	}
}

void CSkillUI::renderGaugeNumber(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight)
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

void CSkillUI::renderExpGauge(HDC _dc, int _iMaxGauge, int _iGauge, float _fHeight)
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

void CSkillUI::renderSkillImage(Graphics& _graphics)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	contentPath += L"Texture\\Skillbar\\";

	//wstring strPlayerName = m_pPlayer->GetCurChar();
	wstring strPlayerName = L"BARK";
	
	Vec2 vPos = GetPos();

	for (int i = 0; i < 6; i++)
	{
		wstring skillPath = contentPath + strPlayerName + std::to_wstring(i) + L".png";
		Image* skillImage = Image::FromFile(skillPath.c_str());
		_graphics.DrawImage(skillImage, Rect(vPos.x + 364 + (i * 55), vPos.y + 13, skillImage->GetWidth(), skillImage->GetHeight()));
		delete skillImage;
	}
}

void CSkillUI::renderSkillCoolTime(Graphics& _graphics)
{
	Vec2 vPos = GetPos();
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	contentPath += L"Texture\\Skillbar\\CoolTime\\";

	if (m_fCurCoolTimeSkill1 > 0)
	{
		int iCurImage = m_fCurCoolTimeSkill1 / m_fCoolTimeSkill1 * 17;
		wstring coolTimePath = contentPath + std::to_wstring(iCurImage) + L".png";
		Image* coolTimeImage = Image::FromFile(coolTimePath.c_str());
		_graphics.DrawImage(coolTimeImage, Rect(vPos.x + 419, vPos.y + 13, coolTimeImage->GetWidth(), coolTimeImage->GetHeight()));
		delete coolTimeImage;

		m_fCurCoolTimeSkill1 -= fDT;

	}

	if (m_fCurCoolTimeSkill2 > 0)
	{
		int iCurImage = m_fCurCoolTimeSkill2 / m_fCoolTimeSkill2 * 17;
		wstring coolTimePath = contentPath + std::to_wstring(iCurImage) + L".png";
		Image* coolTimeImage = Image::FromFile(coolTimePath.c_str());
		_graphics.DrawImage(coolTimeImage, Rect(vPos.x + 474, vPos.y + 13, coolTimeImage->GetWidth(), coolTimeImage->GetHeight()));
		delete coolTimeImage;

		m_fCurCoolTimeSkill2 -= fDT;

	}
}

void CSkillUI::DisplayLevel(Graphics& _graphics)
{
	wstring contentPath = wstring(CPathMgr::GetInst()->GetContentPath());
	const tPlayerInfo tInfo = m_pPlayer->GetPlayerInfo();
	Vec2 vPos = GetPos();

	// 캐릭터 레벨 출력
	wstring lvNumberPath = contentPath + L"Texture\\Skillbar\\lvNumber\\";
	wstring strLv = std::to_wstring(tInfo.iLevel);
	for (int i = 0; i < strLv.length(); i++)
	{
		wstring strNo = lvNumberPath + strLv[i] + L".png";
		Image* numberImage = Image::FromFile(strNo.c_str());
		_graphics.DrawImage(numberImage, vPos.x + (i * 11.f) + 43.f, vPos.y + 10.f);
		delete numberImage; 
	}

}

void CSkillUI::GaugePersentUpdate(const int _iMaxGauge, const int _iGauge, float& _fDisplayGauge)
{
	const tPlayerInfo tInfo = m_pPlayer->GetPlayerInfo();

	float iGauge = _iGauge;
	float iMaxGauge = _iMaxGauge;

	if (iGauge / iMaxGauge != _fDisplayGauge) _fDisplayGauge += ((iGauge / iMaxGauge) - _fDisplayGauge) * fDT;
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
