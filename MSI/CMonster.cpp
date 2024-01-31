#include "global.h"
#include "CMonster.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRandom.h"
#include "CRigidBody.h"
#include "CPlayer.h"
#include "CDamegeMgr.h"

#include "AI.h"

#include "CState.h"
#include "CDamege.h"
#include "CDamegeFactory.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CRandom.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
	, m_fLeftDist(0.f)
	, m_fRightDist(0.f)
	, m_fAlpha(0.f)
	, m_fUnbeatableTime(0.3f)
{
	CreateComponent();
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
	if (CRandom::GetInst()->GetRandomBool()) m_tInfo.iDir = 1;
	else m_tInfo.iDir = -1;
}

void CMonster::TakeDamege(float _fDamege, int _iDir)
{
	if (!m_bDead && m_fUnbeatableTime < 0)
	{
		m_tInfo.fHP -= _fDamege;
		m_fHitTime = 0.5f;
		m_pAI->ChangeState(MON_STATE::HIT);
		m_tInfo.iDir = _iDir * -1;

		// 몬스터 넉백 구현
		GetComponent()->GetRigidbody()->AddVelocity(Vec2(300.f * _iDir, -200.f));

		for (int i = 0; i < 5; i++)
		{
			DAMEGE_INFO tInfo;
			tInfo.eType = DAMEGE_TYPE::NORED;

			_fDamege = CRandom::GetInst()->GetBetweenInt(int(_fDamege * 0.8f), int(_fDamege * 1.2f));
			tInfo.fDamege = _fDamege;
			tInfo.fLatencyTime = 0.1f * i;
			tInfo.pObj = this;
			tInfo.fOffset = 27.f * i;
			CDamegeMgr::GetInst()->AddDamege(tInfo);
		}
	}
}


void CMonster::update()
{
	if (m_pAI)
	{
		m_pAI->update();
	}

	if (GetComponent()->GetAnimator() != nullptr)
	{
		update_animation();
	}

	if (m_bDead) DeleteObject(this);

	if (m_fAlpha < 253) m_fAlpha += (700.f * fDT);
	if (m_fAlpha > 255) m_fAlpha = 255;

	if (m_fUnbeatableTime > 0) m_fUnbeatableTime -= fDT;
}

void CMonster::render(HDC _dc)
{
	CObject::component_render(_dc, m_fAlpha);
	//CObject::render(_dc);

	if (m_pAI && CCore::GetInst()->GetRenderOption())
	{
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
		else if (curState == MON_STATE::DEAD)
		{
			tempStr = L"DEAD";
		}
		else if (curState == MON_STATE::HIT)
		{
			tempStr = L"HIT";
		}

		Graphics graphics(_dc);

		Font font(L"Arial", 10, FontStyle::FontStyleBold);

		SolidBrush brush(Color(255, 0, 0, 0));

		Vec2 vPos = GetPos();
		vPos = CCamera::GetInst()->GetRenderPos(vPos);

		PointF point(vPos.x + 40.f, vPos.y);

		graphics.DrawString(tempStr.c_str(), -1, &font, point, &brush);
	
		// 몬스터의 사정거리 그리기
		Pen pen(Color(255, 0, 0, 255));
		if (m_tInfo.iDir == 1)
		{
			graphics.DrawRectangle(&pen, vPos.x, vPos.y - (GetScale().y / 2.f), m_tInfo.vRecogRange.x, GetScale().y);
		}
		else
		{
			graphics.DrawRectangle(&pen, vPos.x - m_tInfo.vRecogRange.x, vPos.y - (GetScale().y / 2.f), m_tInfo.vRecogRange.x, GetScale().y);
		}

		tempStr = L"HP : " + std::to_wstring(m_tInfo.fHP);
		graphics.DrawString(tempStr.c_str(), -1, &font, PointF(vPos.x + 40.f, vPos.y + 15.f), &brush);
	}
}

void CMonster::update_animation()
{
	wstring currentChar = GetName();

	if (m_pAI == nullptr)
	{
		currentChar += L"_LEFT_IDLE";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
		return;
	}

	MON_STATE curState = m_pAI->GetCurState()->GetType();

	if (m_tInfo.iDir == -1) currentChar += L"_LEFT";
	else currentChar += L"_RIGHT";

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
	else if (curState == MON_STATE::DEAD)
	{
		currentChar += L"_DEAD";
	}
	else if (curState == MON_STATE::HIT)
	{
		currentChar += L"_HIT";
	}
	
	if (currentChar.substr(currentChar.size() - 5, 5) == L"_DEAD")
	{
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), false);
		return;
	}

	GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
}

void CMonster::OnCollision(CCollider* _pOther)
{
	CObject* pObj = _pOther->GetObj();

	if (pObj->GetName() == L"PLAYER")
	{
		CPlayer* pPlayer = (CPlayer*) pObj;
		if (!pPlayer->IsUnbeatable() && !m_bDead)
		{
			pPlayer->CharHit(m_tInfo.iDir, m_tInfo.fAtt);
		}
	}
}
