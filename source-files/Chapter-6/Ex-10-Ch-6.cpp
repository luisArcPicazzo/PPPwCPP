
 
/*
    10. A permutation is an ordered subset of a set. For example, say you wanted to
        pick a combination to a vault. There are 60 possible numbers, and you need
        three different numbers for the combination. Thear are P(60, 3) permutations
        for the combination, where P is defined by the formula:
        
                            P(a, b) = a! / (a-b)!
 
        where ! is used as a suffix factorial operator. For example, 4! is 4*3*2*1.
 
        Combinations are similar to permutations, except that the order of the objects
        doesn't matter. For example, if you were making a "banana split" sundae and
        wished to use three different flavors of ice cream out of five that you had,
        you wouldn't care if you used a scoop of vanilla at the beginning or the end;
        you would still have used vanilla. The formula for combinations is:
 
                            C(a, b) = P(a, b) / b!
 
        
        Design a program that asks users for two numbers, asks them whether they want
        to calculate permutations or combinations, and prints out the result. This will
        have several parts. Do an analysis of the above requirements. Write exactly what
        the program will have to do. Then, go into the design phase. Write pseudo code
        for the program, and break it into sub-components. This program should have error
        checking. Make sure that all erroneous inputs will generate good error messages.
 */


#include "std_lib_facilities.h"

unsigned int getFactorial(int x);
unsigned int getPermutation(int a, int b);
unsigned int getCombination(int a, int b);


int main() {
    int a;
    int b;
    char choice;
    string choiceDisp;
    
    cout << "Enter TWO positive integer numbers" << endl;
    cin >> a >> b;
    
    cout << "Enter p for permutation or enter c for combination" << endl;
    cin >> choice;
    
    cout << endl;
    cout << "Your 1st number: "<< a << endl;
    cout << "Your 2nd number: "<< b << endl;
    cout << endl;
    
    switch (choice) {
        case 'p':
            try {
                cout << "# of permutations is: " << getPermutation(a, b) << endl;
            }
            catch (const char* e) {
                cout << "E r r o r: " << e << endl;
            }
            break;
    
        case 'c':
            try {
                cout << "# of combinations is: " << getCombination(a, b) << endl;
            }
            catch (const char* e){
                cout << "E r r o r: " << e << endl;
            }
            break;
    }
}

unsigned int getFactorial(int n) {
    if (n < 0 ) throw "Factorial is not defined for negative integers\n";
    if (n != 0) {
        return n * getFactorial(n-1);
    } else {
        return 1;
    }
}

unsigned int getPermutation(int a, int b) {
    unsigned int oPermVal;
    if((a-b) == 0) throw "Division by zero not defined \n";
    int numerator = getFactorial(a);
    int denominator = getFactorial(a-b);
    oPermVal = numerator/denominator;

    return oPermVal;
}

unsigned int getCombination(int a, int b) {
    unsigned int oCombVal;
    if((b) == 0) throw "Division by zero not defined \n";
    int numerator = getPermutation(a,b);
    int denominator = getFactorial(b);
    oCombVal = numerator/denominator;

    return oCombVal;
}
