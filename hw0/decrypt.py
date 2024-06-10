# decrypt.py
# Author: Byron Zhong (byronzhong@cs.uchicago.edu)
#
# Description: reference implementation for decryption

import sys

# the number of letters to shift
KEY = 15

# the number of letters in the alphabet
N_LETTERS_IN_ALPHABET = 26

def isupper(c):
    """return True if c is between 'A' and 'Z'

    This function returns False on non-English uppercase letters such as 'é',
    while the isupper function in the standard library does not.
    """
    return ord('A') <= ord(c) and ord(c) <= ord('Z')

def islower(c):
    """return True if c is between 'a' and 'a'

    This function returns False on non-English lowercase letters such as 'É',
    while the islower function in the standard library does not.
    """
    return ord('a') <= ord(c) and ord(c) <= ord('z')

def decrypt(letter: str, base: str, key: int) -> str:
    """shift a letter down relative to the base within the alphabet"""
    n_alphabet = ord(letter) - ord(base)

    if n_alphabet - key < 0:
        n_alphabet = n_alphabet - key + N_LETTERS_IN_ALPHABET
    else:
        n_alphabet = n_alphabet - key

    return chr(ord(base) + n_alphabet)

for line in sys.stdin:
    for c in line:
        if isupper(c):
            result = decrypt(c, 'A', KEY)
        elif islower(c):
            result = decrypt(c, 'a', KEY)
        else:
            result = c

        print(result, end='')

