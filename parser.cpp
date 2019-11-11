// CPSC 323
// Team members: 
//    - Omar Al Nabulsi
//    - Duc Huynh
//    - Victoria Naranjo
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>
using namespace std;
enum Token {
	// Identifier tokens 
	IDENTIFIER, INTEGER, REAL_INTEGER, KEYWORD, ADDITION_OPERATOR, SUBTRACTION_OPERATOR,
	MULTIPLICATION_OPERATOR, DIVISION_OPERATOR, EQUAL_OPERATOR, LESS_THAN_OPERATOR,
	GREATER_THAN_OPERATOR, UP_EQ_OPERATOR, EQUAL_LESS_THAN_OPERATOR, EQUAL_GREATER_THAN_OPERATOR,
	EQUAL_EQUAL_OPERATOR, TRUE_OPERATOR, FALSE_OPERATOR,

	// Separator  tokens 
	COLON, NOT_COMMENT, SEMICOLON, LEFT_PARENTHESES, RIGHT_PARENTHESES, COMMA,
	LEFT_BRACKET, RIGHT_BRACKET, LEFT_CURLY_BRACE, RIGHT_CURLY_BRACE,
	PERCENT, END_OF_FILE
};

enum Character {
	LETTER, DIGIT, DOLLAR_SIGN, PERIOD, COMMENT, UNKNOWN, E_O_F
};
string filename;
class Lexer {
private:
	Character characterClass;
	char lexeme[500];
	char nextCharacter;
	int lexemeLength;
	FILE *in_fp, *fopen();
	void addCharacter();
	void getCharacter();
	void getNonBlank();
	void printResult(Token token);
	Token lookup(char ch);
	Token nextToken;
public:
	Lexer();
	Token lexer();
	void checkFile();
	char* getLexeme();
};

Lexer::Lexer() {
	if ((in_fp = std::fopen(filename.c_str(), "r")) == NULL)
		cout << "ERROR - cannot open file" << endl;
}
void Lexer::checkFile() {

	if ((in_fp = std::fopen(filename.c_str(), "r")) == NULL)
		cout << "ERROR - cannot open file" << endl;
	else {
		getCharacter();
		do {
			lexer();
		} while (nextToken != END_OF_FILE);
	}
}

void Lexer::addCharacter() {
	if (lexemeLength <= 500) {
		lexeme[lexemeLength++] = nextCharacter;
		lexeme[lexemeLength] = 0;
	}
	else {
		cout << "Error - lexeme is too long " << endl;
	}
}

void Lexer::getCharacter() {
	if ((nextCharacter = getc(in_fp)) != EOF) {
		if (isalpha(nextCharacter))
			characterClass = Character::LETTER;
		else if (isdigit(nextCharacter))
			characterClass = Character::DIGIT;
		else if (nextCharacter == '$')
			characterClass = Character::DOLLAR_SIGN;
		else if (nextCharacter == '.')
			characterClass = Character::PERIOD;
		else if (nextCharacter == '!')
			characterClass = Character::COMMENT;
		else characterClass = Character::UNKNOWN;
	}
	else {
		characterClass = Character::E_O_F;
	}
}

void Lexer::getNonBlank() {
	while (isspace(nextCharacter))
		getCharacter();
}

