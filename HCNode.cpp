/************************************************************
 * Name: Gurkirat Singh
 * Description: This class overloads the less than operator
 *              so that we can compare two HCNodes. We compare
 *              the nodes by frequency and if the two frequencies
 *              are the same, we take the lesser symbol.
 ***********************************************************/

#include "HCNode.hpp"

/* Function: operator <
 * Description: This function is overloaded so that we can
 *              compare two HCNodes. We compare them by either
 *              the frequency or the symbol.
 * Return value: True if A is less than B. (example)
 */
bool HCNode::operator< (HCNode const & other)
{
    if (count != other.count){
        return count > other.count;
    }
    
    //counts are the same; so use the symbols for comparison
    return symbol > other.symbol; 
}

/* Function: comp()
 * Description: This function could be used to compare HCNodes
 *              in the priority queue. It utilizes the < operator
 *              overloaded as mentioned above.
 * Return: True if A < B
 */
bool comp(HCNode* one, HCNode* other) 
{
    return one < other;
}
