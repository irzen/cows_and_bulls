#include "cows_bulls.h"
#include <stdio.h>
#include <exception>
#include <random>
#include <ctime>

template<int N>
int& in<N>::operator[](int _n)
{
	return val[_n];
};

template<int N>
int in<N>::operator[](int _n) const
{
	return val[_n];
};

i4 generateTask()
{
	// i saw how people just randomize every digit and if there is dublicates - drop the results and try again
	// i prefer that way: take a random number from the box and be calm no dublicates will occure
	i4 result;
	srand(time(0));
	int ar[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	int n = 10;
	for (int i = 0; i < 4; ++i)
	{
		const int rnd = rand() % n;
		--n;
		result[i] = ar[rnd];
		ar[rnd] = ar[n];
	}

	return result;
};

i2 getFeedback(const i4 request, const i4 task)
{
	i2 result;
	result[0] = 0;
	result[1] = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (request[i] == task[i])
		{
			++result[0];
			continue;
		}

		for (int j = 0; j < 4; ++j)
		{
			if (i != j && request[i] == task[j])
			{
				++result[1];
				break;
			}
		};
	};
	return result;
};

bool validate(i4 value)
{
	for (int i = 0; i < 4; ++i)
		for (int j = i + 1; j < 4; ++j)
			if (value[i] == value[j])
				return false;
	return true;
};