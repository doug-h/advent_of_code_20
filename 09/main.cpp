#include <iostream>
#include <fstream>

#include <algorithm>

#include <string>
#include <deque>

std::deque<long long int> prev_numbers;
std::deque<long long int> all_numbers;

bool is_pair(long long int n)
{
	for (auto i = prev_numbers.begin(); i != prev_numbers.end(); i++)
	{
		for (auto j = std::next(i); j != prev_numbers.end(); j++)
		{
			if (n == *i + *j) { return true; }
		}
	}
	return false;
}


void seesaw()
{
	long long int target = 41682220;
	int start = 0;
	int range = 0;
	int total = 0;

	while (total != target)
	{
		if (total < target)
		{
			total += all_numbers[start+range];
			range++;
		}
		if (total > target)
		{
			total -= all_numbers[start];
			start++;
			range--;
		}
	}
	std::cout << "(" << all_numbers[start] << ", " << all_numbers[start+range-1] << ")" << '\n';

	long long int max = *std::max_element(std::next(all_numbers.begin(), start), std::next(all_numbers.begin(), start+range));
	long long int min = *std::min_element(std::next(all_numbers.begin(), start), std::next(all_numbers.begin(), start+range));
	std::cout << max + min; 
}

int main()
{
	std::ifstream ifs("data.txt");
	size_t preamble = 25;
	while (ifs.good() && prev_numbers.size() <= preamble)
	{
		long long int number;
		ifs >> number;
		std::cout << number << '\n';

		prev_numbers.push_back(number);
		all_numbers.push_back(number);
	}



	while(ifs.good())
	{
		long long int number;
		ifs >> number;
		std::cout << number << '\n';

		if (!is_pair(number)) { std::cout << "broken"; break; }

		prev_numbers.push_back(number);
		prev_numbers.pop_front();

		all_numbers.push_back(number);
	}


	seesaw();

	while (true) {};
	return 0;
}
