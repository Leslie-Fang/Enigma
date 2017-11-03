#include <iostream>
#include <fstream>
#include "errors.h"
#include <string>
#include <vector>
using namespace std;

char word[26];

class enigmamachine
{
public:
    vector<int> inpoutPlugboard;
    vector<int> inpoutreflectors;
    int numberofRotors;
    vector<int> inpoutRotorPosition;
    vector<vector<int> > inpoutRotor;
    vector<int> norchRotor;
    vector<vector<int> > mappingRotor;
    vector<vector<int> > inverseMappingRotor;
    vector<vector<int> > b2rRotor;
};


int main(int argc, char ** argv){
    cout<<"The number of input params is: "<<argc<<endl;
    cerr<<"error"<<endl;
    char inputCharacter;
    enigmamachine enigmaMachine;
    string localInput;
    bool even=false;
    int oddNum =0;
    int evenNum=0;
    vector<int> cRotor;
    vector<int>::iterator it;
    // init the word table 0-A 1-B.... 25-Z
    for(int i=0;i<26;i++){
        word[i] = char (i+65);
    }
    // get the input configuration file
    if(argc <= 2){
        cerr<<"INSUFFICIENT_NUMBER_OF_PARAMETERS"<<endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }else if(argc == 3){
        enigmaMachine.numberofRotors = 0;
    }else if(argc % 2 == 0){
        cerr<<"INSUFFICIENT_NUMBER_OF_PARAMETERS"<<endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }else{
        enigmaMachine.numberofRotors = argc-4;
    }
    enigmaMachine.inpoutRotor.resize(enigmaMachine.numberofRotors);
    // read all the file and init the enigmaMachine OBJ
    for(int i=1;i<argc;i++){
        cout<<argv[i]<<endl;
        if(i == 1){
            for(int j=0;j<26;j++){
                enigmaMachine.inpoutPlugboard.push_back(j);
            }
            // read the configuration file
            ifstream infile(argv[i]);
            if(!infile){
                // if file not exsits, return error
                cerr<<"ERROR_OPENING_CONFIGURATION_FILE"<<endl;
                return ERROR_OPENING_CONFIGURATION_FILE;
            }
            even=false;
            vector<int> findRepeatedConnec;
            while( infile >> localInput )
            {
                for(int k=0;k<localInput.length();k++){
                    if(int(localInput[k]) < 48 || int(localInput[k]) > 57){
                        cerr<<"NON_NUMERIC_CHARACTER"<<endl;
                        return NON_NUMERIC_CHARACTER;
                    }
                }
                if(!even){
                    oddNum = stoi(localInput,nullptr);
                    it = find (findRepeatedConnec.begin(), findRepeatedConnec.end(), oddNum);
                    if(it != findRepeatedConnec.end()){
                        cerr<<"IMPOSSIBLE_PLUGBOARD_CONFIGURATION"<<endl;
                        return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
                    }
                    if(oddNum < 0 || oddNum> 25){
                        cerr<<"INVALID_INDEX"<<endl;
                        return INVALID_INDEX;
                    }
                    findRepeatedConnec.push_back(oddNum);
                }else{
                    evenNum = stoi(localInput,nullptr);
                    it = find (findRepeatedConnec.begin(), findRepeatedConnec.end(), evenNum);
                    if(it != findRepeatedConnec.end()){
                        cerr<<"IMPOSSIBLE_PLUGBOARD_CONFIGURATION"<<endl;
                        return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
                    }
                    if(evenNum < 0 || evenNum> 25){
                        cerr<<"INVALID_INDEX"<<endl;
                        return INVALID_INDEX;
                    }
                    findRepeatedConnec.push_back(evenNum);
                    enigmaMachine.inpoutPlugboard[oddNum] = evenNum;
                    enigmaMachine.inpoutPlugboard[evenNum] = oddNum;
                }
                even = !even;
            }
            if(even){
                // odd number of input parameter
                cerr<<"INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS"<<endl;
                return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
            }
            infile.close();
//            for(int i=0;i<enigmaMachine.inpoutPlugboard.size();i++){
//                cout<<enigmaMachine.inpoutPlugboard[i]<<endl;
//            }
        }
        else if(i == 2){
            ifstream infile(argv[i]);
            if(!infile){
                cerr<<"ERROR_OPENING_CONFIGURATION_FILE"<<endl;
                return ERROR_OPENING_CONFIGURATION_FILE;
            }
            for(int i=0;i<26;i++){
                enigmaMachine.inpoutreflectors.push_back(i);
            }
            even=false;
            vector<int> findRepeatedReflector;
            while( infile >> localInput )
            {
                for(int k=0;k<localInput.length();k++){
                    if(int(localInput[k]) < 48 || int(localInput[k]) > 57){
                        cerr<<"NON_NUMERIC_CHARACTER"<<endl;
                        return NON_NUMERIC_CHARACTER;
                    }
                }
                if(!even){
                    oddNum = stoi(localInput,nullptr);
                    it = find (findRepeatedReflector.begin(), findRepeatedReflector.end(), oddNum);
                    if(it != findRepeatedReflector.end()){
                        cerr<<"INVALID_REFLECTOR_MAPPING"<<endl;
                        return INVALID_REFLECTOR_MAPPING;
                    }
                    if(oddNum < 0 || oddNum> 25){
                        cerr<<"INVALID_INDEX"<<endl;
                        return INVALID_INDEX;
                    }
                    findRepeatedReflector.push_back(oddNum);
                }else{
                    evenNum = stoi(localInput,nullptr);
                    it = find (findRepeatedReflector.begin(), findRepeatedReflector.end(), evenNum);
                    if(it != findRepeatedReflector.end()){
                        cerr<<"INVALID_REFLECTOR_MAPPING"<<endl;
                        return INVALID_REFLECTOR_MAPPING;
                    }
                    if(evenNum < 0 || evenNum> 25){
                        cerr<<"INVALID_INDEX"<<endl;
                        return INVALID_INDEX;
                    }
                    findRepeatedReflector.push_back(evenNum);
                    enigmaMachine.inpoutreflectors[oddNum] = evenNum;
                    enigmaMachine.inpoutreflectors[evenNum] = oddNum;
                }
                even = !even;
            }
            if(findRepeatedReflector.size() != 26){
                cerr<<"INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS"<<endl;
                return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
            }
            infile.close();
        }else if(i == argc-1){
            ifstream infile(argv[i]);
            if(!infile){
                cerr<<"ERROR_OPENING_CONFIGURATION_FILE"<<endl;
                return ERROR_OPENING_CONFIGURATION_FILE;
            }
            while( infile >> localInput )
            {
                for(int k=0;k<localInput.length();k++){
                    if(int(localInput[k]) < 48 || int(localInput[k]) > 57){
                        cerr<<"NON_NUMERIC_CHARACTER"<<endl;
                        return NON_NUMERIC_CHARACTER;
                    }
                }
                if(stoi(localInput,nullptr) < 0 || stoi(localInput,nullptr)> 25){
                    cerr<<"INVALID_INDEX"<<endl;
                    return INVALID_INDEX;
                }
                enigmaMachine.inpoutRotorPosition.push_back(stoi(localInput,nullptr));
            }
            if(enigmaMachine.inpoutRotorPosition.size()<enigmaMachine.numberofRotors){
                cerr<<"NO_ROTOR_STARTING_POSITION"<<endl;
                return NO_ROTOR_STARTING_POSITION;
            }
            infile.close();
        }else{
            cout<<i<<": rotor"<<endl;
            ifstream infile(argv[i]);
            if(!infile){
                cerr<<"ERROR_OPENING_CONFIGURATION_FILE"<<endl;
                return ERROR_OPENING_CONFIGURATION_FILE;
            }
            while( infile >> localInput )
            {
                for(int k=0;k<localInput.length();k++){
                    if(int(localInput[k]) < 48 || int(localInput[k]) > 57){
                        cerr<<"NON_NUMERIC_CHARACTER"<<endl;
                        return NON_NUMERIC_CHARACTER;
                    }
                }
                if(stoi(localInput,nullptr) < 0 || stoi(localInput,nullptr)> 25){
                    cerr<<"INVALID_INDEX"<<endl;
                    return INVALID_INDEX;
                }
                if(cRotor.size() < 26){
                    it = find (cRotor.begin(), cRotor.end(), stoi(localInput,nullptr));
                    if(it != cRotor.end()){
                        cerr<<"INVALID_ROTOR_MAPPING"<<endl;
                        return INVALID_ROTOR_MAPPING;
                    }
                }
                cRotor.push_back(stoi(localInput,nullptr));
            }
            enigmaMachine.inpoutRotor[i-3].resize(cRotor.size());
            enigmaMachine.inpoutRotor[0].push_back(1);
            for(int j=0;j<cRotor.size();j++){
                enigmaMachine.inpoutRotor[i-3][j] = cRotor[j];
            }
            cRotor.clear();
            infile.close();
        }
    }
    // format the rotor input
    enigmaMachine.mappingRotor.resize(enigmaMachine.numberofRotors);
    for(int i=0;i<enigmaMachine.numberofRotors;i++){
        enigmaMachine.mappingRotor[i].resize(26);
        for(int j=0;j<26;j++){
            enigmaMachine.mappingRotor[i][j] = enigmaMachine.inpoutRotor[i][j];
        }
        enigmaMachine.norchRotor.push_back(enigmaMachine.inpoutRotor[i][26]);
    }
    // init the position
    enigmaMachine.b2rRotor.resize(enigmaMachine.numberofRotors);
    for(int i=0;i<enigmaMachine.numberofRotors;i++){
        enigmaMachine.b2rRotor[i].resize(26);
        for(int j=0;j<26;j++){
            enigmaMachine.b2rRotor[i][j] = j;
        }
        for(int k=0;k<enigmaMachine.inpoutRotorPosition[i];k++){
            enigmaMachine.b2rRotor[i].push_back(enigmaMachine.b2rRotor[i][0]);
            enigmaMachine.b2rRotor[i].erase(enigmaMachine.b2rRotor[i].begin());
        }
    }
    // init the inverseMappingRotor
    enigmaMachine.inverseMappingRotor.resize(enigmaMachine.numberofRotors);
    for(int i=0;i<enigmaMachine.numberofRotors;i++){
        enigmaMachine.inverseMappingRotor[i].resize(26);
        for(int j=0;j<26;j++){
            enigmaMachine.inverseMappingRotor[i][enigmaMachine.mappingRotor[i][j]] = j;
        }
    }
//    // check the all the input
//    cout<<"check the inpoutPlugboard."<<endl;
//    for(int i=0;i<enigmaMachine.inpoutPlugboard.size();i++){
//        cout<<enigmaMachine.inpoutPlugboard[i]<<endl;
//    }
//    cout<<"check the ref"<<endl;
//    for(int i=0;i<26;i++){
//        cout<<enigmaMachine.inpoutreflectors[i]<<endl;
//    }
//    cout<<"check the mappingRotor."<<endl;
//    for(int i=0;i<enigmaMachine.numberofRotors;i++){
//        cout<<"rotor: "<<i<<endl;
//        for(int j=0;j<26;j++){
//            cout<<enigmaMachine.mappingRotor[i][j]<<endl;
//        }
//    }
//    cout<<"check the norch of the rotor."<<endl;
//    for(int i=0;i<enigmaMachine.numberofRotors;i++){
//        cout<<enigmaMachine.norchRotor[i]<<endl;
//    }
//    cout<<"check the position of the rotor."<<endl;
//    for(int i=0;i<enigmaMachine.numberofRotors;i++){
//        cout<<enigmaMachine.inpoutRotorPosition[i]<<endl;
//    }
    cout<<"check the rotor absolute 2 releative position"<<endl;
    for(int i=0;i<enigmaMachine.numberofRotors;i++){
        cout<<"rotor: "<<i<<endl;
        for(int j=0;j<26;j++){
            cout<<enigmaMachine.b2rRotor[i][j]<<endl;
        }
    }

    while(1){
        // press enter; then press ctrl+d ;to terminal the input
        cin>>inputCharacter;
        int inputNum = -1;
        int afterPlugBoard = -1;
        vector<int> relativeNumRotor;
        vector<int> afterRotor;
        int afterReflector = -1;
        vector<int> inverserelativeNumRotor;
        vector<int> inverseafterRotor;
        int afterPlugBoardlast = -1;
        int cNum = -1;
        for(int i=0;i<enigmaMachine.numberofRotors;i++){
            relativeNumRotor.push_back(-1);
            afterRotor.push_back(-1);
            inverserelativeNumRotor.push_back(-1);
            inverseafterRotor.push_back(-1);
        }
        if(inputCharacter){
            // the charater are not in A-Z
            if(int(inputCharacter) < 65 || int(inputCharacter) > 90){
                cerr<<"INVALID_INPUT_CHARACTER"<<endl;
                return INVALID_INPUT_CHARACTER;
            }
            //update the mappingRotor before circuit
            for(int i=enigmaMachine.numberofRotors-1;i>=0;i--){
                cout<<enigmaMachine.norchRotor[i]<<endl;
                if(enigmaMachine.b2rRotor[i][0] == enigmaMachine.norchRotor[i]){
                    enigmaMachine.b2rRotor[i].push_back(enigmaMachine.b2rRotor[i][0]);
                    enigmaMachine.b2rRotor[i].erase(enigmaMachine.b2rRotor[i].begin());
                    continue;
                }else{
                    enigmaMachine.b2rRotor[i].push_back(enigmaMachine.b2rRotor[i][0]);
                    enigmaMachine.b2rRotor[i].erase(enigmaMachine.b2rRotor[i].begin());
                    break;
                }
            }
//            for(int i=enigmaMachine.numberofRotors-1;i>=0;i--){
//                if(enigmaMachine.b2rRotor[i][25] == enigmaMachine.norchRotor[i]){
//                    cNum = enigmaMachine.b2rRotor[i][25];
//                    enigmaMachine.b2rRotor[i].insert(enigmaMachine.b2rRotor[i].begin(),cNum);
//                    enigmaMachine.b2rRotor[i].pop_back();
//                    continue;
//                }else{
//                    cNum = enigmaMachine.b2rRotor[i][25];
//                    enigmaMachine.b2rRotor[i].insert(enigmaMachine.b2rRotor[i].begin(),cNum);
//                    enigmaMachine.b2rRotor[i].pop_back();
//                    break;
//                }
//            }

            cout<<"check the rotor absolute 2 releative position"<<endl;
            for(int i=0;i<enigmaMachine.numberofRotors;i++){
                cout<<"rotor: "<<i<<endl;
                for(int j=0;j<26;j++){
                    cout<<enigmaMachine.b2rRotor[i][j]<<endl;
                }
            }
            cout<<"inputCharacter is: "<<inputCharacter<<" : "<<int(inputCharacter)<<endl;
            // Map character to number: A is 0, Z is 25
            inputNum = int(inputCharacter) - 65;
            cout<<"inputCharacter is: "<<inputNum<<endl;
            afterPlugBoard=enigmaMachine.inpoutPlugboard[inputNum];
            cout<<"afterPlugBoard is: "<<afterPlugBoard<<endl;
            for(int i=enigmaMachine.numberofRotors-1;i>=0;i--){
                if(i==enigmaMachine.numberofRotors-1){
                    relativeNumRotor[i] = enigmaMachine.b2rRotor[i][afterPlugBoard];
                    afterRotor[i] = enigmaMachine.mappingRotor[i][relativeNumRotor[i]];
                }else{
                    relativeNumRotor[i] = enigmaMachine.b2rRotor[i][afterRotor[i+1]];
                    afterRotor[i] = enigmaMachine.mappingRotor[i][relativeNumRotor[i]];
                }
                cout<<"relativeNumRotor"<<i<<" is: "<<relativeNumRotor[i]<<endl;
                cout<<"afterRotor"<<i<<" is: "<<afterRotor[i]<<endl;
            }
            afterReflector = enigmaMachine.inpoutreflectors[afterRotor[0]];
            cout<<"afterReflector is: "<<afterReflector<<endl;
            for(int i=0;i<enigmaMachine.numberofRotors;i++){
                if(i == 0){
                    inverseafterRotor[i] = enigmaMachine.inverseMappingRotor[i][afterReflector];
                    for(int k=0;k<26;k++){
                        if(enigmaMachine.b2rRotor[i][k] == inverseafterRotor[i]){
                            inverserelativeNumRotor[i] = k;
                        }
                    }
                }else{
                    inverseafterRotor[i] = enigmaMachine.inverseMappingRotor[i][inverserelativeNumRotor[i-1]];
                    for(int k=0;k<26;k++){
                        if(enigmaMachine.b2rRotor[i][k] == inverseafterRotor[i]){
                            inverserelativeNumRotor[i] = k;
                        }
                    }
                }
            }
            afterPlugBoardlast = enigmaMachine.inpoutPlugboard[inverserelativeNumRotor[enigmaMachine.numberofRotors-1]];
            cout<<"afterPlugBoardlast is: "<<afterPlugBoardlast<<endl;
            cout<<"The output character is: "<<char(afterPlugBoardlast+65)<<endl;
        }else{
            break;
        }
    }
    return NO_ERROR;
}

