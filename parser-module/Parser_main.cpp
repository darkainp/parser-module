#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

/*********************Error list***********************/
const string INVALID = "Invalid input!!!";
const string NOT_FOUND = "Not found!!!";
const string EMPTY_TABLE = "Table is empty!!!";
/******************************************************/

/*************data type size list in bytes!!!!*********/
const int CHAR = 1;//x
const int REAL = 4;//x
const int STR = 80;//max size is 80, this also include the end of the string. Actual string address will be determined by the length of the input/litteral string.
const int PTR = 2;// an address is 16 bits = 2bytes
const int YN = 1;//x
const int HEX = 4;//x
const int NONE = 0;//x
const int INT = 4;//x
/******************************************************/

const int TOKEN_LIST_SIZE = 70;
const string TOKEN[TOKEN_LIST_SIZE] = { "stringLitToken", "stringDecToken", "stringToken", "charLitToken", "charToken", "charDecToken",
"intLitToken", "intDecToken", "intToken", "realLitToken", "realDecToken", "boolLitToken", "boolDecToken", "boolToken", "hexLitToken",
"hexDecToken", "hexToken", "endOfLineToken", "commentToken", "multiCommentToken", "addToken", "subToken", "multToken", "divToken",
"modToken", "incToken", "decToken", "negToken", "equalToToken", "notEqualToToken", "greaterThanToken", "lessThanToken", "greaterThanEqualToken",
"lessThanEqualToken", "orToken", "andToken", "thisToken", "thatToken", "loopToken", "rptToken", "tilToken", "cacToken", "optToken",
"normToken", "defToken", "callToken", "rtToken", "ptrToken", "arrayToken", "noneToken", "getToken", "sendToken", "escToken", "stopToken",
"leftBlockToken", "rightBlockToken", "leftParenthesisToken", "rightParenthesisToken", "leftSquareBracketToken", "rightSquareBracketToken",
"assignmentToken", "semicolonToken", "msgToken" , "errorToken","idToken","yesToken","noToken","haltToken"};

const string RESEVERD_WORD[TOKEN_LIST_SIZE] = { "", "STR", "", "", "", "CHAR",
"", "INT", "", "", "REAL", "", "YN", "", "",
"HEX", "", "", "", "", "", "", "X*", "",
"MOD", "INC", "DEC", "NT", "E2", "NE2", "GT", "LT", "GTE2",
"LTE2", "OR", "AND", "THIS", "THAT", "LOOP", "RPT", "TIL", "CAC", "OPT",
"NORM", "DEF", "CALL", "RT", "PTR", "ARR", "NONE", "GET", "SEND", "", "STOP",
"", "", "", "", "", "",
"", "", "MSG" , "","", "YES", "NO" ,""};

/********** Op-codes *****************************/
const unsigned char INC = 0x01;
const unsigned char DEC = 0x02;
const unsigned char ADD = 0x03;
const unsigned char SUB = 0x04;
const unsigned char MUL = 0x05;
const unsigned char DIV = 0x06;
const unsigned char MOD = 0x07;
const unsigned char NEG = 0x08;

const unsigned char ADDF = 0x11;
const unsigned char SUBF = 0x12;
const unsigned char MULF = 0x13;
const unsigned char DIVF = 0x14;
const unsigned char MODF = 0x15;
const unsigned char NEGF = 0x16;
const unsigned char NOT = 0x17;
const unsigned char AND = 0x18;
const unsigned char OR = 0x19;

const unsigned char XOR = 0x20;
const unsigned char SHR = 0x21;
const unsigned char SHL = 0x22;
const unsigned char SHRA = 0x23;
const unsigned char ROR = 0x25;
const unsigned char ROL = 0x26;
const unsigned char HALT = 0x27;
const unsigned char DER = 0x28;

const unsigned char TPS = 0x31;
const unsigned char TNG = 0x32;
const unsigned char TZE = 0x33;
const unsigned char TNP = 0x34;
const unsigned char TNN = 0x35;
const unsigned char TNZ = 0x36;

const unsigned char TPSF = 0x41;
const unsigned char TNGF = 0x42;
const unsigned char TZEF = 0x43;
const unsigned char TNPF = 0x44;
const unsigned char TNNF = 0x45;
const unsigned char TNZF = 0x46;

const unsigned char PUSL = 0x50;
const unsigned char PUSG = 0x51;
const unsigned char PUSP = 0x52;
const unsigned char PUSS = 0x53;
const unsigned char POPL = 0x54;
const unsigned char POPG = 0x55;
const unsigned char POPP = 0x56;
const unsigned char POPS = 0x57;
const unsigned char INS = 0x58;
const unsigned char OTS = 0x59;

const unsigned char BYES = 0x60;
const unsigned char BNO = 0x61;
const unsigned char JMP = 0x62;
const unsigned char JMPD = 0x63;
const unsigned char JMPI = 0x64;
const unsigned char JMPR = 0x65;

const unsigned char LDD1 = 0x70;
const unsigned char LDDN1 = 0x71;
const unsigned char LDI1 = 0x72;
const unsigned char LDIN1 = 0x73;
const unsigned char LDR1 = 0x74;
const unsigned char LDRN1 = 0x75;
const unsigned char LDM1 = 0x76;
const unsigned char LDLR1 = 0x79;

const unsigned char LDLN1 = 0x80;
const unsigned char LDGR1 = 0x81;
const unsigned char LDGN1 = 0x82;
const unsigned char STD1 = 0x83;
const unsigned char STDN1 = 0x84;
const unsigned char STI1 = 0x85;
const unsigned char STIN1 = 0x86;
const unsigned char STR1 = 0x87;
const unsigned char STRN1 = 0x88;

const unsigned char STLR1 = 0x92;
const unsigned char STLN1 = 0x93;
const unsigned char STGR1 = 0x94;
const unsigned char STGN1 = 0x95;

