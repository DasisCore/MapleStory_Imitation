#include "global.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"

#include "CResMgr.h"
#include "CPathMgr.h"

CAnimation::CAnimation()
	: m_strName()
	, m_pAnimator(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish) return;

	// 시간에 따른 프레임 변화
	m_fAccTime += fDT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		m_iCurFrm++;

		if (m_vecFrm.size() == m_iCurFrm)
		{
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
	{
		m_iCurFrm = (int)m_vecFrm.size() - 1;
	}

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;	// offset만큼 추가 이동

	// 렌더링 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// GDI+로부터 png 파일을 불러왔으므로, BitBlt이 아닌 AlphaBlend.
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;  // 투명도 (0~255, 0이 완전투명, 255가 완전불투명)
	bf.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, bf);

	//BitBlt(_dc
	//	, 500, 100
	//	, 400, 400
	//	, m_pTex->GetDC()
	//	, 0, 0, SRCCOPY);

	//TransparentBlt(_dc
	//	, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
	//	, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
	//	, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
	//	, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
	//	, m_pTex->GetDC()
	//	, (int)(m_vecFrm[m_iCurFrm].vLT.x)
	//	, (int)(m_vecFrm[m_iCurFrm].vLT.y)
	//	, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
	//	, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
	//	, RGB(255, 0, 255)
	//);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Create_rewind(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}

	for (UINT i = _iFrameCount - 2; i > 0; i--)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Animation의 이름을 저장한다. -> 데이터 직렬화
	SaveWString(m_strName, pFile);

	// 텍스쳐 키
	SaveWString(m_pTex->GetKey(), pFile);
	SaveWString(m_pTex->GetRelativePath(), pFile);

	// 프레임 개수
	size_t iFrameCount = m_vecFrm.size();
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// 프레임 정보
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);

	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	// 문자열 읽기
	LoadWString(m_strName, pFile);

	// 텍스쳐
	wstring strTexKey, strTexPath;
	LoadWString(strTexKey, pFile);
	LoadWString(strTexPath, pFile);
	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

	// 프레임 개수
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, pFile);

	// 모든 프레임 정보
	m_vecFrm.resize(iFrameCount);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);

	fclose(pFile);
}


