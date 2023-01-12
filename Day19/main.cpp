#include "./../../../shared/rtpcore.h"
#include "./../../../shared/maths/RTPMath.h"
#include <algorithm>
#include <fstream>
#include <charconv>

#include "../sharedAoC.h"
#define brase break; case

//Regular Input:
//Task 1: 308
//Task 2: 12124
//Alternative Input:
//Task 1: 512
//Task 2: 16802
//Small Input:
//Task 1: 79
//Task 2: 3621

struct Sensor{
	Array<vec3i> beacons{};
	using Trans = tuple<int, vec3i, vec3i>;

	static int inv(int i){
		switch(i){
			brase 0: return 0; //xyz
			brase 1: return 3; //nyxz
			brase 2: return 2; //nxnyz
			brase 3: return 1; //ynxz

			brase 4: return 4; //yxnz
			brase 5: return 5; //nxynz
			brase 6: return 6; //nynxnz
			brase 7: return 7; //xnynz

			brase 8: return 16; //zxy
			brase 9: return 9; //nxzy
			brase 10: return 21; //nznxy
			brase 11: return 12; //xnzy

			brase 12: return 11; //xzny
			brase 13: return 23; //nzxny
			brase 14: return 14; //nxnzny
			brase 15: return 18; //znxny

			brase 16: return 8; //yzx
			brase 17: return 20; //nzyx
			brase 18: return 15; //nynzx
			brase 19: return 19; //znyx

			brase 20: return 17; //zynx
			brase 21: return 10; //nyznx
			brase 22: return 22; //nznynx
			brase 23: return 13; //ynznx
		}
		return -1;
	};

