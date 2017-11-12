# SMB_Checksum_Fixer

A Program to calculate and apply the checksum for Super Mokney Ball 2 save and replay files.

F-Zero GX uses a little endian version of this algorithm. You can see it in the fixChecksumLSB method

## Usage 

### Non-Command Line

Just drag the save file on the executable to apply a new checksum.

### Command Line

     ./SMB_LZ_Tool [FILE...]
     
### Acknowledgements

* Ralf for discovering the F-Zero variant

* suloku for having an implementation of the F-Zero variant in the GCMM repo https://github.com/suloku/gcmm

* dwangoAC for providing comuting power to try and brute force the polynomial

* CraftedCart for helping debug

* Wikipedia for having CRC pseudocode and explanations https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks#Bit_ordering_.28endianness.29
