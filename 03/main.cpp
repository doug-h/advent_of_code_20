#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <iterator>
#include <algorithm>

using namespace std;

int tree_count(vector<string>& lines, int stride_x, int stride_y = 1)
{
	int hits = 0;
	int stride = 0;

	for (auto p_line = lines.begin(); p_line < lines.end(); p_line += stride_y)
	{
		if ((*p_line)[stride] == '#')
			hits ++;

		stride += stride_x;
		stride %= 31;
	}

	return hits;
}

int main()
{
	ifstream ifs("data.txt");
	vector<string> lines;
	copy(istream_iterator<string>(ifs),
    	istream_iterator<string>(),
    	back_inserter(lines));

	

	cout << tree_count(lines, 1) * tree_count(lines, 3) * tree_count(lines, 5) * tree_count(lines, 7) * tree_count(lines, 1, 2);
	while (true) {};

	return 0;
}
