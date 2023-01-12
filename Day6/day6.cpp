#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include "./../../../shared/maths/numerics/BigInt.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

template<typename T>
T simulate(ArrayC<T, 9> mem, size_t days){
	// auto* start = &mem[0];
	// auto* index6 = &mem[6];
	// auto* index8 = &mem[8];
	// auto* end = &mem[8];

	// for(auto i: range(days)){
	// 	index6 == end ? index6 = start : ++index6;
	// 	index8 == end ? index8 = start : ++index8;
	
	// 	*index6 += *index8;
	// }

	for(auto i: range(days)){
		mem[(i + 7) % 9] += mem[i % 9];
	}
	
	return sum(mem);
};

//Regular Input:
//Task 1: 360610
//Task 2: 1631629590423
//Other Leaderboard Input:
//Task 1: 360761
//Task 2: 1632779838045
//Small Input:
//Task 1: 5934
//Task 2: 26984457539

/*
g++ ./day6.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -lgmp -lgmpxx -Wno-attributes -fopenmp -o ./day6.exe && ./day6 ./input.txt && ./day6 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv, true);
	int days = 0;
	if(argc > 2){
		days = atoi(argv[2]);
	}

    //Line buffer
    string s{30};
    int64 task1 = 0;
    int64 task2 = 0;

    //Task start
	ArrayC<int64, 9> fish{};

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			ccp c = s.c_str();
			ccp back = (&s.back() + 1);

			while(c <= back){
				int64 f = 0;
				c = std::from_chars<int64>(c, back, f).ptr;
				c += 1;
				++fish[f];
			}
        }
    } while(infile);

    infile.close();	

	if(days == 0){
    	print("Task 1: {}\nTask 2: {}\n", simulate(fish, 80), simulate(fish, 256));
	} else{
		ArrayC<BigInt, 9> mem{};
		for(auto i: range(size(mem))){ mem[i] = {fish[i]}; }
		print("For {} days: {}\n", days, simulate(mem, days).toString());
	}

};