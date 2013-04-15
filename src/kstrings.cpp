#include "kstrings.h"

kstring::kstring( int length ) {
    this->len = length;
    this->str = (char*) malloc( sizeof(char) * ( length + 1 ) );
    this->str[length] = '\0';
}

kstring::~kstring() {
    free(this->str);
}

kstring::kstring( char* source_string_pointer ) {
    int new_len = strnlen(source_string_pointer, this->MAX_STRING_SIZE);

    this->len = new_len;
    this->str = (char*) malloc( sizeof(char) * ( new_len + 1 ) );
    this->str[new_len] = '\0';

    strncpy(this->str, source_string_pointer, new_len);
}

int kstring::compare(kstring* alien_string) {
    int max = this->len > alien_string->len ? 
        alien_string->len : this->len;
    return strncmp(this->str, alien_string->str, max);
}

int kstring::compare(char* alien_string) {
    int alien_string_len = strnlen(alien_string, this->MAX_STRING_SIZE);
    int max = this->len > alien_string_len ? 
        this->len : alien_string_len;

    return strncmp(this->str, alien_string, max);
}

int kstring::adopt(char* copiable_string) {
    strncpy(this->str, copiable_string, this->len);

    return 0;
}
