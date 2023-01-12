#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"

//Regular Input:
//Task 1: 3699
//Task 2: 4735
//Alternative Input:
//Task 1: 4417
//Task 2: 4796
//Small Input:
//Task 1: 4140
//Task 2: 3993

struct Snailfish{
	Snailfish* left = nullptr;
	int leftVal = 0;
	Snailfish* right = nullptr;
	int rightVal = 0;

	~Snailfish(){
		delete left;
		delete right;
	};

	Snailfish* copy(){
		auto* s = new Snailfish{};
		if(left == nullptr){
			s->leftVal = leftVal;
		} else{
			s->left = left->copy();
		}
		if(right == nullptr){
			s->rightVal = rightVal;
		} else{
			s->right = right->copy();
		}
		return s;
	};

	static Snailfish* loadChild(ccp& c){
		auto* res = new Snailfish{};
		++c;
		if(*c == '['){
			res->left = loadChild(c);
		} else{
			res->leftVal = *c - '0';
			++c;
		}
		if(*c != ','){ ::print("Parse error\n"); }
		++c;
		if(*c == '['){
			res->right = loadChild(c);
		} else{
			res->rightVal = *c - '0';
			++c;
		}
		++c;
		return res;
	};

	static Snailfish* load(ccp c){
		return loadChild(c);
	};

	void print(){
		::print("[");
		left == nullptr ? ::print("{}", leftVal) : left->print();
		::print(",");
		right == nullptr ? ::print("{}", rightVal) : right->print();
		::print("]");
	};

	static Snailfish* add(Snailfish* a, Snailfish* b){
		auto* res = new Snailfish{};
		res->left = a;
		res->right = b;
		return res;
	};

	int& getLeftmost(){
		return left == nullptr ? leftVal : left->getLeftmost();
	};

	int& getRightmost(){
		return right == nullptr ? rightVal : right->getRightmost();
	};

	bool explodeSub(Snailfish* lastLeft, int* lastLeftValue, Snailfish* lastRight, int* lastRightValue, int level){
		//::print("{}\n", level);
		if(level == 3){
			//::print("({}, {})\n", leftVal, rightVal);
			if(left != nullptr){
				if(right == nullptr){
					rightVal += left->rightVal;
				} else{
					right->getLeftmost() += left->rightVal;
				}

				if(lastLeft == nullptr){
					if(lastLeftValue != nullptr){
						*lastLeftValue += left->leftVal;
					}
				} else{
					lastLeft->getRightmost() += left->leftVal;
				}

				delete left;
				left = nullptr;
				leftVal = 0;

				return true;
			} else if(right != nullptr){
				if(left == nullptr){
					leftVal += right->leftVal;
				} else{
					left->getRightmost() += right->leftVal;
				}
				if(lastRight == nullptr){
					if(lastRightValue != nullptr){
						*lastRightValue += right->rightVal;
					}
				} else{
					lastRight->getLeftmost() += right->rightVal;
				}
				delete right;
				right = nullptr;
				rightVal = 0;
				return true;
			}

			return false;
		}
		if(left != nullptr){
			if(right != nullptr){
				auto a = left->explodeSub(lastLeft, lastLeftValue, right, nullptr, level + 1);
				if(a){
					return true;
				}
				auto b = right->explodeSub(left, nullptr, lastRight, lastRightValue, level + 1);
				return b;
			} else{
				auto b = left->explodeSub(lastLeft, lastLeftValue, nullptr, &rightVal, level + 1);
				return b;
			}
		} else if(right != nullptr){
			auto c = right->explodeSub(nullptr, &leftVal, lastRight, lastRightValue, level + 1);
			return c;
		}
		return false;
	};

	bool explode(){
		return explodeSub(nullptr, nullptr, nullptr, nullptr, 0);
	};

	bool split(int level){
		if(left != nullptr){
			if(left->split(level + 1)){ return true; }
		} else if(leftVal > 9){
			left = new Snailfish{};
			left->leftVal = leftVal / 2;
			left->rightVal = leftVal - left->leftVal ;
			leftVal = 0;
			return true;
		}

		if(right != nullptr){
			if(right->split(level + 1)){ return true; }
		} else if(rightVal > 9){
			right = new Snailfish{};
			right->leftVal = rightVal / 2;
			right->rightVal = rightVal - right->leftVal;
			rightVal = 0;
			return true;
		}

		return false;
	};

	void reduce(){
		for(;;){
			//print();
			//::print("\n");
			if(explode()){ continue; }
			if(split(0)){ continue; }
			break;
		}
	};

	uint64 magnitude(){
		uint64 sum = 0;
		sum += 3 * (left == nullptr ? leftVal : left->magnitude());
		sum += 2 * (right == nullptr ? rightVal : right->magnitude());
		return sum;
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
	Array<Snailfish*> snails{};

	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
		} else{
			snails.push_back(Snailfish::load(s.data()));
		}
	} while(infile);

	infile.close();

	auto* sum = snails.front()->copy();

	for(int i = 1; i < snails.size(); ++i){
		sum = Snailfish::add(sum, snails[i]->copy());
		//sum->print();
		//print("\n");
		sum->reduce();
		//sum->print();
		//print("\n");
	};

	sum->print();
	print("\n");

	task1 = sum->magnitude();

	//for(auto s : snails){
	//	s->print();
	//	print("\n");
	//}

	task2 = 0;

	for(int i = 0; i < snails.size(); ++i){
		for(int j = 0; j < snails.size(); ++j){
			auto* sum = Snailfish::add(snails[i]->copy(), snails[j]->copy());
			//sum->print();
			//print("\n");
			sum->reduce();
			task2 = max(task2, sum->magnitude());
			delete sum;
			sum = Snailfish::add(snails[j]->copy(), snails[i]->copy());
			//sum->print();
			//print("\n");
			sum->reduce();
			task2 = max(task2, sum->magnitude());
		}
	}

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};