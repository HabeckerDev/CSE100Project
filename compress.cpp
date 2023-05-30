#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

vector<int> countByteOccurrences(FancyInputStream & in){
    //initializing the vector with 256 ints with value 0 (prevents Seg fault)
    vector<int> byteCounts(256, 0);

    // ifstream file(filename, ios::binary);
    // if (!file) {
    //     cerr << "Error opening file: " << filename << endl;
    //     return byteCounts;
    // }

    // // Read bytes from the file and count occurrences
    // char byte;
    // while (file.get(byte)) {
    //     byteCounts[(static_cast<unsigned char>(byte))]++; // Increment count for the byte value
    //     //byteCounts[(static_cast<int>(byte))]++;
    // }


    // file.close();

    while(in.good()){
        byteCounts[(static_cast<unsigned char>(in.read_byte()))]++;
    }
    
    return byteCounts;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: compress <input_file1> <input_file2>" << endl;
        return 1;
    }

    FancyInputStream inputFile1(argv[1]);
    cout<<"Original file size: " << inputFile1.filesize()<<endl;
    //string outputFile = argv[2];

    //Using direct initialization
    FancyOutputStream outputFile(argv[2]);

    const vector<int> byteCounts = countByteOccurrences(inputFile1);

    HCTree compressedTree;
    compressedTree.build(byteCounts);

    compressedTree.printEdges();

    // //write header to output file
    // int leafCount = compressedTree.get_countLeaves();
    // //outputFile.write_int(2);
    // cout<< "leaf count " << leafCount << endl; //someting wrong with leafCount
    // outputFile.write_int(leafCount);
    // for(int i = 0; i < 256; i++){ //write for char c : byteCounts
    //     if(byteCounts[i] > 0){
    //         outputFile.write_byte(i);
    //         outputFile.write_int(byteCounts[i]);
    //     }
    // }

    for(unsigned int i = 0; i < 256; i++){
        outputFile.write_byte(i);
        outputFile.write_int(byteCounts[(int)i]);
    }

    FancyInputStream inputFile2(argv[1]);
    while(inputFile1.good()){
        compressedTree.encode(inputFile2.read_byte(), outputFile);
    }

    //for each byte run encode (read input file again)
    // ifstream file(inputFile, ios::binary);
    // char byte;
    // int bcount = 0;
    // while (file.get(byte)){
    //     bcount++;
    //     compressedTree.encode(byte, outputFile);
    // }
    // cout << "num of bytes read from file: "<< bcount <<endl;
    // file.close();

    //compressedTree.encode(outputFile);

    // Perform your desired operations here

    // Example: Open and read the first input file
    // ifstream file1(inputFile1);
    // if (!file1.is_open()) {
    //     std::cout << "Error opening " << inputFile1 << std::endl;
    //     return 1;
    // }



    // Close the input files
    //file1.close();
    //file2.close();

    return 0;
}

