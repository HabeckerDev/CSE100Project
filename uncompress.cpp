#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: compress <input_file1> <input_file2>" << endl;
        return 1;
    }

    string input = argv[1];
    string output = argv[2];
    FancyInputStream inputFile(argv[1]);
    FancyOutputStream outputFile(argv[2]);
    vector<int> byteCounts(256, 0);
    //open file and read header
    //int leafCount = inputFile.read_int();
    int freq;
    char byteValue;
    for(int i = 0; i < 256; i++){
        byteValue = inputFile.read_byte();
        freq = inputFile.read_int();
        byteCounts[(static_cast<unsigned char>(byteValue))] = freq;
    }

    HCTree uncompTree;
    uncompTree.build(byteCounts);

    while(inputFile.good()){
        outputFile.write_byte(uncompTree.decode(inputFile));
    }
    
    // ifstream file(inputFile, ios::binary);
    // char byte;
    // while (file.get(byte)){
    //     //compressedTree.encode(byte, outputFile);
    // }
    // file.close();

    return 0;
}

