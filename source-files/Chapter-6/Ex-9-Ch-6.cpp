
/*
    9. Write a program that reads digits and composes them into integers. For
    example, 123 is read as teh characters 1, 2, and 3. The program shourd output
    123 is 1 hundered and 2 tens and 3 ones. The number should be output as and int value.
    Handle the numbers with one, two, three, or four digits. Hint: To get the integer value 5
    from the character '5' substract '0', thata is, '5'-'0' == 5
*/

#include "std_lib_facilities.h"
//#include <vector>

int main() {
    char in;
    int outputVal{0};
    vector<char> vInput;
    
    cout << "Enter a 1, 2, 3 or 4 digit number. Enter 'done' to evaluate" << endl;
    
    while (cin) {
        cin >> in;
        //cin >> usrInput;
        if(in == 'q') break;
                
        vInput.push_back(in);
    }
    
    for (int i = 0; i < vInput.size(); ++i) {
        int vlength = vInput.size() - 1;
      //  if (vInput.size() == 4) {
            // v[0] * 1000 + v[1] * 100 + v[2] * 10 + v[3] * 1
            // outputVal = outputVal + ( (vInput[i]-'0') * ( 10 ^ (vlength - i) ) );
            outputVal = outputVal + ( (vInput[i]-'0') * ( pow(10.0, (vlength - i) ) ) ) ;
            
        //} else if (vInput.size() == 3) {
            // v[0] * 100 + v[1] * 10 + v[2] * 1
            
        //}
    }
    
    //cout << "Your value in int is: " << outputVal << endl;
    
    for (int i = 0; i < vInput.size(); ++i) {
        if (vInput.size() == 4) {
            int oVal;
            switch (i) {
                case 0:
                    oVal = vInput[i] - '0';
                    cout << outputVal << " is " << oVal << " thousand ";
                    break;
                case 1:
                    oVal = vInput[i] - '0';
                    cout << " and " << oVal << " hundred ";
                    break;
                case 2:
                    oVal = vInput[i] - '0';
                    cout << " and " << oVal << " tens ";
                    break;
                case 3:
                    oVal = vInput[i] - '0';
                    cout << " and " << oVal << " ones " << endl;
                    break;
            }
            
        } else if (vInput.size() == 3){
            int oVal;
            switch (i) {
                case 0:
                    oVal = vInput[i] - '0';
                    cout << outputVal << " is " << oVal << " hundred ";
                    break;
                case 1:
                    oVal = vInput[i] - '0';
                    cout << " and " << oVal << " tens ";
                    break;
                case 2:
                    oVal = vInput[i] - '0';
                    cout << " and " << oVal << " ones " << endl;
                    break;
            }
            
        }
        //cout << "\n" << "index: " << i << "--->" << vInput[i] << endl;
        
        
    }
    
}
