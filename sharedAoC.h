#pragma once
#include "./../../shared/rtpcore.h"

namespace AoC{
	auto init(int argc, char** argv, bool extra = false){
		char* file;
		if(argc == 2 || (argc >= 2 && extra)){
			file = argv[1];
		} else if(argc < 2){
			print("A path is required.\n");
			exit(-1);
		} else{
			print("Too many arguments only a path is required.");
			exit(-1);
		}
		std::ifstream infile(file); //std::ios::binary | std::ios::ate);
		if(!infile){
			print("Bad file path: {}\n", file);
			exit(-1);
		} else if(infile.peek() == std::ifstream::traits_type::eof()){
			print("Empty file: {}", file);
			exit(0);
		}

		return infile;
	};

	template <typename T>
	auto toArray(size_t prealloc, std::ifstream& infile){
		string s{30};

		Array<T> res(prealloc);

		do{
			//s.erase();
			std::getline(infile, s);
			if(s.empty()){
			} else{
				res.push_back(atoi(s.data()));
			}
		} while(infile);

		return res;
	};
};