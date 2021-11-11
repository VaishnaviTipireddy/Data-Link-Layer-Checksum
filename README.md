# Data-Link-Layer-Checksum
Create, test, and find undetected errors for a CRC-16 checksum: 

The crcExperiments program provides four key functions given the 16-bit CRC 
x^16 + x^12 + x^7 + x^5 + 1:

1) Compute message with checksum (option flag: -c)

2) Validate the message + checksum (option flag: -v)

3) Output all undetected 4-bit errors for a given message (option flag: -f)

4) Output ratio of undetected 2-bit errors for a given message to total possible 
2-bit errors (option flag: -t)

BUILD/RUN/CLEAN:

To compile the program into the executable crcExperiments, run the command:

make

To run the program, use the command

./crcExecutable -[OPTION] [INPUT STRING]

where the options c, v, f, t provide the respective functionality specified above. 
If no option is specified, the program exits with return code 2. 
To remove the executable, run

make clean


IMPLEMENTATION:

The main() function uses getopt() to parse the command line options. 

For the -c option,
the checksum() function uses the mod2() and do_xor() helper functions to emulate 
left-shifting and mod2 arithmetic on a bitstring, getting the mod2 remainder/checksum
from dividing the left-shifted message by the generator. The remainder is then appended
to the initial message to output the message+checksum. 

For the -v option,
the validate() function uses the mod2() and do_xor() helper functions to check that 
the remainder returned consists of all 0s. If so, the function outputs 1, otherwise 0.  

Note: the helper functions checksum2() and validate2() contain most of the 
functionality described for checksum() and validate(). The checksum() and validate() 
functions have the added funciton of printing the respective results to screen, 
which we don't want when the checksum2() and validate2() are being used as helper functions
for options -f and -t.

For the -f option,
the fourBitErrors() function uses four for loops to parse through all possible allocations
of the the four error bits for an error bitstring the size of the message+checksum. 
The message+checksum is computed using the checksum2() function described above. 
For each possible bitstring allocation, the validate2() function is called to check
if the generator divides evenly into the message+checksum XOR error bitstring. If yes,
the function prints the message+checksum XOR error bitstring to standard output.
When iterating through possible error bit allocations, the lowest value the four error
bits can take on are x^16, x^12, x^7, and x^5, in the order of least nested to most
nested loop. This is the case because the error bitstring must a mulitple of the generator,
whose highest four powers are 16, 12, 7, and 5. The highest value the errors bits can 
take on are the most significant bit positions of the error bit string.


For the -t option,
the twoBitErrors() function uses a similar implementation to the fourBitErrors function,
where there are two for loops to parse through all possible allocations of two error bits
for an error bitstring the size of the message+checksum. Again, the lowest value the two
bit can take on are x^16 and x^12, as the error bitstring must be a multiple of the 
generator. The twoBitErrors() function also computes the total possible 2 bit errors there
are of length len, len chooose 2, where len is the length of message+checksum. The function
outputs the ratio of undetected errors over total possible errors.  
 



