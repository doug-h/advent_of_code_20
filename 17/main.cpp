#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cassert>

struct Point {
	int x;
	int y;
	int z;
	int w;

	bool operator==(const Point& other) { return (x==other.x && y==other.y && z==other.z && w==other.w); }
};

template<typename T>
class RVector {
public:
	RVector() : _v{}, i_origin{0} {}
	RVector(const int lb, const int ub) : _v(1+ub-lb), i_origin{-lb} {}
	RVector(const int lb, const int ub, T value) : _v(1+ub-lb, value), i_origin{-lb} {}

	T& operator[] (int i) { return _v[i+i_origin]; }
	size_t size() { return _v.size(); }

	void push_back(const T& value) { _v.push_back(value); }
	void push_front(const T& value) { _v.insert(_v.begin(), value); ++i_origin; }
	typename std::vector<T>::iterator begin() { return _v.begin(); }
	typename std::vector<T>::iterator end() { return _v.end(); }

private:
	std::vector<T> _v;
	int i_origin; // what element holds the origin ( 0-point )
};


template<typename T>
class Hypergrid {
public:
	Hypergrid() : elem{} {}
	Hypergrid(const int x_begin, const int x_end,
			  const int y_begin, const int y_end, 
			  const int z_begin, const int z_end,
			  const int w_begin, const int w_end) :
	corner_LFT({x_begin, y_begin, z_begin, w_begin}),
	corner_RBB({x_end,	 y_end,	  z_end,   w_end}),

	elem(w_begin, w_end, RVector<RVector<RVector<T>>>(
		 z_begin, z_end, RVector<RVector<T>>(
		 y_begin, y_end, RVector<T>(
		 x_begin, x_end))))
	{}

	T& operator[] (Point p) { return elem[p.w][p.z][p.y][p.x]; }
	auto begin() { return elem.begin(); }
	auto end() 	 { return elem.end(); }

	void pad()
	{
		Point _LFT = corner_LFT;
		Point _RBB = corner_RBB;
		for (int x = corner_LFT.x; x <= corner_RBB.x; ++x)
			for (int y = corner_LFT.y; y <= corner_RBB.y; ++y)
				for (int z = corner_LFT.z; z <= corner_RBB.z; ++z)
					for (int w = corner_LFT.w; w <= corner_RBB.w; ++w)
						if (elem[w][z][y][x] == 1)
						{
							if (x == _LFT.x) 
							{
								_LFT.x = corner_LFT.x-1;
								for (auto& hyperplane : elem)
									for (auto& plane : hyperplane)
										for (auto& row : plane)
											row.push_front(0);
							}
							if (x == _RBB.x)
							{
								_RBB.x = corner_RBB.x+1;
								for (auto& hyperplane : elem)
									for (auto& plane : hyperplane)
										for (auto& row : plane)
											row.push_back(0);
							}
							if (y == _LFT.y)
							{
								_LFT.y = corner_LFT.y-1;
								for (auto& hyperplane : elem)
									for (auto& plane : hyperplane)
										plane.push_front(RVector<int>(_LFT.x, _RBB.x, 0));
							}
							if (y == _RBB.y)
							{
								_RBB.y = corner_RBB.y+1;
								for (auto& hyperplane : elem)
									for (auto& plane : hyperplane)
										plane.push_back(RVector<int>(_LFT.x, _RBB.x, 0));
							}
							if (z == _LFT.z)
							{
								_LFT.z = corner_LFT.z-1;
								for (auto& hyperplane : elem)
									hyperplane.push_front(RVector<RVector<int>>(_LFT.y, _RBB.y, 
														  RVector<int>(_LFT.x, _RBB.x, 0)));
							}
							if (z == _RBB.z)
							{
								_RBB.z = corner_RBB.z+1;
								for (auto& hyperplane : elem)
									hyperplane.push_back(RVector<RVector<int>>(_LFT.y, _RBB.y, 
											   			 RVector<int>(_LFT.x, _RBB.x, 0)));
							}
							if (w == _LFT.w)
							{
								_LFT.w = corner_LFT.w-1;
								elem.push_front(RVector<RVector<RVector<int>>>(_LFT.z, _RBB.z,
												RVector<RVector<int>>(_LFT.y, _RBB.y, 
												RVector<int>(_LFT.x, _RBB.x, 0))));
							}
							if (w == _RBB.w)
							{
								_RBB.w = corner_RBB.w+1;
								elem.push_back(RVector<RVector<RVector<int>>>(_LFT.z, _RBB.z,
											   RVector<RVector<int>>(_LFT.y, _RBB.y, 
											   RVector<int>(_LFT.x, _RBB.x, 0))));
							}
						}
		corner_LFT = _LFT;
		corner_RBB = _RBB;
	}

