#ifndef kstrings
#define kstrings

#include <string.h>
#include <stdlib.h>

class kstring {
    public:
        static const int MAX_STRING_SIZE = 1024;
        char* str;
        int len;

        kstring( int length );
        kstring( char* source_string_pointer );
        ~kstring();

        int compare(kstring* alien_string);
        int compare(char* alien_string);
        int adopt(char*);
};
#endif
