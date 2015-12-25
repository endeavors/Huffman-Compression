/*********************************************************************
 * Name: Gurkirat Singh
 * Description: This is the main driver of the program that compresses
 *              the input file and writes the header to the compressed
 *              file. It firsts goes through the entire input file
 *              to count the frequencies of each symbol and then it
 *              creates the tree for it. We write the header so that
 *              we only store the leaf nodes in it.
 ********************************************************************/

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitOutputStream.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

#define ARGUMENTS_NUM 3
#define TOTAL_SYMBOLS 256

/* Function: outputHeader()
 * Description: Do a post order traversal on the tree and write the
 *              leaf nodes only. We indicate in the heaer that it is 
 *              is a leaf node by placing a bit 1 in front of it, all
 *              other internal nodes have bit 0 in front of them.
 * Return value: None.
 */
void outputHeader(BitOutputStream& out, HCNode* node)
{
    if (node != nullptr){
        //go left and then right
        outputHeader(out, node->c0);
        outputHeader(out, node->c1);

        //we have reached the leaf node
        if (node->c0 == nullptr && node->c1 == nullptr){
            //write bit 1 followed by the symbol of the leaf node
            out.writeBit(1);
            out.writeByte(node->symbol);
            //cout << "1" << node->symbol;
        }else{
            //it's a internal node
            out.writeBit(0);
            //cout << "0";
        }
    }
    
    return;
}

/* Main function that is the driver for compress*/  
int main(int argc, char* argv[]) {
    
    if(argc != ARGUMENTS_NUM){
	    cerr << argv[0] << " called with incorrect arguments." << endl;
        cerr << "Usage: " << argv[0] << " infile outfile" << endl;
        exit(-1);
    }

    ofstream encodeFile;
    ifstream inputFile;

    //initialize the vector size of 256, the total number of symbols in the
    //ASCII table that we care about
    vector<int> freqVector(TOTAL_SYMBOLS);
    HCTree encodeTree;
    int uniqueCount = 0;
    int totalSymbols = 0;

    //zero fill the entire vector
    fill(freqVector.begin(), freqVector.end(),0);

    //open all files in binary mode 
    encodeFile.open(argv[2],ios::binary);
    inputFile.open(argv[1],ios::binary);
    BitOutputStream outputStream(encodeFile);
    
    if(!inputFile){
      cerr << "Couldn't open input file." << endl;
      exit(-1);
    }
  
    if(!encodeFile){
      cerr << "Couldn't open output file." << endl;
      exit(-1);
    }

    //get the file pointer to the end of the file to get the size of the file
    inputFile.seekg(0, inputFile.end);
    int fileSize = inputFile.tellg();
    //reset the file pointer to the beginning
    inputFile.seekg(0, inputFile.beg);

    cout << "Reading from file \""<< argv[1] << "\"... ";
    
    //start reading from the file
    if(inputFile.good()== true){
        while (true){
            //get one byte at a time
            int value = inputFile.get();
            if (!inputFile.good())
                break;
            int curr = freqVector[value];

            //only increment the frequency of the symbol if it has 
            //already been read
            if(curr == 0)
	            ++uniqueCount;
            curr++;

            //keep track of the total symbols that we have read so far
            totalSymbols++;
            freqVector[value]= curr;
        }
    }
    
    cout << "done." << endl;
    
    cout << "Found " << uniqueCount << " unique symbols in input file of size "\
        << fileSize << " bytes." << endl;
 
    if(!inputFile.eof()){
        cerr << "Error reading from input file!" << endl;
        exit(-1);
    }

    cout << "Building Huffman code tree... ";

    //build the huffman tree using the frequencies we collected
    encodeTree.build(freqVector);

    //clear EOF since we are at the end of the file currently
    inputFile.clear (); 
    inputFile.seekg(0, inputFile.beg);
    
    cout << "done." << endl;
    cout << "Writing to file \"" << argv[2] << "\"... ";
    cout << endl;  
    //we need the root to do post order traversal
    HCNode * root = encodeTree.getRoot();

    cout << "Tree Height: " << encodeTree.getTreeHeight(root) << endl;


    //write the header to the compressed file
    if (root != nullptr){
        //we also need to store the total number of symbols in the file
        outputStream.writeInt(totalSymbols);
        cout << "totalsymbols: " <<  totalSymbols << endl;
        outputHeader(outputStream, root);
        
        //to indicate that the header is done
        outputStream.writeBit(0);
        //cout << "0" << endl;
    }

    //read the entire input file again to encode each symbol
    while(true){
        char c = inputFile.get();
        if (!inputFile.good())
            break;
        encodeTree.encode(c,outputStream);
    }

    //Write out anything left in the buffer using flush()
    if (root != nullptr)
        outputStream.flush();
       
    int outputFileSize = encodeFile.tellp();
    float compressionRatio = (float)outputFileSize/fileSize;

    cout << "Output file has size " << outputFileSize << " bytes." << endl;
    cout << "Compression ratio: " << fixed << setprecision(5) <<compressionRatio << endl;
					
    inputFile.close();
    encodeFile.close();
};

