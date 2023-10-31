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
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName, bool _bRepeat);

	void render(HDC _dc);
	void update();


public:
	CAnimator();
	//CAnimator(const CAnimator& _origin);
	~CAnimator();

	friend class CObject;
	friend class CComponent;
};
