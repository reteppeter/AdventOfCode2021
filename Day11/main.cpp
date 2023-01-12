#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

#include <queue>

using Q = Queue<RTP::Maths::Numerics::Vector2<size_t>>;

//Regular Input:
//Task 1: 1739
//Task 2: 324
//Alternative Input:
//Task 1: 1717
//Task 2: 476
//Small Input:
//Task 1: 1656
//Task 2: 195

inline void incN(Array2D<int>& arr, const RTP::Maths::Numerics::Vector2<size_t> pos){
	for(size_t yy = max(pos.y, 1ull) - 1ull, yMax = min(pos.y + 2ull, arr.height()); yy < yMax; ++yy){
		for(size_t xx = max(pos.x, 1ull) - 1ull, xMax = min(pos.x + 2ull, arr.width()); xx < xMax; ++xx){
			if(xx == pos.x && yy == pos.y){ continue; }
			++arr[{xx, yy}];
		}
	}
};

inline void addN(Array2D<int>& arr, Q& next, const RTP::Maths::Numerics::Vector2<size_t> pos){
	for(size_t yy = max(pos.y, 1ull) - 1ull, yMax = min(pos.y + 2ull, arr.height()); yy < yMax; ++yy){
		for(size_t xx = max(pos.x, 1ull) - 1ull, xMax = min(pos.x + 2ull, arr.width()); xx < xMax; ++xx){
			if(xx == pos.x && yy == pos.y){ continue; }
			next.push({xx, yy});
		}
	}
};

bool checkFor9(Array2D<int>& arr, size_t& flashes){
	bool done = true;
	for(size_t y = 0; y < arr.height(); ++y){
		for(size_t x = 0; x < arr.width(); ++x){
			auto cur = arr[{x, y}];
			if(cur >= 10){
				done = false;
				cur = -99;
				++flashes;
				incN(arr, {x, y});
			}
			arr[{x, y}] = cur;
		}
	}
	return !done;
};

inline void solve(Array2D<int>& arr, size_t& flashes){
	for(size_t y = 0; y < arr.height(); ++y){
		for(size_t x = 0; x < arr.width(); ++x){
			arr[{x, y}] = max(arr[{x, y}], 0) + 1;
		}
	}

	while(checkFor9(arr, flashes));
};

inline void solve2(Array2D<int>& arr, size_t& flashes){
	Q next{};

	for(size_t y = 0; y < arr.height(); ++y){
		for(size_t x = 0; x < arr.width(); ++x){
			auto cur = max(arr[{x, y}], 0) + 1;
			if(cur >= 10){
				++flashes;
				cur = -99;
				addN(arr, next, {x, y});
			}
			arr[{x, y}] = cur;
		}
	}

	while(!next.empty()){
		auto f = next.front();
		next.pop();
		arr[f] += 1;
		if(arr[f] >= 10){
			++flashes;
			arr[f] = -99;
			addN(arr, next, f);
		}
	}
};

/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
*/
int main(int argc, char** argv){
	auto infile = AoC::init(argc, argv);

	//Line buffer
	string s{30};
	int task1 = 0;
	int task2 = 0;

	//Task start

	Array2D<int> oct{};
	size_t width = 0;

	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			ccp c = s.c_str();
			ccp back = (&s.back() + 1);
			width = s.size();
			while(c < back){
				oct.push_back((*c) - '0');
				++c;
			}
		}
	} while(infile);

	infile.close();

	oct.setWidth(width);

	size_t flashes = 0;
	size_t comp = oct.width() * oct.height();

	for(auto i : range(100)){
		solve2(oct, flashes);
	}

	task1 = flashes;
	flashes = 0;

	int i = 100;

	for(;;){
		solve2(oct, flashes);

		if(flashes == comp){
			task2 = i + 1;
			goto end;
		}
		++i;
		flashes = 0;
	}

end:

	print("Task 1: {}\nTask 2: {}\n", task1, task2);

};