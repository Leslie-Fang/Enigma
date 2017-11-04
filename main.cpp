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
    vector<int> inpoutPlugboard; // mapping the input to the output after plug board
    vector<int> inpoutreflectors;// mapping the in and out of reflectors
    int numberofRotors; // store the number of rotors
    vector<int> inpoutRotorPosition; // store the start position of each rotor
    vector<vector<int> > inpoutRotor;
    vector<int> norchRotor; // store the norch point of each rotor
    vector<vector<int> > mappingRotor; // mapping the rotor input and output
    vector<vector<int> > inverseMappingRotor;// inverse mapping the rotor's input and output
    vector<vector<int> > b2rRotor;// mapping the rotor's relative and absolute frame
};

int main(int argc, char ** argv){
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
        if(i == 1){
            // process the plugboards/.pb file and get the inpoutPlugboard mapping
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
        }
        else if(i == 2){
            // process the reflectors/.rf file and get the reflectors mapping
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
            // process the rotors/.pos file and get the rotors' init positions
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
            // process the rotors/.rot file and get the rotors' mapping
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
    // init the rotors' position
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
    // init the inverseMappingRotor, when inverse pass the circuit
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
//    cout<<"check the rotor absolute 2 releative position"<<endl;
//    for(int i=0;i<enigmaMachine.numberofRotors;i++){
//        cout<<"rotor: "<<i<<endl;
//        for(int j=0;j<26;j++){
//            cout<<enigmaMachine.b2rRotor[i][j]<<endl;
//        }
//    }

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
            // if the charater are not in A-Z, return INVALID_INPUT_CHARACTER error
            if(int(inputCharacter) < 65 || int(inputCharacter) > 90){
                cerr<<"INVALID_INPUT_CHARACTER"<<endl;
                return INVALID_INPUT_CHARACTER;
            }
            //update the mappingRotor's position before circuit, when enter a character, the rotor would rotor one step
            for(int i=enigmaMachine.numberofRotors-1;i>=0;i--){
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
            cout<<"inputCharacter is: "<<inputCharacter<<" : "<<int(inputCharacter)<<endl;
            // Map character to number: A is 0, Z is 25
            inputNum = int(inputCharacter) - 65;
            // pass the enter character to the plug board and get the output of the plugboard
            afterPlugBoard=enigmaMachine.inpoutPlugboard[inputNum];
            // pass the character to the rotors
            for(int i=enigmaMachine.numberofRotors-1;i>=0;i--){
                if(i==enigmaMachine.numberofRotors-1){
                    relativeNumRotor[i] = enigmaMachine.b2rRotor[i][afterPlugBoard];
                    afterRotor[i] = enigmaMachine.mappingRotor[i][relativeNumRotor[i]];
                }else{
                    relativeNumRotor[i] = enigmaMachine.b2rRotor[i][afterRotor[i+1]];
                    afterRotor[i] = enigmaMachine.mappingRotor[i][relativeNumRotor[i]];
                }
            }
            // pass the character to the reflectors
            afterReflector = enigmaMachine.inpoutreflectors[afterRotor[0]];
            // pass the character inverse to the rotors
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
            // pass the character inverse to the plug board and get the finally output
            afterPlugBoardlast = enigmaMachine.inpoutPlugboard[inverserelativeNumRotor[enigmaMachine.numberofRotors-1]];
            cout<<"The output character is: "<<char(afterPlugBoardlast+65)<<endl;
        }else{
            break;
        }
    }
    return NO_ERROR;
}

