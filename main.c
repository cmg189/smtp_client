#include "headers.h"


int main(int argc, char *argv[]){

    char smtp_server[] = "mail.smtp2go.com";    // hostname of smtp2go
    int smtp_port = 2525;   // port# used by smtp2go

    

    char* encoded_username = base64_encode(argv[1]);        // encode username to base64
    char* encoded_password = base64_encode(argv[2]);        // encode password to base64

    int server_socket_fd = connect_to_server(smtp_server, smtp_port);   // create tcp socket to smtp2go

    authenticate_account(server_socket_fd, encoded_username, encoded_password);    // verify account is valid with smtp2go

    struct Email_info email = get_email_details();      // get email info from user

    printf("\nProgram ended\n\n");
    return 0;
}