Token Lexer::lexer() {
	lexemeLength = 0;
	getNonBlank();
	switch (characterClass) {
		// Parse identifiers 
	case LETTER: {
		addCharacter();
		getCharacter();

		if (!isspace(nextCharacter)) {
			while (characterClass == LETTER || characterClass == DIGIT) {
				addCharacter();
				getCharacter();
				// Check if next character is either a digit or letter
				if (characterClass == LETTER) {
					nextToken = Token::IDENTIFIER;
				}
				else if (characterClass == DIGIT) {
					nextToken = Token::IDENTIFIER;
				}
			}
			if (characterClass == LETTER || characterClass == DOLLAR_SIGN) {
				addCharacter();
				getCharacter();
				nextToken = Token::IDENTIFIER;
			}
		}
		else {
			nextToken = Token::IDENTIFIER;
		}
		std::string lexString = lexeme;
		if (lexString == "true") {
			nextToken = TRUE_OPERATOR;
		}
		else if (lexString == "false") {
			nextToken = FALSE_OPERATOR;
		}

		// Check if there is a keyword match
		std::string keywords[] = { "if", "else", "endif", "while", "return", "get", "put", "function", "int", "boolean", "real" };
		for (auto key : keywords) {
			if (key == lexeme) {
				nextToken = KEYWORD;
			}
		}
		break;
	}
	case DIGIT:
		addCharacter();
		getCharacter();
		while (characterClass == DIGIT) {
			addCharacter();
			getCharacter();
		}
		if (characterClass == PERIOD) {
			addCharacter();
			getCharacter();
			while (characterClass == DIGIT) {
				addCharacter();
				getCharacter();
				nextToken = Token::REAL_INTEGER;
			}
		}
		else {
			nextToken = Token::INTEGER;
		}
		break;
	case DOLLAR_SIGN:
		break;
	case PERIOD:
		break;
	case COMMENT:
		addCharacter();
		getCharacter();
		while (characterClass != COMMENT) {
			addCharacter();
			getCharacter();
		}
		addCharacter();
		getCharacter();
		nextToken = Token::NOT_COMMENT;
		break;
		// Parentheses and operators 
	case UNKNOWN:
		lookup(nextCharacter);
		getCharacter();
		break;
		// end of file 
	case E_O_F:
		nextToken = Token::END_OF_FILE;
		break;
	}
	return nextToken;
}

Token Lexer::lookup(char ch) {
	switch (ch) {
	case '[':
		addCharacter();
		nextToken = LEFT_BRACKET;
		break;
	case ']':
		addCharacter();
		nextToken = RIGHT_BRACKET;
		break;
	case '+':
		addCharacter();
		nextToken = ADDITION_OPERATOR;
		break;
	case '-':
		addCharacter();
		nextToken = SUBTRACTION_OPERATOR;
		break;
	case '*':
		addCharacter();
		nextToken = MULTIPLICATION_OPERATOR;
		break;
	case '/':
		addCharacter();
		nextToken = DIVISION_OPERATOR;
		break;
	case '=':
		addCharacter();
		nextToken = EQUAL_OPERATOR;
		getCharacter();
		if (nextCharacter == '=') {
			addCharacter();
			nextToken = EQUAL_EQUAL_OPERATOR;
		}
		else if (nextCharacter == '>') {
			addCharacter();
			nextToken = EQUAL_GREATER_THAN_OPERATOR;
		}
		else if (nextCharacter == '<') {
			addCharacter();
			nextToken = EQUAL_LESS_THAN_OPERATOR;
		}
		break;
	case '(':
		addCharacter();
		nextToken = LEFT_PARENTHESES;
		break;
	case ')':
		addCharacter();
		nextToken = RIGHT_PARENTHESES;
		break;
	case '^':
		addCharacter();
		nextToken = END_OF_FILE;
		getCharacter();
		if (nextCharacter == '=') {
			addCharacter();
			nextToken = EQUAL_EQUAL_OPERATOR;
		}
		break;
	case '<':
		addCharacter();
		nextToken = LESS_THAN_OPERATOR;
		break;
	case '>':
		addCharacter();
		nextToken = GREATER_THAN_OPERATOR;
		break;
	case ':':
		addCharacter();
		nextToken = COLON;
		break;
	case ';':
		addCharacter();
		nextToken = SEMICOLON;
		break;
	case '{':
		addCharacter();
		nextToken = LEFT_CURLY_BRACE;
		break;
	case '}':
		addCharacter();
		nextToken = RIGHT_CURLY_BRACE;
		break;
	case '%':
		addCharacter();
		getCharacter();
		if (nextCharacter == '%') {
			addCharacter();
			nextToken = PERCENT;
		}
		break;
	case ',':
		addCharacter();
		nextToken = COMMA;
		break;
	default:
		nextToken = END_OF_FILE;
		break;
	}
	return nextToken;
}

char* Lexer::getLexeme() {
	return lexeme;
}


