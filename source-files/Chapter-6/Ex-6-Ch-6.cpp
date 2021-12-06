
/*
    6.  Write a program that checks if a sentence is correct according to the "English"
        grammar in 6.4.1. Assume that every sentecne is terminated by a full stop (.) surrounded by whitespace.
        For example, "birds fly but the fish swim ." is a sentence, but "birds fly but the fish swim" (terminating
        dot missing) and birds fly but the fish swim. (no space before dot) are not. For each sentence entered,
        the program should simply respond "OK" of "not OK". Hint: Don't bother with tokens; just read into a string
        using >>.
 
        -------------------
            G R A M M A R
        -------------------
        SENTENCE:
            Noun Verb
            Sentence Conjunction Sentence
        CONJUNCTION:
            "and"
            "or"
            "but"
        NOUN:
            "birds"
            "fish"
            "C++"
        VERBS:
            "rules"
            "fly"
            "swim"
 
 
 if 1st place = noun
    2nd place = verb    THEN
 
    (1) space and .     OR
    (2) 3rd place = conjunction THEN
    4th place = noun
    5th place = verb    THEN
    (1) space and .     OR
    (2) 3rd place = conjunction THEN
 
 */

#include "std_lib_facilities.h"
#include <map>



struct PredefGrammar {
    vector<string> parsedInput;
    
    int conjPos;
    int nounPos;
    int verbPos;
    
    int conjCount;
    int nounCount;
    int verbCount;
    
    bool isValidSentence;
    
    enum strValue {
        UNDEFINED =-1,
        AND = 0,
        OR,
        BUT,
        BIRDS,
        FISH,
        CPP,
        RULES,
        FLY,
        SWIM,
    };
    
    std::map<string, strValue> optionValue;
    
    PredefGrammar()
        :parsedInput()
        ,conjPos(0) // always defaults to third pos for the first sentence
        ,nounPos(0) // always defaults to zeroeth pos for the first sentence
        ,verbPos(0) // always defaults to first pos fot the first sentence
        ,conjCount(0)
        ,nounCount(0)
        ,verbCount(0)
    {}
};

void checkValidSentence(PredefGrammar& rf);

void initVocabulary(PredefGrammar& rf) {
    rf.optionValue["and"]   = rf.AND;
    rf.optionValue["or"]    = rf.OR;
    rf.optionValue["but"]   = rf.BUT;
    rf.optionValue["birds"] = rf.BIRDS;
    rf.optionValue["fish"]  = rf.FISH;
    rf.optionValue["c++"]   = rf.CPP;
    rf.optionValue["rules"] = rf.RULES;
    rf.optionValue["fly"]   = rf.FLY;
    rf.optionValue["swim"]  = rf.SWIM;
}


int checkValidWord(string usrIn, PredefGrammar refData){
    //PredefGrammar refData;
    int mapValue;
    
    if(refData.optionValue.count(usrIn) > 0){
        // key is found
        mapValue = refData.optionValue.find(usrIn)->second;
    } else {
        // key not found
        mapValue = PredefGrammar::UNDEFINED;
    }
    return mapValue;
}

void parseInput(PredefGrammar& rf) {     // checks whether it's a noun, verb or conjunction.
    
    for (int i = 0; i < rf.parsedInput.size(); ++i) {
        
        switch (checkValidWord(rf.parsedInput[i], rf)) {
            case PredefGrammar::AND:
            case PredefGrammar::OR:
            case PredefGrammar::BUT:
                cout << "I'M A CONJUNCTION" << endl;
                rf.conjPos = i;
                rf.conjCount++;
                cout << "conjPos: " << rf.conjPos << endl;
                cout << "conjCount: " << rf.conjCount << endl << endl;
                break;
        
            case PredefGrammar::BIRDS:
            case PredefGrammar::FISH:
            case PredefGrammar::CPP:
                cout << "I'M A NOUN" << endl;
                rf.nounPos = i;
                rf.nounCount++;
                cout << "nounPos: " << rf.nounPos << endl;
                cout << "nounCount: " << rf.nounCount << endl << endl;
                break;
        
            case PredefGrammar::RULES:
            case PredefGrammar::FLY:
            case PredefGrammar::SWIM:
                cout << "I'M A VERB" << endl;
                rf.verbPos = i;
                rf.verbCount++;
                cout << "verbPos: " << rf.verbPos << endl;
                cout << "verbCount: " << rf.verbCount << endl << endl;
                break;
            default:
                cout << "Word not Found in Vocab" << endl;
                break;
        }
    }
    checkValidSentence(rf);
}

void checkValidSentence(PredefGrammar& rf) {
    if(rf.verbPos - rf.nounPos == 1) {
        cout << "diff: " << rf.verbPos - rf.nounPos << endl;
        cout << "VALID" << endl;
    } else {
        cout << "diff: " << rf.verbPos - rf.nounPos << endl;
        cout << "INVALID" << endl;
    }
}

int main(){
    
    
    string userInput;
    PredefGrammar reference;
    initVocabulary(reference);

    cout << "Enter a sentence; end it with a whitespace and a dot (.)" << endl;
    while (cin) {
        if(userInput == "done") break;
        cin >> userInput;
        reference.parsedInput.push_back(userInput);
    }
    cout << endl;
    parseInput(reference);
    cout << endl;
    cout << reference.parsedInput[0] << endl;
    cout << reference.parsedInput[1] << endl;
    cout << reference.parsedInput[2] << endl;
    cout << reference.parsedInput[3] << endl;
    cout.flush();
}

