#include "ashcon.h"

int ashcon::user_function_register( const char* id, int (*func)(char* args[]) ) {
    int id_length;

    if( user_function_list_num >= USER_FUNCTION_LIST_MAX ) {
        return this->FAILURE;
    }

    id_length = strnlen( id, COMMAND_BUFFER_LENGTH );

    // debugging
    this->printf("ID_LENGTH: %d\n\r", id_length);
    this->printf("FUNC_NAME: %s\n\r", id);

    this->user_function_list[user_function_list_num].id = 
        (char*) malloc( sizeof(char) * (id_length + 1) );
    this->user_function_list[user_function_list_num].id[id_length] = '\0';

    strncpy(this->user_function_list[user_function_list_num].id, (char*) id,
            id_length );
    this->user_function_list[user_function_list_num].func = func;

    this->user_function_list_num++;

    return this->SUCCESS;
}   

int ashcon::user_function_list_debug() {
    this->printf("Testing function list. \n\r");
    for( int i = 0; i < user_function_list_num; i++ ) {
        this->printf("%d. %s\n\r", i, 
                this->user_function_list[user_function_list_num].id );
        this->printf("%d. %x\n\r", i, 
                this->user_function_list[user_function_list_num].func );
    }

    return this->SUCCESS;
}