class Parser {
public:
	Parser();
private:
	Token token, nonComment();
	Lexer lex;
	void printToken(Token token, std::string);
	void RAT19F();
	void OptFunctionDefinitions();
	void FunctionDefinitions();
	void Function();
	void OptParameterList();
	void ParameterList();
	void Parameter();
	void Qualifier();
	void Body();
	void OptDeclarationList();
	void DeclarationList();
	void Declaration();
	void IDs();
	void StatementList();
	void Statement();
	void Compound();
	void Assign();
	void If();
	void Return();
	void Print();
	void Scan();
	void While();
	void Condition();
	void Relop();
	void Expression();
	void Term();
	void Factor();
	void Primary();
	void Empty();
};

Token Parser::nonComment() {
	while (token == NOT_COMMENT) {
		token = lex.lexer();
	}
	return token;
}

void Parser::printToken(Token token, string lexeme) {
	std::string id[] = { "Identifier","Keyword", "Integer", "Real","Separator","Operator","Comment","Boolean" };
	switch (token) {
	case KEYWORD:
		printf("Token: %s\tLexeme: %s\n", id[1].c_str(), lexeme.c_str());
		break;
	case IDENTIFIER:
		printf("Token: %s\tLexeme: %s\n", id[0].c_str(), lexeme.c_str());
		break;
	case INTEGER:
		printf("Token: %s\tLexeme: %s\n", id[2].c_str(), lexeme.c_str());
		break;
	case REAL_INTEGER:
		printf("Token: %s\tLexeme: %s\n", id[3].c_str(), lexeme.c_str());
		break;
	case NOT_COMMENT:
		printf("Token: %s\tLexeme: %s\n", id[6].c_str(), lexeme.c_str());
		break;
	case PERCENT:
	case COMMA:
	case COLON:
	case SEMICOLON:
	case LEFT_BRACKET:
	case LEFT_PARENTHESES:
	case RIGHT_BRACKET:
	case RIGHT_PARENTHESES:
	case LEFT_CURLY_BRACE:
	case RIGHT_CURLY_BRACE:
		printf("Token: %s\tLexeme: %s\n", id[4].c_str(), lexeme.c_str());
		break;
	case ADDITION_OPERATOR:
	case SUBTRACTION_OPERATOR:
	case MULTIPLICATION_OPERATOR:
	case DIVISION_OPERATOR:
	case EQUAL_OPERATOR:
	case LESS_THAN_OPERATOR:
	case GREATER_THAN_OPERATOR:
	case UP_EQ_OPERATOR:
	case EQUAL_LESS_THAN_OPERATOR:
	case EQUAL_GREATER_THAN_OPERATOR:
	case EQUAL_EQUAL_OPERATOR:
	case TRUE_OPERATOR:
	case FALSE_OPERATOR:
		printf("Token: %s\tLexeme: %s\n", id[5].c_str(), lexeme.c_str());
		break;
	default:
		break;
	}
}

Parser::Parser() {
	// Remove Blank
	token = lex.lexer();
	printToken(token, lex.getLexeme());
	RAT19F();
}

void Parser::RAT19F() {
	token = lex.lexer();
	token = nonComment();
	OptFunctionDefinitions();
	if (token == PERCENT) {
		printToken(token, lex.getLexeme());
		token = lex.lexer();
		token = nonComment();
		OptDeclarationList();
		StatementList();
	}
}

void Parser::OptFunctionDefinitions() {
	printf("\t<Opt Function Definitions> ::= <Function Definitions> | <Empty>\n");
	FunctionDefinitions();
	token = nonComment();
}

void Parser::FunctionDefinitions() {
	printf("\t<Function Definitions>  ::= <Function> | <Function><Function Definitions> \n");
	token = nonComment();
	Function();
	token = nonComment();
}

void Parser::Function() {
	string lexeme = lex.getLexeme();
	if (lexeme == "function") {
		printToken(token, lex.getLexeme());
		printf("\t<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>\n");
		token = lex.lexer();
		if (token == IDENTIFIER) {
			printToken(token, lex.getLexeme());
			token = lex.lexer();
			if (token == LEFT_BRACKET) {
				printToken(token, lex.getLexeme());
				token = lex.lexer();
				OptParameterList();
				if (token == RIGHT_BRACKET) {
					token = lex.lexer();
					OptDeclarationList();
					Body();
				}
			}
		}
	}
}

