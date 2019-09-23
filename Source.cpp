// CPSC 323
// Team members: 
//    - Omar Al Nabulsi
//    - Duc Huynh
//    - Victoria Naranjo
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
// operator list
char OPERATORS[21] = { '*','+','-','=','/','>','<','%','\'','(',')','{','}','[',']',',','.',':',';','!',' ' };
// keyword list
string KEYWORDS[20] = { "int", "float", "bool", "if", "else", "then", "get", "while", "otherwise", "do", "fi", "for", "and", "or" , "return", "put", "function" };
// integer list
int INTEGERS[10] = { 0,1,2,3,4,5,6,7,8,9 };
// FSM
int FSM[6][7] = {
 1,2,4,0,0,0,0,
 1,1,4,0,0,0,0,
 5,2,4,0,3,0,0,
 5,3,4,0,5,0,0,
 4,4,0,4,4,4,4,
 0,0,0,0,0,0,0
};
/*
Function to navigate through the State Transition table.
Given the specific character being currently read from the file, the state will
change according to the table. Table is initialized in global scope but can be
referenced as
	 char num '!' sep '.' ops '\n'
new(0)   1, 2, 4, 0, 0, 0, 0
string(1)  1, 1, 4, 0, 0, 0, 0
int(2)   5, 2, 4, 0, 3, 0, 0
float(3)  5, 3, 4, 0, 5, 0, 0
comment(4)  4, 4, 0, 4, 4, 4, 4
invalid(5)  0, 0, 0, 0, 0, 0, 0
*/
int testChar(char ch, int state) {
	int present_state = state;
	if (isalpha(ch)) {
		present_state = FSM[present_state][0];
		return present_state;
	}
	else if (isdigit(ch)) {
		present_state = FSM[present_state][1];
		return present_state;
	}
	else if (ch == OPERATORS[19]) {
		present_state = FSM[present_state][2];
		return present_state;
	}
	else if (ch == OPERATORS[15]) {
		present_state = FSM[present_state][4];
		return present_state;
	}
	else if (isspace(ch)) {
		present_state = FSM[present_state][6];
	}
	else {
		int i = 0;
		for (i = 0; i < 8; i++) {
			if (ch == OPERATORS[i] || isspace(ch)) {
				present_state = FSM[present_state][5];
				return present_state;
			}
		}
		for (i = 8; i < 22; i++) {
			if (ch == OPERATORS[i] || isspace(ch)) {
				present_state = FSM[present_state][3];
				return present_state;
			}
		}
	}
}
// pre defined function
void showTemp(string temp);
void showChar(char ch);
string oFile = "output.txt";
vector<pair<string, string>> output;
int main() {
	string filename, input = "", temp;
	int currentState = 0, stringlength = 0;
	char ch;
	// ask user for input file
	cout << "What is the file name?: "; cin >> filename;
	ifstream inFile(filename);

	// open file
	if (!inFile.is_open()) {
		cout << "Cannot open input file" << endl;
		return 1;
	}
	ofstream outFile(oFile);
	if (!outFile.is_open()) {
		cout << "Cannot open output file" << endl;
		return 1;
	}
	cout << "Token\t\tLexeme" << endl;
	cout << "-------------------" << endl;
	outFile << "Token\t\tLexeme" << endl
		<< "--------------------------" << endl;
	// loop until no more lines to read from the input file
	while (getline(inFile, input)) {
		stringlength = input.length();
		for (int i = 0; i < stringlength; i++) {
			ch = input[i];
			if (currentState != 4) {
				currentState = testChar(ch, currentState); //State Transition Call
				switch (currentState) {
					// when a seperator or operator is entered
				case 0:
					showTemp(temp);
					showChar(ch);
					temp = "";
					break;
					// concatenate as long as valid input
				case 1:
					temp += ch;
					break;
				case 2:
					temp += ch;
					break;
				case 3:
					temp += ch;
					break;
					// invalid inputs
				case 5:
					showTemp(temp);
					showChar(ch);
					temp = "";
					break;
				}
			}
		}
		showTemp(temp);
		temp = "";
	}
	// close file
	inFile.close();
	for (int i = 0; i < output.size(); i++) {
		cout << output[i].first << "\t" << output[i].second << endl;
		outFile << output[i].first << "\t" << output[i].second << endl;
	}
	outFile.close();
	system("pause");
	return 0;
}
/*
This function called when a seperator, operator, or invalid word
is entered. Once entered, showTemp() will test the variable input and decide
whether it is a keyword, identifier, integer(not implemented), real integer(not implemented), or default to invalid
if all others are not found.
*/
void showTemp(string temp) {
	if (temp != "") {
		bool is_keyword = 0;
		for (int i = 0; i < 20; i++) {
			if (KEYWORDS[i] == temp) {
				output.push_back(pair<string, string>("Keyword ", temp));
				return;
			}
		}
		is_keyword = 1;
		if (isalpha(temp[0]) && is_keyword == 1) {
			output.push_back(pair<string, string>("Identifier ", temp));
			return;
		}
		bool is_integer = false;
		for (int i = 0; i < temp.length(); i++) {
			if (isdigit(temp[i])) {
				is_integer = true;
				continue;
			}
			else {
				is_integer = false;
			}
		}
		if (is_integer) {
			output.push_back(pair<string, string>("Integer ", temp));
			return;
		}
		int test_float = 0;
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] == '.') {
				test_float = 1;
				break;
			}
		}
		output.push_back(pair<string, string>("Invalid ", temp));
		return;
	}
}
/*
This function is called when a seperator, operator, or invalid word
is entered. Once entered it will test the variable to see whether
the input was a Separator or Operator WITH EXCEPTION to whitespace
*/
void showChar(char ch) {
	string temp = string(1, ch);
	for (int i = 0; i < 8; i++) {
		if (ch == OPERATORS[i]) {
			output.push_back(pair<string, string>("Operator ", temp));
		}
	}
	for (int i = 8; i < 22; i++) {
		if (ch == OPERATORS[i] && !isspace(ch)) {
			output.push_back(pair<string, string>("Separator ", temp));
		}
	}
}
