#include "headers.h"


int main(int argc, char *argv[]){
    // hostname and port number of smtp2go
    char smtp_server[] = "mail.smtp2go.com";
    int smtp_port = 2525;

    // prompt user for username and password
    struct Account_info account = get_account_info(smtp_server, smtp_port);

    // encode username and password in base64
    char* encoded_username = base64_encode(account.username);
    char* encoded_password = base64_encode(account.password);

    // create tcp socket with smtp2go
    int server_socket_fd = connect_to_server(smtp_server, smtp_port);

    // verify account info is valid
    authenticate_account(server_socket_fd, encoded_username, encoded_password);

    // get email info from user
    struct Email_info email = get_email_details();

    // format commands to send to server
    struct Email_commands commands = format_commands(email);

    // send commands to server
    send_commands(server_socket_fd, commands);

    // close tcp socket with smtp2go
    close_connection(server_socket_fd);

    printf("\nProgram ended\n\n");
    return 0;
}
