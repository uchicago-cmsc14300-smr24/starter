#!/usr/bin/env python3

import sys
from collections import OrderedDict

def add_entry_to_table(table, hometown, name):
    if hometown in table:
        table[hometown].append(name)
    else:
        table[hometown] = [name]

def index_file(file) -> dict:
    table = {}
    while (line := file.readline()):
        hometown, name = line.strip('\n').split('\t')
        add_entry_to_table(table, hometown, name)

    return OrderedDict(sorted(table.items()))

def print_entry(hometown, group):
    print(f"{hometown}:")
    for name in group:
        print(name)

def print_groups(table):
    for digest, group in table.items():
        if len(group) < 2:
            continue

        print_entry(digest, group)
        print()

def main():
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} <file>")
        exit(1)

    filename = sys.argv[1]
    with open(filename, "r") as file:
        table = index_file(file)

    print_groups(table)


if __name__ == '__main__':
    main()

