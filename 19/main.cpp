#include <iostream>
#include <array>
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

std::vector<std::string> string_to_rule(std::string rule, const std::array<std::string, 137>& rules, std::array<std::vector<std::string>, 137>& cache)
{
	std::vector<std::string> results;
	int n = rule.find_first_of("|");
	if(n!=std::string::npos)
	{
		results = string_to_rule(rule.substr(0,n), rules, cache);

		std::vector<std::string> temp = string_to_rule(rule.substr(n+1), rules, cache);
		results.insert(results.end(), temp.begin(), temp.end());
	}
	else if(rule[0]=='"')
	{
		results = {""};
		results[0] += rule[1];
	}
	else
	{
		// we always expect iss >> r1 to succeed, but r2 may fail
		int r1, r2;
		std::istringstream iss{rule};
		iss >> r1;

		auto i = iss.tellg();
		iss >> r2;
		if(iss.fail())
		{
			if(cache[r1].size())
			{
				return cache[r1];
			}
			else
			{
				cache[r1] = string_to_rule(rules[r1], rules, cache);
				return cache[r1];
			}
		}
		else
		{
			if(!(cache[r1].size())) 
			{
				cache[r1] = string_to_rule(rules[r1], rules, cache);
			}
			if(!(cache[r2].size())) 
			{
				cache[r2] = string_to_rule(rules[r2], rules, cache);
			}
			for(auto s1 : cache[r1])
			{
				for(auto s2 : cache[r2])
				{
					results.push_back(s1+s2);
				}
			}
		}
	}
	return results;
}

void input_input(const char* path, std::array<std::string, 137>& rules, std::vector<std::string>& messages)
{
	std::ifstream ifs(path);
	std::string line;

	while(std::getline(ifs, line) && line != "")
	{
		std::istringstream iss {line};
		int index;
		std::string rule;
		iss >> index;
		iss.ignore(1, ':');
		iss.ignore(1, ' ');
		std::getline(iss, rule);

		//std::cout << "Rule: " <<  rule << '\n';
		rules[index] = rule;
	}

	while(std::getline(ifs, line) && line != "")
	{
		messages.push_back(line);
	}

}

std::vector<std::string> prepend_string(std::vector<std::string> input, std::string pre)
{
	for (auto& s : input)
	{
		s.reserve(s.size()+pre.size());
		s.insert(s.end(), pre.begin(), pre.end());
	}
	return input;
}

int main(int argc, char const *argv[])
{
	std::array<std::string, 137> rules;
	std::vector<std::string> messages;
	std::array<std::vector<std::string>, 137> cache;
	input_input("data.txt", rules, messages);
	
	std::vector<std::string> out = string_to_rule(rules[0], rules, cache);
	//turns out that rules been constructed so every message is 24 characters long

	std::map<unsigned long, bool> valid_values;

	for (auto s : out)
	{
		std::bitset<24> b(s, 0, 24, 'a', 'b');
		valid_values[b.to_ulong()] = true;
	}
	
	int count = 0;
	for (auto m : messages)
	{
		if(m.size()!=24)
		{
			continue;
		}
		std::bitset<24> b(m, 0, 24, 'a', 'b');
		if(valid_values[b.to_ulong()])
		{
			++count;
		}
	}

	std::cout << count;


	return 0;
}