void Parser::OptParameterList() {
	printf("\t<Opt Parameter List> ::= <Parameter List> | <Empty>\n");
	ParameterList();
}

void Parser::ParameterList() {
	printf("\t<Parameter List>  ::=  <Parameter> | <Parameter>,<Parameter List> \n");
	Parameter();
}

void Parser::Parameter() {
	printf("\t<Parameter> ::= <IDs> <Qualifier>\n");
	if (token == IDENTIFIER) {
		printToken(token, lex.getLexeme());
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		if (token == COLON) {
			token = lex.lexer();
			Qualifier();
		}
		else {
			printf("ERROR*---* No Colon found\n");
		}
	}
	else {
		printf("ERROR*---* No Identifier found\n");
	}
}

void Parser::Qualifier() {
	string lexeme = lex.getLexeme();
	if (lexeme == "int") {
		printToken(token, lex.getLexeme());
		printf("\t<Qualifier> ::= int\n");
		token = lex.lexer();
	}
	else if (lexeme == "real") {
		printToken(token, lex.getLexeme());
		printf("\t<Qualifier> ::= real\n");
		token = lex.lexer();
	}
	else if (lexeme == "boolean") {
		printToken(token, lex.getLexeme());
		printf("\t<Qualifier> ::= boolean\n");
		token = lex.lexer();
	}
}

void Parser::Body() {
	printf("\t<Body> ::= { <Statement List> }\n");
	if (token == LEFT_CURLY_BRACE) {
		token = lex.lexer();
		token = nonComment();
		printToken(token, lex.getLexeme());
		StatementList();
		if (token != RIGHT_CURLY_BRACE) {
			cout << "ERROR*---* No Right Curly Brace found " << endl;
		}
	}
}

void Parser::OptDeclarationList() {
	printf("\t<Opt Declaration List> ::= <Declaration List> | <Empty>\n");
	DeclarationList();
}

void Parser::DeclarationList() {
	printf("\t<Declaration List>  := <Declaration> ; | <Declaration>;<Declaration List>\n");
	Declaration();
	if (token == SEMICOLON) {
		printToken(token, lex.getLexeme());
		token = lex.lexer();
		token = nonComment();
		printToken(token, lex.getLexeme());
	}
}

void Parser::Declaration() {
	printf("\t<Declaration> ::= <Qualifier> <IDs>\n");
	Qualifier();
	IDs();
}

void Parser::IDs() {
	printf("\t<IDs> ::= <Identifier> | <Idefntifier>,<IDs>\n");
	if (token == IDENTIFIER) {
		printToken(token, lex.getLexeme());
		token = lex.lexer();
	}
}

void Parser::StatementList() {
	printf("\t<Statement List> ::= <Statement> | <Statement><Statement List>\n");
	token = nonComment();
	Statement();
}