	void apply(Trans tran){
		switch(std::get<0>(tran)){
			brase 0: for(auto& b : beacons){ b -= std::get<1>(tran); b = b.xyz(); b += std::get<2>(tran); }
			brase 1 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nyxz(); b += std::get<2>(tran); }
			brase 2 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nxnyz(); b += std::get<2>(tran); }
			brase 3 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.ynxz(); b += std::get<2>(tran); }

			brase 4 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.yxnz(); b += std::get<2>(tran); }
			brase 5 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nxynz(); b += std::get<2>(tran); }
			brase 6 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nynxnz(); b += std::get<2>(tran); }
			brase 7 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.xnynz(); b += std::get<2>(tran); }

			brase 8 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.zxy(); b += std::get<2>(tran); }
			brase 9 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nxzy(); b += std::get<2>(tran); }
			brase 10 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nznxy(); b += std::get<2>(tran); }
			brase 11 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.xnzy(); b += std::get<2>(tran); }

			brase 12 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.xzny(); b += std::get<2>(tran); }
			brase 13 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nzxny(); b += std::get<2>(tran); }
			brase 14 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nxnzny(); b += std::get<2>(tran); }
			brase 15 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.znxny(); b += std::get<2>(tran); }

			brase 16 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.yzx(); b += std::get<2>(tran); }
			brase 17 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nzyx(); b += std::get<2>(tran); }
			brase 18 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nynzx(); b += std::get<2>(tran); }
			brase 19 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.znyx(); b += std::get<2>(tran); }

			brase 20 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.zynx(); b += std::get<2>(tran); }
			brase 21 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nyznx(); b += std::get<2>(tran); }
			brase 22 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.nznynx(); b += std::get<2>(tran); }
			brase 23 : for(auto& b : beacons){ b -= std::get<1>(tran); b = b.ynznx(); b += std::get<2>(tran); }
		}
	};

	static Trans comp(const Sensor& a, const Sensor& b, size_t threshold){
		threshold -= 1; //The points we are comparing to are forced to overlap
		ArrayC<size_t, 24> counts{};
		for(auto vai : a.beacons){
			for(auto vbi : b.beacons){
				for(auto vaj : a.beacons){
					if(vai == vaj){ continue; }
					const auto difa{vaj - vai};
					for(auto vbj : b.beacons){
						//if(vbi == vbj){ continue; }
						const auto difb{vbj - vbi};

						if(difa.L1() != difb.L1()){ continue; }

						// counts[0] += difa == difb.xyz();
						// counts[1] += difa == difb.nyxz();
						// counts[2] += difa == difb.nxnyz();
						// counts[3] += difa == difb.ynxz();
						counts[0] += difa.x == difb.x && difa.y == difb.y && difa.z == difb.z;
						counts[1] += difa.x == -difb.y && difa.y == difb.x && difa.z == difb.z;
						counts[2] += difa.x == -difb.x && difa.y == -difb.y && difa.z == difb.z;
						counts[3] += difa.x == difb.y && difa.y == -difb.x && difa.z == difb.z;

						// counts[4] += difa == difb.yxnz();
						// counts[5] += difa == difb.nxynz();
						// counts[6] += difa == difb.nynxnz();
						// counts[7] += difa == difb.xnynz();
						counts[4] += difa.x == difb.y && difa.y == difb.x && difa.z == -difb.z;
						counts[5] += difa.x == -difb.x && difa.y == difb.y && difa.z == -difb.z;
						counts[6] += difa.x == -difb.y && difa.y == -difb.x && difa.z == -difb.z;
						counts[7] += difa.x == difb.x && difa.y == -difb.y && difa.z == -difb.z;

						// counts[8] += difa == difb.zxy();
						// counts[9] += difa == difb.nxzy();
						// counts[10] += difa == difb.nznxy();
						// counts[11] += difa == difb.xnzy();
						counts[8] += difa.x == difb.z && difa.y == difb.x && difa.z == difb.y;
						counts[9] += difa.x == -difb.x && difa.y == difb.z && difa.z == difb.y;
						counts[10] += difa.x == -difb.z && difa.y == -difb.x && difa.z == difb.y;
						counts[11] += difa.x == difb.x && difa.y == -difb.z && difa.z == difb.y;

						//counts[12] += difa == difb.xzny();
						// counts[13] += difa == difb.nzxny();
						// counts[14] += difa == difb.nxnzny();
						// counts[15] += difa == difb.znxny();
						counts[12] += difa.x == difb.x && difa.y == difb.z && difa.z == -difb.y;
						counts[13] += difa.x == -difb.z && difa.y == difb.x && difa.z == -difb.y;
						counts[14] += difa.x == -difb.x && difa.y == -difb.z && difa.z == -difb.y;
						counts[15] += difa.x == difb.z && difa.y == -difb.x && difa.z == -difb.y;

						// counts[16] += difa == difb.yzx();
						// counts[17] += difa == difb.nzyx();
						// counts[18] += difa == difb.nynzx();
						// counts[19] += difa == difb.znyx();
						counts[16] += difa.x == difb.y && difa.y == difb.z && difa.z == difb.x;
						counts[17] += difa.x == -difb.z && difa.y == difb.y && difa.z == difb.x;
						counts[18] += difa.x == -difb.y && difa.y == -difb.z && difa.z == difb.x;
						counts[19] += difa.x == difb.z && difa.y == -difb.y && difa.z == difb.x;

						// counts[20] += difa == difb.zynx();
						// counts[21] += difa == difb.nyznx();
						// counts[22] += difa == difb.nznynx();
						// counts[23] += difa == difb.ynznx();
						counts[20] += difa.x == difb.z && difa.y == difb.y && difa.z == -difb.x;
						counts[21] += difa.x == -difb.y && difa.y == difb.z && difa.z == -difb.x;
						counts[22] += difa.x == -difb.z && difa.y == -difb.y && difa.z == -difb.x;
						counts[23] += difa.x == difb.y && difa.y == -difb.z && difa.z == -difb.x;
					}
				}
				for(int i = 0; auto v: counts){
					if(v >= threshold){
						return {i, vbi, vai};
					}
					++i;
					//counts[i] = 0;
				}
				counts = {};
			}
		}
		return {-1, {}, {}};
	};
};


