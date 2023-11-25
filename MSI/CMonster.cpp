#include "global.h"
#include "CMonster.h"

#include "CComponent.h"
#include "CCollider.h"

#include "AI.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
{
	CreateComponent();
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(110.f, 60.f));

	CreateAnimation();
	CreateRigidbody();
	
	// 공중 몹은 중력이 필요 없으므로, 
	// 기본으로 넣어주지 않는다.
	//CreateGravity();
}

CMonster::~CMonster()
{
	if (m_pAI != nullptr)
	{
		delete m_pAI;
	}
}

void CMonster::SetAI(AI* _pAI)
{
	m_pAI = _pAI;
	m_pAI->m_pOwner = this;
}

void CMonster::update()
{

	if (m_pAI)
	{
		m_pAI->update();
	}
}
