

# Caesar Cipher Breaker

This is a C++ program that is designed to aid in the breaking of Caesar Ciphers.

---
## COMPILING

In a linux shell, navigate to the directory containing decrypt.cpp

Run the following g++ command:
```bash
$  g++ -std=c++11 decrypt.cpp
```
## EXECUTING THE PROGRAM

3 additional arguments are expected when executing the program.

The outline for executing the program is:
```bash
$  ./a.out [cipherTextFileName] [dictionaryTextFileName] -[analysisTypeFlag]
```

### SUGGESTED METHOD FOR DISPLAYING FREQUENCY ANALYSIS OF CIPHERTEXT

Perform a no analysis execution of the program using the following command:
```bash
$ ./a.out cipher.txt dic.txt -n
```
### SUGGESTED METHOD FOR PERFORMING A DECRYPTION
 
(See CryptAnalysisUsingDecryptCPP.pdf for an outline of this process)

Perform an automatic decryption with the following command:
```bash
$ ./a.out cipher.txt dic.txt -a
```

Once decryption and scoring is complete, look at the decrypted text.
If you see a word that is slightly misspelled, use the 'r' option to
replace the character in the decryption with the correct character.
Do this for as many characters you recognize need replacing. When you
don't have anymore suggestions for replacing, use the 'n' option and copy
in the last key returned by the 'r' option. After the new decryption is complete,
repeat the same process of replacing characters and using the newly created key
until you are satisfied with the decrypted text.

### SUGGESTED METHOD FOR PERFORMING A DECRYPTION USING YOUR OWN KEY

Perform a custom decryption with the following command:
```bash
$ ./a.out cipher.txt dic.txt -c
```

Enter your key in all capital characters
You should see the decryption using your key.

## ARGUMENT DETAILS

**[cipherTextFileName]**

This string should be the full file name of your cipher text, including the extension. 
Example: cipher.txt
The cipher text file should contain only capital ASCII characters and no 
whitespaces in order to avoid errors. Additionally, the cipher text file should be in 
the same directory as decrypt.cpp.


**[dictionaryTextFileName]**

This string should be the full file name of your cipher text, including the extension.
Example: dic.txt
The dictionary text file should be limited to one word per line. Additionally, words
should be in capital ASCII Character format for proper scoring of your decryption.
Additionally, the dictionary text file should be in the same directory as decrypt.cpp.

**-[analysisTypeFlag]**

This string should be two characters long, where the first character is '-' and the
second character is one of three options ['a', 'c', 'n'].
Example: -a


## ANALYSIS TYPES

'a' - Auto Decryption and Analysis
This flag performs an initial automatic decryption based on the frequency of each character.

'c' - Custom Decryption and Analysis
This flag prompts users for a custom key that the user supplies and then performs a
decryption using that key.

'n' - No Decryption
This flag only displays the frequency list of characters in the original ciphertext. No
decryption or scoring of said decryption is performed.


## Optimizations

This program has a few means by which it can be improved. Namely, efficiencies in the current implementation of automated decryption and 'scoring' the current deciphered text. Improvements in scoring can be either in improving the speed by optimizing big O runtime of the method or in replacing the scoring method with an approach that takes a more NLP approach. One such new scoring method could be the use of an N-gram model to inform the most likely colocated words.