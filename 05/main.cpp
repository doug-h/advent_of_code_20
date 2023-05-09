#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <iterator>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <set>
#include <cmath>
#include <locale>

using namespace std;



bool find_seat(char c, int& lower, int& upper, int& lefter, int& righter)
{
	switch (c)
	{
		case 'F' : upper -= (upper - lower + 1)/2; break;
		case 'B' : lower += (upper - lower + 1)/2; break;
		case 'L' : righter -= (righter - lefter + 1)/2; break;
		case 'R' : lefter  += (righter - lefter + 1)/2; break;
	}
	return (upper == lower && lefter == righter);
}

int read_code(string& s)
{
	if (s=="") return 0;

	int up = 127;
	int lo = 0;
	int le = 0;
	int ri = 7;

	int i = 0;
	char c;

	do { c = s[i]; i++; } 
	while (!find_seat(c, lo, up, le, ri));

	return (up*8 + le);
}

int main()
{
	int score;
	set<int> scores;
	int total = 0;
	ifstream ifs("data.txt");

	string code;
	while (ifs.good())
	{
		code = "";
		ifs >> code;
		if (code != "")
		{ 
			score = read_code(code); 
			total += score;
			scores.insert(score);
		}
	}
	int n = scores.size() + 1;
	int calc_total = (n*(n-1))/2 + n*(*(scores.begin()));
	cout << calc_total - total;

	while (true) {};
}