void Parser::Statement() {
	token = nonComment();
	string lexeme = lex.getLexeme();
	if (lexeme == "return") {
		printf("\t<Statement> ::= <Return>\n");
		token = lex.lexer();
		Return();
	}
	else if (lexeme == "if") {
		printf("\t<Statement> ::= <If>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		If();
	}
	else if (lexeme == "get") {
		printf("\t<Statement> ::= <Scan>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Scan();
	}
	else if (lexeme == "put") {
		printf("\t<Statement> ::= <Print>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Print();
	}
	else if (lexeme == "while") {
		printf("\t<Statement> ::= <While>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		While();
	}
	else if (token == LEFT_CURLY_BRACE) {
		printf("\t<Statement> ::= <Compound>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Compound();
	}
	else if (token == IDENTIFIER) {
		printf("\t<Statement> ::= <Assign>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Assign();
	}
}

void Parser::Compound() {
	printf("\t<Compound> ::= { <Statement List> }\n");
	StatementList();
	if (token != RIGHT_CURLY_BRACE) {
		printf("ERROR! No Right Curly Brace found\n");
	}
}

void Parser::Assign() {
	printf("\t<Assign> ::=  <Identifier> = <Expression>;\n");
	if (token == EQUAL_OPERATOR) {
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Expression();
	}
}

void Parser::If() {
	printf("\t<If> ::= if ( <Condition> ) <Statement> fi | if  ( <Condition> ) <Statement> otherwise <Statement> fi\n");
	if (token == LEFT_PARENTHESES) {
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Condition();
		if (token == RIGHT_PARENTHESES) {
			token = lex.lexer();
			printToken(token, lex.getLexeme());
			Statement();
		}
	}
}

void Parser::Return() {
	if (token == SEMICOLON) {
		printf("\t<Return> ::= return ;\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
	}
	else {
		printf("\t<Return> ::= return <Expression>;\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Expression();
	}
}

void Parser::Print() {
	if (token == LEFT_PARENTHESES) {
		printf("\t<Print> ::= put ( <Expression> );\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Expression();
		if (token == RIGHT_PARENTHESES) {
			token = lex.lexer();
			printToken(token, lex.getLexeme());
		}
	}
}

void Parser::Scan() {
	if (token == LEFT_PARENTHESES) {
		printf("\t<Scan> ::= get ( <IDs> );\n");
		token = lex.lexer();
		IDs();
		if (token == RIGHT_PARENTHESES) {
			token = lex.lexer();
			printToken(token, lex.getLexeme());
		}
	}
}

void Parser::While() {
	printf("\t<While> ::= while ( <Condition> ) <Statement> \n");
	if (token == LEFT_PARENTHESES) {
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Condition();
		if (token == RIGHT_PARENTHESES) {
			token = lex.lexer();
			printToken(token, lex.getLexeme());
			Statement();
		}
	}
}

void Parser::Condition() {
	printf("\t<Condition> ::= <Expression> <Relop> <Expression>\n");
	Expression();
	Relop();
	Expression();
}

void Parser::Relop() {
	switch (token) {
	case EQUAL_EQUAL_OPERATOR: {
		printf("\t<Relop> ::=  ==\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case EQUAL_OPERATOR: {
		printf("\t<Relop> ::= /=\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case GREATER_THAN_OPERATOR: {
		printf("\t<Relop> ::= >\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case LESS_THAN_OPERATOR: {
		printf("\t<Relop> ::= <\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case EQUAL_GREATER_THAN_OPERATOR: {
		printf("\t<Relop> ::= =>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case EQUAL_LESS_THAN_OPERATOR: {
		printf("\t<Relop> ::= <=\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	default:
		break;
	}
}

void Parser::Expression() {
	printf("\t<Expression> ::= <Expression> + <Term> | <Expression> - <Term> | <Term>\n");
	Term();
}

void Parser::Term() {
	printf("\t<Term> ::= <Term> * <Factor> | <Term> / <Factor> | <Factor>\n");
	Factor();
}

void Parser::Factor() {
	printf("\t<Factor> ::=  - <Primary> | <Primary>\n");
	if (token == SUBTRACTION_OPERATOR) {
		printf("\t<Factor> ::=  - <Primary>\n");
		Primary();
	}
	else {
		printf("\t<Factor> ::= <Primary>\n");
		Primary();
	}
}

void Parser::Primary() {
	printf("\t<Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false\n");
	switch (token) {
	case IDENTIFIER: {
		printf("\t<Primary> ::= <Identifier>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		if (token == LEFT_PARENTHESES) {
			printf("\t<Primary> ::= <Identifier> ( <IDs> )\n");
			token = lex.lexer();
			IDs();
			if (token == RIGHT_PARENTHESES) {
				token = lex.lexer();
				printToken(token, lex.getLexeme());
			}
		}
		break;
	}
	case INTEGER: {
		printf("\t<Primary> ::= <Integer>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case LEFT_PARENTHESES: {
		printf("\t<Primary> ::= ( <Expression> )\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		Expression();
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case REAL_INTEGER: {
		printf("\t<Primary> ::= <Real>\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case TRUE_OPERATOR: {
		printf("\t<Primary> ::= true\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	case FALSE_OPERATOR: {
		printf("\t<Primary> ::= false\n");
		token = lex.lexer();
		printToken(token, lex.getLexeme());
		break;
	}
	default:
		break;
	}
}

void Parser::Empty() {
	printf("<Empty> ::= ɛ\n");
}

int main() {

	cout << "Enter file name ";
	cin >> filename;
	Parser Parser;
	system("pause");
}