#include<cstdio>
#include<string>
#include<regex>
#include<cstring>
#include<dirent.h>
#include<vector>
#include<iostream>

using std::string;
using std::regex;
using std::vector;
using std::regex_match;
void print_version()
{
	printf("batchren by GrHarris\n");
	printf("version 1.0\n");
}
void print_help()
{
	printf("a batch rename tool\n");
	printf("use like this:\n");
	printf("batchren \\\\d\\.mp3 part\\\\.mp3\n");
	printf("rename 1.mp3, 2.mp3 ... to part1.mp3, part2.mp3 ...\n");
	printf("between the \\\\ is a ECMAScripts regular expression.\n");
	printf("only one \\REGULAR_EXPRESSION\\ shuold exist in the old_pattern, use \"\\\\\" in the new_pattern to refer to it.\n");
}
int do_work(const char* old_pattern, const char* new_pattern)
{
	string old_pattern_s = string(old_pattern);
	int old_l = old_pattern_s.find('\\');
	int old_r = old_pattern_s.rfind('\\');
	if(old_l == old_r || old_l == old_pattern_s.length())
	{
		printf("\\REGULAR_EXPRESSION\\ not found int old_pattern\n");
		return -1;
	}
	string old_prefix = old_pattern_s.substr(0, old_l);
	string old_suffix = old_pattern_s.substr(old_r + 1);
    string _expr = old_pattern_s.substr(old_l + 1, old_r - 1);
	regex expr = regex(_expr);
	
	string new_pattern_s = string(new_pattern);
	int new_l = new_pattern_s.find("\\\\");
	int new_r = new_pattern_s.rfind("\\\\");
	if(new_l != new_r)
	{
		printf("mutiple \"\\\\\" find in new pattern\n");
		return -1;
	}
	if(new_l == new_pattern_s.length())
	{
		printf("\"\\\\\" not find in new pattern\n");
		return -1;
	}
	string new_prefix = new_pattern_s.substr(0, new_l);
	string new_suffix = new_pattern_s.substr(new_l + 2);
	
	DIR* dir = opendir(".");
    dirent* file = readdir(dir);
    vector<string> file_list;
    while(file != nullptr)
    {
        file_list.emplace_back(file->d_name);
        file = readdir(dir);
    }
    closedir(dir);
    for(string s : file_list)
    {
        if(s.starts_with(old_prefix) && s.ends_with(old_suffix))
        {
            string mid = s.substr(old_prefix.length(), s.length() - old_suffix.length());
            if(regex_match(mid, expr))
            {
                string new_name = new_prefix + mid + new_suffix;
                std::cout << s << "=>" << new_name;
                int result = rename(s.c_str(), new_name.c_str());
                if(result)
                    std::cout << " Failed" << std::endl;
                else
                    std::cout << " Success" << std::endl;
            }
        }
    }
    return 0;
}
int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		if(strcmp(argv[0], "-v") == 0)
		{
			print_version();
			return 0;
		}
		print_help();
		if(strcmp(argv[0], "-h") == 0)
			return 0;
		else
			return -1;
	}
	else if(argc == 3 && do_work(argv[1], argv[2]) == 0)
		return 0;
	else
	{
		print_help();
		return -1;
	}
}