const unsigned char LDD2 = 0xA0;
const unsigned char LDDN2 = 0xA1;
const unsigned char LDI2 = 0xA2;
const unsigned char LDIN2 = 0xA3;
const unsigned char LDR2 = 0xA4;
const unsigned char LDRN2 = 0xA5;
const unsigned char LDM2 = 0xA6;
const unsigned char LDLR2 = 0xA7;

const unsigned char LDLN2 = 0xB0;
const unsigned char LDGR2 = 0xB1;
const unsigned char LDGN2 = 0xB2;
const unsigned char STD2 = 0xB3;
const unsigned char STDN2 = 0xB4;
const unsigned char STI2 = 0xB5;
const unsigned char STIN2 = 0xB6;
const unsigned char STR2 = 0xB7;
const unsigned char STRN2 = 0xB8;

const unsigned char STLR2 = 0xC2;
const unsigned char STLN2 = 0xC3;
const unsigned char STGR2 = 0xC4;
const unsigned char STGN2 = 0xC5;

const unsigned char LDD4 = 0xD0;
const unsigned char LDDN4 = 0xD1;
const unsigned char LDI4 = 0xD2;
const unsigned char LDIN4 = 0xD3;
const unsigned char LDR4 = 0xD4;
const unsigned char LDRN4 = 0xD5;
const unsigned char LDM4 = 0xD6;
const unsigned char LDLR4 = 0xD9;

const unsigned char LDLN4 = 0xE0;
const unsigned char LDGR4 = 0xE1;
const unsigned char LDGN4 = 0xE2;
const unsigned char STD4 = 0xE3;
const unsigned char STDN4 = 0xE4;
const unsigned char STI4 = 0xE5;
const unsigned char STIN4 = 0xE6;
const unsigned char STR4 = 0xE7;
const unsigned char STRN4 = 0xE8;

const unsigned char STLR4 = 0xF2;
const unsigned char STLN4 = 0xF3;
const unsigned char STGR4 = 0xF4;
const unsigned char STGN4 = 0xF5;
const unsigned char NOOP = 0xFF;

const unsigned char DSTK = 0xF6;

const unsigned char DROP = 0x47;
const unsigned char FLT = 0x48;
const unsigned char SWAP = 0x49;
const unsigned char OUTC = 0x4A;
const unsigned char OUTI = 0x4B;
const unsigned char OUTR = 0x4C;
const unsigned char COPY = 0x1F;
const unsigned char DSTCK = 0x1B;

const unsigned char INPUTC = 0x5A;
const unsigned char INPUTI = 0x5B;
const unsigned char INPUTR = 0x5C;
/******************************************************/
/**************** Memory ***************************/
const unsigned int MemSize = 256 * 256;    /* 64K needs to be a power of 2*/
const unsigned int AddrMask = MemSize - 1;
unsigned char M[MemSize] = { 0 };           /* RAM*/


/******************Prototype list**********************/
struct Symbol {
    string name;
    string type;
    string kind;
    int scope = 0;
    int address = 0;
};
class SymbolTable;
struct Token
{
    string lexem;                //Lexem of the Token
    string name;                //name of the Token
};
enum TokenList
{
    stringLitToken = 0, stringDecToken = 1, stringToken = 2, charLitToken = 3, charToken = 4, charDecToken = 5, intLitToken = 6,
    intDecToken = 7, intToken = 8, realLitToken = 9, realDecToken = 10, boolLitToken = 11, boolDecToken = 12, boolToken = 13,
    hexLitToken = 14, hexDecToken = 15, hexToken = 16, endOfLineToken = 17, commentToken = 18, multiCommentToken = 19, addToken = 20,
    subToken = 21, multToken = 22, divToken = 23, modToken = 24, incToken = 25, decToken = 26, negToken = 27, equalToToken = 28,
    notEqualToToken = 29, greaterThanToken = 30, lessThanToken = 31, greaterThanEqualToken = 32, lessThanEqualToken = 33, orToken = 34,
    andToken = 35, thisToken = 36, thatToken = 37, loopToken = 38, rptToken = 39, tilToken = 40, cacToken = 41, optToken = 42,
    normToken = 43, defToken = 44, callToken = 45, rtToken = 46, ptrToken = 47, arrayToken = 48, noneToken = 49, getToken = 50,
    sendToken = 51, escToken = 52, stopToken = 53, leftBlockToken = 54, rightBlockToken = 55, leftParenthesisToken = 56,
    rightParenthesisToken = 57, leftSquareBracketToken = 58, rightSquareBracketToken = 59, assignmentToken = 60,
    semicolonToken = 61, msgToken = 62, errorToken = 63, idToken = 64, yesToken = 65, noToken = 66, haltToken = 67
};

//scanner stuff
void ignore_comment();
char get_next();
int isReservedWord(string);
bool isGoodToEnd(char);
string get_rest();
bool isspace(char);
void uppercase(string&);
Token scanner(ifstream&);
Token scanning(ifstream&);

void funDec();
void multiDec();
void match(string);
void match(Token, string);
void declaration();
void ptrDec();
void ptrDecRest();
void array();
void arrayDec();
void arrayDecRest();
void multiArrayDec();
void varDec();
void varDecRest();
void program();
void assignment();
void assignBody();
void arrayInit();
void arrayInitBody();
void arrayCall();
void factor();
void decToken();
void funDecType();
void funDec();
void paramList();
void call();
void passedParam();
void litOrId();
void codeBlock();
void statementList();
void statement();
void forLoop();
void loopRest();
void ifStatement();
void ifElseStatement();
void Switch();
void switchBody();
void optPart();
void opt();
void norm();
void whileLoop();
void doWhileLoop();
void get();
void getBody();
void send();
void sendBody();
void sendToken2();
void litToken();
void addOp();
void mulOp();
void relOp();
Token expr();
Token exprE();
Token exprD();
Token exprC();
Token exprB();
Token exprA();
Token expr1();
Token unary();
void numLit();
void unaryOp();

