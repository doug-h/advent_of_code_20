#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include <tuple>

int find_diff(int target, int period) {
	int d = target % period;
	if (d != 0) {
		return period - d;
	}
	return 0;
}

int main() {
	std::ifstream ifs("data.txt");

	int estimate;
	ifs >> estimate; 

	std::vector<int> bus_times;

	for (;;) {
		if (ifs.peek() == 'x') { bus_times.push_back(-1); }

		int time;
		ifs >> time;

		if (ifs.bad()) { break; }
		else if (ifs.fail()) {
            ifs.clear(); // unset failbit
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), ','); // skip bad input
        } else {
            bus_times.push_back(time);
        }
        if (ifs.eof()) { break; }
    }

    std::vector<int>::iterator longest_bus = std::max_element(bus_times.begin(), bus_times.end());
    int longest_pos = std::distance(bus_times.begin(), longest_bus);

    std::vector<std::tuple<int, int>> bus_data;

    // limit scope of i since no initialise statement till C++20
    { 
    	int i = 0; for (int b : bus_times) {
	    	if (b != -1) { 
	    		bus_data.push_back(std::make_tuple(b, i-longest_pos));
	    	}
		    ++i;
	    }
	}
	std::sort(bus_data.begin(), bus_data.end(), [](auto b1, auto b2){ return (std::get<0>(b1) > std::get<0>(b2)); });

	for (long long int i = 129366106080;; ++i) {
		long long int n = i * *longest_bus;
		if (std::all_of(bus_data.begin(), bus_data.end(), [n](auto b){ auto [time, offset] = b; return (n+offset)%time == 0; })) {
			break;
		}
		if (n%1000000==0) std::cout << n << '\n';
	} 
	for (auto [time, offset] : bus_data)
	{
		std::cout << offset << " " << time <<  '\n';
	}
	// long n = 1068785;
	// std::vector<bool> results;
	// std::transform(bus_data.begin(), bus_data.end(), std::back_inserter(results), [n](auto b){ auto [time, offset] = b; std::cout << (n+offset)%time << '\n'; return time == -1 || (n+offset)%time == 0; });

	// for (std::vector<bool>::iterator i = results.begin(); i != results.end(); ++i){
	// 	std::cout << *i << '\n';	
	// }	

	while (true) {};
	return 0;
}