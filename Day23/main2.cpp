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

struct A{
	ArrayC<byte, 24> d{0};
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
			&& d[15] == o.d[15]
			&& d[16] == o.d[16]
			&& d[17] == o.d[17]
			&& d[18] == o.d[18]
			&& d[19] == o.d[19]
			&& d[20] == o.d[20]
			&& d[21] == o.d[21]
			&& d[22] == o.d[12]
			&& d[23] == o.d[23];
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
			hash_combine(res, v[16] | (v[17] << 8) | (v[18] << 16) | (v[19] << 24));
			hash_combine(res, v[20] | (v[21] << 8) | (v[22] << 16) | (v[23] << 24));
			return res;
		}
	};
};

struct State{
	inline static Map<A, int> states{};
	inline static int minCost = 12521 * 20;//std::numeric_limits<int>::max() / 10;

	A d{0};
	int cost = 0;

	FORCE_INLINE int rightFac(int s){ return 1 + (s != 0 && s != 5); }
	FORCE_INLINE int leftFac(int s){ return 1 + (s != 1 && s != 6); }
	FORCE_INLINE int moveCost(byte c){
		switch(c){
			case 1: return 1;
			case 2: return 10;
			case 3: return 100;
			case 4: return 1000;
		}
		print("Invalid value {}\n", c);
		return 0;
	};

	bool validate(){
		return (d[8] == 1) && (d[9] == 1) && (d[10] == 1) && (d[11] == 1)
			&& (d[12] == 2) && (d[13] == 2) && (d[14] == 2) && (d[15] == 2)
			&& (d[16] == 3) && (d[17] == 3) && (d[18] == 3) && (d[19] == 3)
			&& (d[20] == 4) && (d[21] == 4) && (d[22] == 4) && (d[23] == 4);
	};

	FORCE_INLINE bool validate(int i){
		return d[i] == (i - 4) / 4;
	};

	FORCE_INLINE bool validOr0(int i){
		return (d[i] == 0) || (d[i] == (i - 4) / 4);
	};

	//Check that the hallway is empty
	bool semiValidate(){
		return *reinterpret_cast<uint64*>(&d[0]) == 0;
		//return (d[0] == 0) && (d[1] == 0) && (d[2] == 0) && (d[3] == 0) && (d[4] == 0) && (d[5] == 0) && (d[6] == 0);
	};

	int getHoleTop(int i){
		return d[i] * 4 + 4;
	};

	bool canMoveRight(int i){ return i != 6 && d[i + 1] == 0; };
	bool canMoveLeft(int i){ return i != 0 && d[i - 1] == 0; };

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

	void moveRight(int i, State* back1, State* back2){
		if(i == 6 || d[i + 1] != 0){ return; };
		State s{*this};
		s.d[i + 1] = d[i];
		s.d[i] = 0;
		s.cost += rightFac(i) * moveCost(d[i]);
		//print("mr\n");
		s.next(back1, back2);
		s.moveRight(i + 1, back1, back2);;
	};

	void moveLeft(int i, State* back1, State* back2){
		if(i == 0 || d[i - 1] != 0){ return; };
		State s{*this};
		s.d[i - 1] = d[i];
		s.d[i] = 0;
		s.cost += leftFac(i) * moveCost(d[i]);
		//print("ml\n");
		s.next(back1, back2);
		s.moveLeft(i - 1, back1, back2);
	};

