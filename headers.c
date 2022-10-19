#include "headers.h"


// check if program is executed with email and password
void check_exe(int num_args){

    if(num_args != EXE_ARGS){
        printf("\nUsage: email_client your@email.com password\n\nProgram ended\n\n");
        exit(EXIT_FAILURE);
    }

    return;
}
