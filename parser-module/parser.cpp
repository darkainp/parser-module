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
static int Arr = 0;//this change when the size is known in the parser
/******************************************************/

const int TOKEN_LIST_SIZE = 70;
const string TOKEN[TOKEN_LIST_SIZE] = { "stringLitToken", "stringDecToken", "stringToken", "charLitToken", "charToken", "charDecToken",
"intLitToken", "intDecToken", "intToken", "realLitToken", "realDecToken", "boolLitToken", "boolDecToken", "boolToken", "hexLitToken",
"hexDecToken", "hexToken", "endOfLineToken", "commentToken", "multiCommentToken", "addToken", "subToken", "multToken", "divToken",
"modToken", "incToken", "decToken", "negToken", "equalToToken", "notEqualToToken", "greaterThanToken", "lessThanToken", "greaterThanEqualToken",
"lessThanEqualToken", "orToken", "andToken", "thisToken", "thatToken", "loopToken", "rptToken", "tilToken", "cacToken", "optToken",
"normToken", "defToken", "callToken", "rtToken", "ptrToken", "arrayToken", "noneToken", "getToken", "sendToken", "escToken", "stopToken",
"leftBlockToken", "rightBlockToken", "leftParenthesisToken", "rightParenthesisToken", "leftSquareBracketToken", "rightSquareBracketToken",
"assignmentToken", "semicolonToken", "msgToken" , "errorToken","idToken","yesToken","noToken","haltToken", "dereferenceToken"
, "addressToken" };

const string RESEVERD_WORD[TOKEN_LIST_SIZE] = { "", "STR", "", "", "", "CHAR",
"", "INT", "", "", "REAL", "", "YN", "", "",
"HEX", "", "", "", "", "", "", "X*", "",
"MOD", "INC", "DEC", "NT", "E2", "NE2", "GT", "LT", "GTE2",
"LTE2", "OR", "AND", "THIS", "THAT", "LOOP", "RPT", "TIL", "CAC", "OPT",
"NORM", "DEF", "CALL", "RT", "PTR", "ARR", "NONE", "GET", "SEND", "", "STOP",
"", "", "", "", "", "",
"", "", "" , "","", "YES", "NO" ,"","",
""};

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
unsigned int M[MemSize] = { 0 };           /* RAM*/


/******************Prototype list**********************/
struct Symbol {
    string name;
    string type; //int, real, char...
    string kind; //fun, param, var
    int scope = 0;
    int address = 0;
};

class SymbolTable
{
private:
    Symbol table[100];
    int count;            //count is how many element existed in the table, so counter = count - 1
    int current_scope;
    int current_address[50];        //current_address of scope 0 is current_address[0]
public:
    SymbolTable();

    void add_symbol(Symbol);
    void del_current_scope();

    static Symbol create_symbol(string, string, string);    //need the namespace SymbolTable::create_symbol
    int find_symbol(string) const;
    int find_symbol(int, string) const;
    void print_symbol(int) const;
    void printTable() const;
    Symbol get_symbol(int) const;
    Symbol get_latest_symbol() const;
    int get_count() const { return this->count; }

    //wat is in his paper
    int get_num_args(string);
    int getActRec_size(string);
    void reclaim(Symbol);
};
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
    semicolonToken = 61, msgToken = 62, errorToken = 63, idToken = 64, yesToken = 65, noToken = 66, haltToken = 67, dereferenceToken = 68,
    addressToken = 69

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
void Array();
void arrayDec();
void arrayDecRest();
void multiArrayDec();
void varDec();
void varDecRest();
//void program();
void assignment();
void assignBody();
void arrayInit();
void arrayInitBody();
void arrayCall();
void factor();
void DecToken();
void funDecType();
void funDec();
void paramList();
void paramListRest();
string call(); //return a string of function return type
void passedParam();
void litOrId();
void statementList();
void statement();
void forLoop();
void codeBlock();
void ifStatement();
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
void callLitToken();
void litToken();
void addOp(string);
bool isAddOp();
void mulOp(string);
bool isMulOp();
void relOp(string);
bool isRelOp();
string expr();
string exprE();
string exprD();
string exprC();
string exprB();
string exprA();
string expr1();
string unary();
bool isNumLit();
void unaryOp(string);
bool isUnaryOp();

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
float real(unsigned int);
unsigned int unreal(float);
void error(string);
Token next_token();
//int caseBlockList();
void export_memory();

/******************************************************/

static ifstream source;
static Token lookahead;
static Token curr_token;
static int curr_addr = 0;
static Symbol curr_symbol;
const bool DEBUG = 1;
static int line = 1;
static int G, L, PC;    //hold the address of G, L and PC; G only assign once.
static  bool isMsg = 0;  //how many function is declare in the program
//static int varSize; //the size of a variable
static SymbolTable symbol_table;
static Symbol assign_symbol;
static string curr_fun;
static bool unaryRan = 0;
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
        G = curr_addr;
        emit2(0); //reserve address of G    
        emit1(POPG);

        emit1(LDM2);
        L = curr_addr;
        emit2(0);  //reserve address of L
        emit1(POPL);

        emit1(LDM2);
        PC = curr_addr;
        emit2(0);
        emit1(POPP);

        emit1(DSTK);
        //emit1(HALT);



        lookahead = next_token();
        
        //parsing here
        parser();

        if (!isMsg)
            error("Empty program!!!");

        //print MM out in a file
        export_memory();

        //close source when done, this leave at the end
        source.close();
    }

    if(DEBUG)
        cout << endl << "L = " << M[L] << "\nG = " << M[G] << "\nPC = " << M[PC];

    return 0;
}

