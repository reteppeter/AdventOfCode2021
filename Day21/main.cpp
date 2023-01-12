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

using vec2s = RTP::Maths::Numerics::Vector2<size_t>;

vec2s getVal(auto mem, vec2s ind, int mode){
	if((ind.x > 17) && (mode == 0)){
		return {1, 0};
	} else if((ind.y > 17) && (mode == 1)){
		return {0, 1};
	}
	return mem[ind.x][ind.y][!mode];
};

auto solve(int p1, int p2){
	auto mem = new vec2s[21][21][2]();
	for(int x = 20; x >= 0; --x){
		for(int y = 20; y >= x; --y){
			mem[x][y][0] =
				getVal(mem, vec2s{x + 9, y}, 0)
				+ 3 * getVal(mem, vec2s{x + 8, y}, 0)
				+ 6 * getVal(mem, vec2s{x + 7, y}, 0)
				+ 7 * getVal(mem, vec2s{x + 6, y}, 0)
				+ 6 * getVal(mem, vec2s{x + 5, y}, 0)
				+ 3 * getVal(mem, vec2s{x + 4, y}, 0)
				+ getVal(mem, vec2s{x + 3, y}, 0);
			mem[y][x][1] =
				getVal(mem, vec2s{y, x + 9}, 1)
				+ 3 * getVal(mem, vec2s{y, x + 8}, 1)
				+ 6 * getVal(mem, vec2s{y, x + 7}, 1)
				+ 7 * getVal(mem, vec2s{y, x + 6}, 1)
				+ 6 * getVal(mem, vec2s{y, x + 5}, 1)
				+ 3 * getVal(mem, vec2s{y, x + 4}, 1)
				+ getVal(mem, vec2s{y, x + 3}, 1);
			mem[y][x][0] =
				getVal(mem, vec2s{y + 9, x}, 0)
				+ 3 * getVal(mem, vec2s{y + 8, x}, 0)
				+ 6 * getVal(mem, vec2s{y + 7, x}, 0)
				+ 7 * getVal(mem, vec2s{y + 6, x}, 0)
				+ 6 * getVal(mem, vec2s{y + 5, x}, 0)
				+ 3 * getVal(mem, vec2s{y + 4, x}, 0)
				+ getVal(mem, vec2s{y + 3, x}, 0);
			mem[x][y][1] =
				getVal(mem, vec2s{x, y + 9}, 1)
				+ 3 * getVal(mem, vec2s{x, y + 8}, 1)
				+ 6 * getVal(mem, vec2s{x, y + 7}, 1)
				+ 7 * getVal(mem, vec2s{x, y + 6}, 1)
				+ 6 * getVal(mem, vec2s{x, y + 5}, 1)
				+ 3 * getVal(mem, vec2s{x, y + 4}, 1)
				+ getVal(mem, vec2s{x, y + 3}, 1);
			print("({}, {}) -> {}, ({}, {}) -> {}\n", x, y, mem[x][y][0].str(), y, x, mem[x][y][1].str());
		}
	}
	return mem[p1][p2][0];
};

FORCE_INLINE void inc10(int& p){ p = p == 10 ? 1 : p + 1; };

vec2s dirac1(int p1, int p2, int s1, int s2);

vec2s dirac2(int p1, int p2, int s1, int s2){
	vec2s w{};
	inc10(p2);
	inc10(p2);
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 1}; } else{ w += 1 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 3}; } else{ w += 3 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 6}; } else{ w += 6 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 7}; } else{ w += 7 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 6}; } else{ w += 6 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 3}; } else{ w += 3 * dirac1(p1, p2, s1, s2 + p2); }
	inc10(p2); if(s2 + p2 >= 21){ w += vec2s{0, 1}; } else{ w += 1 * dirac1(p1, p2, s1, s2 + p2); }
	return w;
};

vec2s dirac1(int p1, int p2, int s1, int s2){
	vec2s w{};
	inc10(p1);
	inc10(p1);
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{1, 0}; } else{ w += 1 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{3, 0}; } else{ w += 3 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{6, 0}; } else{ w += 6 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{7, 0}; } else{ w += 7 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{6, 0}; } else{ w += 6 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{3, 0}; } else{ w += 3 * dirac2(p1, p2, s1 + p1, s2); }
	inc10(p1); if(s1 + p1 >= 21){ w += vec2s{1, 0}; } else{ w += 1 * dirac2(p1, p2, s1 + p1, s2); }
	return w;
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
	int p1 = 0;
	int p2 = 0;

	int mode = 0;
    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			if(mode == 0){
				p1 = s[28] - '0';
				++mode;
			}else{
				p2 = s[28] - '0';
			}
        }
    } while(infile);

    infile.close();

	print("Start: {}, {}\n", p1, p2);

	int dice100 = 1;
	int d1 = 0;
	int d2 = 0;
	int m1 = 0;
	int m2 = 0;
	int s1 = 0;
	int s2 = 0;

	auto op1 = p1;
	auto op2 = p2;

	for(;;){
		m1 = 0;
		m2 = 0;
		d1 += 3;
		m1 += dice100;
		dice100 = (dice100) % 100 + 1;
		m1 += dice100;
		dice100 = (dice100) % 100 + 1;
		m1 += dice100;
		dice100 = (dice100) % 100 + 1;
		m1 = (m1 - 1) % 10 + 1;
		p1 = (p1 - 1 + m1) % 10 + 1;
		s1 += p1;
		if(s1 >= 1000){
			task1 = s2 * (d1 + d2);
			break;
		}
		d2 += 3;
		m2 += dice100;
		dice100 = (dice100) % 100 + 1;
		m2 += dice100;
		dice100 = (dice100) % 100 + 1;
		m2 += dice100;
		dice100 = (dice100) % 100 + 1;
		m2 = (m2 - 1) % 10 + 1;
		p2 = (p2 - 1 + m2) % 10 + 1;
		s2 += p2;
		if(s2 >= 1000){
			task1 = s1 * (d1 + d2);
			break;
		}
	}

	//auto w = dirac1(op1, op2, 0, 0);
	ArrayC<vec2s, 7> counts;

	#pragma omp parallel for
	for(size_t i = 0; i < 7; ++i){
		int np1 = (op1 + 2 + i) % 10 + 1;
		counts[i] = dirac2(np1, op2, np1, 0);
	}

	vec2s w = counts[0] + 3 * counts[1] + 6 * counts[2] + 7 * counts[3] + 6 * counts[4] + 3 * counts[5] + counts[6];

	print("{}, {}\n", w.x, w.y);

	w = solve(op1, op2);

	print("{}, {}\n", w.x, w.y);

    print("Task 1: {}\nTask 2: {}\n", task1, task2);
};