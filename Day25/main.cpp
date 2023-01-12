#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"
#define brase break; case

//Regular Input:
//Task 1: 
//Task 2: 
//Alternative Input:
//Task 1: 
//Task 2: 
//Small Input:
//Task 1: 
//Task 2: 

bool step(const Array2D<byte>& cur, Array2D<byte>& final){
	bool move = false;
	for(auto y: range(cur.height())){
		for(auto x: range(cur.width())){
			final[{x, y}] = 0;
		}
	}
	

	for(auto y: range(cur.height())){
		for(auto x: range(cur.width())){
			if(cur[{x,y}] == 1){
				if(cur[{(x + 1) % cur.width(), y}] == 0){
					final[{x, y}] = 0;
					final[{(x + 1) % cur.width(), y}] = 1;
					move = true;
				} else{
					final[{x, y}] = cur[{x, y}];
				}
			}
		}
	}

	for(auto y: range(cur.height())){
		for(auto x: range(cur.width())){
			if(cur[{x,y}] == 2){
				if(cur[{x, (y + 1) % cur.height()}] != 2 && final[{x, (y + 1) % cur.height()}] == 0){
					final[{x, y}] = 0;
					final[{x, (y + 1) % cur.height()}] = 2;
					move = true;
				} else{
					final[{x, y}] = cur[{x, y}];
				}
			}		
		}
	}

	return move;
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


	Array2D<byte> cuc{};
	size_t width = 0;

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			ccp c = s.c_str();
			ccp back = (&s.back() + 1);
			width = s.size();
			while(c < back){
				cuc.push_back((*c == '.') ? 0 : (*c == '>') ? 1 : 2);
				++c;
			}
        }
    } while(infile);

    infile.close();

	cuc.setWidth(width);

	auto cuc2 = cuc;

	int i = 0;
	while(step(cuc, cuc2)){
		for(auto y: range(cuc.height())){
			for(auto x: range(cuc.width())){
				cuc[{x, y}] = cuc2[{x,y}];
			}
		}
		// for(auto y: range(cuc.height())){
		// 	for(auto x: range(cuc.width())){
		// 		auto c = cuc[{x, y}];
		// 		print("{}", (c == 0) ? ' ' : (c == 1) ? '>' : 'v');
		// 	}
		// 	print("\n");
		// }
		// print("{}\n", i);
		++i; 
	}
	task1 = i + 1;



    print("Task 1: {}\nTask 2: {}\n", task1, task2);
};