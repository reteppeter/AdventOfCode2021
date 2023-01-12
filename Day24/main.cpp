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

class ALU{
	//int x, y, z, w;
	public:
	ArrayC<int64, 4> d{0};

	inline static Array<ArrayC<int, 14>> good{};
	inline static size_t bad{};

	struct Op{
		enum class Type{
			NOP,
			INP,
			ADD,
			MUL,
			DIV,
			MOD,
			EQL
		} t = Type::NOP;
		int64 a = 0;
		bool imB = false;
		int64 b = 0;

		bool isInput() const { return t == Type::INP; };
		string toStr(){
			switch(t){
				case Type::ADD: return imB ? format("add {} {}", char(a + 'w'), b) : format("add {} {}", char(a + 'w'), char(b + 'w'));
				case Type::MUL: return imB ? format("mul {} {}", char(a + 'w'), b) : format("mul {} {}", char(a + 'w'), char(b + 'w'));
				case Type::DIV: return imB ? format("div {} {}", char(a + 'w'), b) : format("div {} {}", char(a + 'w'), char(b + 'w'));
				case Type::MOD: return imB ? format("mod {} {}", char(a + 'w'), b) : format("mod {} {}", char(a + 'w'), char(b + 'w'));
				case Type::EQL: return imB ? format("eql {} {}", char(a + 'w'), b) : format("eql {} {}", char(a + 'w'), char(b + 'w'));
				case Type::INP: return format("inp {}", char(a + 'w'));
				case Type::NOP: return "nop";
			}
			return "";
		};
	};

	bool executeOp(const Op& op, int64 next){
		if(op.t == Op::Type::NOP){ return true; }
		auto& a = d[op.a];
		if(op.t == Op::Type::INP){
			a = next;
			return true;
		}
		auto b = op.b;
		if(!op.imB){ b = d[b]; }
		switch(op.t){
			brase Op::Type::ADD: a += b;
			brase Op::Type::MUL: a *= b;
			brase Op::Type::DIV: if(b == 0){ return false; } else{ a /= b; }
			brase Op::Type::MOD: if(a < 0 || b <= 0){ return false; } else{ a %= b; }
			brase Op::Type::EQL: a = (a == b);
		}
		return true;
	};
};

void run(ArrayC<int, 14> cur, const Array<ALU::Op>& ops){
	int j = 0;
	ALU alu{};
	for(int i = 0; i < ops.size(); ++i){
		if(!alu.executeOp(ops[i], cur[j])){ return; }
		if(ops[i].isInput()){
			print("{}, {}, {}, {} - {}, {}, {}\n", alu.d[0], alu.d[1], alu.d[2], alu.d[3], alu.d[3] % (alu.d[2] == 0 ? 1 : alu.d[2]), alu.d[3] % 13, alu.d[2] % 13);
			++j;
		}
		//if(ops[i].t == ALU::Op::Type::MOD){ }
	}
	print("{}, {}, {}, {} - {}, {}, {}\n", alu.d[0], alu.d[1], alu.d[2], alu.d[3], alu.d[3] % (alu.d[2] == 0 ? 1 : alu.d[2]), alu.d[3] % 13, alu.d[2] % 13);
}

