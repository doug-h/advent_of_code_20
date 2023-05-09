#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>


template<typename T>
std::vector<T> get_input(const char* path) {
	std::ifstream ifs(path);
	std::vector<T> as;

	while(ifs.good()) {
		T a;
		ifs >> a;
		as.push_back(a);
	}
	return as;
}



int main()
{
	std::vector<int> adapters = get_input<int>("data.txt");
	adapters.push_back(0);
	std::sort(adapters.begin(), adapters.end());


	std::vector<int> differences;
	//std::vector<int> differences = { adapters[0] };
	for (std::vector<int>::iterator i = adapters.begin();; )
	{
		int d = - *i;
		++i;
		if (i!=adapters.end()) {
			d += *i;
			differences.push_back(d);
		}
		else {
			differences.push_back(3);
			break;
		}
	}
/*
	std::vector<int> differences = { 0, 0, 0 };
	for (std::vector<int>::iterator i = adapters.begin();; )
	{
		int d = - *i;
		++i;
		if (i!=adapters.end()) {
			d += *i;
			++differences[d-1];
		}
		else {
			++differences[3-1];
			break;
		}
	}*/

	//There are no 2-gaps, 3-gaps are non-negotiable and 1-gaps of length n have fixed options
	// equal to the (n+2)'th number of the tribonacci sequence {0, 0, 1, 1, 2, 4, 13}
	// e.g. 44, 45, 46 == 1,1,1 == 3x 1-gap == trib(3+2) == 4.
	int trib[5] = {1, 2, 4, 7, 13};



	{
		int counter = 0;
		long long perms = 1;
		for (auto d : differences) {
			if (d == 1) { 
				++counter;
			}
			else if (d == 3 && counter > 0) {
				perms *= trib[counter-1];
				counter = 0;
			}
			std::cout << d << " " << perms << '\n';
		}
	}
	for (std::vector<int>::iterator i = adapters.begin(); i != adapters.end(); ++i)
	{
		std::cout << *i << '\n';	
	}
	while (true) {};
	return 0;
}