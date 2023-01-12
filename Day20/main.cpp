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

pair<vec2i, vec2i> printSet(const Set<vec2i>& data){
	vec2i minPos{};
	vec2i maxPos{};

	for(auto p: data){
		minPos.x = min(minPos.x, p.x);
		minPos.y = min(minPos.y, p.y);
		maxPos.x = max(maxPos.x, p.x);
		maxPos.y = max(maxPos.y, p.y);
	}

	// for(auto y: range(minPos.y, maxPos.y + 1)){
	// 	for(auto x: range(minPos.x, maxPos.x + 1)){
	// 		putc(data.contains({x,y}) ? '#' : ' ', stdout);
	// 	}
	// 	putc('\n', stdout);
	// }

	return {minPos, maxPos};
};

bool get(const Set<vec2i>& data, vec2i curMin, vec2i curMax, vec2i pos, bool outer){
	if(pos.x >= curMin.x && pos.x < curMax.x && pos.y >= curMin.y && pos.y < curMax.y){
		return data.contains(pos);
	}
	return outer;
};

/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
-fwhole-program -march=native
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start
	int mode = 0;
	Array<bool> enhancer;
	enhancer.reserve(512);
	Set<vec2i> image{};
	vec2i pos{};
    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
			mode = 1;
        } else {
			if(mode == 0){
				for(auto c: s){
					enhancer.push_back(c == '#');
				}
			} else{
				pos.x = 0;
				for(auto c: s){
					if(c == '#'){
						image.insert(pos);
					}
					++pos.x;
				}
				++pos.y;
			}
        }
    } while(infile);

    infile.close();

	bool outer = false;
	vec2i curMin{};
	vec2i curMax = pos;

	for(auto i: range(50)){
		auto [minPos, maxPos] = printSet(image);
		Set<vec2i> newImage{};
		for(auto y: range(curMin.y - 1, curMax.y + 1)){
			for(auto x: range(curMin.x - 1, curMax.x + 1)){
				size_t b = 0;
				b |= get(image, curMin, curMax, {x-1, y-1}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x, y-1}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x+1, y-1}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x-1, y}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x, y}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x+1, y}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x-1, y+1}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x, y+1}, outer); b <<= 1;
				b |= get(image, curMin, curMax, {x+1, y+1}, outer);
				if(enhancer[b]){
					newImage.insert({x, y});
				}
			}
		}
		image = newImage;
		outer ^= enhancer[0];
		curMin -= vec2i{1, 1};
		curMax += vec2i{1, 1};
	}

	task1 = image.size();

    print("Task 1: {}\nTask 2: {}\n", task1, task2);
};