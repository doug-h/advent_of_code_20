#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <iterator>
#include <algorithm>

using namespace std;



int main()
{
	ifstream ifs("data.txt");
	vector<string> words;
	copy(istream_iterator<string>(ifs),
    	istream_iterator<string>(),
    	back_inserter(words));

	int l_bound;
	int u_bound;
	string bound;

	char target;

	//int no_of_target;
	bool success;

	int hits = 0;

	for (auto p_word=words.begin(); p_word<words.end(); p_word+=3)
	{
		istringstream iss(*p_word);
		getline(iss, bound, '-');
		l_bound = stoi(bound);
		getline(iss, bound, '-');
		u_bound = stoi(bound);

		target = (*(p_word+1))[0];

		/*no_of_target = count((p_word+2)->begin(), (p_word+2)->end(), target);
		if (no_of_target >= l_bound && no_of_target <= u_bound)
			hits++;
			*/
		success = false;
		if ((*(p_word+2))[l_bound-1] == target)
			success = !success;
		if ((*(p_word+2))[u_bound-1] == target)
			success = !success;

		if (success)
		{
			cout << *(p_word+2) << endl;
			hits ++;
		}

	}

	cout << hits;
	while (true) {};

	return 0;
}