	bool within(Point p, Point b_1, Point b_2)
	{
		return (b_1.x <= p.x && p.x <= b_2.x
			 && b_1.y <= p.y && p.y <= b_2.y
			 && b_1.z <= p.z && p.z <= b_2.z
			 && b_1.w <= p.w && p.w <= b_2.w);
	}
	bool within(Point p) { return within(p, corner_LFT, corner_RBB); }

	int count_neighbours(Point p)
	{
		int count = 0;
		for (int i : {-1,0,1})
			for (int j : {-1,0,1})
				for (int k : {-1,0,1})
					for (int l : {-1,0,1})
						if (i!=0 || j!=0 || k!=0 || l!=0)
							if (within({p.x+i,p.y+j,p.z+k, p.w+l}))
								if (elem[p.w+l][p.z+k][p.y+j][p.x+i] == 1) 
									++count;
		return count;
	}

	void print(int z, int w)
	{
		for (int x = corner_LFT.x; x <= corner_RBB.x; ++x)
			std::cout << '\t' << x;
		for (int y = corner_LFT.y; y <= corner_RBB.y; ++y)
		{
			std::cout << '\n' << y;
			for (int x = corner_LFT.x; x <= corner_RBB.x; ++x)
			{
				std::cout << '\t';
				if (elem[w][z][y][x]) { std::cout << '#'; }
				else { std::cout << '.'; }
			}
		}
		std::cout << '\n';
	}

	void check()
	{
		assert(1+corner_RBB.w-corner_LFT.w == (int)elem.size());
		for (auto& hyperplane : elem)
		{
			assert(1+corner_RBB.z -corner_LFT.z == (int)hyperplane.size());
			for (auto& plane : hyperplane)
			{
				assert(1+corner_RBB.y -corner_LFT.y == (int)plane.size());
				for (auto& row : plane)
					assert(1+corner_RBB.x -corner_LFT.x == (int)row.size());
			}
		}
	}

	void update_cell(Point p, int n_count)
	{
		if ((*this)[p]==1 && n_count!=2 && n_count!=3)
			(*this)[p]=0;
		else if ((*this)[p]==0 && n_count == 3)
			(*this)[p]=1;
	}

	void update()
	{
		Hypergrid<int> grid2 = (*this);
		for (int x = corner_LFT.x; x <= corner_RBB.x; ++x)
			for (int y = corner_LFT.y; y <= corner_RBB.y; ++y)
				for (int z = corner_LFT.z; z <= corner_RBB.z; ++z)
					for (int w = corner_LFT.w; w <= corner_RBB.w; ++w) 
					{
						update_cell({x,y,z,w}, grid2.count_neighbours({x,y,z,w}));
					}
	}

	int count()
	{
		int count = 0;
		for (int x = corner_LFT.x; x <= corner_RBB.x; ++x)
			for (int y = corner_LFT.y; y <= corner_RBB.y; ++y)
				for (int z = corner_LFT.z; z <= corner_RBB.z; ++z)
					for (int w = corner_LFT.w; w <= corner_RBB.w; ++w) 
						if (elem[w][z][y][x]) ++count;
		return count;
	}

	Point corner_LFT; //left-front-top
	Point corner_RBB; //right-back-bottom

private:
	RVector<RVector<RVector<RVector<T>>>> elem;	
};


int main()
{	
	Point origin = {0, 0, 0, 0};
	//std::vector<std::string> input {".#.", "..#", "###"};
	std::vector<std::string> input {"##...#.#", 
									"#..##..#", 
									"..#.####", 
									".#..#...", 
									"########", 
									"######.#", 
									".####..#",
									".###.#.."};



	Hypergrid<int> grid(origin.x, origin.x+input[0].size(),
				   origin.y, origin.y+input.size(),
				   origin.z, origin.z,
				   origin.w, origin.w);

	for (int x = 0; x < (int)input[0].size(); ++x)
		for (int y = 0; y < (int)input.size(); ++y)
			grid[{x+origin.x, y+origin.y, 0, 0}] = (input[y][x]=='#');

	grid.pad();
	grid.check();

	for(int i = 6; i; --i)
	{
		grid.update();
		grid.pad();
		grid.check();
		grid.print(0,0);
	}
	std::cout << grid.count() << '\n';



	while (true) {};
}