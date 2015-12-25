/**************************************************************
 * Name: Gurkirat Singh
 * Description: This class reads bits and bytes by using a bit
 *              buffer. Once the buffer is full, another byte
 *              is fetched. ReadByte also reads the byte in 
 *              bits so that the header algorithm is read in
 *              form of bits, not bytes.
 *************************************************************/

#include "BitInputStream.hpp"

//Constants
#define BUFFERSIZE 8
#define INT_ITERATOR 24

/* Function: readBit()
 * Description: If get the entire byte and shift the bit to the left
 *              most position of the byte in which we are holding it.
 *              We fetch again once we have read the entire byte.
 *              Bit manipulation is performed to carry the bit to the left.
 * Return Value: The bit we read.
 */
int BitInputStream::readBit()
{
    //buffer is empty so we need to fill it first
    if (bufi == BUFFERSIZE){
        if (in.eof()) return -1;
        //fetch the byte
        buf = in.get();
        //now buffer is full
        bufi = 0;
    }

    bufi++;
    //set the bit on the next left available slot
    //in the buffer
    int bitVal = (buf  >> (BUFFERSIZE - bufi)) & 1; 
    return bitVal;   
}

/*
 * Function: readByte()
 * Description: Read the byte one bit at a time and then only return
 *              when you have read 8 bits, since a byte is 8 bits.
 * Return value: Return the byte we just read.
 */
int BitInputStream::readByte()
{
    if (in.eof()){
        return -1;
    }
    //get the next byte
    int resultVal = 0;
    
    //read byte as bits and if the buffer gets filled
    //before entire byte is read, it will fetch another
    //byte until completion.
    for (int i= BUFFERSIZE-1; i >= 0; i--){
        int bitRead = readBit();
        bitRead <<=  i;
        resultVal |=  bitRead;
    }
    return resultVal;
}

/* Function: readInt()
 * Description: We iterate four times to get the entire int since
 *              int is 32 bits. We then mask the result and append
 *              to the last byte we read until we are done.
 * Value: return the int we read
 */
int BitInputStream::readInt()
{
    //we are going to append one byte at a time to fill up 32 bits
    int resultVal = 0;
    for (int i = INT_ITERATOR; i >= 0; i = i-BUFFERSIZE){
        if (in.eof() | !in.good()) return -1;
        
        //read the integer is form of 4 bytes one at a time
        int intRead = in.get();
        intRead <<=  i;
        //mask the result
        resultVal |=  intRead;
        
    } 
    return resultVal;
    
}

