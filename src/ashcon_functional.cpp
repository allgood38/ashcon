#include "ashcon.h"

int ashcon::call( char* marked_id ) {
    for( int i = 0; i < 50; i++ ) {
        if( this->user_function_list != NULL && 
                strcmp(this->user_function_list->id, marked_id) == 0 ) {
            this->user_function_list->func(this->internal_func_list->argv);
        }
    }
}


