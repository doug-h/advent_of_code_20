#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;


int main()
{

	std::ifstream ifs("data.txt");
  	vector<int> content((istream_iterator<int>(ifs)), (istream_iterator<int>()));

	for (auto i = content.begin(); i!=content.end(); ++i)
		for (auto j = i+1; j<content.end(); ++j)
			if (*i!=*j)
				for (auto k = j+1; k<content.end(); ++k)
					if (*i!=*k && *j!=*k && (*i + *j + *k) == 2020)
						cout << (*i) * (*j) * (*k) << endl;

	while (true) {};

	return 0;
}
