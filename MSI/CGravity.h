#pragma once
class CGravity
{

private:
	CObject* m_pOwner;

	bool m_bGround;

public:
	// 땅에 붙었는지 설정 (Y축 중력을 차단함)
	void SetGround(bool _b);

public:
	void finalupdate();

public:
	CGravity();
	~CGravity();

	friend class CObject;
};

