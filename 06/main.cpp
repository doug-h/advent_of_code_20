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
#include <unordered_set>
#include <cmath>
#include <locale>

using namespace std;

int main()
{
	ifstream ifs("data.txt");
	int total = 0;

	bool new_group = true;
	string prev_answer = "";
	string answer;
	while (getline(ifs, answer))
	{
		cout << "start: " << prev_answer  << " " << answer << endl;
		if (answer == "")
		{
			//new group- get ready
			new_group = true;
			total += prev_answer.size();
			
			prev_answer = "";
		}
		else
		{
			if (new_group)
			{
				//first guy in group
				prev_answer = answer;
				new_group = false;
			}
			else
			{
				//prev_answer -= answer;
				string temp = prev_answer;
				prev_answer = "";
				for (auto c : temp)
					if (answer.find(c)!=string::npos)
						prev_answer += c;
			}
		}
	}

	cout << total;
	while (true) {};
}
