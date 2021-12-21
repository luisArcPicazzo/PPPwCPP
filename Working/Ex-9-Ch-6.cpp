
/*
    9. Write a program that reads digits and composes them into integers. For
    example, 123 is read as teh characters 1, 2, and 3. The program shourd output
    123 is 1 hundered and 2 tens and 3 ones. The number should be output as and int value.
    Handle the numbers with one, two, three, or four digits. Hint: To get the integer value 5
    from the character '5' substract '0', thata is, '5'-'0' == 5
*/

#include "std_lib_facilities.h"

int main(){
    string in;
    vector<string> usrInput;
    
    while (cin) {
        cin >> in;
        usrInput.push_back(in);
        
        if(in == "q") break;
    }
    
    for (auto i : usrInput) {
        cout << i << endl;
    }
}
