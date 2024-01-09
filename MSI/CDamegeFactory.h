#pragma once

class CDamege;
class CObject;
enum class DAMEGE_TYPE;

class CDamegeFactory
{
public:
	static void CreateSingleDamege(CObject* _pObj, int _iDamege, DAMEGE_TYPE _eDamegeType);

private:
	CDamegeFactory(){};
	~CDamegeFactory(){};
};
