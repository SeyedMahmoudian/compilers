Buffers are often used when developing compilers because of their efficiency.This is an impletement buffer that can operate in three different mode:
1. Fixed size 
2. Additive self-incrementing 
3. Multuplicative self-incrementing 

The buffer implementation is base on two associated data structes: 
* Buffer Descriptor
* Array of characters

Both structure are created on demand at run time. 

The Buffer Descriptor or Buffer Handle suggest the purpose of this buffer control data structure containts all the necessary information about the array of characters.
