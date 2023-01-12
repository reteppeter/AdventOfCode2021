#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 14348
//Task 2: 40954
//Alternative Input:
//Task 1: 10607
//Task 2: 59071
//Small Input:
//Task 1: 
//Task 2: 

struct A{
	ArrayC<byte, 16> d{0};
	byte& operator[](size_t pos){ return d[pos]; };
	const byte& operator[](size_t pos) const{ return d[pos]; };
	bool operator==(const A& o) const{
		return d[0] == o.d[0]
			&& d[1] == o.d[1]
			&& d[2] == o.d[2]
			&& d[3] == o.d[3]
			&& d[4] == o.d[4]
			&& d[5] == o.d[5]
			&& d[6] == o.d[6]
			&& d[7] == o.d[7]
			&& d[8] == o.d[8]
			&& d[9] == o.d[9]
			&& d[10] == o.d[10]
			&& d[11] == o.d[11]
			&& d[12] == o.d[12]
			&& d[13] == o.d[13]
			&& d[14] == o.d[14]
			&& d[15] == o.d[15];
	};
};


namespace std{
	template<>
	struct hash<A>{
		std::size_t operator()(const A& v) const{
			using std::hash;
			std::size_t res = 0;
			hash_combine(res, v[0] | (v[1] << 8) | (v[2] << 16) | (v[3] << 24));
			hash_combine(res, v[4] | (v[5] << 8) | (v[6] << 16) | (v[7] << 24));
			hash_combine(res, v[8] | (v[9] << 8) | (v[10] << 16) | (v[11] << 24));
			hash_combine(res, v[12] | (v[13] << 8) | (v[14] << 16) | (v[15] << 24));
			return res;
		}
	};
};

struct State{
	inline static Map<A, int> states{};
	inline static int minCost = 19257 + 1;

	A d{0};
	int cost = 0;

	static void reset(){
		states.clear();
		minCost = 19257 + 1;
	};

	FORCE_INLINE int rightFac(int s) const{ return 1 + (s != 0 && s != 5); }
	FORCE_INLINE int leftFac(int s) const{ return 1 + (s != 1 && s != 6); }
	FORCE_INLINE int moveCost(byte c) const{
		switch(c){
			case 1: return 1;
			case 2: return 10;
			case 3: return 100;
			case 4: return 1000;
		}
		print("Invalid value {}\n", c);
		return 0;
	};

	bool validate() const{
		return (d[8] == 1) && (d[9] == 1) && (d[10] == 2) && (d[11] == 2) && (d[12] == 3) && (d[13] == 3) && (d[14] == 4) && (d[15] == 4);
	};

	bool validate(int i) const{
		return d[i] == (i - 6) / 2;
	};

	//Check that the hallway is empty
	bool semiValidate() const{
		return (d[0] == 0) && (d[1] == 0) && (d[2] == 0) && (d[3] == 0) && (d[4] == 0) && (d[5] == 0) && (d[6] == 0);
	};

	int getHoleTop(int i) const{
		return d[i] * 2 + 6;
	};

	bool canMoveRight(int i){ return !(i == 6 || d[i + 1] != 0); };
	bool canMoveLeft(int i){ return !(i == 0 || d[i - 1] != 0); };

	void moveRight(int& i){
		d[i + 1] = d[i];
		d[i] = 0;
		++i;
		cost += rightFac(i-1) * moveCost(d[i]);
	};
	void moveLeft(int& i){
		d[i - 1] = d[i];
		d[i] = 0;
		--i;
		cost += leftFac(i+1) * moveCost(d[i]);
	};

	void moveRight(int i, const State* back1, const State* back2) const{
		if(i == 6 || d[i + 1] != 0){ return; };
		State s{*this};
		s.d[i + 1] = d[i];
		s.d[i] = 0;
		s.cost += rightFac(i) * moveCost(d[i]);
		//print("mr\n");
		s.next(back1, back2);
		s.moveRight(i + 1, back1, back2);;
	};

	void moveLeft(int i, const State* back1, const State* back2) const{
		if(i == 0 || d[i - 1] != 0){ return; };
		State s{*this};
		s.d[i - 1] = d[i];
		s.d[i] = 0;
		s.cost += leftFac(i) * moveCost(d[i]);
		//print("ml\n");
		s.next(back1, back2);
		s.moveLeft(i - 1, back1, back2);
	};

