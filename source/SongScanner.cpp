#include "SongScanner.h"

//alias to make code shorter (and easier to read)
namespace fs = std::filesystem;

//recursive scan inside folders to find songs
void checkFolder(const fs::path& p, std::vector<SongEntry>& list, std::map<std::string, int>& duplicates) {
	for (const fs::directory_entry& entry : fs::directory_iterator(p)) {
		//if there's a sub-directory, repeat the scan inside that sub-directory
		if (fs::is_directory(entry)) {
			checkFolder(entry.path(), list, duplicates);
		}
		//if it's not a directory, check for song.json/info.ini
		std::string file = p.generic_string() + std::string("/song.json");
		if (fs::exists(fs::path(file))) {
			//found song.json
			std::ifstream stream(file);
			nlohmann::json root = nlohmann::json::parse(stream);

			auto s1 = root["song"]["first"]["name"].get<std::string>();
			auto s2 = root["song"]["second"]["name"].get<std::string>();
			auto a1 = root["song"]["first"]["artist"].get<std::string>();
			auto a2 = root["song"]["second"]["artist"].get<std::string>();
			auto charter = root["song"]["charter"].get<std::string>();
			auto mixer = root["song"]["dj"].get<std::string>();
			auto bpm = root["difficulty"]["bpm"].get<float>();
			auto dTrack = root["difficulty"]["complexity"]["track_complexity"].get<int>();
			auto dTap = root["difficulty"]["complexity"]["tap_complexity"].get<int>();
			auto dCrossfade = root["difficulty"]["complexity"]["cross_complexity"].get<int>();
			auto dScratch = root["difficulty"]["complexity"]["scratch_complexity"].get<int>();

			dTrack = std::min(dTrack, 100);
			dTrack = std::max(dTrack, 0);

			dTap = std::min(dTap, 100);
			dTap = std::max(dTap, 0);

			dCrossfade = std::min(dCrossfade, 100);
			dCrossfade = std::max(dCrossfade, 0);

			dScratch = std::min(dScratch, 100);
			dScratch = std::max(dScratch, 0);

			if (s2 == std::string("NULL")) {
				s2.clear();
			}

			if (a2 == std::string("NULL")) {
				a2.clear();
			}

			for (SongEntry& entry : list) {
				if (entry.s1 == s1) {
					//found same text
					if (duplicates.find(s1) == duplicates.end()) {
						//duplicate not found in dictionary
						duplicates[s1] = 1;
					} else {
						//duplicate found in the dictionary
						duplicates[s1]++;
					}
					std::string temp = s1;
					temp.append(" [");
					temp.append(std::to_string(duplicates[s1]));
					temp.append("]");
					s1 = temp;
				}
			}

			SongEntry t = {
				p.generic_string(),
				s1,
				s2,
				a1,
				a2,
				charter,
				mixer,
				bpm,
				dTrack,
				dTap,
				dCrossfade,
				dScratch,
			};

			list.push_back(t);
			std::cout << "found " << file << std::endl;
			break;
		} else {
			file = p.generic_string() + std::string("/info.ini");
			if (fs::exists(fs::path(file))) {
				//found info.ini
				std::ifstream fileData(file);

				CSimpleIniA ini;
				ini.LoadFile(file.c_str());

				std::string s1;
				std::string s2;
				std::string a1;
				std::string a2;
				std::string charter;
				std::string mixer;
				float bpm;
				int dTrack;
				int dTap;
				int dCrossfade;
				int dScratch;

				s1 = ini.GetValue("song", "name", "NULL");
				s2 = ini.GetValue("song", "name2", "NULL");
				a1 = ini.GetValue("song", "artist", "NULL");
				a2 = ini.GetValue("song", "artist2", "NULL");
				charter = ini.GetValue("song", "charter", "NULL");
				mixer = ini.GetValue("song", "dj", "NULL");

				bpm = (float)ini.GetDoubleValue("song", "bpm", 60.0);

				dTrack = ini.GetLongValue("song", "track_complexity", 0);
				dTap = ini.GetLongValue("song", "tap_complexity", 0);
				dCrossfade = ini.GetLongValue("song", "crossfade_complexity", 0);
				dScratch = ini.GetLongValue("song", "scratch_complexity", 0);

				dTrack = std::min(dTrack, 100);
				dTrack = std::max(dTrack, 0);

				dTap = std::min(dTap, 100);
				dTap = std::max(dTap, 0);

				dCrossfade = std::min(dCrossfade, 100);
				dCrossfade = std::max(dCrossfade, 0);

				dScratch = std::min(dScratch, 100);
				dScratch = std::max(dScratch, 0);

				if (s2 == std::string("NULL")) {
					s2.clear();
				}

				if (a2 == std::string("NULL")) {
					a2.clear();
				}

				for (SongEntry& entry : list) {
					if (entry.s1 == s1) {
						//found same text
						if (duplicates.find(s1) == duplicates.end()) {
							//duplicate not found in dictionary
							duplicates[s1] = 1;
						} else {
							//duplicate found in the dictionary
							duplicates[s1]++;
						}

						std::string temp = s1;
						temp.append(" [");
						temp.append(std::to_string(duplicates[s1]));
						temp.append("] ");
						s1 = temp;
					}
				}

				SongEntry t = {
					p.generic_string(),
					s1,
					s2,
					a1,
					a2,
					charter,
					mixer,
					bpm,
					dTrack,
					dTap,
					dCrossfade,
					dScratch,
				};

				list.push_back(t);
				std::cout << "found " << file << std::endl;
				break;
			}
		}
	}
}

bool compareSongEntries(const SongEntry& a, const SongEntry& b) {
	std::string a1;
	std::string a2;
	std::string b1;
	std::string b2;

	for (char c : a.s1) {
		a1 += std::toupper(c);
	}

	for(char c : b.s1){
		b1 += std::toupper(c);
	}

	if (!a.s2.empty()) {
		for (char c : a.s2) {
			a2 += std::toupper(c);
		}
	}

	if (!b.s2.empty()) {
		for (char c : b.s2) {
			b2 += std::toupper(c);
		}
	}
	int res = a1.compare(b1);
	if (res > 0) {
		return false;
	} else if (res == 0) {
		int res2 = a2.compare(b2);
		if (res2 >= 0) {
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

void SongScanner::load(const std::string& rootPath, std::vector<SongEntry>& list) {
	fs::path root(rootPath);
	if (fs::is_directory(root)) {
		//the root is a directory, so start scanning
		checkFolder(root, list, m_duplicates);

		/*
		for (auto& entry : m_duplicates) {
			std::cout << entry.first << " | " << entry.second << std::endl;
		}
		*/
		std::sort(list.begin(), list.end(), compareSongEntries);
	} else {
		std::cerr << "SongScanner error: Root path is not a folder" << std::endl;
	}
}
