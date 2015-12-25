/*************************************************************************
 * Name: Gurkirat Singh
 * Description: This class takes care of buliding the Huffman tree, encoding,
 *              decoding, calling the decontructor, and as well reading the
 *              header. We use the priority queue to fetch the minimum 
 *              HCNode and build the tree from the bottom. The build function
 *              is overload to include readint the header, which recreates
 *              the tree when uncompress is called. We use post-order when
 *              recreating the tree in uncompress.
 *************************************************************************/

#include "HCTree.hpp"
#include <stack>
#include <algorithm>

#include <iostream>
#include <stdlib.h>
using namespace std;

/* Function: build()
 * Description: This function builds the tree for compress by getting all
 *              the nodes that have atleast a frequency of 1 and then it
 *              pushes them to the priority queue which arranges the nodes
 *              in minimum order. We look at their frequency or symbol
 *              to decide whether or not they are less than the node
 *              we are comparing it to.
 * Return value: None.
 */
void HCTree::build(const vector<int>& freqs)
{

    std::priority_queue<HCNode*, std::vector<HCNode*>,HCNodePtrComp> pQueue;
    for (unsigned int i = 0; i < freqs.size(); i++){
        //skip creating new nodes if a byte doesn't exists
        //Thus, a byte would have frequency of 0
        if (freqs[i] != 0){
            //count, symbol
            leaves[i] = new HCNode(freqs[i],i);
            //add to the queue
            pQueue.push(leaves[i]);  
        }
    }
    cout << "Queue size: " << pQueue.size() << endl; 
    while (pQueue.size() > 1){
        HCNode *nodeOne, *nodeTwo;
        
        //we will pop them twice
        nodeOne = pQueue.top();
        pQueue.pop();
        nodeTwo = pQueue.top();
        pQueue.pop();

        int totalCount = nodeOne->count + nodeTwo->count;
        //create a new parent for the two nodes we extracted
        HCNode *parent = new HCNode(totalCount,nodeOne->symbol, nodeOne,nodeTwo);
        
        //add the two popped nodes to the new parent node
        nodeOne->p = parent;
        nodeTwo->p = parent;
       
        //add the new root node to the queue again
        pQueue.push(parent);
    }


    //there is only one node, that is the root
    if (pQueue.size() == 1){
        root = pQueue.top();
        pQueue.pop();    
    }
}

int HCTree::getTreeHeight(HCNode*node)
{
    if (node == nullptr) return -1;
    else return max(getTreeHeight(node->c0),getTreeHeight(node->c1)) + 1;
}
/* Function: getRoot()
 * Description: Returns the root of the tree we created so that we can get it during
 *              compress and uncompress to check if any tree was created at all.
 * Return value: HCNode pointer to the root.
 */
HCNode* HCTree::getRoot() const
{
    return root;
}

/* Function: build()
 * Desciption: This build function is overloaded. It recreates the Huffman tree
 *             when uncompress is called. It reads the header in series of bits
 *             and bytes, which are placed in a specific header where all leaf
 *             nodes have a bit 1 in front of them and all internal nodes have
 *             the bit 0 in front of them.
 * Return value: None.
 */
void HCTree::build(BitInputStream& in)
{
    //to keep track whether the next thing we are reading is a bit
    int nextIsBit = 1;
    std::stack<HCNode*> stack;


    while (true){
        if (nextIsBit){

            //since next read is a bit, read in a bit
            int bitRead = in.readBit();

            //an error occurred, exit out now
            if (bitRead == -1){
                break;
            }

            //bit was 1, so next read should be byte since leaf node is followed
            //by 1 bit
            if (bitRead){
                nextIsBit = 0;
            }else{

                //bit was zero, so we need to get the stack size to determine
                //whether or not we should stop
                int stackSize = stack.size();

                //if there are atleast two nodes present on the stack, we need
                //create a parent for them
                if(stackSize >=2){
                   
                    HCNode* nodeOne = stack.top(); //right child
                    stack.pop();
                    HCNode* nodeTwo = stack.top(); //left child
                    stack.pop();
                     
                    //create the new parent for the two HCNodes 
                    HCNode *tempRoot = new HCNode(0,nodeTwo->symbol,nodeTwo,\
                        nodeOne,0);
                    nodeOne->p = tempRoot;
                    nodeTwo->p = tempRoot;

                    //push the parent back to the stack
                    stack.push(tempRoot);

                }else if (stackSize == 1){

                    //there is only item on the stack and that is the root
                    root = stack.top();
                    stack.pop();
                    break;
                }

            }

        }else{
            //this ia leaf node, we need to read the symbol of it, which is a
            //byte and then push it on the stack
            char byteRead = in.readByte();
            
            //we don't care for the frequency.
            HCNode *newNode = new HCNode(0,byteRead,0,0,0);
            stack.push(newNode);
            nextIsBit = 1;
        }

    }
}

/* Function: encode()
 * Description: This function starts from the beginning of the tree to get the
 *              encoding for each symbol. 1 is pushed to the stack if the node
 *              is right child of the parent, otherwise 0 is pushed if it's a
 *              left child. We use a stack because we are reading the encoding
 *              in reverse
 * Return value: None.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const
{
    HCNode *currNode = leaves[symbol];
    //stack of int since writeBit takes in int parameter
    std::stack<int> stack;

    //we can't go above root since it has no parent
    while (currNode->p != nullptr){
        HCNode *parentNode = currNode->p;
        if (parentNode->c0 == currNode){
            //node is on the left side of the parent
            stack.push(0);
        }else{
            stack.push(1);
        }
        currNode = parentNode;
    }

    //delete the item from the stack once bit is read
    while(! stack.empty()){
        int encodedBit = stack.top();
        out.writeBit(encodedBit);
        stack.pop(); //remove each element once it's written
    }
}

/* Function: decode()
 * Description: We read in one bit at a time and go down the Huffman
 *              tree to find the leaf with the symbol that we have
 *              encoded in the header. Hence, there are only binary
 *              options, either to go left or right, since a parent
 *              has to have two childs.
 * Return value: Return the symbol (byte) that we encoded.
 */
int HCTree::decode(BitInputStream& in) const
{
    //start from the root and traverse down until if it is either
    //the end of the tree of we find the node
    HCNode *currNode = root;
    
    while (currNode->c0 != nullptr){
        int bit = in.readBit();

        //go left down the tree
        if (bit == 0){
            currNode = currNode->c0;
        }else if (bit == 1){
            //go right down the tree
            currNode = currNode->c1;
        }

    }
    return currNode->symbol;
    
}

/* Function: Decontructor
 * Desciption: Deletes all the nodes that we had created during building the
 *             Huffman tree and sets the root to null pointr.
 * Return value: None.
 */
HCTree::~HCTree()
{
    clearTree(root);
    root = nullptr;
}

/* Function: clearTree()
 * Description: This function does a post order traversal to delete all the
 *              nodes one at a time and then setting them to null pointer
 * Return value: None.
 */
void HCTree::clearTree(HCNode *node)
{
    if (node != nullptr){
        //go left first and then right recursively
        clearTree(node->c0);
        clearTree(node->c1);

        //finally delete the node from the tree
        delete node;
        node = nullptr;
    }
}