//parser stuff
void print(Symbol);
void print(Token);
void parser();
void emitIntValue(char[]);
void emitRealValue(char[]);
void emit1(int);
void emit2(int);
void emit4(int);
void backPatch1(int, int);
void backPatch2(int, int);
void backpatch4(int, int);
unsigned int unreal(float);
void error(string);
Token next_token();
int caseBlockList();
void export_memory();

/******************************************************/

static ifstream source;
static Token lookahead;
static Token curr_token;
static int curr_addr = 0;
const bool DEBUG = 0;
static int line = 0;
static int G, L;    //hold the address of G and L, G only assign once.
static  int fun_count = 0;  //how many function is declare in the program
static int varSize; //the size of a variable

int main()
{
    string name = "source.txt";

    //cout << "Please enter the source code file's name (including .txt): "; cin >> name;

    source.open(name);

    if (!source) //file not open
    {
        cout << "File didn't open correctly!!!" << endl;
        exit(EXIT_FAILURE);
    }
    else //file open
    {
        //set up the file
        emit1(LDM2);
        emit2(0); //reserve address of G
        G = curr_addr;
        emit1(POPG);
        emit1(LDM2);
        emit2(0);  //reserve address of L
        L = curr_addr;
        emit1(POPL);
        emit1(LDM2);
        emit2(12); //address of PC, doesnt change
        emit1(POPP);
        emit1(DSTK);
        //emit1(HALT);

        //parsing here
        parser();

        //print MM out in a file
        export_memory();

        //close source when done, this leave at the end
        source.close();
    }

    return 0;
}

Token expr1()
{
    Token nextToken;
    if (nextToken.name == TOKEN[leftParenthesisToken])
    {
        match(TOKEN[leftParenthesisToken]);
        //exprE();
        match(TOKEN[rightParenthesisToken]);

    }
    else;
    return nextToken;
}

class SymbolTable
{
private:
    Symbol table[100];
    int current_size = 0;            //current_size is how many element existed int the table, so counter = current_size - 1
    int current_scope = 0;
    int current_address[50];        //current_address of scope 0 is current_address[0]
public:
    SymbolTable();

    void add_symbol(Symbol);
    void del_current_scope();

    static Symbol create_symbol(string, string, string);    //need the namespace SymbolTable::create_symbol
    int find_symbol(string) const;
    void print_symbol(int) const;
    void printTable() const;
    Symbol get_symbol(int) const;
};

/*
print the symbol table
*/
void SymbolTable::printTable() const
{
    int width = 10;
    cout << setw(width * 2) << "Name" << setw(width) << "Type" << setw(width) << "Scope" << setw(width) << "Kind" << setw(width) << "Address" << endl;
    for (int i = 0; i < current_size; i++)
    {
        cout << setw(width * 2) << table[i].name << setw(width) << table[i].type << setw(width) << table[i].scope;
        cout << setw(width) << table[i].kind << setw(width) << table[i].address << endl;
    }
}

/*
* @param x: position of the symbol
* @return Symbol at posistion x in the symbol table
*/
Symbol SymbolTable::get_symbol(int x) const
{
    return table[x];
}

/*
* constructor
* @param s: size of the table
*/
SymbolTable::SymbolTable()
{
    current_size = 0;
    current_scope = 0;
    current_address[0] = 0;
}

/*to add a symbol to the table
* @param input: is the symbol to add
*/
void SymbolTable::add_symbol(Symbol input)
{
    table[current_size] = input;

    //get and scope and address
    table[current_size].scope = current_scope;
    table[current_size].address = current_address[current_scope];

    //update address
    switch (table[current_size].type[0])
    {
    case 'C':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += CHAR;
        break;
    case 'I':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += INT;
        break;
    case 'R':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += REAL;
        break;
    case 'H':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += HEX;
        break;
    case 'N':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += NONE;
        break;
    case 'S':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += STR;
        break;
    case 'P':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += PTR;
        break;
    case 'Y':
        table[current_size].address = current_address[current_scope];
        current_address[current_scope] += YN;
        break;
    default:
        cout << "theres a problem!!!" << endl;
        break;
    }

    //update scope
    if (table[current_size].kind == "FUN")
    {
        current_scope++;
        current_address[current_scope] = 0;
    }
    current_size++;
}

//delete the most current scope
void SymbolTable::del_current_scope()
{
    int curr_scope = current_scope;

    if (current_size > 0)
    {
        while (curr_scope == table[current_size - 1].scope)
            current_size--;
        current_scope = table[current_size - 1].scope;
        //current_address[current_scope] = table[current_size - 1].address;
    }
    else cout << EMPTY_TABLE;
}

/*Create a new symbol without scope and address
* @param n: name of the symbol
* @param t: type of the symbol
* @param k: kind of the symbol
* @return a new symbol without scope and address
*/
Symbol SymbolTable::create_symbol(string n, string t, string k)
{
    Symbol sym;

    sym.name = n;
    sym.type = t;
    sym.kind = k;
    sym.scope = 0;
    sym.address = 0;

    return sym;
}

/*to find a symbol in the table with symbol's name.

* @param str: name of the symbol
* @return -1 if not found or the position of the symbol if found
*/
int SymbolTable::find_symbol(string str) const
{
    int pos = -1;
    uppercase(str);

    bool found = false;

    for (int i = current_size - 1; i >= 0; i--)
    {
        if (table[i].name == str)
        {
            found = true;
            pos = i;
            break;
        }
    }
    return pos;
}

/*To print a symbol from the table
* @param pos: location of the symbol on the table
*/
void SymbolTable::print_symbol(int pos) const
{
    cout << "Symbol detail:" << endl;
    cout << "\tName:    " << table[pos].name << endl;
    cout << "\tType:    " << table[pos].type << endl;
    cout << "\tKind:    " << table[pos].kind << endl;
    cout << "\tScope:   " << table[pos].scope << endl;
    cout << "\tAddress: " << table[pos].address << endl;
}

