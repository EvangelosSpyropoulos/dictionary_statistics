# Dictionary Statistics
## About
This program calculates the number of occurences of each uppercase letter in words in the Latin Script 
and was created for an assignment on Computer Programming at the University of West Attica.

If the input file contains only uppercase words in the Latin script, one per line, the occurences in 
descending order and the 2 letter combinations of the 5 most frequent letters are displayed. 
In any other case, the first line that does not conform to these specifications is displayed.

## Notes
While developing this program, I experimented with two architectural decisions.

First, memory deallcoation functions are created for memory that needs to be dynamically allocated 
and get registered to be called when the program terminates (using the standard library function on_exit),
alleviating the need to manually deallocate memory in every place where termination of the program is desired.

In addition, functions which are only needed for implementing the dictionary library but are not to be exposed 
to the user of the library (similar to private functions in object-oriented languages) are declared in a 
separate internal header (dictionary_internal.h) which is only used when compiling the dictionary library object 
file. Functions to be exposed to the user are declared in the "public" header (dictionary.h), which effectively 
allows to present a safe abstraction to the user by having only the functions which are supposed to be called when
using the library declared when including the header (to the extent that the exposed functions have actually been 
designed in a way that the user will not have to deal with memory safety).

The aforementioned architectural decisions were not required for the assignment, albeit developing the program this 
way was more interesting.

The program was initially developed adhering to a strictly defined coding style guide required by the assignment, which included
not allowing static array declarations and the exact form of user-facing messages (this version can be seen in the first
commit of this repository). These requirements have subsequently been relaxed and the code has been changed where this led
to a better design.
