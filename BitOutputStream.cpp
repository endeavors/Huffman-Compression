/****************************************************************
 * Name: Gurkirat Singh
 * Description: This class writes bytes one bit at a time but
 *              ints in series of bytes since our header scheme
 *              only consists of only 1 integer in the beginning
 *              of the file. We flush the bit buffer whenever
 *              8 bits have been written to it.
 *************************************************************/

#include "BitOutputStream.hpp"
#include <iostream>

#define BUFFERSIZE 8
#define INT_ITERATOR 24
#define BIT_MASK 0x1

/* Function: writeBit()
 * Description: write the bit using bit buffer, so only flush
 *              one you have read the entire byte. Do bit 
 *              manipulation to set the bit in the byte we are
 *              storing to the left most available space.
 * Return value: None.
 */
void BitOutputStream::writeBit(int bit)
{
    //buffer is now full
    if (bufi == BUFFERSIZE){
        flush();
    }
    //move the buffer by one bit so we can append one bit 
    //to the least significant position
    buf <<= 1;
    bufi++;
    //now append the bit
    buf |= bit;
}

/* Function: writeByte()
 * Description: Write the byte one bit at a time since our header
 *              is formed of bits and bytes. This function calls
 *              writeBit() to write the bit in the output file
 * Return value: None.
 */
void BitOutputStream::writeByte(int b)
{
    //write the byte in series of bits, so that the header
    //is properly read since it comprises of both bits & bytes
    char byte = (char)b;
    for (int i = BUFFERSIZE -1; i>=0; i--){
        //get the left most bit of the byte
        int bit = (byte >> i) & BIT_MASK;
        writeBit(bit);
    }
}

/* Function: writeInt()
 * Description: Write the int using a for loop that executes 4 times
 *              and writes to the output file one byte at a time.
 * Return value: None.
 */
void BitOutputStream::writeInt(int i)
{
    for(int index = INT_ITERATOR; index >= 0; index -= BUFFERSIZE){
        int byte = i >> index;
        //put each of the 4 bytes one by one and 
        //flush them
        char character = (char)byte;
        out.put(character);
        out.flush();
    }
}

/* Function: flush()
 * Description: Flush the ostream and output whatever is in it to
 *              the output file. This function resets all values
 * Return value: None.
 */
void BitOutputStream::flush()
{
    //if not all 8 bits have been written to the bit buffer, move
    //all the bits to the left most position
    if (bufi != BUFFERSIZE){
        int idxOffset = BUFFERSIZE - bufi;
        buf <<= idxOffset;
    }
    out.put(buf);
    out.flush();
    buf = 0;
    bufi = 0;

}
