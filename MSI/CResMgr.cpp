#include "global.h"

#include "CResMgr.h"
#include "CPathMgr.h"

#include "CTexture.h"
#include "CSound.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
	//Safe_Delete_Map(m_mapSound);
}

// 좌우 반전을 원할 경우 마지막 인자로 1을 넣어주면 된다.
CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath, bool _bIsReverse)
{
	CTexture* pTex = FindTexture(_strKey);

	if (nullptr != pTex) return pTex;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pTex = new CTexture;
	pTex->Load(strFilePath, _bIsReverse);
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	CTexture* pTex = FindTexture(_strKey);

	if (nullptr != pTex) return pTex;

	pTex = new CTexture;
	pTex->Create(_iWidth, _iHeight);
	pTex->SetKey(_strKey);

	m_mapTex.insert(make_pair(_strKey, pTex));
	return pTex;
}

// Texture map 사이에서 해당 텍스쳐를 찾는다.
CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}
	return (CTexture*) iter->second;
}

//CSound* CResMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
//{
//	CSound* pSound = FindSound(_strKey);
//
//	if (nullptr != pSound)
//	{
//		return pSound;
//	}
//
//	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
//	strFilePath += _strRelativePath;
//
//	pSound = new CSound;
//	pSound->Load(strFilePath.c_str());
//	pSound->SetKey(_strKey);
//	pSound->SetRelativePath(_strRelativePath);
//
//	m_mapSound.insert(make_pair(_strKey, pSound));
//
//	return pSound;
//}

//CSound* CResMgr::FindSound(const wstring& _strKey)
//{
//	map<wstring, CRes*>::iterator iter = m_mapSound.find(_strKey);
//
//	if (iter == m_mapSound.end())
//	{
//		return nullptr;
//	}
//
//	return (CSound*)iter->second;
//}