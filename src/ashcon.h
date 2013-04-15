#ifndef ashcon_h
#define ashcon_h

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class ashcon {
    private:
        static const int SUCCESS = 0;
        static const int BUFFER_LENGTH = 128;
        static const int ECHO = 1;
        static const int MAX_ARGUMENT_LIST = 12;
        static const int MAX_WORD_LENGTH = 50;
        Stream* line_in;

        typedef struct {
            int num_args;
            char** argv;
        } func_list;

        char* internal_buffer;
        int internal_buffer_len;

        func_list* internal_func_list;

    public:
        ashcon(Stream* new_line_in);

        int printf(char* fmt, ... );
        int get_line();
        int split_line(func_list*);

        char* get_internal_buffer();

        int prompt();
        int clear_func_list(func_list*);
        int debug_func_list(func_list*);
        func_list* get_internal_func_list();
};

#endif
