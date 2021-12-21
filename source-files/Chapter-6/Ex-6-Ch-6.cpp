
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
    
    int initCjPos;
    int initNnPos;
    int initVbPos;
    
    int cjPos;
    int nnPos;
    int vbPos;
    
    int pstCjPos;
    int pstNnPos;
    int pstVbPos;
    
    int cjCount;
    int nnCount;
    int vbCount;
    
    bool isValidSentence;
    int invalidCount;
    
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
    
    enum partOfSpeechValue {
        UNDEF,
        NOUN,
        VERB,
        CONJUNCTION
    };
    
    std::map<string, strValue> optionValue;
    std::map<strValue, partOfSpeechValue> partsSpchMp;
    
    
    

    
    PredefGrammar()
        :parsedInput()
        ,initCjPos(2)
        ,initNnPos(0)
        ,initVbPos(1)
        ,cjPos(0) // always defaults to third pos for the first sentence
        ,nnPos(0) // always defaults to zeroeth pos for the first sentence
        ,vbPos(0) // always defaults to first pos fot the first sentence
        ,pstCjPos(0)
        ,pstNnPos(0)
        ,pstVbPos(0)
        ,cjCount(0)
        ,nnCount(0)
        ,vbCount(0)
        ,isValidSentence(false)
        ,invalidCount(0)
        ,partsSpchMp{ {AND, CONJUNCTION}
                     ,{OR, CONJUNCTION}
                     ,{BUT, CONJUNCTION}
                     ,{BIRDS, NOUN}
                     ,{FISH, NOUN}
                     ,{CPP, NOUN}
                     ,{RULES, VERB}
                     ,{FLY, VERB}
                     ,{SWIM, VERB}
                     ,{UNDEFINED, UNDEF} }
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


void checkPositions(PredefGrammar& rf, int ptSpchCase, int i) {

        switch (ptSpchCase) {
            case PredefGrammar::CONJUNCTION:
                rf.cjPos = i;
                if (i < rf.initCjPos) {
                    cout << "Await..." << endl;
                }
                
                if (i >= rf.initCjPos &&  rf.cjPos == rf.pstCjPos + 3) {
                    cout << "CONJUNCTION" << endl;
                    rf.pstCjPos = rf.cjPos;
                    rf.cjPos = i;
                } else if (i == rf.initCjPos) {
                    cout << "CONJUNCTION" << endl;
                    rf.pstCjPos = rf.initCjPos;
                    rf.cjPos = i;
                } else {
                    cout << "invalid sentence" << endl;
                    rf.pstCjPos = rf.cjPos;
                }
                break;
                
            case PredefGrammar::NOUN:
                rf.nnPos = i;
//                if (checkValidWord(rf.parsedInput[i-1], rf) == PredefGrammar::CONJUNCTION)
                if (i < rf.initNnPos) {
                    cout << "Await..." << endl;
                }
                if (i >= rf.initNnPos && rf.nnPos == rf.pstNnPos + 3) {
                    cout << "NOUN" << endl;
                    rf.pstNnPos = rf.nnPos;
                    //rf.nnPos = i;
                } else if (i == rf.initNnPos) {
                    cout << "NOUN" << endl;
                    rf.pstNnPos = rf.initNnPos;
                    rf.nnPos = i;

                } else {
                    cout << "Invalid sentence" << endl;
                    rf.pstNnPos = rf.nnPos;

                }
                break;
                
                
            case PredefGrammar::VERB:
                rf.vbPos = i;
                if (i < rf.initVbPos) {
                    cout << "Await..." << endl;
                }
                if (i >= rf.initVbPos && rf.vbPos == rf.pstVbPos + 3) {
                    cout << "VERB" << endl;
                    rf.pstVbPos = rf.vbPos;
                
                } else if (i == rf.initVbPos) {
                    cout << "VERB" << endl;
                    rf.pstVbPos = rf.initVbPos;
                    rf.vbPos = i;
                } else {
                    cout << "Invalid sentence" << endl;
                    //rf.pstVbPos = rf.vbPos;
                    rf.pstNnPos = rf.vbPos;
                    rf.pstCjPos = rf.vbPos;
                    

                }
                break;
                
                
        }

    }


void parseInput(PredefGrammar& rf) {     // checks whether it's a noun, verb or conjunction.
    
    for (int i = 0; i < rf.parsedInput.size(); ++i) {
        
        int mapValue = 0;
        switch (checkValidWord(rf.parsedInput[i], rf)) {
            case PredefGrammar::AND:
            case PredefGrammar::OR:
            case PredefGrammar::BUT:
                //cout << "CONJUNCTION" << endl;
                
                mapValue = PredefGrammar::CONJUNCTION;
                checkPositions(rf, mapValue, i);
                break;
        
            case PredefGrammar::BIRDS:
            case PredefGrammar::FISH:
            case PredefGrammar::CPP:
                //cout << "NOUN" << endl;
                
                mapValue = PredefGrammar::NOUN;
                checkPositions(rf, mapValue, i);
                break;
        
            case PredefGrammar::RULES:
            case PredefGrammar::FLY:
            case PredefGrammar::SWIM:
                //cout << "VERB" << endl;
                
                mapValue = PredefGrammar::VERB;
                checkPositions(rf, mapValue, i);
                break;
                
            default:
                cout << "Word not Found in Vocab" << endl;
                break;
        }
        //checkPositions(rf, i, mapValue);
    }
    // call check positions??
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

}


