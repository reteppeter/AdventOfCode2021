#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>

#include "../sharedAoC.h"

#include  <bitset>

//Regular Input:
//Task 1: 
//Task 2: 
//Other Leaderboard Input:
//Task 1: 3895776
//Task 2: 7928162
//Example Input:
//Task 1: 198
//Task 2: 230

//class bitset2{
//	uint8 size;
//	uint64 data;
//
//};

/*
g++ ./day3.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day3.exe && ./day3 ./input.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv, true);
	int width = 12;
	if(argc>=3){
		width = atoi(argv[2]);
	}

    //Line buffer
    string s{30};
    uint64 task1 = 0;
    uint64 task2 = 0;

    //Task start

	ArrayC<uint64, 64> bits;
	for(auto& i : bits){
		i = 0;
	}

	uint64 count = 0;

	using BitSet = std::bitset<64>;

	BitSet bits2;

	Array<string> data;

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			data.push_back(s);
			for(int i = 0; i < width; ++i){
				bits[i] += s[i] == '1';
			}
			count += 1;
        }
    } while(infile);

	for(int i = 0; i < width; ++i){
		if(bits[i] > count/2){
			bits2.set(width - 1 - i);
		}
	}

	Array<string> ox{data};
	Array<string> co{data};

	for(int i = 0; i < width; ++i){
		Array<string> newOx;
		int most = 0;
		for(auto& v: ox){
			most += v[i] == '1';
		};
		if(most * 2 == ox.size()){
			most += 1;
		}
		char best = most > ox.size()/2 ? '1' : '0';
		for(auto& v: ox){
			if(v[i] == best){
				newOx.push_back(v);
			}
		};
		ox = newOx;
	}

	print("???\n");

	for(int i = 0; i < width; ++i){
		Array<string> newCo;
		int least = 0;
		for(auto& v: co){
			least += v[i] == '1';
		};
		if(least * 2 == co.size()){
			least += 1;
		}
		char best = least > co.size()/2 ? '1' : '0';
		for(auto& v: co){
			if(v[i] != best){
				newCo.push_back(v);
			}
		};
		co = newCo;
	}
	
	print("{}\n", bits2.to_string());
	uint64 a = bits2.to_ullong();
	bits2 ^= (uint64(1) << width) - 1;
	uint64 b = bits2.to_ullong();
	print("{}\n", bits2.to_string());

	print("{}\n", a);
	print("{}\n", b);

	task1 = a * b;

	uint64 x = 0;
	uint64 y = 0;

	print("{}\n", ox.size());
	print("{}\n", co.size());

	for(auto& i : ox[0]){
		x *= 2;
		x += i == '1';
	}

	for(auto& i : co[0]){
		y *= 2;
		y += i == '1';
	}

	print("{}, {}, {}\n",x, ox.size(), ox[0]);
	print("{}, {}, {}\n", y, co.size(), co[0]);

	task2 = x*y;

    infile.close();

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};