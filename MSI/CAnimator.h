#pragma once

class CObject;
class CAnimation;
class CTexture;


class CAnimator
{
private:
	map<wstring, CAnimation*> m_mapAnim;	// ��� Animation
	CAnimation* m_pCurAnim;					// ���� ������� Animation
	CObject* m_pOwner;						// Animator�� ���� ������Ʈ
	bool m_bRepeat;							// �ݺ���� ����


public:
	CObject* GetObj() { return m_pOwner; }

public:
	const map<wstring, CAnimation*> GetMapAnim() { return m_mapAnim; }
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration);
	void CreateAnimation_rewind(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration);
	void LoadAnimation(const wstring& _strRelationPath);
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName, bool _bRepeat);

	void update();
	void finalupdate();
	void render(HDC _dc, float _fAlpha = 255);


public:
	CAnimator();
	//CAnimator(const CAnimator& _origin);
	~CAnimator();

	friend class CObject;
	friend class CComponent;
};

