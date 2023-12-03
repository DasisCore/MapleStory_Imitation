#include "global.h"
#include "CSprite.h"

#include "CCore.h"
#include "CKeyMgr.h"

CSprite::CSprite(wstring _strAbsolutePath)
	: m_pImage(nullptr)
	, m_tInfo{}
{
	Image* image = Image::FromFile(_strAbsolutePath.c_str());
	m_pImage = image;

	m_tInfo.bTarget = true;
	m_tInfo.fOriginWidth = image->GetWidth();
	m_tInfo.fOriginHeight = image->GetHeight();
	m_tInfo.fHeightRatio = 1.f;
	m_tInfo.fWidthRatio = 1.f;
	m_tInfo.fCurWidth = image->GetWidth();
	m_tInfo.fCurHeight = image->GetHeight();

	SetScale(Vec2(m_tInfo.fOriginWidth, m_tInfo.fOriginHeight));
}

CSprite::~CSprite()
{
	if (m_pImage) delete m_pImage;
}

void CSprite::DrawBorder(HDC _dc)
{
	Graphics graphics(_dc);
	float borderWidth = 1.5f;
	Pen borderPen(Color(99, 189, 255), borderWidth);

	float centerX = GetPos().x - m_tInfo.fOriginWidth / 2;
	float centerY = GetPos().y - m_tInfo.fOriginHeight / 2;

	float lineWidth = (m_tInfo.fOriginWidth * m_tInfo.fWidthRatio) + borderWidth;
	float lineHeight = (m_tInfo.fOriginHeight * m_tInfo.fHeightRatio) + borderWidth;

	graphics.DrawRectangle(&borderPen, centerX, centerY, lineWidth, lineHeight);
}

void CSprite::IsLbtnDown()
{
	if (m_tInfo.bTarget && CKeyMgr::GetInst()->IsMouseInObj(this) && KEY_TAP(KEY::LBTN))
	{
		m_vDragStart = CKeyMgr::GetInst()->GetMousePos();
	}
}

void CSprite::IsLbtnDrag()
{
	if (m_tInfo.bTarget && CKeyMgr::GetInst()->IsMouseInObj(this) && KEY_HOLD(KEY::LBTN))
	{
		Vec2 vCurPos = GetPos();
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		vCurPos += vDiff;
		SetPos(vCurPos);
		m_vDragStart = MOUSE_POS;
	}
}

bool CSprite::IsPointInBorder()		// 잘못 만들었는데 임시로 쓸일 있을까 싶어서 놔둠.
{
	float imageWidth = m_tInfo.fOriginWidth * m_tInfo.fWidthRatio;
	float imageHeight = m_tInfo.fOriginHeight * m_tInfo.fHeightRatio;

	// 테두리의 좌상단 좌표
	Vec2 vLT = Vec2(GetPos().x - (imageWidth / 2.f) - 1.5f, GetPos().y - (imageHeight / 2.f) - 1.5f);
	Vec2 vRB = Vec2(GetPos().x + (imageWidth / 2.f) + 1.5f, GetPos().y + (imageHeight / 2.f) + 1.5f);

	// 상단 테두리에 마우스가 접근했다면
	if (vLT.x <= MOUSE_POS.x && MOUSE_POS.x <= vLT.x + imageWidth
		&& vLT.y <= MOUSE_POS.y && MOUSE_POS.y <= vLT.y + 1.5f) return true;

	// 우측 테두리에 마우스가 접근했다면
	if (vRB.x - 1.5f <= MOUSE_POS.x && MOUSE_POS.x <= vRB.x
		&& vLT.y <= MOUSE_POS.y && MOUSE_POS.y <= vRB.y) return true;

	// 하단 테두리에 마우스가 접근했다면
	if (vLT.x <= MOUSE_POS.x && MOUSE_POS.x <= vLT.x + imageWidth
		&& vRB.y - 1.5f <= MOUSE_POS.y && MOUSE_POS.y <= vRB.y) return true;

	// 좌측 테두리에 마우스가 접근했다면
	if (vLT.x <= MOUSE_POS.x && MOUSE_POS.x <= vLT.x + 1.5f
		&& vLT.y <= MOUSE_POS.y && MOUSE_POS.y <= vRB.y) return true;

	return false;
}


void CSprite::update()
{
	if (m_tInfo.bTarget)
	{
		IsLbtnDown();
		IsLbtnDrag();

		int curHandle = IsHandleIn();

		if (curHandle > 0 && KEY_TAP(KEY::LBTN))
		{
			m_vDragStart = MOUSE_POS;
			m_iHandle = curHandle;
			m_bMouseOn = true;
		}
		if (curHandle > 0 && KEY_HOLD(KEY::LBTN))
		{
			HandleDrag(curHandle);
		}

		if (KEY_AWAY(KEY::LBTN))
		{
			m_iHandle = 0;
			m_bMouseOn = false;
		}
	}

}

