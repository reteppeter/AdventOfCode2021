#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 2899
//Task 2: 3528317079545
//Alternative Input:
//Task 1: 5656
//Task 2: 12271437788530
//Small Input:
//Task 1: 1588
//Task 2: 2188189693529

using Mut = RTP::Maths::Numerics::Vector2<char>;

auto getMinMax(const Map<Mut, uint64>& pairs, char last){
	struct{
		uint64 minV = 0;
		char minC = ' ';
		uint64 maxV = 0;
		char maxC = ' ';
	} res{};

	Map<char, uint64> counts{};
	for(auto [p, c]: pairs){
		counts[p.x] += c;
		//counts[p.y] += c;
	}
	++counts[last];

	res.minV = std::numeric_limits<uint64>::max();

	for(auto& [k, v]: counts){
		if(v >= res.maxV){ res.maxV = v; res.maxC = k; }
		if(v <= res.minV){ res.minV = v; res.minC = k; }
	}

	return res;
};

template <typename T>
auto solve(const string& base, Map<Mut, char>& mut, size_t iterations){
	Map<Mut, T> pairs{};
	for(auto& [k, v]: mut){ pairs[k]; }

	auto start = base.cbegin();
	auto cur = start;
	++cur;
	auto end = base.cend();
	while(!(start == end || cur == end)){
		++pairs[{*start, *cur}];
		start = cur;
		++cur;
	}

	//for(auto& [p, c]: pairs){
	//	print("{}{}, {}\n", p.x, p.y, c);
	//}

	for(auto i: range(iterations)){
		Map<Mut, T> next{};
		for(auto& [p, c]: pairs){
			if(c == 0){ continue; }
			auto mid = mut[p];
			next[{p.x, mid}] += c;
			next[{mid, p.y}] += c;
		}
		pairs = next;
	}
	return getMinMax(pairs, last);
};

/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    uint64 task2 = 0;

    //Task start

	int mode = 0;

	Map<Mut, char> mut{};
	string base{};
    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
			mode += 1;
        } else {
			if(mode == 0){
				base = s;
			} else{
				mut[{s[0], s[1]}] = s[6];
			}
        }
    } while(infile);

    infile.close();
	
	auto base2 = base;
	auto last = base2.back();

	auto pairs = solve<int>(base, mut, 10);
	auto res = getMinMax(pairs, last);

	task1 = res.maxV - res.minV;
	
	auto pairs2 = solve<uint64>(base2, mut, 40);
	auto res2 = getMinMax(pairs2, last);

	task2 = res2.maxV - res2.minV;

    print("Task 1: {}\nTask 2: {}\n", task1, task2);

};