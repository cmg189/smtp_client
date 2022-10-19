#include "headers.h"


int main(int argc, char *argv[]){

    // check if program is executed with email and password
    check_exe(argc);

    // get email info from user
    struct Email_info email = get_email_details();
    

    printf("\nProgram ended\n\n");
    return 0;
}