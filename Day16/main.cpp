#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

#define brase break; case
using namespace RTP;

//Regular Input:
//Task 1: 977
//Task 2: 101501020883
//Alternative Input:
//Task 1: 1002
//Task 2: 1673210814091
//Small Input:
//Task 1: 
//Task 2: 

struct Packet{
	byte version{};
	byte id{};

	uint64 literal{};

	Array<Packet> children{};

	static uint64 fromBits(const Array<bool>& bits, size_t offset, size_t size){
		uint64 res = 0;
		for(auto i = offset; i < offset + size; ++i){
			res <<= 1;
			res |= bits[i];
		}
		return res;
	};

	static Packet decode(const Array<bool>& bits, size_t& index){
		Packet packet{};
		packet.version = fromBits(bits, index, 3);
		packet.id = fromBits(bits, index + 3, 3);
		index += 6;
		if(packet.id == 4){
			while(bits[index] != 0){
				packet.literal <<= 4;
				packet.literal |= fromBits(bits, index + 1, 4);
				index += 5;
			}
			packet.literal <<= 4;
			packet.literal |= fromBits(bits, index + 1, 4);
			index += 5;
		} else{
			byte lengthType = bits[index++];
			if(lengthType == 0){
				size_t length = fromBits(bits, index, 15);
				index += 15;
				auto end = index + length;
				while(index < end){
					packet.children.push_back(decode(bits, index));
				}
			} else{
				size_t length = fromBits(bits, index, 11);
				index += 11;
				for(auto i : range(length)){
					packet.children.push_back(decode(bits, index));
				}
			}
		}

		return packet;
	};

	static Packet decode(const Array<bool>& bits){
		size_t s = 0;
		return decode(bits, s);
	};

	static uint64 verSum(const Packet& packet){
		uint64 sum = packet.version;
		for(auto& p : packet.children){
			sum += verSum(p);
		}
		return sum;
	};

	static uint64 exec(const Packet& packet){
		switch(packet.id){
			brase 0: return sum(packet.children, exec);
			brase 1: return prod(packet.children, exec);
			brase 2: return min(packet.children, exec);
			brase 3: return max(packet.children, exec);
			brase 4: return packet.literal;
			brase 5: return exec(packet.children[0]) > exec(packet.children[1]);
			brase 6: return exec(packet.children[0]) < exec(packet.children[1]);
			brase 7: return exec(packet.children[0]) == exec(packet.children[1]);
		}
		return{};
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
	uint64 task1 = 0;
	uint64 task2 = 0;

	//Task start

	Array<bool> bits;

	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			for(auto c : s){
				switch(c){
					case '0': bits.push_back(0); bits.push_back(0); bits.push_back(0); bits.push_back(0); break;
					case '1': bits.push_back(0); bits.push_back(0); bits.push_back(0); bits.push_back(1); break;
					case '2': bits.push_back(0); bits.push_back(0); bits.push_back(1); bits.push_back(0); break;
					case '3': bits.push_back(0); bits.push_back(0); bits.push_back(1); bits.push_back(1); break;
					case '4': bits.push_back(0); bits.push_back(1); bits.push_back(0); bits.push_back(0); break;
					case '5': bits.push_back(0); bits.push_back(1); bits.push_back(0); bits.push_back(1); break;
					case '6': bits.push_back(0); bits.push_back(1); bits.push_back(1); bits.push_back(0); break;
					case '7': bits.push_back(0); bits.push_back(1); bits.push_back(1); bits.push_back(1); break;
					case '8': bits.push_back(1); bits.push_back(0); bits.push_back(0); bits.push_back(0); break;
					case '9': bits.push_back(1); bits.push_back(0); bits.push_back(0); bits.push_back(1); break;
					case 'A': bits.push_back(1); bits.push_back(0); bits.push_back(1); bits.push_back(0); break;
					case 'B': bits.push_back(1); bits.push_back(0); bits.push_back(1); bits.push_back(1); break;
					case 'C': bits.push_back(1); bits.push_back(1); bits.push_back(0); bits.push_back(0); break;
					case 'D': bits.push_back(1); bits.push_back(1); bits.push_back(0); bits.push_back(1); break;
					case 'E': bits.push_back(1); bits.push_back(1); bits.push_back(1); bits.push_back(0); break;
					case 'F': bits.push_back(1); bits.push_back(1); bits.push_back(1); bits.push_back(1); break;
				}
			}
		}
	} while(infile);

	infile.close();

	auto p = Packet::decode(bits);

	task1 = Packet::verSum(p);
	task2 = Packet::exec(p);

	print("Task 1: {}\nTask 2: {}\n", task1, task2);

};