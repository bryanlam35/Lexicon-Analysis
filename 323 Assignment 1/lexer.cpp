#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// tokens to use as temporary storage of characters when looping
string tokenType;
string tokenLexeme;

// containers for all key
vector<string> keywords = {"while", "if", "else", "for", "return", "int", "float", "bool", "char", "double"};
vector<string> operators = {"+", "-", "*", "/", "=", "==", "<", ">", "<=", ">=", "!=", "&&", "||", "!"};
vector<char> separators = {'(', ')', '{', '}', '[', ']', ';', ',', '.', ':'};

// declaring the txt file with out test cases
// intializing ch (character) as whtie space
ifstream testcasesFile;
char ch = ' ';

// check if string lexeme is a keyword by iterating through the keyword vector using .size() to traverse through
bool isKeyword(const string lexeme) {
    for (int i = 0; i < keywords.size(); i++) {
        if (keywords[i] == lexeme) {
            return true;
        }
    }
    return false;
}

// check if string lexeme is an operator by iterating through the operator vector using .size() to traverse through
bool isOperatorChar(char ch) {
    for (int i = 0; i < operators.size(); i++) {
        if (operators[i][0] == ch) {
            return true;
        }
    }
    return false;
}

// check if string lexeme is a separator by iterating through the separator vector using .size() to traverse through
bool isSeparatorChar(char ch) {
    for (int i = 0; i < separators.size(); i++) {
        if (separators[i] == ch) {
            return true;
        }
    }
    return false;
}

// Lexer function
void lexer() {
    tokenType = "unknown";
    tokenLexeme = "";

    // Skip whitespaces by checking if each 'ch' input is a whitespace
    // if the whitespace return is true then the while loop continues
    // when theloop encounters something that is not whitespace it will exit the while loop.
    while (isspace(ch)) {
        ch = testcasesFile.get();
    }

    // Check if it is the end of the file by checking if all characters and whitespace
    // and white spaces have been read. If the whole file has been read
    // return TOKEN_END_OF_FILE which is 8, and 'return' (exit)
    if (testcasesFile.eof()) {
        tokenType = "EOF";
        return;
    }

    // ialpha is a boolean function we use from the string library to check if character is a letter 
    // if ch is a letter or '_' underscore is used for test cases 
    // that have variables written like so: test_variable
    if (isalpha(ch) || ch == '_') {

    // Start state
    // adds the first character to tokenLexeme
    // for example, with "fahr" it adds 'f' to tokenLexeme
    tokenLexeme += ch;
    ch = testcasesFile.get();
    
    // loop state
    // continues to add valid characters to tokenLexeme
    // this will add 'a', 'h', 'r' to tokenLexeme if our word is "fahr"
    while (isalnum(ch) || ch == '_') {
        tokenLexeme += ch;
        ch = testcasesFile.get();
    }
        // check if lexeme is a keyword
        // we just added character by character from the testcase.txt file
        // if the loop stops, that means we have a complete string from adding up the characters
        // we then use isKeyword function created earlier to determine if that string is a keyword or identifier
        if (isKeyword(tokenLexeme)) {
            tokenType = "keyword";
        } else {
            tokenType = "identifier";
        }
        return;
    }

    // Checking if character is a digit
    if (isdigit(ch)) {
    
        // Start state
        // just like previous isAlpha, we are adding each character that is a digit into tokenLexeme
        // the starting state checks if the first character is a digit
        tokenLexeme += ch;
        ch = testcasesFile.get();

        // while the next characters are still digits, we continue to add to tokenLexeme
        while (isdigit(ch)) {
            tokenLexeme += ch;
            ch = testcasesFile.get();
        }
        // here we check if it is a real number
        if (ch == '.') {
            // when a decimal is encountered, we will add the decimal point to tokenLexeme
            // so if we had 12 so far and encounter '.', the program will add the decimal
            // tokenLexeme will then be 12.
            // then set ch to get the next character 
            tokenLexeme += ch;
            ch = testcasesFile.get();
            
            // this loop to check if the next character is a digit
            // continue adding to tokenLexeme if the next character is a digit
            // if it is not a digit, loop is done and we return "real" four the tokenType
            while (isdigit(ch)) {
                tokenLexeme += ch;
                ch = testcasesFile.get();
            }
            tokenType = "real";
        } else {
            //if it does not have a decimal point
            // return "integer" for our tokenType
            tokenType = "integer";
        }
        return;
    }

    // Operator
    if (isOperatorChar(ch)) {
        // if the first character is a operator, add it to tokenLexeme
        tokenLexeme += ch;

        // we use peek to check the next character without directly taking the next character
        // example is if we have the first character as ! and then we peek the next character
        // we will set nextCh to the next character it peeked
        // this is used to check whether the next character is a valid operator and thus a two character operator
        char nextCh = testcasesFile.peek();

        // because we have operators like <= or >= or == we need to check the first character and the second
        string twoCharOp = tokenLexeme + nextCh;

        // boolean variable to check if the operator is a two character variable >= or a one character variable like +
        bool isTwoCharOperator = false;

        // loop for check all values in operators vector that ARE two character operators so we can set isTwoCharOperator to true
        // this is needed so that we can identify and match operators in the testcases.txt file with all operators in our operators vector
        // it is also important to have this loop so that we don't just stop at the first operator we find 
        // without this loop, if the program sees > it would simply stop but we have to remember the >= is a possibility
        for (int i = 0; i < operators.size() && !isTwoCharOperator; i++) {

            //checking if at index i of operators vector if it matches the string we created that combines the first and next character
            // if there is a match, that means the characters read from the testcases.txt matches an operator in our vector
            // thus we change isTwoCharOperator to true
            if (operators[i] == twoCharOp) {
            isTwoCharOperator = true;
        }
    }

        // since we know the character is a two character operator now, we are adding them together in tokenLexeme
        // remember earlier we only peeked and did not actually .get() the second character
        if (isTwoCharOperator) {
            // if it is a two character operator, we add the second character to tokenLexeme
            ch = testcasesFile.get(); 
            tokenLexeme += ch;
            ch = testcasesFile.get();
        } else {
            ch = testcasesFile.get();
        }

        //either way we checked already and it is an operator
        // this means tokenType will always be "Operator" regardless if it is a one or two character operator
        tokenType = "operator";
        return;
    }

    // using our separator function to check if the next character the program checks is a separator
    // pretty simple bc all separators are one character
    if (isSeparatorChar(ch)) {
        tokenLexeme += ch;
        tokenType = "separator";
        ch = testcasesFile.get();
        return;
    }

    // Unknown character
    tokenLexeme += ch;
    tokenType = "unknown";
    ch = testcasesFile.get();
    return;
}

int main() {
    // Open test cases file in the project's directory: testcases.txt)
    // usual error message if the file cannot be found or opened
    testcasesFile.open("testcases.txt");
    if (!testcasesFile.is_open()) {
        cerr << "Failed to open test cases file." << endl;
        return 1;
    }

    // Need to also open our output file
    // our program will write and identify all identifiers, separators, operators, and keywords from the testcases.txt file
    // will also need an error message if output file can't be found or is not opening
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    // Initialize ch (character) of the text file with .get() which tells compiler to get the next ch
    ch = testcasesFile.get();

    // Lexer loop
    do {
        lexer();
        if (tokenType != "unknown" && tokenType != "EOF") {
            outputFile << tokenType << "\t\t" << tokenLexeme << endl;
        }
    } while (tokenType != "EOF");

    // Close files
    testcasesFile.close();
    outputFile.close();

    return 0;
}
