#include <iostream>
#include <string>
#include "dirent/dirent.h"
#include <stdio.h>
#include <regex>

static std::string abs_folder_path;

void process_file(const std::string& file_name) {
	// swap author - song to song - author
	// remove y2mate\.com\s-\s(.+)\s-(.+)\.mp3
	std::regex str_expr("y2mate\\.com\\s-\\s(.+)\\s-(.+)\\.mp3");
	std::smatch sm;

	if (regex_match(file_name, sm, str_expr)) {
		std::string new_name = abs_folder_path + "\\" + sm[2].str()+" - " + sm[1].str()+".mp3";
		std::string old_name = abs_folder_path + "\\" + file_name;
		rename(old_name.c_str(), new_name.c_str());
	}

	// remove target prefix
	std::regex str_expr_2("y2mate\\.com\\s-\\s(.+)");
	std::smatch sm_2;
	if (regex_match(file_name, sm_2, str_expr_2)) {
		std::string new_name = abs_folder_path + "\\" + sm_2[1].str();
		std::string old_name = abs_folder_path + "\\" + file_name;
		rename(old_name.c_str(), new_name.c_str());
	}
}


void read_folder(std::string file_path, void(*processor_func)(const std::string&)) {
	DIR* folder;
	struct dirent* entry;
	int files = 0;


	folder = opendir(file_path.c_str());
	if (folder == NULL)
	{
		perror("Unable to read directory");
	}

	while ((entry = readdir(folder)))
	{
		std::string full_name = file_path+"\\" + entry->d_name;

		std::regex str_expr(".+(\.mp3)");
		if (regex_match(entry->d_name, str_expr)) {
			processor_func(entry->d_name);
			//std::cout << "string:object => matched\n";
			//std::cout << full_name << std::endl; // entry->d_name;
		}
	}

	closedir(folder);
}


int main(int argc, char* argv[]) {
	abs_folder_path = "./";
	read_folder(abs_folder_path, process_file);
	std::cout << "done!" << std::endl;
	std::cin.get();
}