/*
print the symbol table
*/
void SymbolTable::printTable() const
{
    int width = 10;
    cout << setw(width * 2) << "Name" << setw(width) << "Type" << setw(width) << "Scope" << setw(width) << "Kind" << setw(width) << "Address" << endl;
    for (int i = 0; i < count; i++)
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
* @return the newest symbol in the symbol table
*/
Symbol SymbolTable::get_latest_symbol() const
{
    return table[count - 1];
}

int SymbolTable::get_num_args(string fname)
{
    int i = 0;
    int c = 0;
    i = this->find_symbol(fname);
    while (this->get_symbol(++i).kind == "PARAM")
    {
        c++;
    }
    return c;
}

int SymbolTable::getActRec_size(string funName)
{
    Symbol fun;
    int pos, size = 4; //size starts at 4 for the CL and AL 2 bytes each 
    pos = find_symbol(funName);
    fun = get_symbol(pos);
    switch (fun.type[0])   //add size of return type
    {
    case 'C':
        size += CHAR;
        break;
    case 'I':
        size += INT;
        break;
    case 'R':
        size += REAL;
        break;
    case 'H':
        size += HEX;
        break;
    case 'N':
        size += NONE;
        break;
    case 'S':
        size += STR;
        break;
    case 'P':
        size += PTR;
        break;
    case 'Y':
        size += YN;
        break;
    default:
        cout << "Type is undefined!!!" << endl;
        break;
    }
    while (get_symbol(++pos).kind == "PAR")   //add size of each of the parameters
    {
        switch (get_symbol(pos).type[0])
        {
        case 'C':
            size += CHAR;
            break;
        case 'I':
            size += INT;
            break;
        case 'R':
            size += REAL;
            break;
        case 'H':
            size += HEX;
            break;
        case 'N':
            size += NONE;
            break;
        case 'S':
            size += STR;
            break;
        case 'P':
            size += PTR;
            break;
        case 'Y':
            size += YN;
            break;
        default:
            cout << "Type is undefined!!!" << endl;
            break;
        }
    }
    return size;
}

//dec the scope
void SymbolTable::reclaim(Symbol s)
{
    current_scope = s.scope;
    del_current_scope();
}

/*
* constructor
* @param s: size of the table
*/
SymbolTable::SymbolTable()
{
    count = 0;
    current_scope = 0;
    current_address[0] = 0;
}

/*to add a symbol to the table
* @param input: is the symbol to add
*/
void SymbolTable::add_symbol(Symbol input)
{
    table[count] = input;

    //get scope and address
    table[count].scope = current_scope;
    table[count].address = current_address[current_scope];

    //if kind is an arr then address is different:
    if (table[count].kind == "Arr")
    {
        table[count].address = current_address[current_scope];
        current_address[current_scope] += Arr;
    }
    else
        //update address
        switch (table[count].type[0])
        {
        case 'C':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += CHAR;
            break;
        case 'I':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += INT;
            break;
        case 'R':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += REAL;
            break;
        case 'H':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += HEX;
            break;
        case 'N':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += NONE;
            break;
        case 'S':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += STR;
            break;
        case 'P':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += PTR;
            break;
        case 'Y':
            table[count].address = current_address[current_scope];
            current_address[current_scope] += YN;
            break;
        default:
            cout << "Type is undefined!!!" << endl;
            break;
        }

    //update scope
    if (table[count].kind == "FUN")
    {
        current_scope++;
        current_address[current_scope] = 0;
    }

    count++;
}

//delete the most current scope
void SymbolTable::del_current_scope()
{
    int curr_scope = current_scope+1;

    if (count > 0)
    {
        while (curr_scope == table[count - 1].scope)
            count--;
        current_scope = table[count - 1].scope;
        //current_address[current_scope] = table[count - 1].address;
    }
    else
        error(EMPTY_TABLE);
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

    for (int i = count - 1; i >= 0; i--)
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

int SymbolTable::find_symbol(int s, string str) const
{
    int pos = -1;
    uppercase(str);

    bool found = false;

    for (int i = s-1; i >= 0; i--)
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
    case '^':
        token.lexem = "^";
        token.name = TOKEN[dereferenceToken];
        break;
    case '#':
        token.lexem = "#";
        token.name = TOKEN[addressToken];
        break;
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
                        token.lexem = lexeme.substr(2,lexeme.length()-2);
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
    
    token.name = "";
    while (token.name == "")
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
        {
            if (!source) token.name = TOKEN[haltToken];
            //else;
        }
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
        || this_char == '=' || this_char == ';'|| this_char == '^' || this_char == '#' )
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

void Switch()
{
    int save, pos;
    string t;
    match(TOKEN[cacToken]);

    t = expr();

    curr_symbol;
    if (curr_symbol.type != "INT")
        error("Token for switch mismatch!!!");
   

    match(TOKEN[leftBlockToken]);
    switchBody();
    match(TOKEN[rightBlockToken]);

    emit1(LDM4);    //load 1st case statement on expression stack
    save = curr_addr;        // saves a space in the target code
    emit4(0);            // memory  to put the low value in :
    //caseBlockList();  // sends back the low and high values
    
    //now go back into memory  to "save" and put
    //  the actual value of low in

}

void funDec()
{
    curr_symbol.kind = "FUN";
    int pos;
    //update lookahead
    curr_token = lookahead;
    lookahead = next_token();

    funDecType();
    match(TOKEN[idToken]);
    curr_symbol.name = curr_token.lexem;
    
    match(TOKEN[leftSquareBracketToken]);
    paramList();
    match(TOKEN[rightSquareBracketToken]);

    symbol_table.add_symbol(curr_symbol);
    symbol_table.printTable();
    curr_fun = curr_symbol.name;
    if (curr_symbol.name == "MSG")
    {
        isMsg = true;
        if (curr_symbol.scope != 0)
            error("MSG must be declared at scope 0!!!");
        if (curr_symbol.type != "NONE")
            error("MSG must be of type NONE!!!");
        pos = symbol_table.find_symbol("MSG");
        if (symbol_table.find_symbol(pos,"MSG") != -1)
            error("MSG can only be declared once!!!");

        //set PC to the main
        backPatch2(curr_addr, PC);        
    }
    codeBlock();
    //symbol_table.printTable();

    if (curr_fun == "MSG")
    {
        //end of the MSG
        emit1(HALT);
    }
    
    //save new L and PC
    //if (curr_symbol.name == "MSG")
    //{
    //    emit1(LDLR2);
    //    emit2(4);//wtf
    //    emit1(LDLR2);
    //    emit2(2);//wtf
    //    emit1(POPL);
    //    emit1(POPP);
    //}
    //else
    //{
    //    emit1(LDM1);
    //    emit1(0);

    //    emit1(LDLR2);
    //    emit2(4);
    //    emit1(LDLR2);
    //    emit2(2);
    //    emit1(POPL);
    //    emit1(POPP);
    //}

    symbol_table.reclaim(curr_symbol);
}

void paramList()
{
    if (lookahead.name != TOKEN[rightSquareBracketToken])
    {
        curr_symbol.kind = "PARAM";
        DecToken();
        match(TOKEN[idToken]);
        curr_symbol.name = curr_token.lexem;
        symbol_table.add_symbol(curr_symbol);
        paramListRest();
    }
}

void paramListRest()
{
    if (lookahead.name == TOKEN[semicolonToken])
    {
        match(TOKEN[semicolonToken]);
        paramList();
        paramListRest();
    }
}

string call()
{
    return string();
}

void multiDec()
{
    if (lookahead.name == TOKEN[semicolonToken])
    {
        match(TOKEN[semicolonToken]);
        match(TOKEN[idToken]);
        curr_symbol.name = curr_token.lexem;
        symbol_table.add_symbol(curr_symbol);

        multiDec();
    }

    //cout << lookahead.name << "--" << TOKEN[assignmentToken] << endl;

    if (lookahead.name == TOKEN[assignmentToken])
    {
        assignment();
        multiDec();
    }
}

/*
* Checking if the next token's name is match with the string
* Lookahead would be update if match
* @param str: the string to match
*/
void match(string str) 
{
    //lookahead = next_token();
    if (lookahead.name == str) 
    {
        curr_token = lookahead;
        lookahead = next_token(); 
    }
    else {
        error("syntax error - Missing token: " + str);
    }
}

/*
* Checking if the token's name is match with the string
* Lookahead would NOT be update after match
* @param str: the string to match
* @param t: the token to match
*/
void match(Token t, string str)
{
    if (t.name != str)
    {
        error("syntax error - Token mismatch!!!");
        exit(EXIT_FAILURE);
    }
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
    curr_symbol.kind = "PTR";
    curr_token = lookahead;
    lookahead = next_token();

    DecToken();

    ptrDecRest();
}

void ptrDecRest()
{
    match(TOKEN[idToken]);

    curr_symbol.name = curr_token.lexem;
    symbol_table.add_symbol(curr_symbol);

    if (lookahead.name == TOKEN[assignmentToken])
        assignment();
    else
    {
        multiDec();
        //match(lookahead, TOKEN[endOfLineToken]);
        match(TOKEN[endOfLineToken]);
    }
}

void Array()
{
    match(TOKEN[idToken]);

    //update name
    curr_symbol.name = curr_token.lexem;

    match(TOKEN[leftParenthesisToken]);
    match(TOKEN[intLitToken]); //current tokken is now intLit
    
    //get the value, if there is a problem here then stoi is the problem
    Arr = stoi(curr_token.lexem);

    //update the size of the array
    switch(curr_symbol.type[0])
    {
    case 'C':
        Arr *= CHAR;
        break;
    case 'I':
        Arr *= INT;
        break;
    case 'R':
        Arr *= REAL;
        break;
    case 'H':
        Arr *= HEX;
        break;
    case 'S':
        Arr *= STR;
        break;
    case 'P':
        Arr *= PTR;
        break;
    case 'Y':
        Arr *= YN;
        break;
    default:
        cout << "Type is undefined!!!" << endl;
        break;
    }

    match(TOKEN[rightParenthesisToken]);

    //put the symbol in the symbol table
    symbol_table.add_symbol(curr_symbol);
    Arr = 0;
}

void arrayDec()
{
    curr_symbol.kind = "Arr";
    curr_token = lookahead;
    lookahead = next_token();

    DecToken();
    arrayDecRest();
}

void arrayDecRest()
{
    Array();

    if (lookahead.lexem == TOKEN[endOfLineToken])
        match(TOKEN[endOfLineToken]);
    else
    {
        multiArrayDec();
        match(TOKEN[endOfLineToken]);
    } 
}

void multiArrayDec()
{
    if (lookahead.name == TOKEN[semicolonToken])
    {
        Array();
        multiArrayDec();
    }
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

/*Convert an int x to a float
* @param x: an integer
* @return a float number
*/
float real(unsigned int x) {
    int i = x;
    float* f;
    f = (float*)&i;
    return *f;
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
    curr_token = lookahead;
    lookahead = next_token();

    if (curr_token.name == TOKEN[haltToken])
    {
        backPatch2(curr_addr, G);
        //It's just reach the end of the file, we stop compiling.
        
        //code gen of reserving memory location for global var
        int i = 0;
        curr_symbol = symbol_table.get_symbol(i);

        if (DEBUG)
            cout << endl << symbol_table.get_count() << endl;

        while (curr_symbol.scope == 0 && i <= symbol_table.get_count())
        {
            if (curr_symbol.kind == "VAR")
            {
                switch (curr_symbol.type[0])
                {
                case 'C':
                    emit1(0);
                    break;
                case 'I':
                case 'R':
                case 'H':
                    emit4(0);
                    break;
                case 'N':
                    break;
                case 'S':
                    emit4(0);
                    emit4(0);
                    emit4(0);
                    emit4(0);
                    emit4(0);
                    break;
                case 'Y':
                    emit1(0);
                    break;
                default:
                    cout << "Type is undefined!!!" << endl;
                    break;
                }
            }
            else if (curr_symbol.kind == "PTR")
                emit2(0);
            
            curr_symbol = symbol_table.get_symbol(++i);
        }

        backPatch2(curr_addr, L);

    }
    else
    {
        if (DEBUG)
        {
            symbol_table.printTable();
            cout << endl;
        }
        declaration();
        parser();
    }
    //do parsing stuff

}

//this is if-else statement
void ifStatement()
{
    int jumpOut, jump_that;
    string type;

    match(TOKEN[thisToken]);
    type = expr();

    if (type != TOKEN[boolLitToken])
        error("type mismatch in if statement");

    emit1(TNZ);
    emit1(BYES);
    emit1(3);
    emit1(JMPD);
    jump_that = curr_addr;
    emit2(0);

    //process through the whole block the get the next PC
    codeBlock();
    emit1(JMPD);
    jumpOut = curr_addr;
    emit2(0);

    backPatch2(curr_addr, jump_that);
    if (lookahead.name == TOKEN[thatToken])
    {
        match(TOKEN[thatToken]);
        codeBlock();
    }

    backPatch2(curr_addr, jumpOut);
}

//display error msg
void error(string str)
{
    cout << endl << str << endl;
    cout << "At line " << line << endl;
    exit(EXIT_FAILURE);
}

//call the scanner and return the next token
Token next_token()
{
    Token t;
    t = scanning(source);
    if (DEBUG)
    {
        cout << endl;
        print(t);
    }
    return t;
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

string expr()
{
    string t1,t2;

    curr_token = lookahead;
    lookahead = next_token();

    t1 = exprE();
    //t2 = expr();
     
    return t1;
}
string exprE()
{
    string t1,t2,tr;

    if (lookahead.name == TOKEN[orToken])
    {
        t1 = exprD();

        curr_token = lookahead;
        lookahead = next_token();

        t2 = expr();

        emit1(OR);

        tr = TOKEN[boolLitToken];
    }
    else
    {
        tr = exprD();
    }

    //emit or here

    return tr;
}

string exprD()
{
    string t1, t2, tr;

    if (lookahead.name == TOKEN[andToken])
    {
        t1 = exprC();

        curr_token = lookahead;
        lookahead = next_token();

        t2 = expr();

        emit1(AND);

        tr = TOKEN[boolLitToken];
    }
    else
    {
        tr = exprC();
    }

    //emit or here

    return tr;
}

string exprC()
{
    string t1, t2, tr, op;

    if (isRelOp())
    {
        t1 = exprB();

        op = lookahead.lexem;

        curr_token = lookahead;
        lookahead = next_token();

        t2 = expr();

        switch (op[0])
        {
        case 'E':
            //equal to token
            emit1(SUB);
            emit1(TZE);
            break;
        case 'N':
            //not equal to token
            emit1(SUB);
            emit1(TNZ);
            break;
        case 'G':
            if (op == RESEVERD_WORD[greaterThanToken])
            {
                emit1(SUB);
                emit1(TPS);
            }
            else
            {
                //greater than equal to token 
                emit1(SUB);
                emit1(TNN);
            }
            break;
        case 'L':
            if (op == RESEVERD_WORD[lessThanToken])
            {
                emit1(SUB);
                emit1(TNG);
            }
            else
            {
                // less than equal to token
                emit1(SUB);
                emit1(TNP);
            }
            break;
        }

        tr = TOKEN[boolLitToken];
    }
    else
    {
        tr = exprB();
    }

    return tr;
}


string exprB()
{
    string t1, t2, tr, op;

    if (isAddOp())
    {
        t1 = exprA();

        op = lookahead.lexem;

        curr_token = lookahead;
        lookahead = next_token();

        t2 = expr();

        switch (op[0])
        {
        case '+':
            if (t1 == TOKEN[intLitToken] && t2 == TOKEN[intLitToken])
            {
                emit1(ADD);
                tr = TOKEN[intLitToken];
            }
            else
            {
                emit1(ADDF);
                tr = TOKEN[realLitToken];
            }
            break;
        case '-':
            if (t1 == TOKEN[intLitToken] && t2 == TOKEN[intLitToken])
            {
                emit1(SUB);
                tr = TOKEN[intLitToken];
            }
            else
            {
                emit1(SUBF);
                tr = TOKEN[realLitToken];
            }
            break;
        }
    }
    else
    {
        tr = exprA();
    }

    return tr;
}

string exprA()
{
    string t1, t2, tr, op;

    if (isMulOp())
    {
        t1 = expr1();

        op = lookahead.lexem;

        curr_token = lookahead;
        lookahead = next_token();

        t2 = expr();

        switch (op[0])
        {
        case 'X':
            if (t1 == TOKEN[intLitToken] && t2 == TOKEN[intLitToken])
            {
                emit1(MUL);
                tr = TOKEN[intLitToken];
            }
            else
            {
                emit1(MULF);
                tr = TOKEN[realLitToken];
            }
            break;
        case '/':
            emit1(DIVF);
            tr = TOKEN[realLitToken];
            break;
        case 'M':
            emit1(MOD);
            tr = TOKEN[intLitToken];
            break;
        }
    }
    else
    {
        tr = expr1();
    }

    return tr;
}

string expr1()
{
    string type;
    int pos;
    int scopeL = 0;

    if (curr_token.name == TOKEN[leftSquareBracketToken])
    {
        type = expr();

        match(TOKEN[rightSquareBracketToken]);
    }
    else if (curr_token.name == TOKEN[intLitToken] || curr_token.name == TOKEN[realLitToken] || curr_token.name == TOKEN[hexLitToken])
    {
        if (curr_token.name == TOKEN[intLitToken])
        {
            type = curr_token.name;

            emit1(LDM4);
            emit4(stoi(curr_token.lexem));

            //match(TOKEN[intLitToken]);
        }
        else if (curr_token.name == TOKEN[realLitToken])
        {
            type = curr_token.name;

            emit1(LDM4);
            emit4(unreal(stof(curr_token.lexem)));

            //match(TOKEN[realLitToken]);
        }
        else
        {
            type = curr_token.name;

            emit1(LDM4);
            //might get wrong hex value
            emit4((stoi(curr_token.lexem)));

            //match(TOKEN[hexLitToken]);
        }
    }
    else if (curr_token.name == TOKEN[idToken])
    {
        pos = symbol_table.find_symbol(curr_token.lexem);
        if (pos == -1)
            error("Identifier not found!!!");

        //match(TOKEN[idToken]);

        curr_symbol = symbol_table.get_symbol(pos);

        if (curr_symbol.kind == "VAR" || curr_symbol.kind == "PARAM")
        {
            switch (curr_symbol.type[0])
            {
            case 'I':
                //check scope
                if (curr_symbol.scope == 0)
                {
                    emit1(LDGR4);
                    emit2(curr_symbol.address);

                    type = TOKEN[intLitToken];
                }
                else
                {
                    scopeL = curr_symbol.scope;
                    //check L then load with L
                    emit1(LDR4);
                    //this is wrong emit
                    emit2(curr_symbol.address + scopeL);
                }
                break;
            case 'R':
                //check scope
                if (curr_symbol.scope == 0)
                {
                    emit1(LDGR4);
                    emit2(curr_symbol.address);

                    type = TOKEN[realLitToken];
                }
                else
                {
                    scopeL = curr_symbol.scope;
                    //check L then load with L
                    emit1(LDR4);
                    //this is wrong emit
                    emit2(curr_symbol.address + scopeL);
                }
                break;
            case 'Y':
                //check scope
                if (curr_symbol.scope == 0)
                {
                    emit1(LDGR1);
                    emit2(curr_symbol.address);

                    type = TOKEN[boolLitToken];
                }
                else
                {
                    //check L then load with L
                    emit1(LDR1);
                    emit2(curr_symbol.address + scopeL);
                }
                break;
            default:
                error("Identifier's type is not supported in expressions!!!");
                break;
            }
        }
        else if (curr_symbol.kind == "Arr")
        {
            //read in the pos of arr
            match(TOKEN[leftParenthesisToken]);
            type = expr();
            match(TOKEN[rightParenthesisToken]);
            
            //if global
            switch (curr_symbol.type[0])
            {
            case 'I':
            case 'H':
            case 'R':
                emit1(LDM4);
                emit4(4);
                emit1(MUL);
                emit1(LDGN4);
                emit2(curr_symbol.address);
                break;
            case 'C':
            case 'Y':
                emit1(LDGN1);
                emit2(curr_symbol.address);
                break;
            case 'S':
                //is wtf
                break;
            case 'P':
                //wtf
                emit1(LDM4);
                emit4(2);
                emit1(MUL);
                emit1(LDGN2);
                emit2(curr_symbol.address);
                break;
            }
        }
        else
            error("Identifier's kind is not supported in expressions!!!");
    }
    else if (curr_token.name == TOKEN[callToken])
    {
        //do function call
    }
    else
        type = unary();
    return type;
}

bool isNumLit()
{
    bool flag = false;
    if (lookahead.name == TOKEN[intLitToken])
        flag = true;
    else if (lookahead.name == TOKEN[realLitToken])
        flag = true;
    else if (lookahead.name == TOKEN[hexLitToken])
        flag = true;
    return flag;
}

void export_memory() {

    ofstream file, testfile, hexfile;

    file.open("target_code.x");
    testfile.open("test_target_code.txt");
    hexfile.open("hex.txt");

    if (!file) {
        cout << "Error: Could not open file" << endl;
        return;
    }

    for (int i = 0; i < MemSize; i++) {
        file << (unsigned char)M[i];// << endl;
        testfile << M[i] << endl;
        hexfile << hex << M[i] << endl;
    }

    file.close();
    testfile.close();
}

void whileLoop()
{
    int jumpOut, saveAdd;
    string type;

    match(TOKEN[tilToken]);
    saveAdd = curr_addr;
    type = expr();

    if (type != TOKEN[boolLitToken])
        error("type mismatch in while loop expression!!!");
    
    emit1(TZE);
    emit1(BYES);
    emit1(3);
    emit1(JMPD);
    jumpOut = curr_addr;
    emit2(0);

    //process through the whole block the get the next PC
    codeBlock(); 
    emit1(JMPD);
    emit2(saveAdd);
    backPatch2(curr_addr, jumpOut);
}

void doWhileLoop()
{
    int jumpOut, saveAdd;
    string type;

    match(TOKEN[rptToken]);
    saveAdd = curr_addr;
    codeBlock();
    match(TOKEN[tilToken]);
    type = expr();

    if (type != TOKEN[boolToken])
        error("type mismatch in do-while expression!!!");

    emit1(TZE);
    emit1(BYES);
    emit1(3);
    emit1(JMPD);
    jumpOut = curr_addr;
    emit2(0);

    //process through the whole block the get the next PC
    codeBlock();
    emit1(JMPD);
    emit2(saveAdd);
    backPatch2(curr_addr, jumpOut);
}

void varDec()
{
    curr_symbol.kind = "VAR";
    DecToken();
    varDecRest();
}

void varDecRest()
{
    match(TOKEN[idToken]);

    curr_symbol.name = curr_token.lexem;
    symbol_table.add_symbol(curr_symbol);

    if (lookahead.name == TOKEN[assignmentToken])
        assignment();
    else
    {
        multiDec();
        match(TOKEN[endOfLineToken]);
    }
}

void assignment()
{
    if (DEBUG)
        cout << curr_token.lexem << endl;
    assign_symbol = symbol_table.get_symbol(symbol_table.find_symbol(curr_token.lexem));
    
    print(assign_symbol);
    match(TOKEN[assignmentToken]);

    assignBody();

    match(TOKEN[endOfLineToken]);
}

void assignBody()
{
    string type;

    type = expr();

    if (type == TOKEN[intLitToken])
    {
        if (DEBUG)
            cout << assign_symbol.type << endl;
        if (assign_symbol.type != "INT")
            error("Can't assign identifier " + assign_symbol.name + " of type " + assign_symbol.type + " to an " + TOKEN[intLitToken]);

        if (assign_symbol.scope == 0)
        {
            emit1(STGR4);
            emit2(assign_symbol.address);
        }
        else //assign to not a global var
        {
        }
    }
    else if (type == TOKEN[idToken])
    {
    }

    //multi assignment???? wtf
    if (lookahead.lexem == TOKEN[assignmentToken])
        assignment();
}

void DecToken()
{
    if (curr_token.name == TOKEN[intDecToken])
        curr_symbol.type = "INT";
    else if (curr_token.name == TOKEN[realDecToken])
        curr_symbol.type = "REAL";
    else if (curr_token.name == TOKEN[stringDecToken])
        curr_symbol.type = "STR";
    else if (curr_token.name == TOKEN[charDecToken])
        curr_symbol.type = "CHAR";
    else if (curr_token.name == TOKEN[boolDecToken])
        curr_symbol.type = "YN";
    else if (curr_token.name == TOKEN[hexDecToken])
        curr_symbol.type = "HEX";
    else if (curr_symbol.kind == "FUN");
    else
        error("Invalid Declaration!!!");
}

void funDecType()
{
    DecToken();
    if (curr_token.name == TOKEN[noneToken])
        curr_symbol.type = "NONE";
    else
        error("Funtion type mismatch!!!");
}

void codeBlock()
{
    match(TOKEN[leftBlockToken]);
    statementList();
    match(TOKEN[rightBlockToken]);
}

void statementList()
{
    statement();
    if (lookahead.name == TOKEN[defToken] || lookahead.name == TOKEN[intDecToken] 
        || lookahead.name == TOKEN[realDecToken] || lookahead.name == TOKEN[charDecToken] 
        || lookahead.name == TOKEN[stringDecToken] || lookahead.name == TOKEN[hexDecToken] 
        || lookahead.name == TOKEN[boolDecToken] || lookahead.name == TOKEN[rptToken] 
        || lookahead.name == TOKEN[loopToken] || lookahead.name == TOKEN[thisToken] 
        || lookahead.name == TOKEN[cacToken] || lookahead.name == TOKEN[tilToken] 
        || lookahead.name == TOKEN[getToken] || lookahead.name == TOKEN[idToken] 
        || lookahead.name == TOKEN[endOfLineToken] || lookahead.name == TOKEN[rtToken]
        || lookahead.name == TOKEN[sendToken] || lookahead.name == TOKEN[decToken] 
        || lookahead.name == TOKEN[incToken] || lookahead.name == TOKEN[dereferenceToken]
        || lookahead.name == TOKEN[addressToken]
        )
    {
        statementList();
    }
}

void statement()
{
   
    /*else if (lookahead.name == TOKEN[defToken])
    {
        funcdec
    }
    
    if (lookahead.name == TOKEN[intDecToken]
        || lookahead.name == TOKEN[realDecToken]
        || lookahead.name == TOKEN[charDecToken]
        || lookahead.name == TOKEN[stringDecToken]
        || lookahead.name == TOKEN[hexDecToken]
        || lookahead.name == TOKEN[boolDecToken])
    {
        declaration();
    }
    */
    if (lookahead.name == TOKEN[rptToken])
    {
        doWhileLoop();
    }
    if (lookahead.name == TOKEN[loopToken])
    {
        forLoop();
    }
    else if (lookahead.name == TOKEN[thisToken])
    {
        ifStatement();
    }
    //else if (lookahead.name == TOKEN[cacToken])
    //{
    //    Switch();
    //}
    else if (lookahead.name == TOKEN[tilToken])
    {
        whileLoop();
    }
    //else if (lookahead.name == TOKEN[getToken])
    //{
    //    get();
    //}
    else if (lookahead.name == TOKEN[endOfLineToken])
    {
        match(TOKEN[endOfLineToken]);
    }//do nothing
    //else if (lookahead.name == TOKEN[rtToken])
    //{
    //}
    else if (lookahead.name == TOKEN[sendToken])
        send();
    else
    {
        expr();
        if (lookahead.name == TOKEN[assignmentToken])
        {
            assignment();
        }
        else
        {
            match(TOKEN[endOfLineToken]);
        }
        //when should we drop the expression stack???
    }
}

void send()
{
    match(TOKEN[sendToken]);
    match(TOKEN[leftParenthesisToken]);
    sendBody();
    match(TOKEN[rightParenthesisToken]);
    match(TOKEN[endOfLineToken]);

    emit1(LDM1);
    emit1(10);
    emit1(OUTC);
}

void sendBody()
{
    sendToken2();
    if (lookahead.name == TOKEN[semicolonToken])
    {
        match(TOKEN[semicolonToken]);
        sendBody();
    }
}
void sendToken2()
{
    int save_yes, save_out;
    string t;
    Symbol send_symbol;
    if (lookahead.name == TOKEN[idToken])
    {
        //match(TOKEN[idToken]);
        if (lookahead.name == TOKEN[leftParenthesisToken])
        {
            // do array call

        }
        else
        {
            t = expr();
            if (t == TOKEN[intLitToken])
                emit1(OUTI);
            else if (t == TOKEN[realLitToken])
                emit1(OUTR);
            else if (t == TOKEN[boolLitToken])
            {
                emit1(TZE); //if bool = false
                emit1(BYES);//if bool = false
                emit1(3);
                emit1(JMPD);
                save_yes = curr_addr;
                emit2(0);

                emit1(LDM1);
                emit1('N');
                emit1(OUTC);

                emit1(LDM1);
                emit1('O');
                emit1(OUTC);

                emit1(JMPD);
                save_out = curr_addr;
                emit2(0);
                
                backPatch2(curr_addr, save_yes);
                emit1(LDM1);
                emit1('Y');
                emit1(OUTC);

                emit1(LDM1);
                emit1('E');
                emit1(OUTC);

                emit1(LDM1);
                emit1('S');
                emit1(OUTC);

                backPatch2(curr_addr, save_out);
            }
            //pos = symbol_table.find_symbol(curr_token.lexem);
            //if (pos == -1)
            //    error("Identifier not found. It cannot be sent!!!");
            //send_symbol = symbol_table.get_symbol(pos);

            //if (send_symbol.scope == 0)
            //{
            //    if (send_symbol.kind == "VAR") //it's a global var
            //    {
            //        switch (send_symbol.type[0])
            //        {
            //        case 'C':
            //        case 'Y':
            //            emit1(LDGR1);
            //            emit2(send_symbol.address);
            //            emit1(OUTC);
            //            break;
            //        case 'I':                        
            //        
            //        case 'H':
            //            emit1(LDGR4);
            //            emit2(send_symbol.address);
            //            emit1(OUTI);
            //            break;
            //        case 'R':
            //            emit1(LDGR4);
            //            emit2(send_symbol.address);
            //            emit1(OUTR);
            //            break;
            //        case 'S':
            //            //send only the 1st character of the string
            //            emit1(LDGR1);
            //            emit2(send_symbol.address);
            //            emit1(OUTC);
            //            break;
            //        case 'P':
            //            //send only the 1st character of the string
            //            emit1(LDGR2);
            //            emit2(send_symbol.address);
            //            emit1(OUTI);
            //            break;
            //        default:
            //            error("Type is undefined!!!");
            //            break;
            //        }
            //    }
            //}
            else
            {
                //send local
            }
        }

    }
    else if (lookahead.name == TOKEN[callToken])
    {
        call();
    }
    else
        callLitToken();
}

void callLitToken()
{
    string t;
    int save_yes, save_no, save_out;
    char ch = '\n';

    if (lookahead.name == TOKEN[stringLitToken])
    {
        match(TOKEN[stringLitToken]);

        for (int i = 0; i < curr_token.lexem.length(); i++)
        {
            emit1(LDM1);
            emit1(curr_token.lexem[i]);
            emit1(OUTC);
        }
    }
    else if (lookahead.name == TOKEN[charLitToken])
    {
        match(TOKEN[charLitToken]);
        emit1(LDM1);
        curr_token.lexem[0] = ch;
        emit1((int)ch);
        emit1(OUTC);
    }

    else if (lookahead.name == TOKEN[intLitToken])
    {
        

        if (lookahead.name == TOKEN[semicolonToken] || lookahead.name == TOKEN[rightParenthesisToken])
        {
            match(TOKEN[intLitToken]);

            emit1(LDM4);
            emit4(stoi(curr_token.lexem));
            emit1(OUTI);
        }
        else
        {
            t = expr();

            if (t == TOKEN[intLitToken])
                emit1(OUTI);
            else if (t == TOKEN[realLitToken])
                emit1(OUTR);
            else if (t == TOKEN[boolLitToken])
            {
                emit1(TZE); //if bool = false
                emit1(BYES);//if bool = false
                emit1(3);
                emit1(JMPD);
                save_yes = curr_addr;
                emit2(0);

                emit1(LDM1);
                emit1('N');
                emit1(OUTC);

                emit1(LDM1);
                emit1('O');
                emit1(OUTC);

                emit1(JMPD);
                save_out = curr_addr;
                emit2(0);
                
                backPatch2(curr_addr, save_yes);
                emit1(LDM1);
                emit1('Y');
                emit1(OUTC);

                emit1(LDM1);
                emit1('E');
                emit1(OUTC);

                emit1(LDM1);
                emit1('S');
                emit1(OUTC);

                backPatch2(curr_addr, save_out);
            }
        }
    }
    else if (lookahead.name == TOKEN[boolLitToken])
    {
        match(TOKEN[boolLitToken]);
        emit1(LDM1);
        if (curr_token.lexem == "YES")
        {
            emit1(1);
        }
        else
            emit1(0);
        emit1(OUTI);
    }
    else if (lookahead.name == TOKEN[hexLitToken])
    {
        match(TOKEN[hexLitToken]);
        emit1(LDM4);
        emit4(stoi(curr_token.lexem));
        emit1(OUTI);
    }
    else if (lookahead.name == TOKEN[realLitToken])
    {
        match(TOKEN[realLitToken]);
        emit1(LDM4);
        emit4(unreal(stof(curr_token.lexem)));
        emit1(OUTR);
    }
}
void litToken()
{
}

void arrayCall()
{
    match(TOKEN[idToken]);
    match(TOKEN[leftParenthesisToken]);
    factor();
    match(TOKEN[rightParenthesisToken]);
    match(TOKEN[endOfLineToken]);

}
//void passedParam()
//{
//    if (lookahead.name == TOKEN[rightSquareBracketToken])
//    {
//        match(TOKEN[rightSquareBracketToken]);
//    }
//    else
//    {
//        litOrId();
//        if (lookahead.name == TOKEN[semicolonToken])
//        {
//            match(TOKEN[semicolonToken]);
//            passedParam();
//        }
//    }
//}
void litOrId()
{
    if (lookahead.name == TOKEN[idToken])
    {
        match(TOKEN[idToken]);
    }
    else if (lookahead.name == TOKEN[stringLitToken] || lookahead.name == TOKEN[charLitToken] || lookahead.name == TOKEN[intLitToken] || lookahead.name == TOKEN[boolLitToken] || lookahead.name == TOKEN[hexLitToken] || lookahead.name == TOKEN[realLitToken])
    {
        litToken();
    }
    else
    {
        error("ERROR: Parameter must be an id or literal!!!");
    }
}


void switchBody()
{
    if (lookahead.name == TOKEN[normToken])
    {
        norm();
    }
    else
    {
        optPart();
        norm();
    }

}

void optPart()
{
    if (lookahead.name == TOKEN[optToken])
    {
        opt();
        optPart();
    }
}

void opt()
{
    int jumpout;
    match(TOKEN[optToken]);
    factor();
    emit1(SUB);
    emit1(TZE);
    emit1(BYES);
    emit1(3);
    emit1(JMPD);
    jumpout = curr_addr;
    emit2(0);
    codeBlock();
    backPatch2(curr_addr, jumpout);
}

void norm()
{
    if (lookahead.name == TOKEN[normToken])
    {
        match(TOKEN[normToken]);
        codeBlock();
    }
}
void factor()
{
    if (lookahead.name == TOKEN[intLitToken])
    {
        match(TOKEN[intLitToken]);
        emit1(STLR4);
    }
    else if (lookahead.name == TOKEN[idToken])
    {
        match(TOKEN[idToken]);
        emit1(STLR2);
    }
    else
    {
        error("ERROR: must have id or integer!!!");
    }
}

bool isRelOp()
{
    bool op = false;
    if (lookahead.name == TOKEN[equalToToken] || lookahead.name == TOKEN[notEqualToToken] || lookahead.name == TOKEN[greaterThanToken] || lookahead.name == TOKEN[lessThanToken] || lookahead.name == TOKEN[greaterThanEqualToken] || lookahead.name == TOKEN[lessThanEqualToken])
        op = true;
    return op;
}
bool isAddOp()
{
    bool isAdd = true;

    if (lookahead.name == TOKEN[addToken] || lookahead.name == TOKEN[subToken])
    {
        return isAdd;
    }
    else
        return !isAdd;
}
bool isMulOp()
{
    bool isMul = true;


    if (lookahead.name == TOKEN[multToken] || lookahead.name == TOKEN[divToken] || lookahead.name == TOKEN[modToken])
    {
        return isMul;
    }
    else
        return !isMul;
}
bool isUnaryOp()
{
    bool isUnary = true;
    if (lookahead.name == TOKEN[incToken] || lookahead.name == TOKEN[decToken] || lookahead.name == TOKEN[negToken] || lookahead.name == TOKEN[dereferenceToken] || lookahead.name == TOKEN[addressToken])
    {
        return isUnary;
    }
    else
        return !isUnary;
}

string unary()
{
    string op = curr_token.lexem;
    string save = lookahead.name;
    string t;
    int pos;

 /*   curr_token = lookahead;
    lookahead = next_token();*/

    t = expr();

    if (op == "NT")
        emit1(NEG);
    else if (op == "^")
        emit1(DER);
    else if (op == "#")
    {
        emit1(LDM2);
        pos = symbol_table.find_symbol(curr_token.lexem);
        curr_symbol = symbol_table.get_symbol(pos);
        if (curr_symbol.scope == 0)
        {
            emit2(curr_symbol.address + M[G]);
        }
        else
        {
            // do local things 
        }
    }
    else if (op == "INC")
        emit1(INC);
    else if (op == "DEC")
        emit1(DEC);

    if (save == TOKEN[idToken])
    {
        if (curr_symbol.scope == 0)
        {
            //do global thing
            if (curr_symbol.kind == "VAR")
            {
                switch (curr_symbol.type[0])
                {
                case 'C':
                    emit1(STGR1);
                    emit2(curr_symbol.address);
                    break;
                case 'I':
                case 'R':
                case 'H':
                    emit1(STGR4);
                    emit2(curr_symbol.address);;
                    break;
                case 'N':
                    break;
                case 'S':
                    emit1(STGR1);
                    emit2(curr_symbol.address);
                    break;
                case 'Y':
                    emit1(STGR2);
                    emit2(curr_symbol.address);
                    break;
                default:
                    error("Type is undefined!!!");
                    break;
                }
            }
            else
                error("Can't operate unary operation on identifier of kind " + curr_symbol.kind+"!!!");
        }
        else
        {
            //do local thing
        }
    }
    return t;
}

void forLoop()
{
    string t;
    int start_loop, end_loop;
    match(TOKEN[loopToken]);

    t = expr();

    if (t != TOKEN[intLitToken])
        error("Type mismatch in forLoop statement!!!");

    start_loop = curr_addr;
    emit1(COPY);
    emit1(TNZ);
    emit1(BYES);
    emit1(3);
    emit1(JMPD);
    end_loop = curr_addr;
    emit2(0);

    //process through the whole block the get the next PC
    emit1(DEC);
    codeBlock();
    emit1(JMPD);
    emit2(start_loop);

    backPatch2(curr_addr, end_loop);
}