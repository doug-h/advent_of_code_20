#include <iostream>
#include <fstream>

#include <string>
#include <vector>

std::vector<std::pair<std::string, int>> instructions;

bool run_instructions()
{
	int accum = 0;
	std::vector<bool> visited (instructions.size(), false);
	size_t i = 0;
	while (i < instructions.size())
	{
		if (visited[i] == true) return false;
		visited[i] = true;

		const auto& [instruction, value] = instructions[i]; // structured bindings
		if      (instruction == "acc") { accum += value; i++; }
		else if (instruction == "jmp") { i += value; }
		else if (instruction == "nop") { i++; }
	}
	std::cout << accum << '\n';
	return true;
}

int main()
{
	std::ifstream ifs("data.txt");
	while (ifs.good())
	{
		std::string instruction;
		std::string value;
		ifs >> instruction >> value;

		instructions.push_back(std::make_pair(instruction, stoi(value)));
	}
	
	for (auto& [i,_] : instructions)
	{
		if (i != "acc")
		{
			auto swap = [](std::string& s){ return (s=="jmp") ? "nop" : "jmp"; };
			i = swap(i);
			if (run_instructions())
			{
				std::cout << "found it: " << line_test;
				break;
			}
			else
			{
				i = swap(i);
			}

		}
	}
	while (true) {};
	return 0;
}
