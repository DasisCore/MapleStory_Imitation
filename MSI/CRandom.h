#pragma once

#include <random>

class CRandom
{
	SINGLE(CRandom);

public:
	// -1 과 1 중 랜덤값 얻기
	int GetRandomOne()
	{
		// 시드 설정
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1부터 30 사이의 정수를 생성하는 분포 생성
		std::uniform_int_distribution<int> distribution(-1, 1);

		// 난수 생성
		int random_number = distribution(generator);
		
		while (random_number == 0)
		{
			random_number = distribution(generator);
		}
		
		return random_number;
	}

	// 사이의 랜덤 값 얻어오기
	template <class T>
	T GetBetweenReal(T from, T to)
	{
		// 시드를 현재 시간으로 설정
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1부터 30 사이의 정수를 생성하는 분포 생성
		std::uniform_real_distribution<T> distribution(from, to);

		// 난수 생성
		T random_number = distribution(generator);

		return random_number;
	}

	template <class T>
	T GetBetweenInt(T from, T to)
	{
		// 시드를 현재 시간으로 설정
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1부터 30 사이의 정수를 생성하는 분포 생성
		std::uniform_int_distribution<T> distribution(from, to);

		// 난수 생성
		T random_number = distribution(generator);

		return random_number;
	}

	bool GetRandomBool()
	{
		// 시드를 현재 시간으로 설정
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1부터 30 사이의 정수를 생성하는 분포 생성
		std::uniform_int_distribution<int> distribution(0, 1);

		// 난수 생성
		int random_number = distribution(generator);

		return !!random_number;
	}
};

