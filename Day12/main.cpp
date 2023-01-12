#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 3856
//Task 2: 116692
//Alternative Input:
//Task 1: 5920
//Task 2: 155477
//Small Input:
//Task 1: 10
//Task 2: 36

class Solution{
	const size_t start;
	const size_t end;
	const size_t lastUpper;

	public:
	Solution(size_t start, size_t end, size_t lastUpper): start(start), end(end), lastUpper(lastUpper){};
	
	int solve(size_t current, const Array<Array<size_t>>& tree, Array<byte>& counts) const{
		int total = 0;
		for(auto s: tree[current]){
			if(s == end){
				++total;
			} else if(s == start){
				continue;
			}else{
				if(s > lastUpper && counts[s] != 0){ continue; }
				++counts[s];
				total += solve(s,tree, counts);
				--counts[s];
			}
		};
		return total;
	};

	int solve2(size_t current, const Array<Array<size_t>>& tree, Array<byte>& counts, bool doubled) const{
		int total = 0;
		for(auto s: tree[current]){
			if(s == end){
				++total;
			} else if(s == start){
				continue;
			}else{
				bool d = doubled;
				if(s > lastUpper){
					if(counts[s] >= 1 && doubled){ continue; }
					if(counts[s] == 1){ d = true; }
				}
				++counts[s];
				total += solve2(s, tree, counts, d);
				--counts[s];
			}
		};
		return total;
	};
};



/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int task1 = 0;
    int task2 = 0;

    //Task start
	Map<string, Array<string>> tree{};

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			auto ss = s.split('-');
			tree[ss[0]].push_back(ss[1]);
			tree[ss[1]].push_back(ss[0]);
        }
    } while(infile);

	Array<Array<size_t>> effTree(tree.size());
	size_t endInd = tree.size() - 1;
	size_t startInd = 0;
	Map<string, size_t> mapping{};
	for(auto&& [k, v]: tree){
		if('a' <= k[0] && k[0] <= 'z'){
			mapping[k] = endInd--;
		} else{
			mapping[k] = startInd++;
		}
	}
	for(auto&& [k, v]: tree){
		for(auto& vv: v){
			effTree[mapping[k]].push_back(mapping[vv]);
		}
	}

    infile.close();

	Set<string> m{};
	Array<byte> count(tree.size());
	//task1 = solveX("start", tree, m);
	Solution sol{mapping["start"], mapping["end"], endInd};
	task1 = sol.solve(mapping["start"], effTree, count);
	task2 = sol.solve2(mapping["start"], effTree, count, false);
	//task2 = solve2X("start", tree, m, false);

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};