##Lossless Human Compression

This program lets you compress any of file using Huffman Compression. Huffman Compression tends to give you around 50% compression ratio on average, which is pretty good but not anything like the Lempel-Ziv algorithms or other advanced algorithms that give you 30 to 35% compression ratio.

####Overview of algorithm:
Firstly, the input file is scanned line by line and a frequency of each letter is calculated by using an array. We use that frequency array to create the Huffman Tree where the most frequency letters have the smallest binary encoding and the lesser frequent ones have increasingly larger encoding. This way the tree is created bottom-up rather than up-down. We then serialize the tree by doing post order or in-order traversal and store the tree in the temporary output file. When uncompressing the output file, you recreate the tree from the serialization and find the characters that were attached to the leaf nodes. We put these characters to another output file, whose output is going to be same as our original input file.

**Run:** 
```c++
>>> make 

//Compress File
>>> ./compress <input.txt> <output.txt>

//Uncompress file
>>> ./uncompress <output.txt> <origOutput.txt>

//Check if input and output files are same
>>> diff <input.txt> <origOutput.txt>
```