void CSprite::render(HDC _dc)
{
	// CObject가 하는 render는 기본적으로 모두 함.
	/*CObject::render(_dc);*/

	float imageWidth = m_tInfo.fOriginWidth * m_tInfo.fWidthRatio;
	float imageHeight = m_tInfo.fOriginHeight * m_tInfo.fHeightRatio;

	float centerX = GetPos().x - m_tInfo.fOriginWidth / 2;
	float centerY = GetPos().y - m_tInfo.fOriginHeight / 2;

	Graphics graphics(_dc);
	graphics.DrawImage(m_pImage, centerX, centerY, imageWidth, imageHeight);

	if (m_tInfo.bTarget)
	{
		DrawBorder(_dc);
		renderImageHandle(_dc);
	}

	Font font(L"Arial", 10);
	SolidBrush brush(Color(255, 0, 0, 0));
	graphics.DrawString((L"현재 너비 / " + std::to_wstring(m_tInfo.fCurWidth)).c_str(), -1, &font, PointF(10, 100), &brush);
	graphics.DrawString((L"현재 높이 / " + std::to_wstring(m_tInfo.fCurHeight)).c_str(), -1, &font, PointF(10, 120), &brush);
	graphics.DrawString((L"너비 비율 / " + std::to_wstring(m_tInfo.fWidthRatio)).c_str(), -1, &font, PointF(10, 140), &brush);
	graphics.DrawString((L"높이 비율 / " + std::to_wstring(m_tInfo.fHeightRatio)).c_str(), -1, &font, PointF(10, 160), &brush);
}



void CSprite::renderImageHandle(HDC _dc)
{
	float imageWidth = m_tInfo.fOriginWidth * m_tInfo.fWidthRatio;
	float imageHeight = m_tInfo.fOriginHeight * m_tInfo.fHeightRatio;

	// 테두리의 좌상단 좌표
	Vec2 vLT = Vec2(GetPos().x - (imageWidth / 2.f), GetPos().y - (imageHeight / 2.f));
	Vec2 vRB = Vec2(GetPos().x + (imageWidth / 2.f), GetPos().y + (imageHeight / 2.f));

	Graphics graphics(_dc);
	
	float borderWidth = 1.5f;

	Pen pen(Color(99, 189, 255), borderWidth); // 테두리와 같은 색의 펜
	SolidBrush whiteBrush(Color(255, 255, 255));  // 흰색 브러시


	// 좌상단 사각형
	graphics.FillRectangle(&whiteBrush, vLT.x - 3.f, vLT.y - 3.f, 5.f, 5.f);	// 사각형 그리기
	graphics.DrawRectangle(&pen, vLT.x - 3.f, vLT.y - 3.f, 5.f, 5.f);

	// 우상단 사각형
	graphics.FillRectangle(&whiteBrush, vRB.x - 1.5f, vLT.y - 3.f, 5.f, 5.f);	// 사각형 그리기
	graphics.DrawRectangle(&pen, vRB.x - 1.5f, vLT.y - 3.f, 5.f, 5.f);

	// 좌하단 사각형
	graphics.FillRectangle(&whiteBrush, vLT.x - 3.f, vRB.y - 1.5f, 5.f, 5.f);	// 사각형 그리기
	graphics.DrawRectangle(&pen, vLT.x - 3.f, vRB.y - 1.5f, 5.f, 5.f);

	// 우하단 사각형
	graphics.FillRectangle(&whiteBrush, vRB.x - 2.5f, vRB.y - 1.5f, 5.f, 5.f);	// 사각형 그리기
	graphics.DrawRectangle(&pen, vRB.x - 2.5f, vRB.y - 1.5f, 5.f, 5.f);
}

int CSprite::IsHandleIn()
{
	float imageWidth = m_tInfo.fOriginWidth * m_tInfo.fWidthRatio;
	float imageHeight = m_tInfo.fOriginHeight * m_tInfo.fHeightRatio;

	Vec2 vLT = Vec2(GetPos().x - (imageWidth / 2.f), GetPos().y - (imageHeight / 2.f));
	Vec2 vRB = Vec2(GetPos().x + (imageWidth / 2.f), GetPos().y + (imageHeight / 2.f));

	// 좌상단 핸들
	if (vLT.x - 3.f <= MOUSE_POS.x && MOUSE_POS.x <= vLT.x + 5.f
		&& vLT.y - 3.f <= MOUSE_POS.y && MOUSE_POS.y <= vLT.y + 5.f) return 1;

	// 우상단 핸들
	if (vRB.x - 1.5f <= MOUSE_POS.x && MOUSE_POS.x <= vRB.x + 5.f
		&& vLT.y - 3.f <= MOUSE_POS.y && MOUSE_POS.y <= vLT.y + 5.f) return 2;

	// 좌하단 핸들
	if (vLT.x - 3.f <= MOUSE_POS.x && MOUSE_POS.x <= vLT.x + 5.f
		&& vRB.y - 1.5f <= MOUSE_POS.y && MOUSE_POS.y <= vRB.y + 5.f) return 3;

	// 상단 핸들
	if (vRB.x - 2.5f <= MOUSE_POS.x && MOUSE_POS.x <= vRB.x + 5.f
		&& vRB.y - 1.5f <= MOUSE_POS.y && MOUSE_POS.y <= vRB.y + 5.f) return 4;

	return 0;
}

void CSprite::HandleDrag(int _point)
{
	Vec2 vDelta = MOUSE_POS - m_vDragStart;

	Vec2 vScale = GetScale();

	

	if (_point == 1)
	{
		update_tInfo();
		vScale += (vDelta.x * -1);
		vScale += (vDelta.y);
		SetScale(vScale);
		m_vDragStart = MOUSE_POS;
	}
	else if (_point == 2)
	{

	}
	else if (_point == 3)
	{

	}
	else if (_point == 4)
	{

	}
}

void CSprite::update_tInfo()
{
	Vec2 vScale = GetScale();

	m_tInfo.fCurWidth = vScale.x;
	m_tInfo.fCurHeight = vScale.y;
	m_tInfo.fWidthRatio = m_tInfo.fOriginWidth / vScale.x;
	m_tInfo.fHeightRatio = m_tInfo.fOriginHeight / vScale.y;
}



