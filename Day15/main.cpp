#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include <queue>

#include "../sharedAoC.h"

class AStar{
	using WeightedPos = std::pair<int, vec2i>;
	using PQ = std::priority_queue<WeightedPos, Array<WeightedPos>, std::greater<WeightedPos>>;
	using Path = Map<vec2i, vec2i>;
	using Scores = Map<vec2i, int>;

	using CostFunc = std::function<int(vec2i, vec2i)>;
	
	static Array<vec2i> reconstructPath(Map<vec2i, vec2i> cameFrom, vec2i current){
		Array<vec2i> path{current};
		while(cameFrom.contains(current)){
			current = cameFrom[current];
			path.push_back(current);
		}
		return path;
	};

	static auto checkNeighbour(Path& cameFrom, Scores& gScore, PQ& openSet, CostFunc travelCost, WeightedPos current, vec2i neighbour){
		auto tentative_gScore = gScore[current.second] + travelCost(current.second, neighbour);
		if(!gScore.contains(neighbour) || tentative_gScore < gScore[neighbour]){
			// This path to neighbor is better than any previous one. Record it!
			cameFrom[neighbour] = current.second;
			gScore[neighbour] = tentative_gScore;
			//fScore[neighbour] = tentative_gScore; + h(neighbour);
			openSet.push({tentative_gScore, neighbour});
		}
	};

	public:
	// A* finds a path from start to goal.
	// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
	static Array<vec2i> solve(vec2i start, vec2i goal, vec2i dims, CostFunc d){
		PQ openSet{};
		openSet.push({0, start});

		// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start
		// to n currently known.
		Path cameFrom{};

		// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
		Scores gScore{};
		gScore[start] = 0;

		// For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
		// how short a path from start to finish can be if it goes through n.
	//    Map<vec2i, int>  fScore{};//map with default value of Infinity
		//fScore[start] = 0;//h[start];

		while(!openSet.empty()){
			auto current = openSet.top();
			if(current.second == goal){
				return reconstructPath(cameFrom, current.second);
			}
			openSet.pop();
			vec2i neighbour;
			//cardinal neighbours
			if(current.second.x != 0){ checkNeighbour(cameFrom, gScore, openSet, d, current, {current.second.x - 1, current.second.y}); }
			if(current.second.y != 0){ checkNeighbour(cameFrom, gScore, openSet, d, current, {current.second.x, current.second.y - 1}); }
			if(current.second.x != dims.x - 1){ checkNeighbour(cameFrom, gScore, openSet, d, current, {current.second.x + 1, current.second.y}); }
			if(current.second.y != dims.y - 1){ checkNeighbour(cameFrom, gScore, openSet, d, current, {current.second.x, current.second.y + 1}); }
		}
		// Open set is empty but goal was never reached
		return {};
	};

	static int score(const Array<vec2i>& path, CostFunc d){
		int s = 0;
		for(auto p: path){
			s += d({}, p); //TODO: previous and current
		}
		return s;
	};
};

//Regular Input:
//Task 1: 
//Task 2: 
//Alternative Input:
//Task 1: 
//Task 2: 
//Small Input:
//Task 1: 
//Task 2: 

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

	Array2D<int> data{};
	size_t width = 0;

	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			ccp c = s.c_str();
			ccp back = (&s.back() + 1);
			width = s.size();
			while(c < back){
				data.push_back((*c) - '0');
				++c;
			}
		}
	} while(infile);

	infile.close();

	data.setWidth(width);

	auto score = [&data](vec2i current, vec2i next){
		return data[next];
	};
	auto path = AStar::solve(vec2i{data.width() - 1, data.height() - 1}, {0 , 0}, vec2i{data.width(), data.height()}, score);

	auto score2 = [&data](vec2i current, vec2i next){
		vec2i newNext{next.x % data.width(), next.y % data.height()};
		auto cost = data[newNext];
		return (cost + (next.x / data.width()) + (next.y / data.height()) - 1) % 9 + 1;
	};
	auto path2 = AStar::solve({0 , 0}, vec2i{5*data.width() - 1, 5*data.height() - 1}, vec2i{5*data.width(), 5*data.height()}, score2);

	task1 = AStar::score(path, score);
	task1 -= score(vec2i{data.width() - 1, data.height() - 1}, {});
	task2 = AStar::score(path2, score2);
	task2 -= score2(vec2i{5*data.width() - 1, 5*data.height() - 1}, {});

	//for(auto p: path2){
	//	print("({}, {}) - {}\n", p.x, p.y, score2({}, p));
	//}

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};