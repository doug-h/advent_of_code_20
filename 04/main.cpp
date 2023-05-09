#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <locale>

using namespace std;

unordered_map<string, bool (*)(string)> field_validate;


int main()
{
	ifstream ifs("data.txt");
	char head;
	string entry;
	vector<string> passport;
	vector<vector<string>> passports;

	while (ifs.good())
	{
		ifs >> noskipws >> head;
		if (head == ' ' || head == '\n')
		{
			if (entry == "")
			{
				passports.push_back(passport);
				passport = {};
			}
			else
			{
				passport.push_back(entry);
				entry = "";
			}
		}
		else
			entry += head;
	}

	field_validate["byr"] = [](string s) { return (stoi(s) >= 1920 && stoi(s) <= 2002); };
	field_validate["iyr"] = [](string s) { return (stoi(s) >= 2010 && stoi(s) <= 2020); };
	field_validate["eyr"] = [](string s) { return (stoi(s) >= 2020 && stoi(s) <= 2030); };

	field_validate["hgt"] = [](string s) 
	{	
		string unit = s.substr(s.size()-2);
		string s_value = s.substr(0,s.size()-2);
		if (s_value == "") { return false; }
		else
		{
			int value = stoi(s_value);
			if 		(unit == "cm") { return (value >= 150 && value <= 193); }
			else if (unit == "in") { return (value >=  59 && value <=  76); }
			else 				   { return false; }
		}
	};

	field_validate["hcl"] = [](string s)
	{
		if (s[0] == '#' 
			&& s.size()==7 
			&& all_of(s.begin()+1, s.end(), 
				[](int i){ return (i <= 58 && i>= 47) || (i <= 102 && i>= 97); }))
		{ return true; }
		else 
		{ return false; }
	};
	field_validate["ecl"] = [](string s) 
	{ 
		unordered_set<string> eye_colours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
		return (eye_colours.count(s) == 1); 
	};

	field_validate["pid"] = [](string s) { return (s.size() == 9 && all_of(s.begin(), s.end(), [](int i){ return isdigit(i); })); };
	field_validate["cid"] = [](string s) { return (true); };


	bool valid;
	int hits = 0;
	string field;
	string value;
	for (auto& p : passports)
	{
		valid = true;

		// first we check all required fields are present
		switch (p.size())
		{
			case 8 : break;
			case 7 : 
			{
				for (auto& e : p)
				{
					if (e[0] == 'c' && e[1] == 'i' && e[2] == 'd')
					{
						valid = false;
						break;
					}
				}
				break;
			}
			default : valid = false;
		}

		// now we validate the values of each field
		if (valid && all_of(p.begin(), p.end(), 
			[](string s) { return field_validate[s.substr(0,3)](s.substr(4)); }))
			{ hits += 1; }

	}

	cout << hits;
	while (true) {};
}
