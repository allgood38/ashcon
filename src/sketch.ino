#include "sketch.h"

void test_kcompare() {
    // Nothing yet
}

void test_kstring_adopt() {
    char* test1 = "blergthis";
    kstring* blerg = new kstring( (char*) test1 );
    kstring* test2 = new kstring("blergthis");

    mycon->printf("STRING: %s\n\r", blerg->str);
    mycon->printf("Calling compare does: %d", 
        blerg->compare(test2) );


    delete test2;
    delete blerg; // Note the new keyword calls memory off heap

    return;
}

void test_string_from_console() {
    kstring* user_input;

    mycon->printf("> ");
    mycon->get_line();
    user_input = new kstring( mycon->get_internal_buffer() );

    mycon->printf("You typed this: %s \n\r", user_input->str);
    mycon->split_line(mycon->get_internal_func_list());
    mycon->printf("CHECK:\n\r");
    mycon->debug_func_list( mycon->get_internal_func_list() );
    mycon->clear_func_list( mycon->get_internal_func_list() );
    mycon->printf("CHECK:\n\r");
    mycon->debug_func_list( mycon->get_internal_func_list() );

    delete user_input;
}

void setup() {
    Serial.begin(115200);

    testk = new kstring( 100 );
    mycon = new ashcon(&Serial);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Testing kstrings");
}

void loop() {

    while(true) test_string_from_console();

    mycon->printf("[ARD]> ");
    mycon->get_line();
    mycon->printf("%s\n\r", mycon->get_internal_buffer() );

    mycon->printf("Shutdown. \n\r");
    delete mycon;
    delete testk;
    while( true ) delay( 100 );
}

