# B+ tree with insertion and internal split

Explanation: B+ tree with insertion, leaf split and non-leaf split. Detailed explanation commented out on the code. "Key:" is the input, returns a list of the leaf.
Numbers in bracket is the number in a leaf, the number between brackets are the key of the root node that indicates which data is where.

Key: 1 
[1]
Key: 2
[1 2]
Key: 3
[1 2 3]
Key: 4
[[1 2]3[3 4]]
Key: 5
[[1 2]3[3 4 5]]
Key: 6
[[1 2]3[3 4]5[5 6]]
Key: 7
[[1 2]3[3 4]5[5 6 7]]
Key: 8
[[1 2]3[3 4]5[5 6]7[7 8]]
Key: 9
[[1 2]3[3 4]5[5 6]7[7 8 9]]
Key: 10
[[[1 2]3[3 4]5[5 6]]7[[7 8]9[9 10]]]
