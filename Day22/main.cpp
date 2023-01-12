#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 
//Task 2: 
//Alternative Input:
//Task 1: 
//Task 2: 
//Small Input:
//Task 1: 
//Task 2: 

struct Operation{
	bool type = false; //off
	int minX;
	int maxX;
	int minY;
	int maxY;
	int minZ;
	int maxZ;

	bool overlap(const Operation& other) const{
		return
			(minX <= other.minX && other.minX <= maxX)
			|| (minX <= other.maxX && other.maxX <= maxX)
			|| (other.minX <= minX && minX <= other.maxX)
			|| (other.minX <= maxX && maxX <= other.maxX)

			&& (minY <= other.minY && other.minY <= maxY)
			|| (minY <= other.maxY && other.maxY <= maxY)
			|| (other.minY <= minY && minY <= other.maxY)
			|| (other.minY <= maxY && maxY <= other.maxY)

			&& (minZ <= other.minZ && other.minZ <= maxZ)
			|| (minZ <= other.maxZ && other.maxZ <= maxZ)
			|| (other.minZ <= minZ && minZ <= other.maxZ)
			|| (other.minZ <= maxZ && maxZ <= other.maxZ);
	};
};

size_t flatten(size_t x, size_t y, size_t z, size_t width, size_t height){
	return (z * height + y) * width + x;
};

void findUnique(auto&& r){
	std::sort(begin(r), end(r));
	auto last = std::unique(begin(r), end(r));
	r.erase(last, end(r));
};

int getIndexIn(auto&& r, auto&& v){
	return std::find(begin(r), end(r), v) - begin(r);
};

size_t getVol(bool b001, bool b010, bool b011, bool b100, bool b101, bool b110, bool b111, int64 difX, int64 difY, int64 difZ){
	int64 cur = 1;
	if(b001 && b010 && b011 && b100 && b101 && b110 && b111){
		//print("Full: {}\n", difX * difY * difZ);
		cur = difX * difY * difZ;
	} else if(!(b001 || b010 || b011 || b100 || b101 || b110 || b111)){
		//print("Empty: {}\n", difX * difY * difZ);
		cur = 1;
	} else{
		auto pXY = b001 && b010 && b011;
		auto pXZ = b001 && b100 && b101;
		auto pYZ = b010 && b100 && b110;
		// if(pXY){ cur += difX * difY - 1; }
		// if(pXZ){ cur += difX * difZ - 1; }
		// if(pYZ){ cur += difY * difZ - 1; }
		// if(pXY && pXZ){ cur -= difX - 1; }
		// if(!(pXY || pXZ) && b001){ cur += difX - 1; }
		// if(pXY && pYZ){ cur -= difY - 1; }
		// if(!(pXY || pYZ) && b010){ cur += difY - 1; }
		// if(pXZ && pYZ){ cur -= difZ - 1; }
		// if(!(pXZ || pYZ) && b100){ cur += difZ - 1; }
		// if(b001 && b010 && b100){
		// 	cur -= 2;
		// } else if((b001 && b010) || (b001 && b100) || (b010 && b100)){
		// 	cur -= 1;
		// }
		// print("{}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n", b001, b010, b011, b100, b101, b110, b111, difX, difY, difZ);
		// print("{}\n", cur);
		if(pXY && pXZ && pYZ){
			cur = difX * difY + difX * difZ + difY * difZ - difX - difY - difZ + 1;
		} else if(pXY && !pXZ && !pYZ){
			cur = difX * difY + b100 * (difZ - 1);
		} else if(!pXY && pXZ && !pYZ){
			cur = difX * difZ + b010 * (difY - 1);
		} else if(!pXY && !pXZ && pYZ){
			cur = difY * difZ + b001 * (difX - 1);
		} else if(pXY && pXZ && !pYZ){
			cur = difX * difY + difX * difZ - difX;
		} else if(pXY && !pXZ && pYZ){
			cur = difX * difY + difY * difZ - difY;
		} else if(!pXY && pXZ && pYZ){
			cur = difX * difZ + difY * difZ - difZ;
		} else if(!pXY && !pXZ && !pYZ){
			if(b001 && b010 && b100){
				cur = difX + difY + difZ - 2;
			} else if(b001 && b010 && !b100){
				cur = difX + difY - 1;
			} else if(b001 && !b010 && b100){
				cur = difX + difZ - 1;
			} else if(!b001 && b010 && b100){
				cur = difY + difZ - 1;
			} else if(b001 && !b010 && !b100){
				cur = difX;
			} else if(!b001 && b010 && !b100){
				cur = difY;
			} else if(!b001 && !b010 && b100){
				cur = difZ;
			} else if(!b001 && !b010 && !b100){
				cur = 1;
			}
		}
	}
	//print("{}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n", b001, b010, b011, b100, b101, b110, b111, difX, difY, difZ);
	//print("{}\n", cur);
	return cur;
};

