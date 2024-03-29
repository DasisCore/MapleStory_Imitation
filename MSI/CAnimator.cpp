#include "global.h"
#include "CAnimator.h"

#include "CAnimation.h"
#include "CScene_Ani_Workshop.h"

CAnimator::CAnimator()
	: m_pOwner(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnim);
}

void CAnimator::update()
{

}

void CAnimator::finalupdate()
{
	if (m_pCurAnim != nullptr)
	{
		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::render(HDC _dc, float _fAlpha)
{
	if (m_pCurAnim != nullptr)
	{
		m_pCurAnim->render(_dc, _fAlpha);
	}
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;

	pAnim->Create(_pTex, _vFrame, _fDuration);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::CreateAnimation_rewind(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create_rewind(_pTex, _vFrame, _fDuration);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::LoadAnimation(const wstring& _strRelationPath)
{
	CAnimation* pAnim = new CAnimation;
	pAnim->Load(_strRelationPath);

	pAnim->m_pAnimator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end()) return nullptr;
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

