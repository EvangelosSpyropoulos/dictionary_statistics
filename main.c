#include "dictionary.h"


void print_usage(FILE* restrict output_stream);


int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid number of parameters\n");
        print_usage(stderr);
        return -1;
    }

    // Do not free this pointer; memory deallocation gets registered to happen
    // at normal process termination.
    dictionary* dict = read_dictionary_from_file(argv[1]);

    print_statistics(dict, stdout);

    return 0;
}


void print_usage(FILE* restrict output_stream) {
    fprintf(output_stream,
        "Usage: program dictionary_file\n\n"
        "program: path to executable\n"
        "dictionary_file: path to input text file\n\n"
        "Valid input text files contain only uppercase words in the Latin "
        "script separated by newlines.\n\n"
        "Prints the number of occurences of each letter in dictionary_file in descending "
        "order and the 2 letter combinations of the 5 most frequent letter.\n"
    );
}