/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
-fwhole-program -march=native
//../../shared/maths/random/RTPRandom.cpp
*/
int main(int argc, char** argv){
	auto infile = AoC::init(argc, argv);

	//Line buffer
	string s{30};
	int task1 = 0;
	int64 task2 = 0;

	//Task start
	Array<Operation> ops{};
	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			if(s[0] == '/'){ continue; }
			Operation op{};
			op.type = s[1] == 'n';
			ccp c = &s[5], cend = (&s.back()) + 1;
			c += !op.type;
			c = std::from_chars(c, cend, op.minX).ptr;
			c += 2;
			c = std::from_chars(c, cend, op.maxX).ptr;
			c += 3;
			c = std::from_chars(c, cend, op.minY).ptr;
			c += 2;
			c = std::from_chars(c, cend, op.maxY).ptr;
			c += 3;
			c = std::from_chars(c, cend, op.minZ).ptr;
			c += 2;
			c = std::from_chars(c, cend, op.maxZ).ptr;
			ops.push_back(op);
			//print("{} - ({}, {}), ({}, {}), ({}, {})\n", op.type, op.minX, op.maxX, op.minY, op.maxY, op.minZ, op.maxZ);
		}
	} while(infile);

	infile.close();

	Array<byte> reactor(101 * 101 * 101, 0);

	for(auto& op : ops){
		auto minY = max(-50, min(op.minY, 51)) + 50;
		auto maxY = min(50, max(op.maxY, -51)) + 1 + 50;
		auto minX = max(-50, min(op.minX, 51)) + 50;
		auto maxX = min(50, max(op.maxX, -51)) + 1 + 50;
		for(auto z : range(max(-50, min(op.minZ, 51)) + 50, min(50, max(-51, op.maxZ)) + 1 + 50)){
			for(auto y : range(minY, maxY)){
				for(auto x : range(minX, maxX)){
					reactor[flatten(x, y, z, 101, 101)] = op.type;
					auto p = flatten(x, y, z, 101, 101);
					if(p > 1036153){
						//print("{} - ({}, {} ({}, {}), {})\n", p, x, y, minY, maxY, z);
					}
				}
			}
		}
	}

	for(auto z : range(-50, 51)){
		for(auto y : range(-50, 51)){
			for(auto x : range(-50, 51)){
				task1 += reactor[flatten(x + 50, y + 50, z + 50, 101, 101)];
			}
		}
	}

	// Queue<Operation> cubeOps{};
	// cubeOps.push(ops[20]);
	// for(auto i : range<size_t>(21, ops.size())){
	// 	Queue<Operation> newCubeOps{};
	// 	Queue<Operation> cur{};
	// 	cur.push(ops[i]);
	// 	Queue<Operation> cur2{};
	// 	while(!cubeOps.empty()){
	// 		auto c = cubeOps.front();
	// 		cubeOps.pop();
	// 		while(!cur.empty()){

	// 		}
	// 	}
	// }

	Array<int> xVals{};
	Array<int> yVals{};
	Array<int> zVals{};

	for(auto i : range<size_t>(0/*20*/, ops.size())){
		xVals.push_back(ops[i].minX);
		xVals.push_back(ops[i].maxX);
		yVals.push_back(ops[i].minY);
		yVals.push_back(ops[i].maxY);
		zVals.push_back(ops[i].minZ);
		zVals.push_back(ops[i].maxZ);
		//if(!ops[i].type){
			xVals.push_back(ops[i].minX-1);
			xVals.push_back(ops[i].maxX+1);
			yVals.push_back(ops[i].minY-1);
			yVals.push_back(ops[i].maxY+1);
			zVals.push_back(ops[i].minZ-1);
			zVals.push_back(ops[i].maxZ+1);
		//}
	}

	findUnique(xVals);
	findUnique(yVals);
	findUnique(zVals);

	print("{}, {}, {}\n", xVals.size(), yVals.size(), zVals.size());

	Array<bool> bigGrid(xVals.size() * yVals.size() * zVals.size());

	Array<Operation> newOps{};
	for(auto& op : ops){
		Operation newOp{};
		newOp.type = op.type;
		newOp.minX = getIndexIn(xVals, op.minX);
		newOp.maxX = getIndexIn(xVals, op.maxX) + 1;
		newOp.minY = getIndexIn(yVals, op.minY);
		newOp.maxY = getIndexIn(yVals, op.maxY) + 1;
		newOp.minZ = getIndexIn(zVals, op.minZ);
		newOp.maxZ = getIndexIn(zVals, op.maxZ) + 1;
		newOps.push_back(newOp);
	}

	for(auto& op : newOps){
		for(auto z : range(op.minZ, op.maxZ)){
			for(auto y : range(op.minY, op.maxY)){
				for(auto x : range(op.minX, op.maxX)){
					bigGrid[flatten(x, y, z, xVals.size(), yVals.size())] = op.type;
				}
			}
		}
	}

	for(auto z : range(zVals.size())){
		for(auto y : range(yVals.size())){
			for(auto x : range(xVals.size())){
				auto b000 = bigGrid[flatten(x, y, z, xVals.size(), yVals.size())];
				if(!b000){ continue; }
				auto eX = (x + 1) != xVals.size();
				auto eY = (y + 1) != yVals.size();
				auto eZ = (z + 1) != zVals.size();
				auto b001 = eX && bigGrid[flatten(x + 1, y, z, xVals.size(), yVals.size())];
				auto b010 = eY && bigGrid[flatten(x, y + 1, z, xVals.size(), yVals.size())];
				auto b011 = eY && eX && bigGrid[flatten(x + 1, y + 1, z, xVals.size(), yVals.size())];
				auto b100 = eZ && bigGrid[flatten(x, y, z + 1, xVals.size(), yVals.size())];
				auto b101 = eZ && eX && bigGrid[flatten(x + 1, y, z + 1, xVals.size(), yVals.size())];
				auto b110 = eZ && eY && bigGrid[flatten(x, y + 1, z + 1, xVals.size(), yVals.size())];
				auto b111 = eZ && eY && eX && bigGrid[flatten(x + 1, y + 1, z + 1, xVals.size(), yVals.size())];

				auto difX = eX ? xVals[x + 1] - xVals[x] : 1;
				auto difY = eY ? yVals[y + 1] - yVals[y] : 1;
				auto difZ = eZ ? zVals[z + 1] - zVals[z] : 1;
				auto v = getVol(b001, b010, b011, b100, b101, b110, b111, difX, difY, difZ);
				//print("{} - ({}, {}, {}), ({}, {}, {})\n", v, x, y, z, difX, difY, difZ);
				task2 += v;
			}
		}
	}

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};