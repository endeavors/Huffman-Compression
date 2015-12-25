#include <iostream>
#include <fstream>
#include "BitOutputStream.hpp"
#include "BitInputStream.hpp"

using namespace std;
int main(int argc, char*argv[]){
    ofstream stm;
    ifstream ifs;
    stm.open(argv[1], ios::binary);
    BitOutputStream output(stm);
    output.writeInt(976);
    output.writeByte('b');
    for (int i = 0; i < 6; i++)
        if (i %2 == 0)
            output.writeBit(1);
        else
            output.writeBit(0);
    output.writeByte('c');
    output.writeBit(1);
    output.writeByte('d');
    output.writeByte('e');
    for (int i = 0; i < 4; i++)
        output.writeBit(1);
    output.writeByte('f');
    output.flush();
   
    stm.close();

    ifs.open(argv[1], ios::binary);
    BitInputStream input(ifs);
    int readInt = input.readInt();
    cout << endl << "Int is " << readInt << endl;
    char byte = input.readByte();
    cout << "Byte is " << byte << endl;
    
    for (int i = 0; i < 6; i++){
        int bit = input.readBit();
        cout  << bit;    
    }
    cout << endl;
    byte = input.readByte();
    cout << "Byte is " << byte << endl;
    int bit = input.readBit();
    cout << bit << endl;
    byte = input.readByte();
    cout << "Byte is " << byte << endl;
    byte = input.readByte();
    cout << "Byte is " << byte << endl;
    for (int i = 0; i < 4; i++){
        bit = input.readBit();
        cout << bit;
    }
    cout << endl;
    byte = input.readByte();
    cout << "Byte is " << byte << endl;
    ifs.close();
    return 0;
}
