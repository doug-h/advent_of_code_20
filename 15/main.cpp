#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <limits>

using Vector = std::vector<int>;


class Game {
public:
	Game(const Vector v) : tn{0}, start{v}, been_there{std::vector<bool>(std::numeric_limits<int>::max(), false)} {};
	int turn() { return tn; }

	void step()
	{
		tn += 1;
		prev_answer = what_to_say();
		
	}
	void operator++() { step(); }
	void print() { std::cout << (double)100*tn/30000000 << ": " << prev_answer << '\n';  }

private:
	int what_to_say()
	{
		int answer = 0;
		if (tn <= (int)start.size()) 
		{ 
			answer = start[tn-1];
			if (tn <  (int)start.size()) 
			{

				been_there[answer] = true;
				history[answer] = tn;
			}
		}
		else 
		{
			if (been_there[prev_answer])
			{ 
				answer = tn - history[prev_answer] - 1;
			}
			else been_there[prev_answer] = true;
			history[prev_answer] = tn-1;
			
		}
		return answer;
	}


	int tn;
	int prev_answer;
	std::unordered_map<int, int> history;
	const Vector start;
	std::vector<bool> been_there;
};

int main()
{
	const Vector input {6,3,15,13,1,0};
	
	Game game(input);
	while (game.turn() < 30000000) { ++ game; }
	game.print();
	while (true) {};
	return 0;
}