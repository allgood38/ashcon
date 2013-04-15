#include "ashcon.h"

/** Initialize a console
 *
 * Pass a serial line to be used as the command console. Initialise the
 * internal buffer off the heap.
 */
ashcon::ashcon(Stream* new_line_in) {
    if( new_line_in != NULL ) {
        this->line_in = new_line_in;
    }

    this->internal_buffer = (char*)
                            malloc( sizeof(char) * (BUFFER_LENGTH + 1) );
    this->internal_buffer[BUFFER_LENGTH] = '\0'; //note the offset
    this->internal_buffer_len = BUFFER_LENGTH;

    this->internal_func_list = (func_list*) malloc( sizeof(func_list) );
    this->internal_func_list->num_args = 0;
    this->internal_func_list->argv = (char**) 
        malloc( sizeof(char*) * MAX_ARGUMENT_LIST );

}

/** Emulated printf
 *
 * Taken from arduino forum, emulates printf without having to
 * include the true version which requires a FILE handle and a lot more
 * memory.
 */
int ashcon::printf( char* fmt, ... ) {
    // Taken from http://playground.arduino.cc/Main/Printf
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    this->line_in->print(tmp);

    return this->SUCCESS;
}

/** get_line
 *
 * Sit and wait for the user to type a command line into the arduino.
 * Expects to wait until either the buffer has filled or the user has
 * enetered a newline character.
 *
 * Currently newline character is hardcoded.
 */
int ashcon::get_line() {
    int i = 0; // how many characters we've pulled
    char char_in = '\0';

    // Sit and wait until the line ends or the buffer fills,
    // Danger, it could wait forever without a timer.
    while( i < this->internal_buffer_len ) {
        if( this->line_in->available() > 0 ) {
            // A character is ready, grab it
            char_in = this->line_in->read();

            // Newline detection, varies on the monitor program...
            if( char_in == '\r' ) {
                //this->printf("Found an \\r \n\r");
                break;
            } else if ( char_in == '\n' ) {
                //this->printf("Found a \\n \n\r");
                break;
            }

            this->internal_buffer[i] = char_in;

            if( this->ECHO ) {
                this->line_in->print(char_in);
            }
            i++;
        }
    }

    this->internal_buffer[i] = '\0';

    if( this->ECHO ) {
        this->line_in->print("\n\r");
    }

    return this->SUCCESS;
}

char* ashcon::get_internal_buffer() {
    return this->internal_buffer;
}

// this is seriously sketchy in operation, but it works
// should a memory leak be found, examine this first
/** Splitline
 *
 * Populate the function list with words to be used as arguments in a
 * function call.
 */
int ashcon::split_line(func_list* dest_list) {
    int num_args = 0;
    char word_in[this->MAX_WORD_LENGTH] = {'\0'};

    while( strncmp(word_in, this->internal_buffer, 
                this->BUFFER_LENGTH ) != 0 ) {

        sscanf(this->internal_buffer, "%s %[^\n\t]",
                word_in, this->internal_buffer);

        this->printf("STORING: %d. %s\n\r", num_args, word_in);
        this->internal_func_list->argv[num_args] = (char*) 
            malloc( sizeof(char) * (strlen(word_in) + 1) );
        this->internal_func_list->argv[num_args][strlen(word_in)] = '\0';
        
        strncpy(this->internal_func_list->argv[num_args], word_in,
                strlen(word_in) );

        this->printf("STORE SUCCESS\n\r");

        num_args++;
    }
    this->internal_func_list->num_args = num_args;

    return 0;
}

int ashcon::clear_func_list(func_list* marked_func_list) {
    for( int i = 0; i < marked_func_list->num_args; i++ ) {
        if( marked_func_list->argv[i] != NULL ) {
            free(marked_func_list->argv[i]);
        }
    }
    marked_func_list->num_args = 0;

    return 0;
}

int ashcon::debug_func_list( func_list* marked_func_list ) {
    for(int i = 0; i < marked_func_list->num_args; i++ ) {
        if( marked_func_list->argv[i] != NULL ) {
            this->printf("%s\n\r", marked_func_list->argv[i]);
        }
    }
}

ashcon::func_list* ashcon::get_internal_func_list() {
    return this->internal_func_list;
}