/*
* Scan for lexeme
*
* Only use for scanning function
*
* @param source: file pointer of the source code file
* @return a token
*/
Token scanner(ifstream& source)
{
    Token token;
    char next_char;
    char this_char;
    string lexeme = "";
    bool endyet = false;
    int x;

    this_char = get_next();
    switch (this_char)
    {
        //single-character Tokens
    case '<':
        token.lexem = "<";
        token.name = TOKEN[leftBlockToken];
        break;
    case '>':
        token.lexem = ">";
        token.name = TOKEN[rightBlockToken];
        break;
    case '.':
        token.lexem = ".";
        token.name = TOKEN[endOfLineToken];
        break;
    case '[':
        token.lexem = "[";
        token.name = TOKEN[leftSquareBracketToken];
        break;
    case ']':
        token.lexem = "]";
        token.name = TOKEN[rightSquareBracketToken];
        break;
    case '(':
        token.lexem = "(";
        token.name = TOKEN[leftParenthesisToken];
        break;
    case ')':
        token.lexem = ")";
        token.name = TOKEN[rightParenthesisToken];
        break;
    case '+':
        token.lexem = "+";
        token.name = TOKEN[addToken];
        break;
    case '-':
        token.lexem = "-";
        token.name = TOKEN[subToken];
        break;
    case '/':
        token.lexem = "/";
        token.name = TOKEN[divToken];
        break;
    case '=':
        token.lexem = "=";
        token.name = TOKEN[assignmentToken];
        break;
    case ';':
        token.lexem = ";";
        token.name = TOKEN[semicolonToken];
        break;

        //Number literal regconition
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        next_char = this_char;
        do //get 1st number part
        {
            lexeme += this_char;
            this_char = get_next();
        } while (isdigit(this_char));
        token.name = TOKEN[intLitToken];
        token.lexem = lexeme;

        if (this_char == ',') //there is a comma after the 1st number part ex: 123,
        {
            lexeme += this_char;
            token.name = TOKEN[realLitToken];
            token.lexem = lexeme;

            next_char = get_next();
            if (isdigit(next_char)) //if next char after comma is digit
            {
                this_char = next_char;
                do //get the 2nd number part after comma ex: 123,123
                {
                    lexeme += this_char;
                    this_char = get_next();
                } while (isdigit(this_char));
                token.name = TOKEN[realLitToken];
                token.lexem = lexeme;

                if (this_char == 'E') //if after 2nd number part is 'e'
                {
                    lexeme += this_char;
                    next_char = get_next();
                    if (isdigit(next_char)) //if next char after 'e' is digit
                    {
                        this_char = next_char;
                        do //get 3rd number part ex: 123,123e123
                        {
                            lexeme += this_char;
                            this_char = get_next();
                        } while (isdigit(this_char));
                        token.name = TOKEN[realLitToken];
                        token.lexem = lexeme;
                    }
                    else if (next_char == '-') //if next char after 'e' is '-'
                    {
                        lexeme += next_char;
                        this_char = next_char;
                        next_char = get_next();
                        if (isdigit(next_char))    //if next char after '-' is digit
                        {
                            this_char = next_char;
                            do //get the 3rd number part ex: 123,123e-123
                            {
                                lexeme += this_char;
                                this_char = get_next();
                            } while (isdigit(this_char));
                            token.lexem = lexeme;
                            token.name = TOKEN[realLitToken];
                        }
                        else //if next char after '-' is not digit
                            token.name = TOKEN[errorToken];
                    }
                    else //if next char after 'e' is not digit or '-'
                    {
                        token.name = TOKEN[errorToken];
                        if (next_char == '\n') line--;
                        lexeme += get_rest();
                        token.lexem = lexeme;
                    }
                }
            }
            else if (next_char == 'E')
            {
                this_char = next_char;
                lexeme += this_char;
                next_char = get_next();
                if (isdigit(next_char))
                {
                    this_char = next_char;
                    do
                    {
                        lexeme += this_char;
                        this_char = get_next();
                    } while (isdigit(this_char));

                    if (!isGoodToEnd(this_char))
                    {
                        if (this_char == '\n') line--;
                        lexeme += get_rest();
                        token.name = TOKEN[errorToken];
                        token.lexem = lexeme;
                    }
                    else
                    {
                        token.name = TOKEN[realLitToken];
                        token.lexem = lexeme;
                    }
                }
                else if (next_char == '-')
                {
                    this_char = next_char;
                    lexeme += this_char;
                    next_char = get_next();
                    if (isdigit(next_char))
                    {
                        this_char = next_char;
                        do
                        {
                            lexeme += this_char;
                            this_char = get_next();
                        } while (isdigit(this_char));
                        if (!isGoodToEnd(this_char))
                        {
                            //if (this_char == '\n') line--;
                            lexeme += get_rest();
                            token.name = TOKEN[errorToken];
                            token.lexem = lexeme;
                        }
                        else
                        {
                            token.name = TOKEN[realLitToken];
                            token.lexem = lexeme;
                        }
                    }
                    else
                    {
                        if (next_char == '\n') line--;
                        source.unget();

                        if (next_char == '\n') line--;
                        source.unget();
                        token.name = TOKEN[errorToken];
                        token.lexem = "0,E";
                    }
                }
                else if (isGoodToEnd(next_char))
                {
                    token.name = TOKEN[errorToken];
                    token.lexem = lexeme;
                }
                else
                {
                    if (this_char == '\n') line--;
                    lexeme += get_rest();
                    token.name = TOKEN[errorToken];
                    token.lexem = lexeme;
                }
            }
            else if (!isGoodToEnd(next_char))
            {
                //if (next_char == '\n') line--;
                lexeme += get_rest();
                token.name = TOKEN[errorToken];
                token.lexem = lexeme;
            }
            else
            {
                token.name = TOKEN[realLitToken];
                token.lexem = lexeme;
            }
        }
        else if (this_char == 'X')
        {
            if (lexeme.length() == 1 && lexeme == "0")
            {
                lexeme += this_char; //lexeme now is 0x
                next_char = get_next();
                while (isdigit(next_char) || next_char >= 'A' && next_char <= 'F')
                {
                    lexeme += next_char;
                    next_char = get_next();
                }
                if (isGoodToEnd(next_char))
                {
                    if (lexeme.length() >= 3)
                    {
                        token.name = TOKEN[hexLitToken];
                        token.lexem = lexeme;
                    }
                    else
                    {
                        token.name = TOKEN[errorToken];
                        token.lexem = lexeme;
                    }
                }
                else
                {
                    if (this_char == '\n') line--;
                    lexeme += get_rest();
                    token.name = TOKEN[errorToken];
                    token.lexem = lexeme;
                }
            }
            else //get the rest of the word and return error
            {
                if (this_char == '\n') line--;
                lexeme += get_rest();
                token.lexem = lexeme;
                token.name = TOKEN[errorToken];
            }
        }
        else if (!isGoodToEnd(this_char))
        {
            do
            {
                lexeme += this_char;
                this_char = get_next();
            } while (!isGoodToEnd(this_char));
            token.name = TOKEN[errorToken];
            token.lexem = lexeme;
        }
        else
        {
            if (this_char == '\n') line--;
            source.unget();
        }
        break;

        //string lit or char lit token
    case ':': //1st char is ':'
        lexeme += this_char;
        next_char = get_next(); //getting 2nd char
        if (next_char != ':') //if 2nd char is not ':' possible charLitToken
        {
            lexeme += next_char;
            token.name = TOKEN[charLitToken];

            if (next_char == '@')    //if 2nd char is '@' possible charLitToken of the use of an escape character
            {
                this_char = next_char;

                next_char = get_next();
                if (next_char == ':') //if 3rd char is ':' => charLitToken of :@: maybe invalid
                {
                    lexeme += next_char;
                    this_char = next_char;
                    next_char = get_next();
                    if (next_char == ':') //if 4th char is ':' => valid escape char used
                    {
                        token.lexem = ':';
                        token.name = TOKEN[charLitToken];
                    }
                    else //invalid
                    {
                        token.name = TOKEN[errorToken];
                        token.lexem = lexeme;
                    }
                }
                else //if 3rd char is not ':' => possible charLitToken with the use of an escape character Ex: :@@:
                {
                    lexeme += next_char;
                    this_char = next_char;
                    if (this_char == '@') // the 2nd use of escape character atm
                    {
                        next_char = get_next();
                        if (next_char == ':') //if 4th char is ':' => valid charLitToken :@@:
                        {
                            token.lexem = this_char; //equal the 3rd char scanned
                        }
                        else //if 4th char is not ':' => invalid charLitToken
                        {
                            this_char = next_char;
                            //taking everything till seeing a ':'
                            do
                            {
                                lexeme += this_char;
                                this_char = get_next();
                            } while (this_char != ':');
                            lexeme += this_char;
                            token.name = TOKEN[errorToken];
                            token.lexem = lexeme;
                        }
                    }
                    else //invalid use of escape character;
                    {
                        //taking everything till seeing a ':'
                        do
                        {
                            this_char = get_next();
                            lexeme += this_char;

                        } while (this_char != ':');
                        lexeme += this_char;
                        token.name = TOKEN[errorToken];
                        token.lexem = lexeme;
                    }
                }//end else
            }
            else //if 2nd char is not '@' => possible charLitToken
            {
                this_char = next_char;

                next_char = get_next();
                if (next_char == ':') //if 3rd char is ':' => valid charLit Ex: :x:
                {
                    token.lexem = this_char; //return the 2nd char
                }
                else //if 3rd char is not ':' => invalid charLit
                {
                    this_char = next_char;
                    //taking everything till seeing a ':'
                    do
                    {
                        lexeme += this_char;
                        this_char = get_next();
                    } while (this_char != ':' || !source);
                    lexeme += this_char;
                    token.lexem = lexeme;
                    token.name = TOKEN[errorToken];
                }
            }
        }
        else //2nd char is another ':' => possible string lit
        {
            token.name = TOKEN[stringLitToken];
            this_char = next_char;

            //take things till see '::' => means endyet = true or meet end of file
            do {
                //keep getting things till see ':' or '@' or end of fike
                do
                {
                    lexeme += this_char;
                    this_char = get_next();
                } while (this_char != ':' && this_char != '@' && source);

                switch (this_char)
                {
                case ':': //3rd ':'
                    lexeme += this_char;

                    next_char = get_next();
                    if (next_char == ':') //4th ':'
                    {
                        endyet = true;
                        token.lexem.assign(lexeme, 2, lexeme.length() - 3); //wtf is valid string
                    }
                    else
                    {
                        token.name = TOKEN[errorToken]; //invalid use of ':' without an escape character
                        endyet = true;

                        token.lexem = lexeme;
                    }
                    break;
                case '@': //possible use of escape character
                    next_char = get_next();

                    switch (next_char)
                    {
                    case '@':
                        this_char = next_char;
                        break;
                    case ':':
                        this_char = next_char;
                        break;
                    default:
                        lexeme += '@';
                        lexeme += next_char;
                        token.name = TOKEN[errorToken]; //invalid use of an escape character
                        token.lexem = lexeme;
                        endyet = true;
                        break;
                    }
                    break;
                default:
                    if (!source) //source end without 2nd "::"
                        token.name = TOKEN[errorToken];
                    break;
                }
            } while (!endyet && source);
        }

        break;

        //idToken recognition
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        do
        {
            lexeme += this_char;
            this_char = get_next();
        } while (!isGoodToEnd(this_char));

        if (this_char == '\n') line--;
        source.unget();

        x = isReservedWord(lexeme);
        if (x == -1)
        {
            token.name = TOKEN[idToken];
            token.lexem = lexeme;
        }
        else
        {
            token.name = TOKEN[x];
            token.lexem = lexeme;
        }
        break;
    default:
        do
        {
            lexeme += this_char;
            this_char = get_next();
        } while (!isGoodToEnd(this_char));
        token.name = TOKEN[errorToken];
        token.lexem = lexeme;
    }

    return token;
}

