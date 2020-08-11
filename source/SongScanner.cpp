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
		//if it's not a directory, check for song.json/info.ini and chart files

		//chart
		std::string chart = p.generic_string() + std::string("/chart.xmk");
		std::string expert = p.generic_string() + std::string("/DJ_EXPERT.xmk");
		std::string hard = p.generic_string() + std::string("/DJ_HARD.xmk");
		std::string medium = p.generic_string() + std::string("/DJ_MEDIUM.xmk");
		std::string easy = p.generic_string() + std::string("/DJ_EASY.xmk");
		std::string beginner = p.generic_string() + std::string("/DJ_BEGINNER.xmk");

		int difficulties = 0;
		if (fs::exists(chart) || fs::exists(expert)) {
			difficulties |= EXPERT;
		}
		if (fs::exists(hard)) {
			difficulties |= HARD;
		}
		if (fs::exists(medium)) {
			difficulties |= MEDIUM;
		}
		if (fs::exists(easy)) {
			difficulties |= EASY;
		}
		if (fs::exists(beginner)) {
			difficulties |= BEGINNER;
		}

		//audio
		std::string redStream = p.generic_string() + std::string("/red.ogg");
		std::string greenStream = p.generic_string() + std::string("/green.ogg");
		std::string blueStream = p.generic_string() + std::string("/blue.ogg");
		std::string singleStream = p.generic_string() + std::string("/song.ogg");

		int streams = 0;
		if (fs::exists(redStream) && fs::exists(greenStream) && fs::exists(blueStream)) {
			streams = 3;
		} else if (fs::exists(singleStream)) {
			streams = 1;
		}

		//metadata
		std::string file = p.generic_string() + std::string("/song.json");
		if (fs::exists(fs::path(file))) {
			//found song.json
			std::ifstream stream(file);
			nlohmann::json root = nlohmann::json::parse(stream);

			auto s1 = root["song"]["first"]["name"].get<std::string>();
			auto s2 = root["song"]["second"]["name"].get<std::string>();
			auto a1 = root["song"]["first"]["artist"].get<std::string>();
			auto a2 = root["song"]["second"]["artist"].get<std::string>();
			//auto charter = root["song"]["charter"].get<std::string>();
			//auto mixer = root["song"]["dj"].get<std::string>();
			auto bpm = root["difficulty"]["bpm"].get<float>();
			//auto dTrack = root["difficulty"]["complexity"]["track_complexity"].get<int>();
			//auto dTap = root["difficulty"]["complexity"]["tap_complexity"].get<int>();
			//auto dCrossfade = root["difficulty"]["complexity"]["cross_complexity"].get<int>();
			//auto dScratch = root["difficulty"]["complexity"]["scratch_complexity"].get<int>();

			/*
			dTrack = std::min(dTrack, 100);
			dTrack = std::max(dTrack, 0);

			dTap = std::min(dTap, 100);
			dTap = std::max(dTap, 0);

			dCrossfade = std::min(dCrossfade, 100);
			dCrossfade = std::max(dCrossfade, 0);

			dScratch = std::min(dScratch, 100);
			dScratch = std::max(dScratch, 0);
			*/

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

			SongEntry s;
			s.path = p.generic_string();
			s.s1 = s1;
			s.s2 = s2;
			s.a1 = a1;
			s.a2 = a2;
			s.bpm = bpm;
			s.difficulties = difficulties;
			s.streams = streams;

			list.push_back(s);
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
				//std::string charter;
				//std::string mixer;
				float bpm;
				//int dTrack;
				//int dTap;
				//int dCrossfade;
				//int dScratch;

				s1 = ini.GetValue("song", "name", "NULL");
				s2 = ini.GetValue("song", "name2", "NULL");
				a1 = ini.GetValue("song", "artist", "NULL");
				a2 = ini.GetValue("song", "artist2", "NULL");
				//charter = ini.GetValue("song", "charter", "NULL");
				//mixer = ini.GetValue("song", "dj", "NULL");

				bpm = (float)ini.GetDoubleValue("song", "bpm", 60.0);

				//dTrack = ini.GetLongValue("song", "track_complexity", 0);
				//dTap = ini.GetLongValue("song", "tap_complexity", 0);
				//dCrossfade = ini.GetLongValue("song", "crossfade_complexity", 0);
				//dScratch = ini.GetLongValue("song", "scratch_complexity", 0);

				/*
				dTrack = std::min(dTrack, 100);
				dTrack = std::max(dTrack, 0);

				dTap = std::min(dTap, 100);
				dTap = std::max(dTap, 0);

				dCrossfade = std::min(dCrossfade, 100);
				dCrossfade = std::max(dCrossfade, 0);

				dScratch = std::min(dScratch, 100);
				dScratch = std::max(dScratch, 0);
				*/

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

				SongEntry s;
				s.path = p.generic_string();
				s.s1 = s1;
				s.s2 = s2;
				s.a1 = a1;
				s.a2 = a2;
				s.bpm = bpm;
				s.difficulties = difficulties;
				s.streams = streams;

				list.push_back(s);
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

	for (char c : b.s1) {
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
	if (res != 0) {
		return res < 0;
	} else {
		int res2 = a2.compare(b2);
		return res2 < 0;
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

void SongScanner::writeCache(std::vector<SongEntry>& list) {
	std::ofstream cache("songs/songCache.txt");
	if (cache.is_open()) {
		std::cout << "SongScanner Message: Writing to cache" << std::endl;
		cache << CACHE_VER << std::endl;
		cache << list.size() << std::endl;
		for (SongEntry& entry : list) {
			cache << (!entry.path.empty() ? entry.path : std::string("NULL")) << std::endl;
			cache << (!entry.s1.empty() ? entry.s1 : std::string("NULL")) << std::endl;
			cache << (!entry.s2.empty() ? entry.s2 : std::string("NULL")) << std::endl;
			cache << (!entry.a1.empty() ? entry.a1 : std::string("NULL")) << std::endl;
			cache << (!entry.a2.empty() ? entry.a2 : std::string("NULL")) << std::endl;
			//cache << (!entry.charter.empty() ? entry.charter : std::string("NULL")) << std::endl;
			//cache << (!entry.mixer.empty() ? entry.mixer : std::string("NULL")) << std::endl;
			cache << entry.bpm << std::endl;
			//cache << (entry.dTrack != -1 ? entry.dTrack : -1) << std::endl;
			//cache << (entry.dTap != -1 ? entry.dTap : -1) << std::endl;
			//cache << (entry.dCrossfade != -1 ? entry.dCrossfade : -1) << std::endl;
			//cache << (entry.dScratch != -1 ? entry.dScratch : -1) << std::endl;
			cache << entry.difficulties << std::endl;
			cache << entry.streams << std::endl;
		}
		std::cout << "SongScanner Message: updated song cache" << std::endl;
	}
}

void SongScanner::readCache(std::vector<SongEntry>& list) {
	std::ifstream cache("songs/songCache.txt");
	if (cache.is_open()) {
		list.clear();
		int ver = 0;
		int n;
		std::string token;
		std::getline(cache, token);
		ver = stoi(token);
		try {
			std::getline(cache, token);
			n = stoi(token);
			for (int i = 0; i < n; ++i) {
				std::string path;
				std::string s1;
				std::string s2;
				std::string a1;
				std::string a2;
				//std::string charter;
				//std::string mixer;
				float bpm;
				//int dTrack;
				//int dTap;
				//int dCrossfade;
				//int dScratch;
				int difficulties;
				int streams;

				std::getline(cache, token);
				path = token;
				std::getline(cache, token);
				s1 = token;
				std::getline(cache, token);
				s2 = token;
				std::getline(cache, token);
				a1 = token;
				std::getline(cache, token);
				a2 = token;
				//std::getline(cache, token);
				//charter = token;
				//std::getline(cache, token);
				//mixer = token;
				std::getline(cache, token);
				bpm = std::stof(token);
				//std::getline(cache, token);
				//dTrack = std::stoi(token);
				//std::getline(cache, token);
				//dTap = std::stoi(token);
				//std::getline(cache, token);
				//dCrossfade = std::stoi(token);
				//std::getline(cache, token);
				//dScratch = std::stoi(token);
				std::getline(cache, token);
				difficulties = std::stoi(token);
				std::getline(cache, token);
				streams = std::stoi(token);

				if (s2 == std::string("NULL")) {
					s2.clear();
				}

				if (a2 == std::string("NULL")) {
					a2.clear();
				}

				SongEntry t;
				t.path = path;
				t.s1 = s1;
				t.s2 = s2;
				t.a1 = a1;
				t.a2 = a2;
				t.bpm = bpm;
				t.difficulties = difficulties;
				t.streams = streams;

				list.push_back(t);
			}
			cache.close();
			std::cout << "SongScanner Message: loaded songs from cache" << std::endl;
		}
		catch(...){
			std::cerr << "SongScanner Error: cache is invalid/too old. Please update it"<< std::endl;
		}
	} else {
		std::cerr << "SongScanner Error: Could not open cache / Cache file not found" << std::endl;
	}
}