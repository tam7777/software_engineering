# Created a B+ tree only insertion

Explanation: When the input data is smaller then the smallest one of the node then shift all the value and insert the input data in the smallest number.
If not then using a for loop, shift all the value that is bigger than the input data and allocate the input data at the correct position.

Input: 
[10,12]
Key: 11

Output:
[10,11,12]
