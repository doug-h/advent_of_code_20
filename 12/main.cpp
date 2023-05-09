#include <iostream>
#include <fstream>
#include <string>

//char bearings[4] = {'N', 'E', 'S', 'W'};

struct Ship
{
	int x{ 0 };
	int y{ 0 };
	int wpt_x { 10 }; //relative to ship
	int wpt_y {  1 };
	//unsigned bear_index { 1 };
};

Ship ship;

void rotate(unsigned d)
{
	int old_x = ship.wpt_x;
	int old_y = ship.wpt_y;
	unsigned b = (d/90) % 4;

	switch(b)
	{
		case 1 : { ship.wpt_x = old_y;  ship.wpt_y = -old_x; break; }
		case 2 : { ship.wpt_x = -old_x; ship.wpt_y = -old_y; break; }
		case 3 : { ship.wpt_x = -old_y; ship.wpt_y =  old_x; break; }
	}
}

void move_ship(char b, unsigned m)
{
	//if (b == 'F') { b = bearings[ship.bear_index]; }

	switch (b)
	{
		case 'N' : { ship.wpt_y += m; break; }
		case 'E' : { ship.wpt_x += m; break; }
		case 'S' : { ship.wpt_y -= m; break; }
		case 'W' : { ship.wpt_x -= m; break; }

		case 'L' : 
		{
			rotate(360-m);
			//ship.bear_index -= (m/90);
			//ship.bear_index %= 4;
			break; 
		}
		case 'R' :
		{
			rotate(m);
			//ship.bear_index += (m/90);
			//ship.bear_index %= 4;
			break; 
		}
		case 'F' : { ship.x += (m*ship.wpt_x); ship.y += (m*ship.wpt_y); break; }
	}
}



int main()
{
	std::ifstream ifs("data.txt");

	char bearing;
	unsigned magnitude;
	while (ifs.good())
	{
		ifs >> bearing >> magnitude;
		std::cout << "(" << ship.x << "," << ship.y << ") --- " << "===========" << "(" << bearing << "," << magnitude << ")\n";
		move_ship(bearing, magnitude);
	}
	std::cout << abs(ship.x) + abs(ship.y);
	while (true) {};
	return 0;
}