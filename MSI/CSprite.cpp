#include "global.h"
#include "CSprite.h"

#include "CCore.h"
#include "CKeyMgr.h"

CSprite::CSprite(wstring _strAbsolutePath)
	: m_pImage(nullptr)
	, m_bTarget(true)
	, m_fRatio(1.f)
{
	Image* image = Image::FromFile(_strAbsolutePath.c_str());
	m_pImage = image;

	SetScale(Vec2((float)image->GetWidth(), (float)image->GetHeight()));
}

CSprite::~CSprite()
{
	if (m_pImage) delete m_pImage;
}

void CSprite::DrawLine(HDC _dc)
{
	Graphics graphics(_dc);
	float borderWidth = 1.f;
	Pen borderPen(Color(99, 189, 255), borderWidth);

	graphics.DrawRectangle(&borderPen, GetPos().x, GetPos().y, m_pImage->GetWidth() + borderWidth, m_pImage->GetHeight() + borderWidth);
}

void CSprite::update()
{
	if(m_bTarget && CKeyMgr::GetInst()->IsMouseInObj(this) && KEY_HOLD(KEY::LBTN))
	{
		SetPos(CKeyMgr::GetInst()->GetMousePos());
	}
}

void CSprite::render(HDC _dc)
{
	// CObject가 하는 render는 기본적으로 모두 함.
	CObject::render(_dc);

	Graphics graphics(_dc);
	graphics.DrawImage(m_pImage, GetPos().x, GetPos().y, m_pImage->GetWidth() * m_fRatio, m_pImage->GetHeight() * m_fRatio);

	if (m_bTarget)
	{
		DrawLine(_dc);

	}
}




