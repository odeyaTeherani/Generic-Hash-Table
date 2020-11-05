# Generic-Hash-Table
Dynamic memory Allocation, Generic Programming, Function Pointers and Libraries

# Background: Hash Tables
Hash Tables are data structures that support these three operations:
1. Adding an element
2. Removing an element
3. Searching for an element
All of the operations listed above are performed in an average time of O(1). In your data structures course, you have encountered hash tables. It is acceptable and common to implement hash tables using an array. In this exercise the size of the array is dynamic, i.e. the size of the array can grow. Mapping of an object to the appropriate location in this array is accomplished using a hash function:
i=d*H(k,m)
We can assume that for each object there is a key – k, which identifies the object. The hash function receives the key and the original size of the table - m (array size), and returns a pointer to the location that this object is supposed to be stored in - i. this pointer is multiply by d, which is the ratio between the current size of the table to its original size, initially, d=1.

# Handling Collisions
The size of an array is generally smaller than the number of possible keys given. Thus, more than one key can be mapped by a specific hash function to a single location in the array which forms the hash table. There are various ways to deal with this issue. In this exercise we will use two simple methods together: linked lists and dynamic expansion of the hash table.

a. Linked Lists:
  In each element in the hash table there is a pointer to the head of a list. Any new object that maps to
  this element in the hash table, will be added to the end of this list.
b. Table Duplication
  We will build an implementation in which no more than t elements can be mapped to the same entry in
  the hash table. When we need to add an element to an index i where there are already t elements, go to
  the first location after i where there is room.
  If no place was found before the next original location in the table, or the end of the table was reached,
  the entire table size will be cloned (doubling the size of the table) in the following manner:
   All existed entries of the table 0-(n-1) will be mapped to the respective even entries in the new
  table (this is accomplished by multiplying the original index by 2).
   If an additional expansion of this table is required, this process is repeated.
   In this manner the distance between entries of the original table are always a power of two, e.g.
  after one expansion the original entries are at location 0,2,4,6… after two expansions
  0,4,8,12,16… after three expansions 0,8,16,24… etc.

# Interface
The interface for the hash table shall be defined in the file GenericHashTable.h.
You shall implement a hash table that supports the following operations
1. Adding an element
2. Removing an element
3. Searching for an object
4. Doubling the Size of the table
In the interface file GenericHashTable.h that you get from us, there are various types of declarations,
however the declarations of the names and types are missing in the structs Table and Object (although
there are function declarations that use these objects). You need to define the structures Table and
Object in GenericHashTable.h . The struct Table represents the table described above. The structure
Object is an element in the table.
# Objects:
The table is designed for use with general objects. To do this one must declare a struct (called Object)
which includes a void* pointer named key as the object's key. The struct named Object can include
additional fields as necessary in your implementation.
Implement the “constructor” of this object that is defined in the header file (function CreateObject), you
may implement other constructors as you wish
The function freeObject is designed to release the object's memory.
# Table Representation:
You shall define a struct named Table which includes the relevant fields of the struct. In the header
there is a constructor which returns a pointer to the table (CreateTable). This function you must
implement. You may implement other constructors as you wish.
Mapping of elements to the table is done with a key as described above. Implementation of insertion to
be done as described above.

# Print Table:
The function printTable will print out the table in the following format:
[idx] \t elm1 \t --> \t elm2 \t …
The left column contains a number in brackets which is the index, and the index points to the
strings/integers that are pointed to by this index.
For example, the line: [0] 20 --> 40 -->
Means that the numbers 20 and 40 are chained to entry 0 in the table.
Note that there is only a tab between the index to the first element and between the elements to the
arrow.



