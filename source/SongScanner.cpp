#include "SongScanner.h"

//alias to make code shorter (and easier to read)
namespace fs = std::filesystem;

//recursive scan inside folders to find songs 
void checkFolder(fs::path p, std::vector<SongEntry>&list) {
	for (const fs::directory_entry entry : fs::directory_iterator(p)) {
		//if there's a sub-directory, repeat the scan inside that sub-directory
		if (fs::is_directory(entry)) {
			checkFolder(entry.path(), list);
		}
		//if it's not a directory, check for info.txt
		std::string file = p.generic_string() + std::string("/info.txt");
		fs::path filePath(file);
		if (fs::exists(filePath)) {
			std::ifstream fileData(file);

			std::string s1;
			std::string s2;
			std::string a1;
			std::string a2;
			std::string charter;
			std::string mixer;
			int dTrack;
			int dTap;
			int dCrossfade;
			int dScratch;
			
			std::getline(fileData, s1);
			std::getline(fileData, s2);
			std::getline(fileData, a1);
			std::getline(fileData, a2);
			std::getline(fileData, charter);
			std::getline(fileData, mixer);

			std::string token;
			fileData >> token;
			dTrack = std::stoi(token);

			fileData >> token;
			dTap = std::stoi(token);

			fileData >> token;
			dCrossfade = std::stoi(token);

			fileData >> token;
			dScratch = std::stoi(token);

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

			SongEntry t = {
				p.generic_string(),
				s1, s2,
				a1, a2,
				charter, mixer,
				dTrack, dTap,
				dCrossfade, dScratch
			};

			list.push_back(t);
			std::cout << "found " << file << std::endl;
			break;
		}
	}
}


void SongScanner::load(const std::string& rootPath,std::vector<SongEntry>& list){
	fs::path root(rootPath);
	if (fs::is_directory(root)) {
		//the root is a directory, so start scanning
		checkFolder(root, list);
	}
	else {
		std::cerr << "SongScanner error: Root path is not a folder" << std::endl;
	}
}
