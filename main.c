#include "headers.h"


int main(int argc, char *argv[])
{
    struct Account_info account;
    struct Email_info email;
    struct Email_commands commands;
    const char smtp_server[] = "mail.smtp2go.com";  // hostname
    const int smtp_port = 2525;                     // port number
    char* encoded_username = NULL;
    char* encoded_password = NULL;
    int server_socket_fd = 0;

    // prompt user for username and password
    account = get_account_info(smtp_server, smtp_port);

    // encode username and password in base64
    encoded_username = base64_encode(account.username);
    encoded_password = base64_encode(account.password);

    // create tcp socket with smtp2go
    server_socket_fd = connect_to_server(smtp_server, smtp_port);

    // verify account info is valid
    authenticate_account(server_socket_fd, encoded_username, encoded_password);

    // get email info from user
    email = get_email_details();

    // format commands to send to server
    commands = format_commands(email);

    // send commands to server
    send_commands(server_socket_fd, commands);

    // close tcp socket with smtp2go
    close_connection(server_socket_fd);

    printf("\nProgram ended\n\n");
    return 0;
}
