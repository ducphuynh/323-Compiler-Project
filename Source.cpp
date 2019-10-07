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
 5,3,4,0,3,0,0, // Changed [3][4] to 3 to ensure it stays in state 3 for real numbers
 4,4,0,4,4,4,4,
 0,0,0,0,0,0,0
};
/*
Function to navigate through the State Transition table.
Given the specific character being currently read from the file, the state will
change according to the table. Table is initialized in global scope but can be
referenced as
	 char num '!' sep '.' ops '\n'
new(0)      1, 	2, 4, 	0, 0, 	0, 0
string(1)   1, 	1, 4, 	0, 0, 	0, 0
int(2)      5, 	2, 4, 	0, 3, 	0, 0
float(3)    5, 	3, 4, 	0, 5, 	0, 0
comment(4)  4, 	4, 0, 	4, 4, 	4, 4
invalid(5)  0, 	0, 0, 	0, 0, 	0, 0
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
	else if (ch == OPERATORS[16]) { // Checked for decimal point for real numbers
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
bool showChar(char ch, char ch2);
bool showchar2(char ch, char ch2);
string oFile = "output.txt";
vector<pair<string, string>> output;
int main() {
	string filename, input = "", temp;
	int currentState = 0, stringlength = 0;
	char ch, ch2;
	bool state;
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
	cout << "-----------------------" << endl;
	outFile << "Token\t\tLexeme" << endl
		<< "--------------------------" << endl;
	// loop until no more lines to read from the input file
	while (getline(inFile, input)) {
		stringlength = input.length();
		for (int i = 0; i < stringlength; i++) {
			ch = input[i];
			ch2 = input[i + 1]; // made a second char input so we can look through char1 and char2
			if (currentState != 4) {
				currentState = testChar(ch, currentState);//State Transition Call
				switch (currentState) {
					// when a seperator or operator is entered
				case 0:
					showTemp(temp);
					state=showChar(ch, ch2);
					if (state == true) {
						i++;
					}
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
					state=showChar(ch, ch2);
					if (state == true) {
						i++;
					}
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
whether it is a keyword, identifier, integer, real integer, or invalid
if all others are not found.
*/
void showTemp(string temp) {
	if (temp != "") {
		for (int i = 0; i < 20; i++) {
			if (KEYWORDS[i] == temp) {
				output.push_back(pair<string, string>("Keyword ", temp));
				return;
			}
		}
		if (isalpha(temp[0])) {
			output.push_back(pair<string, string>("Identifier ", temp));
			return;
		}
		bool isInteger = false;
		for (int i = 0; i < temp.length(); i++) {
			if (isdigit(temp[i])) {
				isInteger = true;
				continue;
			}
			else {
				isInteger = false;
				break;
			}
		}
		if (isInteger) {
			output.push_back(pair<string, string>("Integer ", temp));
			return;
		}
		bool hasDecimalPoint = false;
		int decimalPointPosition;
		for (decimalPointPosition = 0; decimalPointPosition < temp.length(); decimalPointPosition++) {
			if (temp[decimalPointPosition] == '.') {
				hasDecimalPoint = true;
				break;
			}
		}
		if (hasDecimalPoint) {
			bool lefthandIsInteger = false;
			bool righthandIsInteger = false;
			for (int i = 0; i < decimalPointPosition; i++) {
				if (isdigit(temp[i])) {
					lefthandIsInteger = true;
					continue;
				}
				else {
					lefthandIsInteger = false;
					break;
				}
			}
			for (int i = decimalPointPosition + 1; i < temp.length(); i++) {
				if (isdigit(temp[i])) {
					righthandIsInteger = true;
					continue;
				}
				else {
					righthandIsInteger = false;
					break;
				}
			}
			if (lefthandIsInteger && righthandIsInteger) {
				output.push_back(pair<string, string>("Real    ", temp));
				return;
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

bool showchar2(char ch, char ch2) { // this would go through the second char
	string temp = string(1, ch);
	string temp2 = string(1, ch2);
	string temp3 = temp + temp2; // this worked but not strcat
	int notMatch = 0; // keep count if ch2 does not match with operators
	for (int i = 0; i < 8; i++) {
		if (ch2 == OPERATORS[i]) {
			output.push_back(pair<string, string>("operator ", temp3));// gives the operators together
		} else {
			notMatch++;
		}
	}
	if (notMatch == 8) { // if this it true then it will only pushback ch1 that was the only operator
		output.push_back(pair<string, string>("Operator ", temp));
		return false;
	} else 
		return true;
}

bool showChar(char ch, char ch2) {
	string temp = string(1, ch);
	//string temp2 = string(2, ch2);
	for (int i = 0; i < 8; i++) {
		if (ch == OPERATORS[i]) {
			bool state=showchar2(ch, ch2); // go through first char
			if (state == true) {
				return true;
			}
		}

	}
	for (int i = 8; i < 22; i++) {
		if (ch == OPERATORS[i] && !isspace(ch)) {
			output.push_back(pair<string, string>("Separator ", temp));
			return false;
		}
	}
}
