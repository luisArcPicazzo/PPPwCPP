

/*
    2. Provide an assignment operator, =, so that you can change the value of a
       variable after you introduce it using "let". Discuss why that can be useful
       and how it can be a source of problems.
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

Token Token_stream::get()
{
    if (full) { full = false; return buffer; }
    char ch;
    cin >> ch;
    switch (ch) {
    case '(': case ')': case '+': case '-': case '*':
    case '/': case '%': case ';': case '=': case ',':
        return Token(ch);
    //case '_':
    //    return Token(let);
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
    Variable(string n, double v) :name(n), value(v) { }
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
        return get_value(t.name);
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

double declaration()
{
    Token t = ts.get();
    if (t.kind != 'a') error("name expected in declaration");     // not a name ???
    string name = t.name;
    
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", name);
    double d = expression();
    
    if (is_declared(name)) set_value(name, d);
    else names.push_back(Variable(name, d));
    
    return d;
}

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case let:
        return declaration();
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
    names.push_back(Variable("K", 1000));
    
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