//big scanning here
Token scanning(ifstream& source)
{
    char next_char;
    Token token;

    while (source)
    {
        next_char = get_next();

        //check if it is a comment
        if (next_char == '?')
        {
            if (next_char == '\n') line--;
            source.unget();
            ignore_comment();
        }
        //check if it is a blank
        else if (isspace(next_char) || !source)
            if (!source) token.name = TOKEN[haltToken];
            else
            {
                if (next_char == '\n') line--;
                source.unget();

                token = scanner(source);
            }
    }

    return token;
}

/*
* Move the file pointer pass the comment
*
* @param source: the file pointer
* @return a character if it is not a comment or the character at the end of the comment.
*/
void ignore_comment()
{
    char ch;                //most recent char
    char lookahead;         //the one right next to ch, to determine valid end multi-line comment
    bool endyet = false;    //is the comment end yet

    source >> noskipws >> ch;
    if (ch == '\n') line++;

    if (ch == '?')    //is this a start of a comment
    {
        source >> noskipws >> ch;
        if (ch == '\n') line++;

        if (ch == '?') //multiline comment
        {
            source >> noskipws >> ch;
            if (ch == '\n') line++;

            while (!endyet)
            {
                while (ch != '?')
                {
                    source >> noskipws >> ch;
                }
                source >> noskipws >> lookahead;
                if (ch == '\n') line++;

                if (lookahead == '?')
                {
                    source >> noskipws >> ch;
                    if (ch == '\n') line++;

                    endyet = true;
                }
                if (!endyet)
                {
                    source >> noskipws >> ch;
                    if (ch == '\n') line++;
                }
            }

            if (ch == '\n') line--;
            source.unget();
        }
        else //this is a single-line comment
        {
            do
            {
                source >> noskipws >> ch;
            } while (ch != '\n');
            line++;
        }
    }
}

