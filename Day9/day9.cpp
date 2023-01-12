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

int count(RTP::Maths::Numerics::Vector2<size_t> loc, int basinIndex, Array2D<int>& basin, Array2D<int>& heights){
	if(basin[loc] != -1){
		return 0;
	}

	if(heights[loc] == 9){
		basin[loc] = -2;
		return 0;
	}

	basin[loc] = basinIndex;

	int res = 1;

	if(loc.x != 0){
		res += count({loc.x-1, loc.y}, basinIndex, basin, heights);
	}
	if(loc.x != heights.width() - 1){
		res += count({loc.x+1, loc.y}, basinIndex, basin, heights);
	}
	if(loc.y != 0){
		res += count({loc.x, loc.y-1}, basinIndex, basin, heights);
	}
	if(loc.y != heights.height() - 1){
		res += count({loc.x, loc.y+1}, basinIndex, basin, heights);
	}

	return res;
};

/*
g++ ./day9.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day9.exe && ./day9 ./input.txt && ./day9 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start

	Array2D<int> heights{};
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
				heights.push_back((*c) - '0');
				++c;
			}
        }
    } while(infile);

    infile.close();

	heights.setWidth(width);

	Array2D<int> basin(heights.width(), heights.height(), -1);
	//Array<int> basins{};
	Array<int> basinSizes{};

	for(size_t y = 0; y < heights.height(); ++y){
		for(size_t x = 0; x < heights.width(); ++x){
			auto cur = heights[{x, y}];
			//print("{}", cur);
			if(x != 0){
				if(cur >= heights[{x-1, y}]){ continue; }
			}
			if(x != heights.width() - 1){
				if(cur >= heights[{x+1, y}]){ continue; }
			}
			if(y != 0){
				if(cur >= heights[{x, y-1}]){ continue; }
			}
			if(y != heights.height() - 1){
				if(cur >= heights[{x, y+1}]){ continue; }
			}
			task1 += cur + 1;

			//if(basin[{x, y}] != -1){ continue; }
			int basinIndex = basinSizes.size();
			auto size = count({x, y}, basinIndex, basin, heights);
			//basins.push_back(basinIndex);
			basinSizes.push_back(size);
			//print("{}\n", size);
		}
		//print("\n");
	}

	std::sort(basinSizes.begin(), basinSizes.end());

	print("{}, {}\n", basinSizes[0], basinSizes.back());
	task2 = basinSizes[basinSizes.size()-1] * basinSizes[basinSizes.size()-2] * basinSizes[basinSizes.size()-3];

    print("Task 1: {}\nTask 2: {}\n",task1, task2);
};