#pragma once

struct Vec2
{
	float x;
	float y;

public:

	bool IsZero()
	{
		if (x == 0.f && y == 0.f) return true;
		return false;
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}

	Vec2& Normalize()
	{
		float fLen = Length();

		// 디버깅용 함수 (치명적인 오류 가능성)
		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}

	// x를 _l과 _r 사이로 한정 짓는다.
	Vec2& ClampX(float _l, float _r)
	{
		x = max(_l, x);
		x = min(_r, x);
		return *this;
	}

	// y를 _l과 _h 사이로 한정 짓는다.
	Vec2& ClampY(float _l, float _h)
	{
		y = max(_l, y);
		y = min(_h, y);
		return *this;
	}

public:

	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}
	
	Vec2 operator + (float _f)
	{
		return Vec2(x + _f, y + _f);
	}

	Vec2 operator + (Vec2 _vOther)
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	Vec2 operator += (float _f)
	{
		x += _f;
		y += _f;
	}

	void operator += (Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}

	Vec2 operator - (Vec2 _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}


	void operator -= (float _f)
	{
		x -= _f;
		y -= _f;
	}

	void operator -= (Vec2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}

	Vec2 operator * (int _i)
	{
		return Vec2(x * (float)_i, y * (float)_i);
	}

	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}

	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}

	Vec2 operator / (Vec2 _vOther)
	{
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vec2(x / _vOther.x, y / _vOther.y);
	}

	Vec2 operator / (float _f)
	{
		assert(!(0.f == _f));
		return Vec2(x / _f, y / _f);
	}


	bool operator == (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y) return true;
		else return false;
	}

public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}
	
	Vec2(int _x, int _y)
		:x((float)_x)
		,y((float)_y)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(double _x, double _y)
		: x((float)_x)
		, y((float)_y)
	{}

	Vec2(const POINT& _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}
};