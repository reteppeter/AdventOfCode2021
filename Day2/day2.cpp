#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 2322630
//Task 2: 2105273490
//Other Leaderboard Input:
//Task 1: 1924923
//Task 2: 1982495697

/*
g++ ./day2.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day2.exe && ./day2 ./input.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    uint64 task1 = 0;
    uint64 task2 = 0;

    //Task start
	uint64 x = 0;
	uint64 y = 0;
	uint64 y2 = 0;

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			switch(s[0]){
				case 'f':{
					int i = atoi(&s[8]);
					x += i;
					y2 += y * i;
				}break;
				case 'b':{
					x -= atoi(&s[5]);
				}break;
				case 'u':{
					int i = atoi(&s[3]);
					y -= i;
				}break;
				case 'd':{
					int i = atoi(&s[5]);
					y += i;
				}break;
			};
        }
    } while(infile);

    infile.close();

	task1 = x * y;
	task2 = x * y2;

    print("Task 1: {}\nTask 2: {}\n", task1, task2);
};