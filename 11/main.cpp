#include <iostream>
#include <fstream>
#include <string>


int main()
{
	std::ifstream ifs("data.txt");

	int i = 0;
	while (ifs.good() && i < DATA_H)
	{
		ifs >> seats[i].data();
		++i;
	}

	while (step()) { print_seats(); }
	std::cout << seat_count();

	while (true) {};
	return 0;
}