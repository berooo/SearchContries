#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "countryInfo.h"

using namespace std;


// reads 1 line of the given input data file and returns all the pieces
void readline(string s, string res[]) {

	int r = 0;
	//split strings with '\t' as a delimiter
	for (int i = 0; s.length() - i>0; i++) {

		if (s[i] == '\t') {

			if (i == 0) {
				res[r] = "";

			}
			else {
				res[r] = s.substr(0, i);

			}
			s = s.substr(i + 1);
			i = -1;
			r++;
		}
	}
	res[18] = s;

}
//desplay information
string desplay(string s) {
	string result;
	string res[19];
	string attributes[19] = { "ISO","ISO3","ISO Numeric","Fips","Country","Capital","Area","Population","Continent","TopLevelDomain","Currency Code","Currency Name","Phone","Postal Code","Postal Code Regex","Languages","Geonameid","Neighbours","Equivalent Fips Code" };
	readline(s, res);
	for (int i = 0; i < 19; i++) {
		result += attributes[i] + ":\t" + res[i] + "\n\t";
	}
	return result;
}
//country function
string country(string s, string file) {
	string result;
	string command;
	stringstream ss;
	ss << s;
	while (ss >> command) {
		if (command != "country") {
			ifstream ifile;
			ifile.open(file);
			string str;
			while (getline(ifile, str)) {
				if (str[0] != '#') {
					string res[19];
					readline(str, res);
					if (res[0] == command) {
						result += desplay(str);
					}
				}

			}
			ifile.close();
		}
	}
	if (result.length() == 0) {
		result = "Country " + command + " not found.\n";
	}

	return result;
}

//neighbor function
string neighbor(string s, string file) {
	string result;
	string command[2];
	stringstream ss;
	ss << s;
	for (int i = 0; i < 2; i++) {
		ss >> command[i];
	}
	ifstream ifile;
	ifile.open(file.data());
	string str;
	int count = 0;
	while (getline(ifile, str)) {
		if (str[0] != '#') {
			string res[19];
			readline(str, res);
			string neighbors = res[17];
			//split strings with ',' as a delimiter
			for (int i = 0; neighbors.length() > 0; i++) {
				if (neighbors[i] == ',') {
					string nstr = neighbors.substr(0, i);
					if (nstr == command[1]) {
						count++;
						result += "Neighbor " + to_string(count) + "\n\t";
						result += desplay(str);
						break;
					}
					neighbors = neighbors.substr(i + 1);
					i = -1;
				}
				else if (i == neighbors.length() - 1) {
					if (neighbors == command[1]) {
						count++;
						result += "Neighbor " + to_string(count) + "\n\t";
						result += desplay(str);
					}
					break;

				}
			}
		}
	}
	if (result.length() == 0) {
		result = "Country " + command[1] + " has no neighbors.";
	}

	ifile.close();
	return result;
}
//size function
string size(string s, string file) {
	string result;
	string command[2];
	stringstream ss;
	ss << s;
	for (int i = 0; i < 2; i++) {
		ss >> command[i];
	}
	ifstream infile;
	infile.open(file.data());
	string res[19];
	if (command[1] == "largest") {
		float largest = 0;
		string str;
		string flag;
		while (getline(infile, str))
		{
			if (str[0] != '#') {
				readline(str, res);
				string areas = res[6];
				//transfer string to float
				float areaf = atof(areas.c_str());
				if (areaf > largest) {
					largest = areaf;
					flag = str;
				}
			}

		}
		result = desplay(flag);
	}
	else if (command[1] == "smallest") {
		float smallest = (float)INT_MAX;
		string str;
		string flag;
		while (getline(infile, str))
		{
			if (str[0] != '#') {
				readline(str, res);
				string areas = res[6];
				float areaf = atof(areas.c_str());
				if (areaf < smallest) {
					smallest = areaf;
					flag = str;
				}
			}
		}
		result = desplay(flag);
	}
	else if (command[1] == "average") {
		float sum = 0;
		int count = 0;
		string str;
		while (getline(infile, str))
		{
			if (str[0] != '#') {
				readline(str, res);
				string areas = res[6];
				float areaf = atof(areas.c_str());
				sum += areaf;
				count += 1;
			}
		}
		float average = sum / count;
		result = "Sum:" + to_string(sum) + "\nCountry count:" + to_string(count) + "\nAverage Size:" + to_string(average) + "\n";
	}

	infile.close();
	return result;
}
void countryInfo(string input, string& output)
{
	string filepath = "";

	output = "";
	for (int i = 0; input.length()>0; i++) {
		if (input[i] == '\n') {
			string str = input.substr(0, i);
			stringstream mmp;
			string s;
			if (str.length() > 0) {
				switch (str[0])
				{
				case 'n':
					output += '\n' + str + '\n' + neighbor(str, filepath);
					break;
				case 's':
					output += '\n' + str + '\n' + size(str, filepath);
					break;
				case 'c':
					output += '\n' + str + '\n' + country(str, filepath);
					break;
				case 'D':
					output += str + '\n';
					mmp << str;
					while (mmp >> s) {
						int length = s.length();
						if (length > 4 && s.substr(length - 4) == ".txt") {
							filepath = s;
						}
					}
					break;
				default:
					break;
				}
			}
			input = input.substr(i + 1);
			i = -1;
		}
	}

	if (input.length() > 0) {
		switch (input[0])
		{
		case 'n':
			output += '\n' + input + '\n' + neighbor(input, filepath);
			break;
		case 's':
			output += '\n' + input + '\n' + size(input, filepath);
			break;
		case 'c':
			output += '\n' + input + '\n' + size(input, filepath);
			break;
		default:
			break;
		}
	}
}