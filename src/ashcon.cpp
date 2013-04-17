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

    this->command_buffer = (char*) 
        malloc( sizeof(char) * (COMMAND_BUFFER_LENGTH + 1) );
    this->command_buffer[COMMAND_BUFFER_LENGTH] = '\0';

    // NOTE THIS!! Forget to do this and everything goes wrong!
    this->command_arg_num = 0;
    this->command_arg_init();

    this->user_function_list_num = 0;
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
 * Sit and wait for the user to type a command line into the Arduino.
 * Expects to wait until either the buffer has filled or the user has
 * entered a newline character.
 *
 * Currently newline character is hard coded.
 */
int ashcon::get_line() {
    int i = 0; // how many characters we've pulled
    char char_in = '\0';

    // Sit and wait until the line ends or the buffer fills,
    // Danger, it could wait forever without a timer.
    while( i < this->COMMAND_BUFFER_LENGTH ) {
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

            this->command_buffer[i] = char_in;

            if( this->ECHO ) {
                this->line_in->print(char_in);
            }
            i++;
        }
    }

    this->command_buffer[i] = '\0';

    if( this->ECHO ) {
        this->line_in->print("\n\r");
    }

    return this->SUCCESS;
}

/** Split the command_buffer by spaces, store them in the argument list
 */
int ashcon::get_line_splitline() {
    // A huge amount of memory, but whatever
    char current_word[COMMAND_BUFFER_LENGTH] = { '\0' }; 
    char buffer_copy[COMMAND_BUFFER_LENGTH] = { '\0' };

    strncpy(buffer_copy, this->command_buffer, COMMAND_BUFFER_LENGTH);

    // Isolate each word
    while ( strncmp(buffer_copy, current_word, COMMAND_BUFFER_LENGTH) ) {
        sscanf(buffer_copy, "%s %[^\n\t]", current_word, buffer_copy);
        
        this->command_arg_append(current_word);
    }

    return this->SUCCESS;
}

/** Debugging showed that even statically allocated string pointers
 * contain garbage, need to initialise them. I don't like having this
 * function here, maybe make a part of the constructor.
 */
void ashcon::command_arg_init() {
    for( int i = 0; i < this->COMMAND_ARG_NUM_MAX; i++ ) {
        command_arg_list[i] = NULL;
    }

    return;
}

// Given a string, determine its length and copy the string to new memory
int ashcon::command_arg_append( char* new_command ) {
    int new_command_length;

    if( this->command_arg_num >= this->COMMAND_ARG_NUM_MAX ) {
        return 1;
    }

    // Should never get above the length of the buffer
    new_command_length = strnlen(new_command, this->COMMAND_BUFFER_LENGTH );
    
    command_arg_list[command_arg_num] = (char*) 
        malloc( sizeof(char) * (new_command_length + 1) );
    command_arg_list[command_arg_num][new_command_length] = '\0';

    strncpy(command_arg_list[command_arg_num], new_command, 
            new_command_length);

    // NOTE THIS!! Forget to do this and everything goes wrong!
    command_arg_num++;

    return this->SUCCESS;
}

/** Dump the contents of the argument list to console yo
 */
void ashcon::command_arg_dump_debug() {
    this->printf("DEBUG: Drop command arg list\n\r");
    for( int i = 0; i < COMMAND_ARG_NUM_MAX; i++ ) {
        if( command_arg_list[i] ) {
            this->printf("%d. %s\n\r", i, command_arg_list[i]);
        }
    }

    return;
}

/** Dealloc all the string pointers. Forget to call this? You got a
 * memory leak bro.
 *
 * Checks up to the maximum, doesn't honor the current command_num_args
 */
int ashcon::command_arg_clear() {
    for( int i = 0; i < COMMAND_ARG_NUM_MAX; i++ ) {
        if( command_arg_list[i] ) {
            free(command_arg_list[i]);
            command_arg_list[i] = NULL;
        }
    }

    this->command_arg_num = 0;

    return this->SUCCESS;
}

// Allows other functions to mess with the internals, definitely not
// ideal. Use for debugging only
char* ashcon::get_command_buffer() {
    return this->command_buffer;
}
