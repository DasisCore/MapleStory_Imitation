#include "global.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"

#include "CResMgr.h"
#include "CPathMgr.h"

#include "CCollider.h"
#include "CComponent.h"

CAnimation::CAnimation()
	: m_strName()
	, m_pAnimator(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_bIsReverse(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish) return;
	
	// �ð��� ���� ������ ��ȭ
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

	// �ݶ��̴��� �����¸�ŭ �ִϸ��̼� ��� ��ġ ����
	if(pObj->GetComponent()->GetCollider() != nullptr) vPos += pObj->GetComponent()->GetCollider()->GetOffsetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;	// offset��ŭ �߰� �̵�

	// ������ ��ǥ�� ��ȯ
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// GDI+�κ��� png ������ �ҷ������Ƿ�, BitBlt�� �ƴ� AlphaBlend.
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;  // ���� (0~255, 0�� ��������, 255�� ����������)
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

//_inAtlasNxtLine�� ��Ʋ�� �̹������� 2�� �̻��� ������������ �����.
void CAnimation::Create(CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _vFrame.size(); i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vFrame[i].vSlice;
		frm.vLT = _vFrame[i].vLT;
		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Create_rewind(CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _vFrame.size(); i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vFrame[i].vSlice;
		frm.vLT = _vFrame[i].vLT;
		m_vecFrm.push_back(frm);
	}
	for (UINT i = _vFrame.size() - 2; i > 0; i--)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vFrame[i].vSlice;
		frm.vLT = _vFrame[i].vLT;
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

	// Animation�� �̸��� �����Ѵ�. -> ������ ����ȭ
	SaveWString(m_strName, pFile);

	// �ؽ��� Ű
	SaveWString(m_pTex->GetKey(), pFile);
	SaveWString(m_pTex->GetRelativePath(), pFile);

	// ������ ����
	size_t iFrameCount = m_vecFrm.size();
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// ������ ����
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);

	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	std::wifstream file(strFilePath);
	if (!file.is_open()) assert(nullptr);

	wstring temp1, temp2;

	// [Animation Name];
	file >> m_strName >> m_strName >> m_strName;

	// [Texture Key], [Relative Path]
	wstring strTexKey, strTexPath;
	file >> strTexKey >> strTexKey >> strTexKey;
	file >> strTexPath >> strTexPath >> strTexPath;

	// [Reversed]
	file >> temp1 >> temp1;
	m_bIsReverse = temp1 == L"0" ? false : true;

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath, m_bIsReverse);

	// [Frame Count]
	size_t iFrameCount = 0;
	file >> temp1 >> temp1 >> temp1;
	iFrameCount = stoi(temp1);

	// [Frame Data]
	file >> temp1 >> temp1;
	for (int i = 0; i < iFrameCount; i++)
	{
		file >> temp1;	// Frame-n;
		file >> temp1;	// Duration;
		float duration;
		file >> temp1;
		duration = stof(temp1);

		file >> temp1;	// LeftTop;
		float vLTx, vLTy;
		file >> temp1 >> temp2;
		vLTx = stof(temp1);
		vLTy = stof(temp2);

		file >> temp1;	// Offset;
		float vOffsetX, vOffsetY;
		file >> temp1 >> temp2;
		vOffsetX = stof(temp1);
		vOffsetY = stof(temp2);

		file >> temp1;	// Slice;
		float vSliceX, vSliceY;
		file >> temp1 >> temp2;
		vSliceX = stof(temp1);
		vSliceY = stof(temp2);

		tAnimFrm tFrm = {};
		tFrm.fDuration = duration;
		tFrm.vLT = Vec2(vLTx, vLTy);
		tFrm.vOffset = Vec2(vOffsetX, vOffsetY);
		tFrm.vSlice = Vec2(vSliceX, vSliceY);
		m_vecFrm.push_back(tFrm);
	}
}


