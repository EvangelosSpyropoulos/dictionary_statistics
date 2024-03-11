#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>

typedef struct dictionary {
        char** words;
        size_t num_of_words;
        size_t* word_lengths;
} dictionary;

/*!
 * Returns an address pointing to a dictionary containing the words read from
 * file at pathname, if the file contains only capital latin characters and
 * newlines.
 * Memory for the dictionary is allocated by this function and gets registered
 * to be freed at normal process termination.
*/
dictionary* read_dictionary_from_file(const char* restrict pathname);


/*!
 * Prints to output_stream the frequencies of the capital letters in all words
 * of the dictionary pointed to by dict in the form "Y XXXXXXX" where Y is the
 * letter and XXXXXXX is the frequencies with 7 digits sorted by declining
 * order and the 2 letter combinations of the 5 most frequent letters in the
 * form "XY", where X and Y are the 2 letters.
*/
void print_statistics(dictionary* const dict, FILE* restrict output_stream);


/*!
 * Prints "Memory Problem" followed by a newline to ouptut_stream and causes
 * normal process termination with status code set to status
*/
void print_memory_problem_and_exit(FILE* restrict output_stream, int status);
#endif
