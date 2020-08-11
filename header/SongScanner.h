#pragma once
#include "SimpleIni.h"
#include "json.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

enum diffs {
	EXPERT = 1,
	HARD = 2,
	MEDIUM = 4,
	EASY = 8,
	BEGINNER = 16
};

const int CACHE_VER = 1;
struct SongEntry {
	std::string path;
	std::string s1;
	std::string s2;
	std::string a1;
	std::string a2;
	//std::string charter;
	//std::string mixer;
	float bpm = -1;
	//int dTrack;
	//int dTap;
	//int dCrossfade;
	//int dScratch;
	int difficulties = 0;
	int streams = 0;
};

class SongScanner {
public:
	void load(const std::string& root, std::vector<SongEntry>& list);
	static void writeCache(std::vector<SongEntry>& list);
	static void readCache(std::vector<SongEntry>& list);

private:
	std::map<std::string, int> m_duplicates;
};
