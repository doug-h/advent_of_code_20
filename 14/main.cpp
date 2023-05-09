#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <numeric>

using bits = std::bitset<36>;

void bifurcate_memory(int index, std::vector<bits>& memories)
{
	std::vector<bits> temp = memories;
	for (auto m : memories)
	{
		m[35-index] = ~m[35-index];
		temp.push_back(m);
	}
	memories = temp;
}

int main() {
	std::ifstream ifs("data.txt");
	std::map<unsigned long long int, long long int> memory;

	std::string mask;

	while (ifs.good()) {
		std::string command;
		std::getline(ifs, command, ' ');

		ifs.get();
		ifs.get();

		if (command[1] == 'a') {

			/*std::string mask_template;
			std::getline(ifs, mask_template, '\n');

			std::string xs;
			for (auto& c : mask_template) {
				if (c == 'X') {
					xs.push_back('1');
					c = '0';
				}
				else xs.push_back('0');
			}

			mask = std::bitset<36>(mask_template);
			wildcard = std::bitset<36>(xs);*/

			std::getline(ifs, mask, '\n');
		}

		else {
			int vl;
			ifs >> vl;
			ifs.get();

			bits address(stoi(std::string(std::next(command.begin(), 4), 
				 					 	  std::prev(command.end(),   1))));


			for (int i = 0; i < 36; ++i)
			{
				if (mask[i] == '1') 
				{ 
					address[35-i] = 1;
				}
			}

			std::vector<bits> addresses = {address};
			for (int i = 0; i < 36; ++i)
			{
				if (mask[i] == 'X') 
				{ 
					bifurcate_memory(i, addresses);
				}
			}
			for (auto& a : addresses)
			{
				memory[a.to_ullong()] = vl;
			}

		}
	}
	long long unsigned total = 0;
	for (auto& [k,v] : memory) { total += v; }
	std::cout << total << '\n';
	while (true) {};
	return 0;
}