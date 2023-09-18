# Cipher Program

## Overview

The program can perform encryption and decryption using block and stream ciphers.

## Usage

To use this program, follow these steps:
For testing you'll need to create a test.txt with your input, a key.txt, encrypted.txt (empty), and decrypted.txt(empty).

1. Compile the Program

   Compile the program using the provided Makefile.
   Open a terminal, navigate to the program's directory, and run the command:
        make

2. Encryption Example: ./cipher_program B test.txt encrypted.txt key.txt E
    B:              specifies block cipher mode.
    test.txt:       input file.
    encrypted.txt:  output file for encrypted data.
    key.txt:        key file.
    E:              specifies encryption mode.

    You should see encrypted output in the encrypted.txt file.

3. Decryption Example: ./cipher_program B encrypted.txt decrypted.txt key.txt D
    B:              specifies block cipher mode.
    encrypted.txt:  input file containing the encrypted data.
    decrypted.txt:  output file for decrypted data.
    key.txt:        key file.
    D:              specifies decryption mode.

    You should see the decrypted text in decrypted.txt.
    NOTE: you'll use the same type of cipher to decrypt as you used for encrypt.

4. cleanup by running the command:
    make clean
