#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

struct Board{
	Array<int> data;

	Board(size_t size): data(size){};

	size_t mark(int value){
		size_t index = 0;
		for(auto& v: data){
			if(v == value){
				v = -value - 1;
				break;
			}
			++index;
		};
		return index;
	};

	bool check(size_t width, size_t height){
		for(size_t i = 0; i < height; ++i){
			bool win = true;
			for(size_t j = 0; j < width; ++j){
				win &= data[i*width + j] < 0;
				if(!win){break;}
			}
			if(win){
				return true;
			}
		}
		for(size_t i = 0; i < width; ++i){
			bool win = true;
			for(size_t j = 0; j < height; ++j){
				win &= data[i + j*width] < 0;
				if(!win){break;}
			}
			if(win){
				return true;
			}
		}
		return false;
	};

	bool check(size_t width, size_t height, size_t index){
		bool win = true;
		size_t y = (index / width) * width;
		for(size_t j = 0; j < width; ++j){
			win &= data[y + j] < 0;
			if(!win){break;}
		}
		if(win){
			return true;
		}
		
		win = true;
		for(size_t i = index % width; i < width*height; i += width){
			win &= data[i] < 0;
			if(!win){break;}
		}
		if(win){
			return true;
		}

		return false;
	};

	int score(int last){
		int s = 0;
		for(auto v: data){
			if(v >= 0){
				s += v;
			}
		};
		return s * last;
	};

	int solve(Array<int>& nums, int width, int height){
		int count = 0;
		for(auto n: nums){
			++count;
			auto res = mark(n);
			if(res != width * height && check(width, height, res)){
				break;
			}
		};
		return count;
	};
};

//Regular Input:
//Task 1: 45031
//Task 2: 2568
//Other Leaderboard Input:
//Task 1: 34506
//Task 2: 7686
//Small Input:
//Task 1: 2192
//Task 2: 1924

/*
g++ ./day4.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day4.exe && ./day4 ./input.txt && ./day4 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv, true);

	int width = 5;
	int height = 5;
	if(argc >= 4){
		width = atoi(argv[2]);
		height = atoi(argv[3]);
	}

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start

	std::getline(infile, s);
	Array<int> nums{};
	for(auto& ss: s.split(",")){
		if(ss.size() == 0){continue;}
		nums.push_back(atoi(ss.data()));
	}
	std::getline(infile, s);

	print("Size: {}\n", nums.size());


	Board b(width * height);
	int index = 0;

	int best = std::numeric_limits<int>::max();
	int worst = 0;

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			if(index == width * height){
				int sol = b.solve(nums, width, height);
				if(sol < best){
					best = sol;
					task1 = b.score(nums[sol-1]);
				}
				if(sol >= worst){
					worst = sol;
					task2 = b.score(nums[sol-1]);
				}

				index = 0;
			}

			auto* back = (&s.back() + 1);
			for(auto* c = s.c_str(); c < back; ++c){
				if(*c == ' '){ continue; }
				c = std::from_chars<int>(c, back, b.data[index]).ptr;
				++index;
			}
        }
    } while(infile);

    infile.close();

	int sol = b.solve(nums, width, height);
	if(sol < best){
		best = sol;
		task1 = b.score(nums[sol-1]);
	}
	if(sol >= worst){
		worst = sol;
		task2 = b.score(nums[sol-1]);
	}

    print("Task 1: {}\nTask 2: {}\n",task1, task2);
};