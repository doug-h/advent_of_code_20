#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <array>
#include <numeric>


using Bound = std::pair<int, int>;
using Ticket = std::array<int, 20>;

struct Field {
	std::string name {""};
	Bound b1 {0, 0};
	Bound b2 {0, 0};
};

Field field_parse(const std::string& l)
{
	std::smatch match;
	std::regex r_bounds("(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
	std::regex_match(l, match, r_bounds);

	return {match[1], {stoi(match[2]), stoi(match[3])}, {stoi(match[4]), stoi(match[5])}};
}

bool check_ticket(Ticket& t, std::unordered_map<int, bool>& m)
{
	for (auto i : t)
		if (m[i] == 0)
			return false;
	return true;
}

bool check_field(int v, Field& f)
{
	if (v >= std::get<0>(f.b1) && v <= std::get<1>(f.b1))
		return true;
	if (v >= std::get<0>(f.b2) && v <= std::get<1>(f.b2))
		return true;
	return false;
}

Ticket ticket_parse(std::ifstream& ifs)
{
	Ticket t;
	for (int i = 0; i < 20; )
	{
		int value;
		ifs >> value;

		if (ifs.fail()) // line with strings on it we will dump
		{
			ifs.clear();
			std::string s;
			ifs >> s;
		}
		else 
		{
			t[i] = value;
			ifs.ignore(); // ignore ','
			++i;
		}
	}
	return t;
}

Field find_field(int i, std::vector<Field>& fs, std::vector<Ticket>& ts)
{
	std::vector<int> candidates (fs.size());
	std::iota(candidates.begin(), candidates.end(), 0);

	auto t = ts.begin();
	while (candidates.size() > 1 && t != ts.end())
	{
		std::vector<int>::iterator to_drop = candidates.end();
		for (auto j=candidates.begin(); j!=candidates.end(); ++j)
		{
			if (!check_field((*t)[i], fs[*j]))
			{
				to_drop = j;
				break;
			}
		}
		if (to_drop != candidates.end())
			candidates.erase(to_drop);
		else
			++t;
	}
	if (t == ts.end())
		// we will try again later
	Field result = fs[candidates[0]];
	std::cout << "found: " << result.name << '\n';

	fs.erase(fs.begin()+candidates[0]);
	return result;
}



int main()
{
	std::ifstream ifs("data.txt");

	std::vector<Field> fields;
	std::vector<Ticket> tickets;
	std::unordered_map<int, bool> in_any_field;

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line == "") break; // end of part one

		Field f = field_parse(line);
		fields.push_back(f);

		for (int i = std::get<0>(f.b1); i <= std::get<1>(f.b1); ++i)
		{
			in_any_field[i] = true;
		}
		for (int i = std::get<0>(f.b2); i <= std::get<1>(f.b2); ++i)
		{
			in_any_field[i] = true;
		}		
	}

	while (ifs.good())
	{
		Ticket t = ticket_parse(ifs);
		if (check_ticket(t, in_any_field)) { tickets.push_back(t); }
	}
	
	std::vector<Field> ordered_fields;
/*	for (int i = 0; i<20; ++i)
	{
		Field f = find_field(i, fields, tickets);
		ordered_fields.push_back(f);
	}
*/	
	std::vector<Field> candidates = fields;
	std::vector<int> columns(20);
	std::iota(columns.begin(), columns.end(), 0);

	bool all_found = false;
	while(!all_found)
	{
		all_found = true;
		for (auto i : columns)
		{
			std::vector<Field> column_candidates = candidates;
			bool all_fit = false;
			while (!all_fit)
			{
				all_fit = true;
				for (int j = 0; (size_t)j < column_candidates.size(); ++j)
				{
					if (!fits(i, column_candidates[j]))
					{
						all_fit = false;
						column_candidates.erase(column_candidates.begin()+j);
						break;
					}
				}
			}
			if (column_candidates.size() != 1)
			{
				all_found = false;
			}
			else
			{
				candidates.erase()
				break;
			}
		}
	}

	int answer = 1;
	for (int i = 0; (size_t)i < ordered_fields.size(); ++i)
	{
		std::cout << ordered_fields[i].name << " " << tickets[0][i] << '\n';
		if (ordered_fields[i].name.size() >= 9)
			if (ordered_fields[i].name.substr(0, 9) == "departure")
			{
				answer *= tickets[0][i];
				std::cout << ordered_fields[i].name << '\n';
			}
	}
	std::cout << answer << '\n';

	for (auto& t : tickets)
		for (int i = 0; (size_t)i < t.size(); ++i)
			if (!check_field(t[i], ordered_fields[i]))
				std::cout << "bad found: "<< t[i] << " " << ordered_fields[i].name << '\n';
	while (true) {};
	return 0;
}