

/*
    1. Starting from the file calculator08buggy.cpp, get the calculator to compile. ------  D O N E
    
    2. Go through the entire progaram and add appropriate comments. ----------------------- D O N E
    
    3. As you commented, you found errors (deviously inserted especially
       for you to find). Fix them; they are not in the text of the book. ------------------ D O N E
    
    4. Testing: prepare a set of inputs and use them to test the calculator.
       Is your list pretty complete? What should you look for? Include negative values,
       0 very small, very large, and "silly" inputs. -------------------------------------- D O N E
    
    5. Do the testing and fix any bugs that you missed when you commented. ---------------- D O N E
 
    6. Add a predefined name k meaning 1000. ---------------------------------------------- D O N E
 
    7. Give the usera a square root function sqrt(), for example,
       sqrt(2+6.7). Naturally, the value of sqrt(x) is the square root
       of x; for example, sqrt(9) is 3. Use the standard library sqrt()
       function that is available through the header std_lib_facilities.h. ---------------- Fixed it with help from a solution (still don't fully understand the grammar part)
       Remember to update the comments, including the grammar. ---------------------------- D O N E
    
    8. Catch attempts to take the square root of a negative number and
       print an appropriate error message. ------------------------------------------------ D O N E
    
    9. Allow the user to use pow(x, i) to mean "Multiply x with itself
       i times"; for example, pow(2.5, 3) is 2.5*2.5*2.5. Require i
       to be an integer using the technique we used for %. -------------------------------- D O N E
 
    10. Change the "declaration keyword" from let to #. ----------------------------------- T R I V I A L
 
    11. Change the "quit keyword" from quit to exit. That will involve defining
        a string for quit just as we did for let in Ss 7.8.2. ----------------------------- T R I V I A L
 */

/*
    calculator08buggy.cpp

    Helpful comments removed.

    We have inserted 3 bugs that the compiler will catch and 3 that it won't.
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
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) {   // reads a character into ch as long as ch is a letter or a digit..
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
    if (is_declared(name)) error(name, " declared twice");
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", name);
    double d = expression();
    names.push_back(Variable(name, d));
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
