/*
 * Encrypting text using Caesar Encryption.
 *
 * Description: this program shifts all letter from STDIN by KEY while leaving
 * non-letter characters intact, and outputs the result to STDOUT. Uppercase
 * letters will remain uppercase after shifting, so will lowercase letters.
 *
 * Note: this code is extremely over-commented.
 */
#include <stdio.h>
#include <stdlib.h>

/* the number of letters we are shifting */
const char KEY = 15;

/* the number of letters in the alphabet */
const int N_LETTERS_IN_ALPHABET = 26;

/******************************************************************************/
/*                             Helper Functions                               */
/******************************************************************************/

/*
 * Given any character c, return a non-zero value if c is between 'A' and 'Z'
 * and zero otherwise.
 */
int is_upper_letter(char c);

/*
 * Given any character c, return a non-zero value if c is between 'a' and 'z'
 * and zero otherwise.
 */
int is_lower_letter(char c);

/*
 * Given any character c, the first letter in the alphabet, and a shift amount,
 * return the shifted character, wrapping around if the shifted letter is
 * outside of the alphabet.
 */
char shift_by(char c, char first, char shift_amount);

/******************************************************************************/
/*                                    main                                    */
/******************************************************************************/

/*
 * This is the entry point of the program. Since it is not expecting any
 * command-line arguments, we put void as its argument.
 */
int main(void)
{
    int c;

    /* assign the result of getchar() to c and compare the result to EOF */
    while ((c = getchar()) != EOF) {
        char result;
        if (is_upper_letter(c)) {
            result = shift_by(c, 'A', KEY);
        } else if (is_lower_letter(c)) {
            result = shift_by(c, 'a', KEY);
        } else {
            result = c;
        }
        putchar(result);
    }

    /* the program terminates normally */
    return EXIT_SUCCESS;
}

/******************************************************************************/
/*                        Helper Function Definitions                         */
/******************************************************************************/

/*
 * is_upper_letter exploits the ordering of ASCII representation of characters
 * by comparing the numeric value of c against those of 'A' and 'Z'.
 */
int is_upper_letter(char c)
{
    return 'A' <= c && c <= 'Z';
}

/*
 * is_lower_letter exploits the ordering of ASCII representation of characters
 * by comparing the numeric value of c against those of 'a' and 'z'.
 */
int is_lower_letter(char c)
{
    return 'a' <= c && c <= 'z';
}

/*
 * shift_by uses the ordering of ASCII representation of characters to
 * calculate the letter position in the alphabet; wrapping is achieved by using
 * the modulo operator
 */
char shift_by(char c, char first, char shift_amount)
{
    char n_alphabet = c - first;
    n_alphabet = (n_alphabet + shift_amount) % N_LETTERS_IN_ALPHABET;
    return first + n_alphabet;
}

