#ifndef DICTIONARY_INTERNAL_H
#define DICTIONARY_INTERNAL_H

/*
 * To use dictionary, include "dictionary.h".
 *
 * This header contains declarations for functions used for the implementation
 * of the functions declared in dictionary.h and are not exposed when including
 * dictionary.h. The functions declared in dictionary.h are safe abstractions
 * over the implementation details. Functions in this header are considered
 * unsafe.
*/


// Required for the struct dictionary definition to avoid duplication.
#include "dictionary.h"

/*!
 * Calculates the frequencies of the capital letters in all words of the
 * dictionary pointed to by dict.
 * Allocates memory for an array of 'Z' - 'A' + 1 long integers where the
 * frequencies are stored and returns a pointer to it. Returns NULL if dict is
 * NULL or any word in dict is NULL.
 * Memory allocated by this function gets registered to be freed at normal
 * process termination.
*/
long* calculate_letter_frequencies(dictionary* const dict);


/*!
* Sorts capital latin letters in letters in-place by declining frequency using
* the quicksort algorithm.
* frequencies are used for comparisons but are not modified.
* frequencies must contain the frequencies of the letters in alphabetical
* order.
*/
void sort_letters_by_declining_frequency(
    char* letters, const long const* frequencies, size_t left, size_t right
);


/*
 * Swaps in-place the elements at array[first_index] and array[second_index]
 * Does no error-checking; array must not be null and array+first_index and
 * array+second_index must point to valid memory
*/
void swap(char* array, size_t first_index, size_t second_index);


/*!
 * Frees the allocated memory pointed to by the fields of the dictionary at
 * dictionary_addr.
 * Intended to be registered to the function on_exit.
*/
void free_dictionary(int status, void* dictionary_addr);


/*!
 * Frees the allocated memory at address.
 * Intended to be registered to the function on_exit.
*/
void free_callback(int status, void* address);
#endif