/*File pointer get updated after the function
*
* @param source: file pointer
* @return The next character in the file
*/
char get_next()
{
    char ch;
    source >> noskipws >> ch;
    if (ch == '\n') line++;
    ch = (char)toupper(ch);
    return ch;
}

/*
* @param id: string identifier
* @return -1 if identifier is not a reserved word
* @return a number from 0 to TOKEN_LIST_SIZE if it is a reserved word
*/
int isReservedWord(string id)
{
    int x = -1;
    for (int i = 0; i <= TOKEN_LIST_SIZE; i++)
    {
        if (RESEVERD_WORD[i] == id)
        {
            x = i;
            break;
        }
    }

    return x;
}

/*
* @return true if the passed character is a single-character token
* or a start of a comment
* or a space
* @return false otherwise
*/
bool isGoodToEnd(char this_char)
{
    bool yes = false;
    if (isspace(this_char) || this_char == '.' || this_char == '?'
        || this_char == '<' || this_char == '>' || this_char == '[' || this_char == ']'
        || this_char == '{' || this_char == '}' || this_char == '(' || this_char == ')'
        || this_char == '+' || this_char == '-' || this_char == '/'
        || this_char == '=' || this_char == ';')
        yes = true;
    return yes;
}

string get_rest()
{
    string str;
    char ch;

    source.unget();
    ch = get_next();
    do
    {
        str += ch;
        ch = get_next();
    } while (!isGoodToEnd(ch));

    if (ch == '\n') line--;
    source.unget();

    return str;
}

bool isspace(char h)
{
    bool result = false;
    switch (h)
    {
    case '\n':
    case '\t':
    case ' ':
    case '\v':
    case '\r':
    case '\f':
        result = true;
        break;
    default:
        break;
    }
    return result;
}

//turn a string into upercase
void uppercase(string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = (char)toupper(str[i]);
    }
}

void Switch(int x)
{
    //int save;

    //match("cacToken");
    //match("leftSquareBracketToken ");
    //expr();
    //match(TOKEN[rightSquareBracketToken]);
    //emit1(LDM4);    //load 1st case statement on expression stack
    //save = curr_addr;        // saves a space in the target codeís
    //emit4(0);            // memory  to put the low value in   :
    //caseBlockList();  // sends back the low and high values
    //
    ////now go back into memory  to ìsaveî and put
    ////  the actual value of low in

}

void multiDec()
{

}

/*
* Checking if the next token's name is match with the string
* @param str: the string to match
*/
void match(string str)
{
    if (curr_token.name == str) curr_token = next_token();
    else {
        error("syntax error - Token mismatch!!!");
        exit(EXIT_FAILURE);
    }
}

void match(Token t, string str)
{
    if (t.name != str)
        error("Token mismatch!!!");
}

void declaration()
{
    if (curr_token.name == TOKEN[defToken])
        funDec();
    else if (curr_token.name == TOKEN[arrayToken])
        arrayDec();
    else if (curr_token.name == TOKEN[ptrToken])
        ptrDec();
    else
        varDec();
}

void ptrDec()
{
    match(TOKEN[ptrToken]);
    decToken();
    ptrDecRest();
}

//wtf
void assign()
{
   /* lvalue(type1, address, storage class);
     storage class - auto(L) or static(G)
    match("=");
    expr(type2);
     type checking stuff
    if (type1 == "int" && type2 == "real");
     truncate type2 to int
    else if (type1 == "real" && type2 == "int");
         FLT type2(expr result)*/

}

void emitIntValue(char z[])
{
    emit1(LDM4);
    emit4(atoi(z));
}

void emitRealValue(char z[])
{
    float zf;
    zf = atof(z);
    emit1(LDM4);
    emit4(unreal(zf));
}

void emit1(int opcode)
{
    M[curr_addr++] = opcode & 0xFF;
    cout.setf(ios::hex);
    if (DEBUG)
    {
        printf("\n emit1 at %d:%x", curr_addr, M[curr_addr - 1]);
    }
}

