#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

ifstream instr, in;
ofstream out;
bool isRNAToC;
char *tempStrg;
string instructions_filename = "instructions-rna.txt";
string line, item;
map<string,string> rna2c, c2rna;
vector<string> tokens;
int tabs = 0;

string trim(string str, string chars) {
	// trim leading spaces
	size_t startpos = str.find_first_not_of(chars);
	if( string::npos != startpos )
	    str = str.substr( startpos );
	// trim trailing spaces
	size_t endpos = str.find_last_not_of(chars);
	if( string::npos != endpos )
	    str = str.substr( 0, endpos+1 );
	return str;
}

int main(int argc, const char **argv) {
	
	if(argc<4) {
		printf("\nThe RNA Programming Language -- Conversor \nUsage: %s <input_filename> <output_filename> <conversion_type> \n\n\t<conversion_type> = rna2c (default) or c2rna\n\n", argv[0]);
		
		return EXIT_FAILURE;
	}
	
	instr.open(instructions_filename.c_str());
	if(!instr.is_open()) {
		printf("Conversor error: Cannot open instructions' file.\nTerminated.\n");
		
		return EXIT_FAILURE;
	}
	
	in.open(argv[1]);
	if(!in.is_open()) {
		printf("Conversor error: Cannot open input file: %s.\nTerminated.\n", argv[1]);
		instr.close();
		
		return EXIT_FAILURE;
	}
	
	out.open(argv[2]);
	if(!out.is_open()) {
		printf("Conversor error: Cannot write to output file: %s.\nTerminated.\n", argv[2]);
		instr.close();
		in.close();
		
		return EXIT_FAILURE;
	}
	
	if (strcmp(argv[3], "c2rna") == 0) {
		printf("C to RNA conversion chosen!\n");
		isRNAToC = false;
	} else {
		printf("RNA to C conversion chosen!\n");
		isRNAToC = true;
	}
	
	while (getline(instr,line)) {
		tokens.clear();
		stringstream ss(line);
		while (getline(ss,item,'\t'))
			tokens.push_back(item);
	    rna2c[tokens[0]] = tokens[1];
		c2rna[tokens[1]] = tokens[0];
	}
	
	if (isRNAToC) {
		while (getline(in, line, ' ')) {
			string line_trimmed = trim(line, "\n\t");
			if (rna2c[line_trimmed].size() > 0) {
				if (rna2c[line_trimmed].find("}") != string::npos)
					tabs--;
				out << string(tabs, '\t') << rna2c[line_trimmed] << endl;
				if (rna2c[line_trimmed].find("{") != string::npos)
					tabs++;
			} else {
				if (line.find_first_not_of("\n\t") != string::npos)
					cout << "Problem reading command " << line << "!" << endl;
			}
		}
	} else { // C to RNA
		while (getline(in, line, '\n')) {
			string line_trimmed = trim(line, "\t ");
			if (c2rna[line_trimmed].size() > 0) {
				out << c2rna[line_trimmed] << " ";
			} else {
				if (line.find_first_not_of("\t ") != string::npos)
					cout << "Problem reading line " << line << "!" << endl;
			}
		}
	}
	printf("Conversion done!\n");
	
	instr.close();
	in.close();
	out.close();
	
	return EXIT_SUCCESS;
}