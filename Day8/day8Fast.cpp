#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 534
//Task 2: 1070188
//Other Leaderboard Input:
//Task 1: 355
//Task 2: 983030
//Small Input:
//Task 1: 26
//Task 2: 61229

int map(size_t i){
	switch(i){
		case 42: return 0;
		case 17: return 1;
		case 34: return 2;
		case 39: return 3;
		case 30: return 4;
		case 37: return 5;
		case 41: return 6;
		case 25: return 7;
		case 49: return 8;
		case 45: return 9;
	};
	return 0;
}

/*
g++ ./day8Fast.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day8F.exe && ./day8F ./input.txt && ./day8F ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int64 task1 = 0;
    int64 task2 = 0;

    //Task start

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			ArrayC<int, 7> counts{0};

			ccp c = s.c_str(), back = (&s.back() + 1);
			bool other = false;
			for(;*c != '|';++c){
				size_t index = (*c) - 'a';
				if(0 <= index && index < 7){
					++counts[index];
				}
			};
			c+=2;
			size_t current = 0;
			size_t size = 0;
			size_t n = 0;
			while(*c != ' '){ current += counts[(*c) - 'a']; ++size; ++c; }
			task1 += size <= 4 || size == 7;
			n += map(current);
			current = 0;
			size = 0;
			++c;
			while(*c != ' '){ current += counts[(*c) - 'a']; ++size; ++c; }
			task1 += size <= 4 || size == 7;
			n *= 10;
			n += map(current);
			current = 0;
			size = 0;
			++c;
			while(*c != ' '){ current += counts[(*c) - 'a']; ++size; ++c; }
			task1 += size <= 4 || size == 7;
			n *= 10;
			n += map(current);
			current = 0;
			size = 0;
			++c;
			while(c < back){ current += counts[(*c) - 'a']; ++size; ++c; }
			task1 += size <= 4 || size == 7;
			n *= 10;
			n += map(current);
			task2 += n;
        }
    } while(infile);

    infile.close();

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};