void emit2(int value)
{
    M[curr_addr++] = value & 0xFF;
    M[curr_addr++] = (value & 0xFF00) >> 8;
    if (DEBUG)
    {
        printf("\n emit2 at %d:%x", curr_addr - 1, M[curr_addr - 1]);
    }

}

void emit4(int value)
{
    M[curr_addr++] = value & 0xFF;
    M[curr_addr++] = (value & 0xFF00) >> 8;
    M[curr_addr++] = (value & 0xFF0000) >> 16;
    M[curr_addr++] = (value & 0xFF000000) >> 24;
    if (DEBUG)
    {
        printf("\n emit4 %x", M[curr_addr - 1]);
    }

}

void backPatch1(int opcode, int addr)
{
    M[addr] = opcode & 0xFF;
}

void backPatch2(int value, int addr)
{
    M[addr] = value & 0xFF;
    M[addr + 1] = (value & 0xFF00) >> 8;
}

void backpatch4(int value, int addr)
{
    M[addr] = value & 0xFF;
    M[addr + 1] = (value & 0xFF00) >> 8;
    M[addr + 2] = (value & 0xFF0000) >> 16;
    M[addr + 3] = (value & 0xFF000000) >> 24;
}

void print(Symbol s)
{
    cout << "Symbol detail:";
    cout << "\tName---" << s.name;
    cout << "\tType---" << s.type;
    cout << "\tKind---" << s.kind;
    cout << "\tScope---" << s.scope;
    cout << "\tAddress---" << s.address << endl;
}

void print(Token t)
{
    cout << "Token detail:";
    cout << "\tLexeme---" << t.lexem;
    cout << "\tName---" << t.name << endl;
}

void parser()
{
    curr_token = next_token();
    if (curr_token.name == TOKEN[haltToken])
    {
        emit1(HALT);
        backPatch2(curr_addr, G);
    }
    else if (curr_token.name == TOKEN[defToken])
    {
        funDec();
    }
    else
    {
        declaration();
        parser();
    }

    //do parsing stuff

}

void ifStatement()
{
    //Token token;
    //string type;
    //int save, saveAdd;

    //match(TOKEN[thisToken]);
    //token = expr();
    //token.name = type;

    //if (type != TOKEN[boolToken])
    //    error("type mismatch in if statement");

    //emit1(BNO);
    //saveAdd = curr_addr;
    //emit4(0);
    //save = curr_addr;

    ////process through the whole block the get the next PC
    //codeBlock();
    //
    //backpatch4(curr_addr - save, saveAdd);
}

//display error msg
void error(string str)
{
    cout << str << endl;
    cout << "At line " << line << endl;
    exit(EXIT_FAILURE);
}

//call the scanner and return the next token
Token next_token()
{
    return scanning(source);
}

/*Convert an real number x to an int
* @param x: a float number
* @return an int
*/
unsigned int unreal(float x) {
    float f = x;
    int* i;
    i = (int*)&f;
    return *i;
}

Token expr()
{
    Token token;

    return token;
}

//void ifStatement_inClass()
//{
//    Token token;
//    string type;
//    int saveAdd1,  //address of BNO expr1
//        saveAdd2,  //address of BNO expr2
//        saveAdd,  //address of the end of all things
//        save;
//
//    match("if");
//    token = expr1();
//    token.name = type;
//
//    if (type != "boolToken")
//        error("type mismatch in if statement");
//
//    add a BNO
//    emit1(BNO);
//    saveAdd = curr_addr;
//    emit2(0);
//    save = curr_addr;
//
//    process CB1
//    match("{");
//
//    codeBlock();
//    emit1(JMPD);
//    saveAdd1 = curr_addr;
//    emit2(0);//add dummy 1
//
//    match("}"); //closing CB1
//    backPatch2(curr_addr - save - 2, saveAdd);
//
//    match("else");
//    match("if");
//
//    token = expr2();
//    token.name = type;
//    if (type != "boolToken")
//        error("type mismatch in if statement");
//
//    add a BNO for expr2
//    emit1(BNO);
//    saveAdd = curr_addr;
//    emit2(0);
//    save = curr_addr;
//
//    process CB2
//    match("{");
//
//    codeBlock();
//    emit1(JMPD);
//    saveAdd2 = curr_addr;
//    emit2(0);//add dummy 2
//
//    match("}"); //closing CB2
//    backPatch2(curr_addr - save-2, saveAdd);
//
//    match("else");
//    process CB3
//    match("{");
//    codeBlock();
//    match("}"); //closing CB3
//
//    backPatch2(curr_addr, saveAdd1);
//    backPatch2(curr_addr, saveAdd2);
//}

void export_memory() {

    ofstream file;

    file.open("target_code.txt");

    if (!file) {
        cout << "Error: Could not open file" << endl;
        return;
    }

    for (int i = 0; i < MemSize; i++) {
        file << M[i] << endl;;
    }

    file.close();
}

void whileLoop()
{
    int exadd, saveadd, save;
    Token token;

    match(TOKEN[rptToken]);
    exadd = curr_addr;
    token = expr();
    if (token.name != TOKEN[boolLitToken])
        error("Type mismatch!");

    emit1(BYES); //cus this is a repeat not a while
    saveadd = curr_addr;
    emit1(0);

    save = curr_addr;
    codeBlock(); //or one statement
    emit1(JMPD);
    emit2(exadd);
    backPatch2(curr_addr - save, saveadd);
}

void varDec()
{
    decToken();
    varDecRest();
}

void varDecRest()
{
    lookahead = next_token();

    if (lookahead.name == TOKEN[assignmentToken])
        assignment();
    else if (lookahead.name == TOKEN[idToken])
    {
        curr_token = lookahead;
        multiDec();
        match(TOKEN[endOfLineToken]);
    }
    else
        error("Invalid Declaration!!!");
}

