#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include "./../../../shared/maths/numerics/BigInt.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 415953
//Task 2: 2292863731
//Alternative Input:
//Task 1: 364389
//Task 2: 2870201088
//Small Input:
//Task 1: 26397
//Task 2: 288957

int bracketMap(char c){
	switch(c){
		case '(': return 1;
		case ')': return -1;
		case '[': return 2;
		case ']': return -2;
		case '{': return 3;
		case '}': return -3;
		case '<': return 4;
		case '>': return -4;
	}
	return 0;
};

/*
g++ ./day10.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -lgmp -lgmpxx -Wno-attributes -fopenmp -fno-exceptions -o ./day10.exe && ./day10 ./input.txt && ./day10 ./input2.txt
*/
int main(int argc, char** argv){
	auto infile = AoC::init(argc, argv);

	//Line buffer
	string s{30};
	int task1 = 0;
	BigInt task2 = 0;

	//Task start
	ArrayC<int, 4> bad{0,0,0,0};

	Array<BigInt> inc{};
	Array<int> stack{};
	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			stack.resize(0);
			for(ccp c = s.c_str(), back = (&s.back() + 1); c <= back; ++c){
				int i = bracketMap(*c);
				if(i > 0){
					stack.push_back(i);
				} else if(i < 0 && stack.pop() != -i){
					++bad[-i - 1];
					stack.resize(0);
					break;
				}
			}

			if(stack.size() != 0){
				BigInt sum2 = 0;
				for(int i = 0; i < stack.size(); ++i){
					sum2 *= 5;
					sum2 += stack[stack.size() - 1 - i];
				}
				inc.push_back(sum2);
			}
		}
	} while(infile);

	infile.close();

	task1 = bad[0] * 3 + bad[1] * 57 + bad[2] * 1197 + bad[3] * 25137;
	//print("{}, {}, {}, {}\n", bad[0], bad[1], bad[2], bad[3]);

	//std::sort(inc.begin(), inc.end());
	std::nth_element(inc.begin(), inc.begin() + inc.size()/2, inc.end());
	task2 = inc[inc.size() / 2];

	print("Task 1: {}\nTask 2: {}\n", task1, task2.get_str());
};