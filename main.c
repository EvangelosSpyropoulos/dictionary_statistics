#include "dictionary.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid number of parameters\n");
        return -1;
    }

    // Do not free this pointer; memory deallocation gets registered to happen
    // at normal process termination.
    dictionary* dict = read_dictionary_from_file(argv[1]);

    print_statistics(dict, stdout);

    return 0;
}
