

/*
    3. Provide named constants that you really can't change the value of. Hint:
       You have to add a member to Variable that distinguishes between constants
       and variable and check for it in set_value(). If you want to let the user
       define constants (rather than just having pi and e defined as constants),
       you'll have to add a notation to let the user express that, for example,
       const pi = 3.14;
 */


#include "std_lib_facilities.h"

struct Token {
    char kind;
    double value;
    string name;
    Token(char ch) :kind(ch), value(0) { }
    Token(char ch, double val) :kind(ch), value(val) { }
    Token(char ch, string str): kind(ch), name(str) { } // bug (1) -- Added additional constructor
};

class Token_stream {
    bool full;
    Token buffer;
public:
    Token_stream() :full(0), buffer(0) { }

    Token get();
    void unget(Token t) { buffer = t; full = true; }

    void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqRoot = 'S';
const char power = 'P';
const char cLet = 'M';

Token Token_stream::get()
{
    if (full) { full = false; return buffer; }
    char ch;
    cin >> ch;
    switch (ch) {
    case '(': case ')': case '+': case '-': case '*':
    case '/': case '%': case ';': case '=': case ',':
        return Token(ch);

    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.unget();
        double val;
        cin >> val;
        return Token(number, val);
    }
    default:
        {
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) {   // reads a character into ch as long as ch is a letter or a digit..
                    s += ch; // bug 4... Missed a plus sign in s+= ch. so it can appendi it to the string s. (only if it's a letter or a digit)
                             // The get() member function works just like >> except that it doesn't by default skip whitespace.
                }
                cin.unget();
                if (s == "let") return Token(let);
                if (s == "const") return Token(cLet);
                if (s == "quit") return Token(quit);
                if (s == "sqrt") return Token(sqRoot);
                if (s == "pow") return Token(power);
                return Token(name, s);  // Bug (1) fixed... Needed additional constructor.
            }
            error("Bad token");
        }
    }
}

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    char ch;
    while (cin >> ch)
        if (ch == c) return;
}

struct Variable {
    string name;
    double value;
    bool isConstVal;
    Variable(string n, double v, bool cV) : name(n), value(v), isConstVal(cV) { }
};

vector<Variable> names;

double get_value(string s)
{
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return names[i].value;
    error("get: undefined name ", s);
}

void set_value(string s, double d)
{
    for (int i = 0; i < names.size(); ++i)                    // potential bug <= ???
        if (names[i].name == s) {
            if (names[i].isConstVal == true) error("can't redefine constant ", s);
    
            names[i].value = d;
            return;
        }
    error("set: undefined name ", s);
}

bool is_declared(string s)
{
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return true;
    return false;
}

double defineConstVarName(string s, double val, bool isConstVal = true) {
    if (is_declared(s)) error(s, " declared twice");
    names.push_back(Variable(s, val, isConstVal));
    return val;
}

Token_stream ts;
double expression();

double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':
    {
        double d = expression();
        t = ts.get();                                       // look for closing parens
        if (t.kind != ')') error("')' expected");
        return d;                                           // missing this return (subtle bug 2)
    }
    case '-':
        return -primary();
    case '+':                                               // maybe missing...
        return primary();
    case number:
        return t.value;
    case name:
    {
        Token nxtTkn = ts.get();
        if (nxtTkn.kind == '=') {
            double d = expression();
            set_value(t.name, d);
            return d;
        } else {
            ts.unget(nxtTkn);         // not an assignmen; return the value.
            return get_value(t.name); // return the variable's value.
        }
    }
    case sqRoot:
    {
        Token pkNxtTkn = ts.get();
        if (pkNxtTkn.kind != '(') error("Syntax Error: missing '('");
        double param = expression();
        Token pkNxtTkn2 = ts.get();
        if (pkNxtTkn2.kind != ')' ) error("Syntax Error: missing ')'");
        if (param < 0.0) error ("Syntax Error: no negative parameters allowed...");
        return sqrt(param);
    }
    case power:
    {
        Token pkNxtTkn = ts.get();
        if (pkNxtTkn.kind != '(') error("Syntax Error: missing '('");
        double param = expression();
        
        Token pkNxtTkn2 = ts.get();
        if (pkNxtTkn2.kind != ',' ) error("Syntax Error: missing ','");
        
        double pwrExpr = expression();
        int pwrParam = (int)pwrExpr;  // don't understand very well.. but aids to check for int values
        
        if (pwrParam != pwrExpr) error("Syntax Error: power parameters must be integers"); // compares / checks for int values....
        
        Token pkNxtTkn3 = ts.get();
        if (pkNxtTkn3.kind != ')' ) error("Syntax Error: missing ')'");
        
        return pow(param, pwrParam);
    }
    default:
        error("primary expected");
        break;
    }
}

double term()
{
    double left = primary();
    while (true) {
        Token t = ts.get();
        switch (t.kind) {
        case '*':
            left *= primary();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            break;
        }
        case '%':
        {
            double d = primary();
            if (d == 0) error("%: divide by zero");
            left = fmod(left, d); // fmod as in floating modulo. in order to accept floating point values. <cmath> library
            break;
        }
                
        default:
            ts.unget(t);
            return left;
        }
    }
}

double expression()
{
    double left = term();
    while (true) {
        Token t = ts.get();
        switch (t.kind) {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.unget(t);
            return left;
        }
    }
}

double declaration(Token crTkn)
{
    Token t = ts.get();
    if (t.kind != 'a') error("name expected in declaration");     // not a name ???
    string name = t.name;
    
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", name);
    double d = expression();
    
    defineConstVarName(name, d, crTkn.kind == cLet);
    return d;
}

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case cLet:
    case let:
        return declaration(t.kind);
    default:
        ts.unget(t);
        return expression();
    }
}

void clean_up_mess()
{
    ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
    while (true) {                                                  // MISSING BRACES (of while loop) POSSIBLE BUG (3)
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();                   // ignore prints???
            if (t.kind == quit) return;
            ts.unget(t);
            cout << result << statement() << endl;
        }
        catch (runtime_error& e) {
            cerr << e.what() << endl;
            clean_up_mess();
        }
    }
}

int main()

try {
    defineConstVarName("K", 1000, false);
    defineConstVarName ("pi", 3.1415926535, true);
    defineConstVarName ("e", 2.7182818284, true);
    
    calculate();
    return 0;
}
catch (exception& e) {
    cerr << "exception: " << e.what() << endl;
    char c;
    while (cin >> c && c != ';');
    return 1;
}
catch (...) {
    cerr << "exception\n";
    char c;
    while (cin >> c && c != ';');
    return 2;
}
