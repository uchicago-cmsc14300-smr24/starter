# freq.py
# Author: Byron Zhong (byronzhong@cs.uchicago.edu)
#
# Description: reference implementation of the letter frequency analysis

import sys

# the number of letters in the alphabet
N_LETTERS_IN_ALPHABET = 26

def isupper(c):
    return ord('A') <= ord(c) and ord(c) <= ord('Z')

def islower(c):
    return ord('a') <= ord(c) and ord(c) <= ord('z')

# number of occurrences of each letter; initialized all to 0
freq = [0 for _ in range(N_LETTERS_IN_ALPHABET)]

# total number of letters read
total = 0

for line in sys.stdin:
    for c in line:
        if isupper(c):
            freq[ord(c) - ord('A')] += 1
            total += 1
        elif islower(c):
            freq[ord(c) - ord('a')] += 1
            total += 1

for i, f in enumerate(freq):
    letter = chr(i + ord('A')) # i-th letter after 'A'
    percentage = (f / total) * 100 if total != 0 else 0.0
    print("%c %6.2f" % (letter, percentage))
