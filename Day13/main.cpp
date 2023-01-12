#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 
//Task 2: 
//Alternative Input:
//Task 1: 
//Task 2: 
//Small Input:
//Task 1: 
//Task 2: 

Set<vec2i> fold(const Set<vec2i>& prev, vec2i edge){
	Set<vec2i> res{};
	if(edge.x == 0){
		for(auto v : prev){
			if(v.x == edge.y){ continue; }
			if(v.x > edge.y){ v.x = 2 * edge.y - v.x; }
			res.insert(v);
		}
	} else{
		for(auto v : prev){
			if(v.y == edge.y){ continue; }
			if(v.y > edge.y){ v.y = 2 * edge.y - v.y; }
			res.insert(v);
		}
	}
	return res;
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

	int mode = 0;
	Set<vec2i> points{};
	Array<vec2i> folds{};

	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
			mode = 1;
		} else{
			if(mode == 0){
				auto ss = s.split(',');
				vec2i p = {atoi(ss[0].data()), atoi(ss[1].data())};
				points.insert(p);
			} else if(mode = 1){
				folds.push_back({s[11] - 'x', atoi(&s[13])});
			}
		}
	} while(infile);

	infile.close();

	auto f = fold(points, folds[0]);
	task1 = f.size();
	Set<vec2i> res = points;
	for(auto ff : folds){
		res = fold(res, ff);
	}
	vec2i dims{};
	Array<vec2i> res2{};
	res2.reserve(res.size());
	for(auto p : res){
		dims.max2(p);
		res2.push_back(p);
	}
	dims += {1, 1};
	std::sort(res2.begin(), res2.end(),
		[](const auto& lhs, const auto& rhs){
			return lhs.y != rhs.y ? lhs.y < rhs.y : lhs.x < rhs.x;
		}
	);
	res2.push_back(dims);
	auto c = res2.begin();
	auto p = *c;
	string test = "";
	test.reserve((dims.x + 1) * (dims.y + 1));
	for(size_t y : range(dims.y)){
		for(size_t x : range(dims.x)){
			if(p.y == y && p.x == x){
				//putc('#', stdout);
				test += "#";
				++c;
				p = *c;
			} else{
				test += " ";
				//putc(' ', stdout);
			}
		}
		test += "\n";
		//putc('\n', stdout);
	}
	puts(test.data());

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};