#include "global.h"
#include "CMonster.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRandom.h"
#include "CRigidBody.h"

#include "AI.h"

#include "CState.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
{
	CreateComponent();
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(60.f, 60.f));

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

void CMonster::SetRandomDir()
{
	if (CRandom::GetInst()->GetRandomBool()) m_tInfo.mDir = 1;
	else m_tInfo.mDir = -1;
}



void CMonster::update()
{
	Vec2 vV = GetComponent()->GetRigidbody()->GetVelocity();
	if (vV.x < 0) m_tInfo.mDir = -1;
	else m_tInfo.mDir = 1;

	if (m_pAI)
	{
		m_pAI->update();
	}
	update_animation();
}

void CMonster::render(HDC _dc)
{
	CObject::render(_dc);

	wstring tempStr;

	MON_STATE curState = m_pAI->GetCurState()->GetType();

	if (curState == MON_STATE::IDLE)
	{
		tempStr = L"IDLE";
	}
	else if (curState == MON_STATE::PATROL)
	{
		tempStr = L"PATROL";
	}
	else if (curState == MON_STATE::ATT)
	{
		tempStr = L"ATTACK";
	}
	else if (curState == MON_STATE::RUN)
	{
		tempStr = L"RUN";
	}
	else if (curState == MON_STATE::TRACE)
	{
		tempStr = L"TRACE";
	}

	Vec2 vV = GetComponent()->GetRigidbody()->GetVelocity();
	tempStr = std::to_wstring(vV.x) + L" / " + std::to_wstring(vV.y);

	Graphics graphics(_dc);

	Font font(L"Arial", 10, FontStyle::FontStyleBold);

	SolidBrush brush(Color(255, 0, 0, 0));

	Vec2 vPos = GetPos();

	PointF point(vPos.x + 40.f, vPos.y);

	graphics.DrawString(tempStr.c_str(), -1, &font, point, &brush);
}

void CMonster::update_animation()
{
	wstring currentChar = m_wCurChar;

	MON_STATE curState = m_pAI->GetCurState()->GetType();

	if (m_tInfo.mDir == -1)
	{
		currentChar += L"_LEFT";
	}
	else
	{
		currentChar += L"_RIGHT";
	}

	if (curState == MON_STATE::IDLE)
	{
		currentChar += L"_IDLE";
	}
	else if (curState == MON_STATE::PATROL)
	{
		currentChar += L"_WALK";
	}
	else if (curState == MON_STATE::ATT)
	{
		currentChar += L"_ATTACK";
	}
	else if (curState == MON_STATE::RUN)
	{
		currentChar += L"_WALK";
	}
	else if (curState == MON_STATE::TRACE)
	{
		currentChar += L"_WALK";
	}

	GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
}