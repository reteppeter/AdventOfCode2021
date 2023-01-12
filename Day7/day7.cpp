#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 
//Task 2: 
//Other Leaderboard Input:
//Task 1: 
//Task 2: 
//Small Input:
//Task 1: 
//Task 2: 

/*
g++ ./day7.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day7.exe && ./day7 ./input.txt && ./day7 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start
	Array<int> crabs{};

	int64 avg = 0;

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
				crabs.push_back(f);
				avg += f;
			}
        }
    } while(infile);

    infile.close();
	int64 mid1 = int64(floor(double(avg)/size(crabs)));
	int64 mid2 = int64(ceil(double(avg)/size(crabs)));
	int64 fuel1 = 0;
	int64 fuel2 = 0;

	for(auto c: crabs){
		auto dif = abs(c - mid1);
		fuel1 += (dif * (dif + 1))/2;
		dif = abs(c - mid2);
		fuel2 += (dif * (dif + 1))/2;
	}

	int64 fuel = min(fuel1, fuel2);

	print("Other: {}\n", fuel);

	std::sort(crabs.begin(), crabs.end());

	int64 mid = crabs[crabs.size()/2];
	fuel = 0;

	for(auto c: crabs){
		fuel += abs(c - mid);
	}
	print("Median: {}\n", fuel);

    print("Task 1: {}\nTask 2: {}\n",task1, task2);
};