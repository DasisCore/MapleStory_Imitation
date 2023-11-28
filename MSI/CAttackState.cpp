#include "global.h"
#include "CAttackState.h"
#include "CCollider.h"
#include "CCore.h"
#include "CTexture.h"

void CAttackState::Enter()
{
	m_pCollider = new CCollider;
	m_pCollider->SetScale(Vec2(200.f, 100.f));
}

void CAttackState::Exit()
{
	if (m_pCollider != nullptr) delete m_pCollider;
}

void CAttackState::update()
{
	HDC dc = CCore::GetInst()->GetMemTex()->GetDC();

	m_pCollider->render(dc);
}

CAttackState::CAttackState()
	:CState(MON_STATE::ATT)
{
}

CAttackState::~CAttackState()
{
}