vec3i apply(vec3i& b, Sensor::Trans tran){
	switch(std::get<0>(tran)){
		brase 0: b -= std::get<1>(tran); b = b.xyz(); b += std::get<2>(tran);
		brase 1:  b -= std::get<1>(tran); b = b.nyxz(); b += std::get<2>(tran);
		brase 2:  b -= std::get<1>(tran); b = b.nxnyz(); b += std::get<2>(tran);
		brase 3:  b -= std::get<1>(tran); b = b.ynxz(); b += std::get<2>(tran);

		brase 4: b -= std::get<1>(tran); b = b.yxnz(); b += std::get<2>(tran);
		brase 5: b -= std::get<1>(tran); b = b.nxynz(); b += std::get<2>(tran);
		brase 6: b -= std::get<1>(tran); b = b.nynxnz(); b += std::get<2>(tran);
		brase 7: b -= std::get<1>(tran); b = b.xnynz(); b += std::get<2>(tran);

		brase 8: b -= std::get<1>(tran); b = b.zxy(); b += std::get<2>(tran);
		brase 9: b -= std::get<1>(tran); b = b.nxzy(); b += std::get<2>(tran);
		brase 10: b -= std::get<1>(tran); b = b.nznxy(); b += std::get<2>(tran);
		brase 11: b -= std::get<1>(tran); b = b.xnzy(); b += std::get<2>(tran);

		brase 12: b -= std::get<1>(tran); b = b.xzny(); b += std::get<2>(tran);
		brase 13: b -= std::get<1>(tran); b = b.nzxny(); b += std::get<2>(tran);
		brase 14: b -= std::get<1>(tran); b = b.nxnzny(); b += std::get<2>(tran);
		brase 15: b -= std::get<1>(tran); b = b.znxny(); b += std::get<2>(tran);

		brase 16: b -= std::get<1>(tran); b = b.yzx(); b += std::get<2>(tran);
		brase 17: b -= std::get<1>(tran); b = b.nzyx(); b += std::get<2>(tran);
		brase 18: b -= std::get<1>(tran); b = b.nynzx(); b += std::get<2>(tran);
		brase 19: b -= std::get<1>(tran); b = b.znyx(); b += std::get<2>(tran);

		brase 20: b -= std::get<1>(tran); b = b.zynx(); b += std::get<2>(tran);
		brase 21: b -= std::get<1>(tran); b = b.nyznx(); b += std::get<2>(tran);
		brase 22: b -= std::get<1>(tran); b = b.nznynx(); b += std::get<2>(tran);
		brase 23: b -= std::get<1>(tran); b = b.ynznx(); b += std::get<2>(tran);
	}
	return {};
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
	Array<Sensor> sens{};

	int mode = 0;
	do{
		s.erase();
		std::getline(infile, s);
		if(s.empty()){
			mode = 0;
		} else{
			if(mode == 0){
				sens.push_back({});
				mode = 1;
				continue;
			}
			vec3i v{};
			ccp c = s.c_str(), back = (&s.back() + 1);
			c = std::from_chars<int>(c, back, v.x).ptr;
			c = std::from_chars<int>(++c, back, v.y).ptr;
			c = std::from_chars<int>(++c, back, v.z).ptr;
			sens.back().beacons.push_back(v);
		}
	} while(infile);

	infile.close();

	Map<size_t, Array<pair<size_t, Sensor::Trans>>> ops{};
	Map<size_t, Array<pair<size_t, Sensor::Trans>>> opsi{};

	for(auto i : range(sens.size())){
		for(auto j : range(i + 1, sens.size())){
			auto c = Sensor::comp(sens[i], sens[j], 12);
			if(std::get<0>(c) == -1){ continue; }
			//print("{} vs. {} - {}, {}, {}\n", i, j, std::get<0>(c), std::get<1>(c).str(), std::get<2>(c).str());
			ops[i].push_back({j, c});
			ops[j].push_back({i, {Sensor::inv(std::get<0>(c)), std::get<2>(c), std::get<1>(c)}});
			//opsi[j].push_back({i, c});
		}
	}

	Queue<int> todo{};
	todo.push(0);
	Array<Array<Sensor::Trans>> trans{};
	Array<bool> done(sens.size(), false);
	Array<bool> done2(sens.size(), false);
	Array<vec3i> pos{};
	pos.resize(sens.size());
	done2[0] = true;
	Array<Sensor> newSens = sens;
	trans.resize(sens.size());

	while(!todo.empty()){
		auto i = todo.front();
		todo.pop();
		if(done[i]){ continue; }
		done[i] = true;
		if(ops.contains(i)){
			for(auto& op : ops[i]){
				if(done2[op.first]){ continue; }
				trans[op.first] = trans[i];
				trans[op.first].push_back(op.second);
				for(auto t = trans[op.first].rbegin(); t != trans[op.first].rend(); ++t){
					newSens[op.first].apply(*t);
					apply(pos[op.first], *t);
				}
				done2[op.first] = true;
				todo.push(op.first);
			}
		}
	}

	Set<vec3i> beacons{};

	for(auto& s : newSens){
		for(auto b : s.beacons){
			beacons.insert(b);
		}
	}

	task1 = beacons.size();

	task2 = 0;

	for(auto i : range(sens.size())){
		for(auto j : range(i + 1, sens.size())){
			auto pp = (pos[i] - pos[j]).L1();
			task2 = max(task2, pp);
		}
	}

	print("Task 1: {}\nTask 2: {}\n", task1, task2);
};