#include "headers.h"


int main(int argc, char *argv[]){

    char smtp_server[] = "mail.smtp2go.com";    // hostname of smtp2go
    int smtp_port = 2525;   // port# used by smtp2go

    // check if program is executed with email and password
    check_exe(argc);

    // get email info from user
    struct Email_info email = get_email_details();
    
    int server_socket_fd = connect_to_server(smtp_server, smtp_port);

    authenicate_account(server_sock_fd);

    printf("\nProgram ended\n\n");
    return 0;
}