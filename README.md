# Credit-Card-Project-using-Chained-Hash-Table

Compile & Execute main.c which takes input from data.txt

### Hash Function Used

H(x) = ((x%10000)*5 + (((x/10000)%10000)*3) +(((x/100000000)%10000)) % 1000

where x is credit card number and is of type unsigned long long

The Hash map data structure designed will have 1000 index and 10 buckets. The load density 
for 4096 entries is 0.41. The memory efficiency is not up to the mark as hash tables is used, 
almost 2.5 times more than actual required storage is used but on the bright side it is giving 
search time complexity of O(1)

### Check size of the structure card

Compile & run checksize.c which checks the size of structure card which is located in card.h

Struct Card will be the main structure for the entire project. This structure contains 
9 different elements which stores credit card details. The structure Card is aligned in the most 
efficient manner. In a 64 bit machine 5 bytes padding will be present in this structure. The 
total size of this structure is 151 + 5(padding) + 8(next pointer) = 264.
