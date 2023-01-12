#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 2628
//Task 2: 1334
//Alternative Input:
//Task 1: 2628
//Task 2: 1334
//Small Input:
//Task 1: 
//Task 2: 112

bool simulate(vec2i vel, recti box){
	vec2i pos{};
	//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
	while(pos.y >= box.y){
		pos.x += vel.x;
		pos.y += vel.y;
		vel.x -= direction(vel.x);
		vel.y -= 1;
		//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
		if(box.contains(pos)){ return true; }
	}
	return false;
};

bool simulateX(int vel, vec2i range){
	int pos{};
	//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
	while(pos < range.y && vel != 0){
		pos += vel;
		vel -= direction(vel);
		//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
		if(range.inRange(pos)){ return true; }
	}
	return false;
};

bool simulateY(int vel, vec2i range){
	int pos{};
	//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
	while(pos < range.y){
		pos += vel;
		vel -= 1;
		//print("({}, {}), ({}, {})\n", pos.x, pos.y, vel.x, vel.y);
		if(range.inRange(pos)){ return true; }
	}
	return false;
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
	recti box{};
    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			box.x = atoi(&s[15]);
			box.w = atoi(&s[20]) - box.x + 1;
			
			box.y = atoi(&s[27]);
			box.h = atoi(&s[32]) - box.y + 1;
        }
    } while(infile);

    infile.close();

	print("({}, {}) - ({}, {})\n", box.x, box.y, box.x + box.w, box.y + box.h);

	vec2i vel{23, 72};

	task1 = (box.y * (box.y + 1)) / 2; //2628

	size_t validX{};
	vec2i spanX{box.x, box.x + box.w};
	size_t validY{};
	vec2i spanY{box.y, box.y + box.h};

	for(int x = 0; x <= 281; ++x){
		if(simulateX(x, spanX)){
			for(int y = -73; y <= 72; ++y){
				if(simulateY(y, spanY)){
					++task2;
					//print("({}, {})\n", x, y);
				}
			}
		}
	}

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};