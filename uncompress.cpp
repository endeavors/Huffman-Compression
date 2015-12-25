/*****************************************************************************
 * Name: Gurkirat Singh
 * Description: This file uncomresses the compressed file, which is the input
 *              file. It calls on the overloaded build function to read the
 *              header and then calls decode on the encoding part of the 
 *              compressed file until we have read the total number of symbols.
 ****************************************************************************/

#include "HCTree.hpp"
#include <iomanip>

#define PRECISION_COUNT 5
#define ARGUMENTS_NUM 3

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != ARGUMENTS_NUM){
        cerr << "You must enter the input and output files to uncompress."<< endl;
        exit(-1);
    }

    int symbolsRead = 0;
    HCTree tree;
    ifstream inputFile;
    ofstream outputFile;

    //open all files in binary mode
    inputFile.open(argv[1], ios:: binary);
    outputFile.open(argv[2], ios::binary);

    BitInputStream inputStream(inputFile);
    BitOutputStream outputStream(outputFile);

    if (!inputFile){
        cerr << "Error opening input file. Program will now exit." << endl;
        exit(-1);
    }

    if (!outputFile){
        cerr<< "Error opening output file. Program will now exit."<< endl;
        exit(-1);
    }
    
    //Get size of the file before reading the header
    inputFile.seekg(0, inputFile.end);
    int fileSize = inputFile.tellg();
    inputFile.seekg(0,inputFile.beg);

    if (fileSize < 0) fileSize = 0;

    //get the totall number of symbols we need to read
    int totalSymbols = inputStream.readInt();
    
    cout << "Reading header from file \"" << argv[1] <<"\"... done."<<endl; 
    
    int ttlSymbols = 0;
    if (totalSymbols > 0) ttlSymbols = totalSymbols;

    cout << "Uncompressed file will have size of " << ttlSymbols << " bytes." << endl;
    
    cout << "Building Huffman code tree... ";
    //This will build the Huffman tree without frequencies
    tree.build(inputStream);
    cout << "done." << endl;
    cout << "Writing to file \"" << argv[2] << "\"... ";

    HCNode *mainRoot = tree.getRoot();
    if (mainRoot != nullptr){
        //decode each encoding of the symbol and then right that symbol to the 
        //output file
        while (symbolsRead <= totalSymbols){
            int symbol = tree.decode(inputStream);
            if (symbol != -1){
                outputStream.writeByte(symbol);
                symbolsRead++;
            }
        } 
    }
    cout << "done." <<endl;
    float uncompRatio = 0;

    //get the uncompression ratio
    if (fileSize != 0) uncompRatio= (float)outputFile.tellp()/fileSize;
    
    cout << "Uncompression ratio: " << fixed << setprecision(PRECISION_COUNT)<< uncompRatio << endl;

    inputFile.close();
    outputFile.close();
     
   
    return 0;
}
