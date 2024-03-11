#include "dictionary.h"
#include "dictionary_internal.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

dictionary* read_dictionary_from_file(const char* restrict pathname) {
    dictionary* dict = malloc(sizeof(dictionary));
    if (!dict) {
        print_memory_problem_and_exit(stdin, -2);
    }

    dict->words = NULL;
    dict->num_of_words = 0;

    // Register dictionary memory deallocation function free_dictionary with
    // dictionary address dict as an argument to be called at normal process
    // termination (via exit or via return from main())
    on_exit(free_dictionary, dict);

    FILE* file = fopen(pathname, "r");
    if (!file) {
        printf("Cannot read input file\n");
        exit(-3);
    }

    int character;
    size_t word_length = 0;
    while ((character = getc(file)) != EOF) {
        word_length++;

        if (!isupper(character) && character != '\n') {
            printf("Invalid Data in Line %07ld\n", dict->num_of_words + 1);
            exit(-4);
        }

        if (character == '\n') {
            dict->num_of_words++;

            // Allocate memory to store a pointer to word.
            dict->words = reallocarray(dict->words, dict->num_of_words, sizeof(char*));
            if (!dict->words) {
                print_memory_problem_and_exit(stdin, -2);
            }

            // Allocate enough memory to store the word and '\0' in place of '\n'
            dict->words[dict->num_of_words - 1] = calloc(word_length, sizeof(char));
            if (!dict->words[dict->num_of_words - 1]) {
                print_memory_problem_and_exit(stdin, -2);
            }

            // Move to start of line
            fseek(file, -word_length, SEEK_CUR);

            // Store the word without the newline
            fgets(
                dict->words[dict->num_of_words - 1],
                word_length,
                // dict->word_lengths[dict->num_of_words],
                file
            );
            getc(file); // Read the newline

            word_length = 0;
        }
    }
    fclose(file);

    return dict;
}


void print_statistics(dictionary* const dict, FILE* restrict output_stream) {
    if (!dict) { return; }

    long* letter_frequencies = calculate_letter_frequencies(dict);
    if (!letter_frequencies) { return; }

    char letters['Z' - 'A' + 1];

    // Initialize letters with capital latin character in alphabetical order
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        letters[letter - 'A'] = letter;
    }

    sort_letters_by_declining_frequency(
        letters, letter_frequencies, 0, 'Z' - 'A'
    );

    // Print each letter by declining frequency and its respective frequency
    for (size_t i = 0; i < 'Z' - 'A' + 1; i++) {
        fprintf(
            output_stream,
            "%c %07ld\n", letters[i],
            letter_frequencies[letters[i] - 'A']
        );
    }

    // Print the the 2 letter combinations of the 5 most frequent letters
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = i + 1; j < 5; j++) {
            fprintf(output_stream, "%c%c\n", letters[i], letters[j]);
        }
    }
}


long* calculate_letter_frequencies(dictionary* const dict) {
    if (!dict) { return NULL; }

    long* letter_frequencies = calloc('Z' - 'A' + 1, sizeof(long));
    if (!letter_frequencies) {
        print_memory_problem_and_exit(stdin, -2);
    }
    on_exit(free_callback, letter_frequencies);

    // Initialize all letter frequencies to 0.
    memset(letter_frequencies, 0, 'Z' - 'A' + 1);

    for (size_t w_ind = 0; w_ind < dict->num_of_words; w_ind++) {
        if (!dict->words[w_ind]) {
            if (letter_frequencies) { free(letter_frequencies); }
            return NULL;
        }

        for (size_t l_ind = 0; dict->words[w_ind][l_ind] != '\0'; l_ind++) {
            // Find the letter's index in letter_frequencies and increment the
            // frequency
            letter_frequencies[dict->words[w_ind][l_ind] - 'A']++;
        }
    }

    return letter_frequencies;
}


/*
 * In this implementation of the quicksort algorithm the elements of the letters
 * array are used to index into the frequencies array to allow sorting the
 * letters array without modifying the frequencies array.
 * This is possible because of the one-to-one (bijective) mapping from the
 * capital latin letters, as a continuous range of integers in the system's
 * character set, to their respective frequencies in the alphabetically sorted
 * frequencies array.
*/
void sort_letters_by_declining_frequency(
    char* letters, const long const* frequencies, size_t left, size_t right
) {
    if (!letters || !frequencies || left >= right) { return; }

    // Partition frequencies

    const long partitioning_element = frequencies[letters[right] - 'A'];
    size_t i = left, j = right;

    while (i < j) {
        // Move i to the index of the first element smaller than the
        // partitioning element (staying in bounds)
        while (
            frequencies[letters[i] - 'A'] >= partitioning_element
            && i < right
        ) {
            i++;
        }

        // Move j to the index of the last element bigger than the
        // partitioning element (staying in bounds)
        while (
            frequencies[letters[j] - 'A'] <= partitioning_element
            && j > left
        ) {
            j--;
        }


        if (i < j) { // the partitioning element's position has not been found
            // Swap letters so that bigger or equal elements (letters with
            // higher or equal frequencies) are left of the partitioning
            // element and smaller or equal elements (letters with smaller or
            // equal frequencies) are right of the partitioning_element.
            swap(letters, i, j);
        }
    }

    // The partitioning elements' position has been found
    // Swap the partitioning element with the first element that is smaller.
    swap(letters, i, right);


    // Recursively sort the left and right subarrays/partitions
    if (i != left) {
        sort_letters_by_declining_frequency(letters, frequencies, left, i-1);
    }
    if (i != right - 1) {
        sort_letters_by_declining_frequency(letters, frequencies, i+1, right);
    }
}


inline void swap(char* array, size_t first_index, size_t second_index) {
    char tmp = array[first_index];
    array[first_index] = array[second_index];
    array[second_index] = tmp;
}


void free_dictionary(int status, void* dictionary_addr) {
    if (!dictionary_addr) { return; }

    dictionary dict = *(dictionary*)dictionary_addr;

    if (dict.words) {
        for (size_t i = 0; i < dict.num_of_words; i++) {
            if ( dict.words[i]) { free(dict.words[i]); }
        }
        free(dict.words);
    }

    free(dictionary_addr);
}


void free_callback(int status, void* address) {
    if (address) { free(address); }
}


void print_memory_problem_and_exit(FILE* restrict output_stream, int status) {
    fprintf(output_stream, "Memory Problem\n");
    exit(status);
}
