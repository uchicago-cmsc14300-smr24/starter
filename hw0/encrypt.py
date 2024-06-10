# encrypt.py
# Author: Byron Zhong (byronzhong@cs.uchicago.edu)
#
# Description: reference implementation of the encryption function

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

def encrypt(letter: str, base: str, key: int) -> str:
    """shift a letter up relative to the base within the alphabet"""
    n_alphabet = ord(letter) - ord(base)
    shifted = (n_alphabet + KEY) % N_LETTERS_IN_ALPHABET
    return chr(ord(base) + shifted)


for line in sys.stdin:
    for c in line:
        if isupper(c):
            result = encrypt(c, 'A', KEY)
        elif islower(c):
            result = encrypt(c, 'a', KEY)
        else:
            result = c

        print(result, end='')
