#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

/*class SAT{
	public:
	Array2D<byte> possible;
	
	SAT(size_t size): possible(size, size, true){};

	void reset(){
		for(auto& p: possible){ p = 0; }
	};

	template <size_t N>
	void apply(size_t index, const BitSet<N>& bits){
		for(size_t i = 0; i < N; ++i){
			possible[index + i] &= bits[i];
		}
	}

	template <size_t N>
	void apply(const BitSet<N>& index, const BitSet<N>& bits){
		size_t ij = 0;
		for(size_t j = 0; j < N; ++j){
			for(size_t i = 0; i < N; ++i, ++ij){
				possible[ij] &= ~bits[i] ^ index[j];
			}
		}
	}
};*/

template <size_t N>
class SAT{
	public:
	ArrayC<byte, N * N> possible;
	
	SAT(){
		possible.fill(true);
	};

	void apply(size_t index, const BitSet<N>& bits){
		for(size_t i = 0; i < N; ++i){
			possible[index + i] &= bits[i];
		}
	}

	void apply(const BitSet<N>& index, const BitSet<N>& bits){
		size_t ij = 0;
		for(size_t j = 0; j < N; ++j){
			for(size_t i = 0; i < N; ++i, ++ij){
				possible[ij] &= ~bits[i] ^ index[j];
			}
		}
	}
};

//Regular Input:
//Task 1: 534
//Task 2: 1070188
//Other Leaderboard Input:
//Task 1: 355
//Task 2: 983030
//Small Input:
//Task 1: 26
//Task 2: 61229

/*
g++ ./day8.cpp ../../../shared/String.cpp -O3 -std=c++20 -lfmt -Wno-attributes -fopenmp -fno-exceptions -o ./day8.exe && ./day8 ./input.txt && ./day8 ./input2.txt
*/
int main(int argc, char** argv){
    auto infile = AoC::init(argc, argv);

    //Line buffer
    string s{30};
    int64 task1 = 0;
    int64 task2 = 0;

    //Task start

	struct Line{
		using BS = BitSet<7>;
		struct Data{BS bs; size_t size; };
		//ArrayC<Data, 10> in{};
		BS one;
		BS four;
		ArrayC<Data, 4> out{};

		inline int fromValue(const BS& bs) const{
			switch(bs.to_ulong()){
				case 0b1110111: return 0;
				case 0b0100100: return 1;
				case 0b1011101: return 2;
				case 0b1101101: return 3;
				case 0b0101110: return 4;
				case 0b1101011: return 5;
				case 0b1111011: return 6;
				case 0b0100101: return 7;
				case 0b1111111: return 8;
				case 0b1101111: return 9;
			}
			return 0;
		};

		int solve(){
			//SAT<7> sat{};

			//ArrayC<int, 7> charMap{};
			
			//ArrayC<BS, 3> fiveB;
			//size_t fiveI = 0;
			//ArrayC<BS, 3> sixB;
			//size_t sixI = 0;
			//for(auto& s: in){
			//	switch(s.size){
			//		case 2: one = s.bs; sat.apply(s.bs, BS{0b0100100}); break;
			//		case 3: sat.apply(s.bs, BS{0b0100101}); break;
			//		case 4: four = s.bs; sat.apply(s.bs, BS{0b0101110}); break;
			//		case 7: break;
			//		case 5: fiveB[fiveI++] = s.bs; break;
			//		case 6: sixB[sixI++] = s.bs; break;
			//	}
			//}

			//auto shared3B5 = fiveB[0] & fiveB[1] & fiveB[2];
			//sat.apply(shared3B5, BS{0b1001001});
			//sat.apply(fiveB[0] ^ fiveB[1] ^ fiveB[2] ^ shared3B5, BS{0b0010010});
			//
			//sat.apply(sixB[0] & sixB[1] & sixB[2], BS{0b1100011});

			//size_t ip = 0;
			//for(size_t p: range(7)){
			//	for(size_t i: range(7)){
			//		if(sat.possible[ip]){
			//			charMap[p] = i;
			//		}
			//		++ip;
			//	}
			//}

			int res = 0;

			for(auto& s: out){
				res *= 10;
				switch(s.size){
					case 0: continue;
					case 2: res += 1; break;
					case 3: res += 7; break;
					case 4: res += 4; break;
					case 7: res += 8; break;
					case 5:
						if((s.bs | four) == 0b1111111){
							res += 2;
						} else if((s.bs | ~one) == 0b1111111){
							res += 3;
						} else{
							res += 5;
						}break;
					case 6:
						if((s.bs | one) == 0b1111111){
							res += 6;
						} else if((s.bs | four) == 0b1111111){
							res += 0;
						} else{
							res += 9;
						} break;
					//[[unlikely]] default:
						//BS segs{0};
						//for(size_t i: range(7)){
						//	if(s.bs.test(i)){
						//		segs.set(charMap[i]);
						//	}
						//};
						//res += fromValue(segs.to_ulong());
				}
			}
			return res;
		};

		int solve(Data s){
			switch(s.size){
				case 0: return 0;
				case 2: return 1;
				case 3: return 7;
				case 4: return 4;
				case 7: return 8;
				case 5:
					if((s.bs | four) == 0b1111111){
						return 2;
					} else if((s.bs | ~one) == 0b1111111){
						return 3;
					}
					return 5;
				case 6:
					if((s.bs | one) == 0b1111111){
						return 6;
					} else if((s.bs | four) == 0b1111111){
						return 0;
					}
					return 9;
			}
			return 0;
		};
	};

    do{
        s.erase();
        std::getline(infile, s);
        if(s.empty()){
        } else {
			Line l2{};

			ccp c = s.c_str(), back = (&s.back() + 1);
			bool other = false;
			for(;;){
				Line::Data d{};
				while(*c != ' '){	
					d.bs.set((*c) - 'a');
					++d.size;
					++c;
				}
				if(d.size == 2){
					l2.one = d.bs;
					if(other){ break; }
					other = true;
				} else if(d.size == 4){
					l2.four = d.bs;
					if(other){ break; }
					other = true;
				}
				++c;
			}
			for(;*c != '|';++c){};
			c+=2;
			while(*c != ' '){ l2.out[0].bs.set((*c) - 'a'); ++l2.out[0].size; ++c; }
			task1 += l2.out[0].size == 2 || l2.out[0].size == 3 || l2.out[0].size == 4 || l2.out[0].size == 7;
			++c;
			while(*c != ' '){ l2.out[1].bs.set((*c) - 'a'); ++l2.out[1].size; ++c; }
			task1 += l2.out[1].size == 2 || l2.out[1].size == 3 || l2.out[1].size == 4 || l2.out[1].size == 7;
			++c;
			while(*c != ' '){ l2.out[2].bs.set((*c) - 'a'); ++l2.out[2].size; ++c; }
			task1 += l2.out[2].size == 2 || l2.out[2].size == 3 || l2.out[2].size == 4 || l2.out[2].size == 7;
			++c;
			while(c < back){ l2.out[3].bs.set((*c) - 'a'); ++l2.out[3].size; ++c; }
			task1 += l2.out[3].size == 2 || l2.out[3].size == 3 || l2.out[3].size == 4 || l2.out[3].size == 7;

			task2 += 1000 * l2.solve(l2.out[0]) + 100 * l2.solve(l2.out[1]) + 10 * l2.solve(l2.out[2]) + l2.solve(l2.out[3]);
        }
    } while(infile);

    infile.close();

    print("Task 1: {}\nTask 2: {}\n",task1, task2);

};