	void moveDownRight(int i, const State* back1, const State* back2){
		int top = 2 * (i - 1) + 8;
		if(d[top] != 0){ return; }
		if(d[top + 1] == 0){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if(d[top + 1] == d[i]){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return;
		}
		//print("mdr\n");
		next(back1, back2);
	};

	void moveDownLeft(int i, const State* back1, const State* back2){
		int top = 2 * (i - 2) + 8;
		if(d[top] != 0){ return; }
		if(d[top + 1] == 0){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if(d[top + 1] == d[i]){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return;
		}
		//print("mdl\n");
		next(back1, back2);
	};

	void moveDown(int i, const State* back1, const State* back2){
		auto top = getHoleTop(i);
		if(d[top] != 0 || !((d[top+1] == 0) || validate(top+1))){ return; }

		if(i == 0){
			if(canMoveRight(i)){ moveRight(i); } else{ return; }
		} else if(i == 6){
			if(canMoveLeft(i)){ moveLeft(i); } else{ return; }
		}

		if(i == 1){
			if(d[i] == 1){
				return moveDownRight(i, back1, back2);
			} else{
				if(canMoveRight(i)){ moveRight(i); } else{ return; }
			}
		} else if(i == 5){
			if(d[i] == 4){
				return moveDownLeft(i, back1, back2);
				return;
			} else{
				if(canMoveLeft(i)){ moveLeft(i); } else{ return; }
			}
		}

		if(i == 2){
			if(d[i] == 1){
				return moveDownLeft(i, back1, back2);
				return;
			} else if(d[i] == 2){
				return moveDownRight(i, back1, back2);
			}
			if(canMoveRight(i)){ moveRight(i); } else{ return; }
			if(d[i] == 3){
				return moveDownRight(i, back1, back2);
			}
			if(canMoveRight(i)){ moveRight(i); } else{ return; }
			if(d[i] == 4){
				return moveDownRight(i, back1, back2);
			}
		} else if(i == 4){
			if(d[i] == 4){
				return moveDownRight(i, back1, back2);
			} else if(d[i] == 3){
				return moveDownLeft(i, back1, back2);
			}
			if(canMoveLeft(i)){ moveLeft(i); } else{ return; }
			if(d[i] == 2){
				return moveDownLeft(i, back1, back2);
			}
			if(canMoveLeft(i)){ moveLeft(i); } else{ return; }
			if(d[i] == 1){
				return moveDownLeft(i, back1, back2);
			}
		} else{
			if(d[i] == 2){
				return moveDownLeft(i, back1, back2);
			} else if(d[i] == 3){
				return moveDownRight(i, back1, back2);
			} else if(d[i] == 1){
				if(canMoveLeft(i)){ moveLeft(i); } else{ return; }
				return moveDownLeft(i, back1, back2);
			} else if(d[i] == 4){
				if(canMoveRight(i)){ moveRight(i); } else{ return; }
				return moveDownRight(i, back1, back2);
			}
		}
	};

	bool canMoveUp(int i) const{
		int dif = i % 2;
		int start = i - dif;
		switch(dif){
			case 0: return !(validate(start) && validate(start + 1));
			case 1: return (d[start] == 0) && !(validate(start + 1));
		}
		return false;
	};

	void moveUpLeft(int i, const State* back1, const State* back2, int dest) const{
		if(d[dest] != 0){ return; }
		State s{*this};
		s.d[dest] = d[i];
		s.d[i] = 0;
		s.cost += (2 + (i & 1)) * moveCost(d[i]);
		//print("mul\n");
		s.next(back1, back2);
		s.moveLeft(dest, back1, back2);
	};

	void moveUpRight(int i, const State* back1, const State* back2, int dest) const{
		if(d[dest] != 0){ return; }
		State s{*this};
		s.d[dest] = d[i];
		s.d[i] = 0;
		s.cost += (2 + (i & 1)) * moveCost(d[i]);
		//print("mur\n");
		s.next(back1, back2);
		s.moveRight(dest, back1, back2);
	};

	FORCE_INLINE void moveUp(int i, const State* back1, const State* back2) const{
		if(!canMoveUp(i)){ return; }
		int top = getHoleTop(i);
		int curTop = i - (i & 1);
		int dest = (i - 8) / 2;
		if(d[top] == 0){
			if(top <= curTop){
				moveUpLeft(i, back1, back2, dest + 1);
			}
			if(top >= curTop){
				moveUpRight(i, back1, back2, dest + 2);
			}
		} else{
			moveUpLeft(i, back1, back2, dest + 1);
			moveUpRight(i, back1, back2, dest + 2);
		}
	};

	char toChar(byte i){
		return i == 0 ? ' ' : char(i - 1 + 'A');
	};

	void next(const State* back1 = nullptr, const State* back2 = nullptr) const{
		if(back2 && (this->d == back2->d)){ return; }
		//if(cost == 12521){
			// print("{}, {}, {}, {}\n", cost, uint64(this), uint64(back1), uint64(back2));
			// print(
			// 	"#############\n"
			// 	"#{}{}.{}.{}.{}.{}{}#\n"
			// 	"###{}#{}#{}#{}###\n"
			// 	"  #{}#{}#{}#{}#  \n"
			// 	"  #########  \n\n",
			// 	toChar(d[0]), toChar(d[1]), toChar(d[2]), toChar(d[3]),
			// 	toChar(d[4]), toChar(d[5]), toChar(d[6]),
			// 	toChar(d[8]), toChar(d[10]), toChar(d[12]), toChar(d[14]),
			// 	toChar(d[9]), toChar(d[11]), toChar(d[13]), toChar(d[15])
			// );
		//}
		if(validate()){
			minCost = min(minCost, cost);
			return;
		}
		
		if(cost >= minCost){
			return;
		} else if(true || semiValidate()){
			if(states.contains(d)){
				if(states[d] <= cost){ return; }
			}
			states[d] = cost;
		}

		// static int iter = 0;
		// print("{}, {}, {}, {}\n", states.size(), iter, cost, minCost);
		// ++iter;

		for(int i = 0; i < 7; ++i){
			if(d[i] == 0){ continue; }
			State s{*this};
			s.moveDown(i, &s, back1);
		}

		// for(int i = 8; i < 16; ++i){
		// 	if(d[i] == 0){ continue; }
		// 	moveUp(i, this, back1);
		// }
		for(int i = 8; i < 16; i += 2){
			for(int j = i; j < i+2; ++j){
				if(d[j] != 0){ moveUp(j, this, back1); break; }
			}
		}
	};
};



/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
-fwhole-program -march=native
*/
int main(int argc, char** argv){
	if(argc == 1){
		int curMin = std::numeric_limits<int>::max();
		int curMax = 0;

		State initial{};

		initial.d[8] = 'A' - 'A' + 1;
		initial.d[10] = 'B' - 'A' + 1;
		initial.d[12] = 'C' - 'A' + 1;
		initial.d[14] = 'D' - 'A' + 1;

		initial.d[9] = 'A' - 'A' + 1;
		initial.d[11] = 'B' - 'A' + 1;
		initial.d[13] = 'C' - 'A' + 1;
		initial.d[15] = 'D' - 'A' + 1;
		std::sort(&initial.d[8], initial.d.d.end());
		std::next_permutation(&initial.d[8], initial.d.d.end());
		do {
			State::reset();
			initial.next();
			curMin = min(curMin, State::minCost);
			curMax = max(curMax, State::minCost);
			//print("{}{}{}{} {}{}{}{}\n", initial.d[8], initial.d[10], initial.d[12], initial.d[14], initial.d[9], initial.d[11], initial.d[13], initial.d[15]);
		} while(std::next_permutation(&initial.d[8], initial.d.d.end()));

		print("Min: {}\nMax: {}\n", curMin, curMax);

		return 0;
	}

	auto infile = AoC::init(argc, argv);

	//Line buffer
	string s{30};
	int task1 = 0;
	int task2 = 0;

	int mode = 0;
	State initial{};
	//Task start
	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			if(mode <= 1){ ++mode; continue; } else if(mode == 2){
				++mode;
				initial.d[8] = s[3] - 'A' + 1;
				initial.d[10] = s[5] - 'A' + 1;
				initial.d[12] = s[7] - 'A' + 1;
				initial.d[14] = s[9] - 'A' + 1;
			} else if(mode == 3){
				++mode;
				initial.d[9] = s[3] - 'A' + 1;
				initial.d[11] = s[5] - 'A' + 1;
				initial.d[13] = s[7] - 'A' + 1;
				initial.d[15] = s[9] - 'A' + 1;
			}
		}
	} while(infile);
	infile.close();

	initial.next();
	task1 = State::minCost;

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};