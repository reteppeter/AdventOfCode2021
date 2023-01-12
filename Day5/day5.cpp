#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

using Point = vec2i;
struct Line{ Point a, b; };

template <typename T>
void drawLine(Line l, T& data, vec2i& counts){
	if(l.a.x == l.b.x){
		if(l.a.y > l.b.y){
			std::swap(l.a.y, l.b.y);
		}
		for(Point v = {l.a.x, l.a.y}; v.y <= l.b.y; ++v.y){
			auto& d = data[v];
			if(d.x == 1){
				++counts.x;
			}
			if(d.x + d.y == 1){
				++counts.y;
			}
			++d.x;
		}
	} else if(l.a.y == l.b.y){
		if(l.a.x > l.b.x){
			std::swap(l.a.x, l.b.x);
		}
		for(Point v = {l.a.x, l.a.y}; v.x <= l.b.x; ++v.x){
			auto& d = data[v];
			if(d.x == 1){
				++counts.x;
			}
			if(d.x + d.y == 1){
				++counts.y;
			}
			++d.x;
		}
	} else{
		if(l.a.x > l.b.x){
			std::swap(l.a.x, l.b.x);
			std::swap(l.a.y, l.b.y);
		}
		if(l.a.y > l.b.y){
			for(Point v{l.a.x, l.a.y}; v.x <= l.b.x; ++v.x, --v.y){
				auto& d = data[v];
				if(d.x + d.y == 1){
					++counts.y;
				}
				++d.y;
			}
		} else{
			for(Point v{l.a.x, l.a.y}; v.x <= l.b.x; ++v.x, ++v.y){
				auto& d = data[v];
				if(d.x + d.y == 1){
					++counts.y;
				}
				++d.y;
			}
		}
	}
};

//Regular Input:
//Task 1: 5294
//Task 2: 21698
//Other Leaderboard Input:
//Task 1: 4728
//Task 2: 17717
//Small Input:
//Task 1: 
//Task 2: 

/*
g++ ./day5.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day5.exe && ./day5 ./input.txt && ./day5 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start
	Map<Point, RTP::Maths::Numerics::Vector2<uint8>> map{};

	vec2i counts{};

	Array<Line> lines;

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			Line l{};

			ccp c = s.c_str();
			ccp back = (&s.back() + 1);

			c = std::from_chars<int>(c, back, l.a.x).ptr;
			c += 1;
			c = std::from_chars<int>(c, back, l.a.y).ptr;
			c += 4;
			c = std::from_chars<int>(c, back, l.b.x).ptr;
			c += 1;
			c = std::from_chars<int>(c, back, l.b.y).ptr;

			drawLine(l, map, counts);
        }
    } while(infile);

    infile.close();

    RTP::Print::print("Task 1: {}\nTask 2: {}\n", counts.x, counts.y);
};