#pragma once

#include <random>

class CRandom
{
	SINGLE(CRandom);

public:
	// -1 �� 1 �� ������ ���
	int GetRandomOne()
	{
		// �õ� ����
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1���� 30 ������ ������ �����ϴ� ���� ����
		std::uniform_int_distribution<int> distribution(-1, 1);

		// ���� ����
		int random_number = distribution(generator);
		
		while (random_number == 0)
		{
			random_number = distribution(generator);
		}
		
		return random_number;
	}

	// ������ ���� �� ������
	template <class T>
	T GetBetweenReal(T from, T to)
	{
		// �õ带 ���� �ð����� ����
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1���� 30 ������ ������ �����ϴ� ���� ����
		std::uniform_real_distribution<T> distribution(from, to);

		// ���� ����
		T random_number = distribution(generator);

		return random_number;
	}

	template <class T>
	T GetBetweenInt(T from, T to)
	{
		// �õ带 ���� �ð����� ����
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1���� 30 ������ ������ �����ϴ� ���� ����
		std::uniform_int_distribution<T> distribution(from, to);

		// ���� ����
		T random_number = distribution(generator);

		return random_number;
	}

	bool GetRandomBool()
	{
		// �õ带 ���� �ð����� ����
		std::random_device rd;
		std::default_random_engine generator(rd());

		// 1���� 30 ������ ������ �����ϴ� ���� ����
		std::uniform_int_distribution<int> distribution(0, 1);

		// ���� ����
		int random_number = distribution(generator);

		return !!random_number;
	}

	// a ~ b�� �������� c ������ŭ ���� �� ��������
	vector<int> GetBetweenIntCnt(int from, int to, int cnt)
	{
		// �õ带 ���� �ð����� ����
		std::random_device rd;
		std::default_random_engine generator(rd());

		vector<int> vec;
		while (vec.size() != cnt)
		{
			// 1���� 30 ������ ������ �����ϴ� ���� ����
			std::uniform_int_distribution<int> distribution(from, to);

			// ���� ����
			int random_number = distribution(generator);

			bool flag = false;
			for (int i = 0; i < vec.size(); i++)
			{
				if (vec[i] == random_number) flag = true;
			}

			// vector ���� random_number�� ���� ���� �ִٸ� �ٽ� �õ�
			if (flag == true) continue;
			else vec.push_back(random_number);
		}

		return vec;
	}
};