void decToken()
{
    if (curr_token.name == TOKEN[intDecToken])
        varSize = INT;
    else if (curr_token.name == TOKEN[realDecToken])
        varSize = REAL;
    else if (curr_token.name == TOKEN[stringDecToken])
        varSize = STR;
    else if (curr_token.name == TOKEN[charDecToken])
        varSize = CHAR;
    else if (curr_token.name == TOKEN[boolDecToken])
        varSize = YN;
    else if (curr_token.name == TOKEN[hexDecToken])
        varSize = HEX;
    else
        error("Invalid Declaration!!!");
}
// uses an idToken
void ptrDecRest()
{

}

// uses an idToken
void multiDec()
{
    
}

void arrayDec()
{
    match(TOKEN[arrayToken]);
    decToken();
    arrayDecRest();
}

void arrayDecRest()
{
    if(curr_token.name == TOKEN[idToken])
    {
        array();
        multiArrayDec();
        match(TOKEN[endOfLineToken]);
    }
    else
        assignment();
}

void multiArrayDec()
{
    if()
    {
        match(TOKEN[semicolonToken]);
        array();
        multiArrayDec();
    }
    else
        // lambda
}

// uses id token
void array()
{
    
}

void varDec()
{
    decToken();
    varDecRest();
}

// uses id token
void varDecRest()
{
    
}

void program()
{
    if()
    {
        declaration();
        program();
    }
    else
        funDec();
}

// uses id Token
void assignment()
{
    
}

void assignBody()
{
    if()
    {
        match(TOKEN[assignmentToken]);
        expr();
        assignBody();
    }
    else
        match(TOKEN[assignmentToken]);
        expr();
}

void arrayInit()
{
    match(TOKEN[leftParenthesisToken]);
    arrayInitBody();
    match(TOKEN[rightParenthesisToken]);
}

void arrayInitBody()
{
    if()
    {
        litToken();
        match(TOKEN[semicolonToken]);
        arrayInitBody();
        
    }
    else if()
    {
        litToken();
    }
    else
        // lambda
}

// uses id token
void arrayCall()
{
    
}

// uses id token
void factor()
{
    
}

void funDecType()
{
    if()
    {
        match(TOKEN[decToken]);

    }
    else
        noneToken;
}

// uses id token
void funDec()
{
   
}

void paramList()
{
    if()
    {
        varDec();
        match(TOKEN[semicolonToken]);
        paramList();
    }
    else if()
    {
        varDec();
    }
    else
        // lambda
}

// uses id token
void call()
{
    
}


void passedParam()
{
    if()
    {
        litOrId();
        match(TOKEN[semicolonToken]);
        passedParam();
    }
    else if()
    {
        litOrId();
    }
    else
        // lambda
}

// uses id token
void litOrId()
{
    
}

void codeBlock()
{
    match(TOKEN[leftBlockToken]);
    statementList();
    match(TOKEN[rightBlockToken]);
}

void statementList()
{
    if()
    {
        statement();
        statementList();
    }
    else
        // lambda
}

void statement()
{
    
}

// void return()
//{
    
//}

void forLoop()
{
    match(TOKEN[loopToken]);
    match(TOKEN[leftSquareBracketToken]);
    factor();
    match(TOKEN[rightSquareBracketToken]);
    loopRest();
}

void loopRest()
{
    if()
    {
        codeBlock();
    }
    else
        statement();
        match(TOKEN[endOfLineToken]);
}

void ifStatement()
{
    match(TOKEN[thisToken]);
    match(TOKEN[leftSquareBracketToken]);
    expr();
    match(TOKEN[rightSquareBracketToken]);
    loopRest();
}

void ifElseStatement()
{
    ifStatement();
    match(TOKEN[thisToken]);
    loopRest();
}

void Switch()
{
    match(TOKEN[cacToken]);
    match(TOKEN[leftSquareBracketToken]);
    factor();
    match(TOKEN[rightSquareBracketToken]);
    match(TOKEN[leftBlockToken]);
    switchBody();
    match(TOKEN[rightBlockToken]);
}

void switchBody()
{
    if()
    {
        optPart();
        norm();
    }
    else if()
    {
        norm();
    }
    else
        optPart();
}

void optPart()
{
    if()
    {
        opt();
        optPart();
    }
    else
        // lambda
}

void opt()
{
    match(TOKEN[optToken]);
    factor();
    codeBlock();
}

void norm()
{
    match(TOKEN[normToken]);
    codeBlock();
}

void whileLoop()
{
    match(TOKEN[tilToken]);
    match(TOKEN[leftSquareBracketToken]);
    expr();
    match(TOKEN[rightSquareBracketToken]);
    loopRest();
}

void doWhileLoop()
{
    match(TOKEN[rptToken]);
    loopRest();
    match(TOKEN[tilToken]);
    match(TOKEN[leftSquareBracketToken]);
    expr();
    match(TOKEN[rightSquareBracketToken]);
}

void get()
{
    match(TOKEN[getToken]);
    match(TOKEN[leftParenthesisToken]);
    getBody();
    match(TOKEN[rightParenthesisToken]);
    match(TOKEN[endOfLineToken]);
}

// uses id token
void getBody()
{
    
}

void send()
{
    match(TOKEN[sendToken]);
    match(TOKEN[leftParenthesisToken]);
    sendBody();
    match(TOKEN[rightParenthesisToken]);
    match(TOKEN[endOfLineToken]);
}

void sendBody()
{
    if()
    {
        sendToken2();
        match(TOKEN[semicolonToken]);
        sendBody();
    }
    else
        sendToken2();
}

void sendToken2()
{
    
}

void litToken()
{
    
}

void addOp()
{
    
}

void mulOp()
{
    
}

void relOp()
{
    
}

Token expr()
{
    
}

Token exprE()
{
    
}

Token exprD()
{
    
}

Token exprC()
{
    
}
Token exprB()
{
    
}

Token exprA()
{
    
}

Token expr1()
{
    
}

Token unary()
{
    
}

void numLit()
{
    
}

void unaryOp()
{
    
}