bool sim(size_t ind, ArrayC<int, 14> cur, ALU alu, size_t i, const Array<ALU::Op>& ops){
	if((ind == 1 || ind == 12) && alu.d[3] > 1000){ return false; }
	if(ind >= 6 && alu.d[3] > 1000000){ return false; }
	//auto X = alu.d[3] % (alu.d[2] == 0 ? 1 : alu.d[2]);
	//auto Y = alu.d[3] % 13;
	//auto Z = alu.d[2] % 13;
	//if(ind != 0 && ((alu.d[1] != 1) || (Y != Z) || ((X == 0) && (Y != 0) && ind != 1))){ return; } 
	if(ALU::good.size() >= 1){ return false; }
	alu.executeOp(ops[i], cur[ind]);
	int ch = 0;
	for(++i; i < ops.size(); ++i){
		if(ops[i].isInput()){ break; }
		if(!alu.executeOp(ops[i], 0)){/* print("{} {} - {} {} {} {}\n", ind, i, alu.d[0], alu.d[1], alu.d[2], alu.d[3]);*/ return false; }
		if(ops[i].t == ALU::Op::Type::MOD){ ch = alu.d[1]; }
	}
	//auto c = char(ch + 'A');
	//if((c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') ^ (ind % 2)){ return; }
	//if((c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') ^ (ind % 2)){ return false; }
	//print("{}", char(ch + 'A'));
	//print("{}{}{}{}{} - {} - {}\n", ind > 0 ? "    " : "", ind > 1 ? "    " : "", ind > 2 ? "    " : "", ind > 3 ? "    " : "", char(ch + 'A'), cur[ind], alu.d[3]);
	//if(ind == 6){ print("\n"); exit(0); return; }
	if(i == ops.size()){
		if(alu.d[3] == 0){
			print("{}, {}, {}, {} - {}, {}, {}\n", alu.d[0], alu.d[1], alu.d[2], alu.d[3], alu.d[3] % (alu.d[2] == 0 ? 1 : alu.d[2]), alu.d[3] % 13, alu.d[2] % 13);
			print(
				"{}{}{}{}{}{}{}{}{}{}{}{}{}{}\n",
				cur[0], 
				cur[1], 
				cur[2], 
				cur[3],
				cur[4], 
				cur[5], 
				cur[6], 
				cur[7],
				cur[8], 
				cur[9], 
				cur[10], 
				cur[11],
				cur[12], 
				cur[13]
			);
			ALU::good.push_back(cur);
			return true;
		} else{
			++ALU::bad;
		}
		if((ALU::bad % 100000000) == 0){
			print(
				"{}{}{}{}{}{}{}{}{}{}{}{}{}{}\n",
				cur[0], 
				cur[1], 
				cur[2], 
				cur[3],
				cur[4], 
				cur[5], 
				cur[6], 
				cur[7],
				cur[8], 
				cur[9], 
				cur[10], 
				cur[11],
				cur[12], 
				cur[13]
			);
			print("{}, {}\n", ALU::good.size(), ALU::bad);
		}
		return false;
	}
	for(auto j: range(9)){
		if(sim(ind + 1, cur, alu, i, ops)){
			print("{}, {}, {}, {} - {}, {}, {}\n", alu.d[0], alu.d[1], alu.d[2], alu.d[3], alu.d[3] % (alu.d[2] == 0 ? 1 : alu.d[2]), alu.d[3] % 13, alu.d[2] % 13);
			return true;
		}
		cur[ind + 1]++;
	};
	return false;
};

/*
g++ ./main.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./solution.exe && ./solution ./input.txt && ./solution ./input2.txt
-fwhole-program -march=native
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int64 task1 = 0;
    int64 task2 = 0;

    //Task start
	Array<ALU::Op> ops{};
    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			ALU::Op op;
			switch(s[0]){
				brase 'i': op.t = ALU::Op::Type::INP;
				brase 'a': op.t = ALU::Op::Type::ADD;
				brase 'm':
					switch(s[1]){
						brase 'u': op.t = ALU::Op::Type::MUL;
						brase 'o': op.t = ALU::Op::Type::MOD;
					}
				brase 'd': op.t = ALU::Op::Type::DIV;
				brase 'e': op.t = ALU::Op::Type::EQL;
			}
			op.a = s[4] - 'w';
			if(op.t != ALU::Op::Type::INP){
				ccp c = &(s[6]);
				if(*c >= 'w' && *c <= 'z'){
					op.b = *c - 'w';
				}else{
					std::from_chars<int64>(c, (&s.back()) + 1, op.b);
					op.imB = true;
				}
			}
			ops.push_back(op);
        }
    } while(infile);

    infile.close();

	// for(auto& op: ops){
	// 	print("{}\n", op.toStr());
	// }

	// run({9,1,9,6,7,1,4,9,9,4,9,8,8,1}, ops);
	// print("\n");
	// return 0;

	//ArrayC<int, 14> cur{9,9,9,9,9,9,9,9,9,9,9,9,9,9};
	ArrayC<int, 14> cur{1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	for(auto j: range(9)){
		sim(0, cur, {}, 0, ops);
		cur[0]++;
	};

	task1 = ALU::good.size();
	if(ALU::good.size() > 0){
		print(
			"{}{}{}{}{}{}{}{}{}{}{}{}{}{}\n",
			ALU::good[0][0], 
			ALU::good[0][1], 
			ALU::good[0][2], 
			ALU::good[0][3],
			ALU::good[0][4], 
			ALU::good[0][5], 
			ALU::good[0][6], 
			ALU::good[0][7],
			ALU::good[0][8], 
			ALU::good[0][9], 
			ALU::good[0][10], 
			ALU::good[0][11],
			ALU::good[0][12], 
			ALU::good[0][13]
		);
	}
	task2 = ALU::bad;

    print("Task 1: {}\nTask 2: {}\n", task1, task2);
};