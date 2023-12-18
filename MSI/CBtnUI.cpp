#include "CBtnUI.h"

#include "CScene.h"
#include "CObject.h"

#include "CCamera.h"

void CBtnUI::render(HDC _dc)
{
	CUI::render(_dc);

	Graphics graphics(_dc);
	Font font(L"Arial", 8);
	SolidBrush brush(Color(255, 0, 0, 0));

	Vec2 vPos = GetFinalPos() + Vec2(0.f, 5.f);

	graphics.DrawString(GetName().c_str(), -1, &font, PointF(vPos.x, vPos.y), &brush);
}

void CBtnUI::Dedicate_workshop_render(HDC _dc)
{
	Graphics graphics(_dc);

	Pen pen(Color(0, 0, 0), 1.f);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	graphics.DrawRectangle(&pen, Rect(vPos.x, vPos.y, vScale.x, vScale.y));
}

CBtnUI::CBtnUI()
	: CUI(false)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(nullptr)
	, m_pSceneFunc(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}

void CBtnUI::MouseLbtnClicked()
{
	if (m_pFunc != nullptr)
	{
		m_pFunc(m_param1, m_param2);
	}

	if (m_pSceneFunc && m_pSceneInst)
	{	// 둘다 포인터이므로, 원형을 불러옴.
		((*m_pSceneInst).*m_pSceneFunc)();
	}

}

