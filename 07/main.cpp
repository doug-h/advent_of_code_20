#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <unordered_map>

#include <regex>
#include <algorithm>


std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> bags;
std::unordered_map<std::string, bool> bag_cache;

std::regex r_find_name("(\\w+ \\w+) bags? contain.*");
std::regex r_find_sub_bags("(\\d) (\\w+ \\w+) bags?");

void load_bags()
{
	std::ifstream ifs("data.txt");


	std::string line;
	while(std::getline(ifs, line))
	{
		std::smatch match;
		std::string name;
		std::vector<std::pair<std::string, int>> sub_bags;

		if (std::regex_match(line, match, r_find_name))
		{
			if (match.size() > 1)
			{
				name = match[1].str();
			}
		}

		auto r_iter_begin = std::sregex_iterator(line.begin(), line.end(), r_find_sub_bags);
		auto r_iter_end   = std::sregex_iterator();
		for (std::sregex_iterator i = r_iter_begin; i != r_iter_end; ++i) 
		{
		    match = *i;
		    auto temp = std::make_pair(match[2].str(), stoi(match[1].str()));
		    sub_bags.push_back(temp);
		}
		bags[name] = sub_bags;

	}
}

bool gold_in(const std::string& bag)
{
	if (bag_cache.count(bag) != 0)
	{
		return bag_cache[bag];
	}
	else
	{
		for (const auto& [b,_] : bags[bag])
		{
			if (b=="no other")
			{
				return false;
			}
			else if (b == "shiny gold")
			{
				bag_cache[bag] = true;
				return true;
			}
			else if (gold_in(b))
			{
				bag_cache[bag] = true;
				return true;
			}
		}
	}
	bag_cache[bag] = false;
	return false;
}

int count_sub_bags(const std::string& bag)
{
	int sub_bags = 0;
	for (const auto& [b,n] : bags[bag])
	{
		sub_bags += n;
		sub_bags += n*count_sub_bags(b);
	}
	return sub_bags;
}

int main()
{
	load_bags();

	int hits = 0;
	for (const auto& [bag,_] : bags)
		if (gold_in(bag)) hits += 1;

	const std::string s = "shiny gold";
	std::cout << hits << " " << count_sub_bags(s);
	while (true) {};
	return 0;
}