	void moveDownRight(int i, State* back1, State* back2){
		int top = 4 * (i - 1) + 8;
		if(d[top] != 0){ return; }
		if((d[top + 3] == 0) && (d[top + 2] == 0) && (d[top + 1] == 0)){
			d[top + 3] = d[i];
			d[i] = 0;
			cost += 5 * moveCost(d[top + 3]);
		} else if((d[top + 2] == 0) && (d[top + 1] == 0) && (d[top+3] == d[i])){
			d[top + 2] = d[i];
			d[i] = 0;
			cost += 4 * moveCost(d[top + 2]);
		} else if((d[top + 1] == 0) && (d[top+3] == d[i]) && (d[top+2] == d[i])){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if((d[top + 3] == d[i]) && (d[top+2] == d[i]) && (d[top+1] == d[i])){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return;
		}
		//print("mdr\n");
		next(back1, back2);
	};

	//TODO: Make a for loop
	void moveDownLeft(int i, State* back1, State* back2){
		int top = 4 * (i - 2) + 8;
		if(d[top] != 0){ return; }
		if((d[top + 3] == 0) && (d[top + 2] == 0) && (d[top + 1] == 0)){
			d[top + 3] = d[i];
			d[i] = 0;
			cost += 5 * moveCost(d[top + 3]);
		} else if((d[top + 2] == 0) && (d[top + 1] == 0) && (d[top+3] == d[i])){
			d[top + 2] = d[i];
			d[i] = 0;
			cost += 4 * moveCost(d[top + 2]);
		} else if((d[top + 1] == 0) && (d[top+3] == d[i]) && (d[top+2] == d[i])){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if((d[top + 3] == d[i]) && (d[top+2] == d[i]) && (d[top+1] == d[i])){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return;
		}
		//print("mdl\n");
		next(back1, back2);
	};

	void moveDown(int i, State* back1, State* back2){
		auto top = getHoleTop(i);
		if(d[top] != 0 || !((d[top+1] == 0) || validate(top+1))){ return; }
		//if(!validCol(top)){ return; }

		auto c = d[i];
		if(i <= c){
			for(auto j: range(c - i)){ if(canMoveRight(i)){ moveRight(i); } else{ return; } }
			return moveDownRight(i, back1, back2);
		} else{
			for(auto j: range(i - c - 1)){ if(canMoveLeft(i)){ moveLeft(i); } else{ return; } }
			return moveDownLeft(i, back1, back2);
		}
	};

	bool moveDownRightIP(int i, State* back1, State* back2){
		int top = 4 * (i - 1) + 8;
		if((d[top + 3] == 0) && (d[top + 2] == 0) && (d[top + 1] == 0)){
			d[top + 3] = d[i];
			d[i] = 0;
			cost += 5 * moveCost(d[top + 3]);
		} else if((d[top + 2] == 0) && (d[top + 1] == 0) && (d[top+3] == d[i])){
			d[top + 2] = d[i];
			d[i] = 0;
			cost += 4 * moveCost(d[top + 2]);
		} else if((d[top + 1] == 0) && (d[top+3] == d[i]) && (d[top+2] == d[i])){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if((d[top + 3] == d[i]) && (d[top+2] == d[i]) && (d[top+1] == d[i])){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return false;
		}
		return true;
		//print("mdr\n");
		//next(back1, back2);
	};

	//TODO: Make a for loop
	bool moveDownLeftIP(int i, State* back1, State* back2){
		int top = 4 * (i - 2) + 8;
		if((d[top + 3] == 0) && (d[top + 2] == 0) && (d[top + 1] == 0)){
			d[top + 3] = d[i];
			d[i] = 0;
			cost += 5 * moveCost(d[top + 3]);
		} else if((d[top + 2] == 0) && (d[top + 1] == 0) && (d[top+3] == d[i])){
			d[top + 2] = d[i];
			d[i] = 0;
			cost += 4 * moveCost(d[top + 2]);
		} else if((d[top + 1] == 0) && (d[top+3] == d[i]) && (d[top+2] == d[i])){
			d[top + 1] = d[i];
			d[i] = 0;
			cost += 3 * moveCost(d[top + 1]);
		} else if((d[top + 3] == d[i]) && (d[top+2] == d[i]) && (d[top+1] == d[i])){
			d[top] = d[i];
			d[i] = 0;
			cost += 2 * moveCost(d[top]);
		} else{
			return false;
		}
		return true;
		//print("mdl\n");
		//next(back1, back2);
	};


	bool moveDownIP(int i, State* back1, State* back2){
		auto top = getHoleTop(i);
		if(d[top] != 0 || !((d[top+1] == 0) || validate(top+1))){ return false; }
		//if(!validCol(top)){ return; }

		auto c = d[i];
		if(i <= c){
			for(auto j: range(c - i)){ if(canMoveRight(i)){ moveRight(i); } else{ return false; } }
			return moveDownRightIP(i, back1, back2);
		} else{
			for(auto j: range(i - c - 1)){ if(canMoveLeft(i)){ moveLeft(i); } else{ return false; } }
			return moveDownLeftIP(i, back1, back2);
		}
	};

	// bool canMoveUp(int i){
	// 	int dif = i % 4;
	// 	int start = i - dif;
	// 	switch(dif){
	// 		case 0: return !(validate(start) && validate(start + 1) && validate(start + 2) && validate(start + 3));
	// 		case 1: return (d[start] == 0) && !(validate(start + 1) && validate(start + 2) && validate(start + 3));
	// 		case 2: return (d[start] == 0) && (d[start + 1] == 0) && !(validate(start + 2) && validate(start + 3));
	// 		case 3: return (d[start] == 0) && (d[start + 1] == 0) && (d[start + 2] == 0) && !(validate(start + 3));
	// 	}
	// 	return false;
	// };

	bool canMoveUp(int i){
		int dif = i % 4;
		int start = i - dif;
		switch(dif){
			case 0: return !(validate(start) && validate(start + 1) && validate(start + 2) && validate(start + 3));
			case 1: return !(validate(start + 1) && validate(start + 2) && validate(start + 3));
			case 2: return !(validate(start + 2) && validate(start + 3));
			case 3: return !(validate(start + 3));
		}
		return false;
	};

	FORCE_INLINE bool validCol(int i){
		return validOr0(i) && validOr0(i + 1) && validOr0(i + 2) && validOr0(i + 3);
	};

	void moveUpLeft(int i, State* back1, State* back2, int dest){
		if(d[dest] != 0){ return; }
		State s{*this};
		s.d[dest] = d[i];
		s.d[i] = 0;
		s.cost += (2 + (i % 4)) * moveCost(d[i]);
		//print("mul\n");
		s.next(back1, back2);
		s.moveLeft(dest, back1, back2);
	};

	void moveUpDown(int i, State* back1, State* back2, int dest){
		if(d[dest] != 0){ return; }
		State s{*this};
		s.d[dest] = d[i];
		s.d[i] = 0;
		s.cost += (2 + (i % 4)) * moveCost(d[i]);
		//print("mul\n");
		s.moveDown(dest, back1, back2);
	};

	void moveUpRight(int i, State* back1, State* back2, int dest){
		if(d[dest] != 0){ return; }
		State s{*this};
		s.d[dest] = d[i];
		s.d[i] = 0;
		s.cost += (2 + (i % 4)) * moveCost(d[i]);
		//print("mur\n");
		s.next(back1, back2);
		s.moveRight(dest, back1, back2);
	};
	
	FORCE_INLINE void moveUp(int i, State* back1, State* back2){
		if(!canMoveUp(i)){ return; }
		int top = getHoleTop(i);
		int curTop = i - (i % 4);
		int dest = (i - 8) / 4;
		if(
			top != curTop
			&& d[top] == 0
			&& (d[top + 1] == 0 || validate(top+1))
			&& (d[top + 2] == 0 || validate(top+2))
			&& (d[top + 3] == 0 || validate(top+3))
		){
			moveUpDown(i, back1, back2, dest + 1 + (top > curTop));
		} else{
			moveUpLeft(i, back1, back2, dest + 1);
			moveUpRight(i, back1, back2, dest + 2);
		}
	};

	char toChar(byte i){
		return i == 0 ? ' ' : char(i - 1 + 'A');
	};

	void next(State* back1 = nullptr, State* back2 = nullptr){
		if(back2 && (this->d == back2->d)){ return; }
		//if(cost == 12521){
			// print("{}, {}, {}, {}, {}\n", minCost, cost, uint64(this), uint64(back1), uint64(back2));
			// print(
			// 	"#############\n"
			// 	"#{}{}.{}.{}.{}.{}{}#\n"
			// 	"###{}#{}#{}#{}###\n"
			// 	"  #{}#{}#{}#{}#  \n"
			// 	"  #{}#{}#{}#{}#  \n"
			// 	"  #{}#{}#{}#{}#  \n"
			// 	"  #########  \n\n",
			// 	toChar(d[0]), toChar(d[1]), toChar(d[2]), toChar(d[3]),
			// 	toChar(d[4]), toChar(d[5]), toChar(d[6]),
			// 	toChar(d[8]), toChar(d[12]), toChar(d[16]), toChar(d[20]),
			// 	toChar(d[9]), toChar(d[13]), toChar(d[17]), toChar(d[21]),
			// 	toChar(d[10]), toChar(d[14]), toChar(d[18]), toChar(d[22]),
			// 	toChar(d[11]), toChar(d[15]), toChar(d[19]), toChar(d[23])
			// );
		//}
		if(semiValidate() && validate()){
			minCost = min(minCost, cost);
			return;
		}
		
		if(cost >= minCost){
			return;
		}/* else if(semiValidate()){
			if(states.containsKey(d)){
				if(states[d] <= cost){ return; }
			}
			states[d] = cost;
		}*/

		// static int iter = 0;
		// print("{}, {}, {}, {}\n", states.size(), iter, cost, minCost);
		// ++iter;

		for(int i = 0; i < 7; ++i){
			if(d[i] == 0){ continue; }
			State s{*this};
			if(s.moveDownIP(i, &s, back1)){
				//moveDown(i, back1, back2);
				//d = s.d;
				//cost = s.cost;
				return s.next(this, back1);
			}
		}

		for(int i = 8; i < 24; i += 4){
			for(int j = i; j < i+4; ++j){
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
				initial.d[12] = s[5] - 'A' + 1;
				initial.d[16] = s[7] - 'A' + 1;
				initial.d[20] = s[9] - 'A' + 1;
			} else if(mode == 3){
				++mode;
				initial.d[11] = s[3] - 'A' + 1;
				initial.d[15] = s[5] - 'A' + 1;
				initial.d[19] = s[7] - 'A' + 1;
				initial.d[23] = s[9] - 'A' + 1;
			}
		}
	} while(infile);

	initial.d[9] = 'D' - 'A' + 1;
	initial.d[13] = 'C' - 'A' + 1;
	initial.d[17] = 'B' - 'A' + 1;
	initial.d[21] = 'A' - 'A' + 1;

	initial.d[10] = 'D' - 'A' + 1;
	initial.d[14] = 'B' - 'A' + 1;
	initial.d[18] = 'A' - 'A' + 1;
	initial.d[22] = 'C' - 'A' + 1;

	initial.next();
	task1 = State::minCost;

	infile.close();

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};