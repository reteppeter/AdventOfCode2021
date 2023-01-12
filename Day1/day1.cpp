#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 1581
//Task 2: 1618
//Other Leaderboard Input:
//Task 1: 1715
//Task 2: 1739

/*
g++ day1.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o day1.exe
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start
	std::getline(infile, s);
	int last = atoi(s.data());
	std::getline(infile, s);
	int mid = atoi(s.data());
	std::getline(infile, s);
	int mid2 = atoi(s.data());
	task1 += last < mid;
	task1 += mid < mid2;
    do{
        std::getline(infile, s);
        if(s.empty()){
        } else {
			int cur = atoi(s.data());
			task1 += cur > mid2;
			task2 += cur > last;
			last = mid;
			mid = mid2;
			mid2 = cur;
        }
    } while(infile);

    infile.close();

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};