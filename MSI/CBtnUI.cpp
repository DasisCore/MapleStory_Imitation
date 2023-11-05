#include "CBtnUI.h"

#include "CScene.h"
#include "CObject.h"

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

