#include "headers.h"

// get account info from user
struct Account_info get_account_info(const char *smtp_server, const int smtp_port)
{
    struct Account_info info;

    printf("\n\n\t\t\t\tSMTP Client\n\n\n");
    printf("smtp2go's server can be found at: %s on port: %d\n\n", smtp_server, smtp_port);
    printf("\nEnter your smtp2go username: ");
    fgets(info.username, sizeof(info.username), stdin);

    printf("\n\nEnter the password associated with your username: ");
    fgets(info.password, sizeof(info.password), stdin);

    return info;
}

// establish connection to server
int connect_to_server(const char *smtp_server, const int smtp_port)
{
    struct hostent *server_info = NULL;
    struct sockaddr_in server_socket;
    const char server_command[] = "EHLO\n";
    const char success[] = "250";             // response status
    char response_buffer[BODY_SIZE] = { 0 };
    char *server_ip = NULL;
    char check_status[8] = { 0 };
    int sock_fd = 0;

    printf("\n\nAttempting to connect to the smtp2go server.....\n\n");

    // get IP address of smtp server
    server_info = gethostbyname(smtp_server);
    if(!server_info)
    {
        printf("\nServer address not found\n\n");
        printf("Program ended\n\n");
        exit(EXIT_FAILURE);
    }

    // convert IP address into string and create TCP socket
    server_ip = inet_ntoa( *((struct in_addr*)server_info->h_addr_list[0]) );
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // initalize TCP socket
    bzero(&server_socket, sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons((unsigned short)smtp_port);

    // convert TCP socket to binary
    inet_pton(AF_INET, server_ip, &(server_socket.sin_addr));

    // connect to server
    connect(sock_fd, (struct sockaddr *)&server_socket, sizeof(server_socket));

    // read response from server
    read(sock_fd, response_buffer, sizeof(response_buffer));
    bzero(response_buffer, sizeof(response_buffer));

    // send EHLO command to server
    write(sock_fd, server_command, strlen(server_command));

    // wait and read response from server
    sleep(1);
    read(sock_fd, response_buffer, sizeof(response_buffer));

    // checking server response status for 250
    strncpy(check_status, strtok(response_buffer, "-"), sizeof(check_status));
    if( strcmp(check_status, success) )
    {
        printf("\nConnection to the smtp2go server: failed\n\nProgram ended\n\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    bzero(response_buffer, sizeof(response_buffer));

    printf("\nConnection to the smtp2go server: successful\n\n");
    return sock_fd;
}

// encode username and password
char *base64_encode(char *data)
{
    //
    // special thanks to the user who suggested this function for base64 encoding
    //	https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
    //
    static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
    static char *decoding_table = NULL;
    static int mod_table[] = {0, 2, 1};

    size_t input_length = strlen(data);

    unsigned long int output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;)
    {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
    {
        encoded_data[output_length - 1 - i] = '=';
    }

    return encoded_data;
}

// send username and password to server
void authenticate_account(int sock_fd, char* encoded_username, char* encoded_password)
{
    const char server_command[] = "AUTH LOGIN\n";
    const char auth_success[] = "235";
    const char newline[] = "\n";
    char response_buffer[BODY_SIZE] = { 0 };
    char check_status[8] = { 0 };

    // send AUTH LOGIN command to server
    write(sock_fd, server_command, strlen(server_command));

    // wait and read response from server
    sleep(1);
    read(sock_fd, response_buffer, sizeof(response_buffer));
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded username to server
    strncat(encoded_username, newline, strlen(newline));

    printf("\nAuthenticating username and password.....\n\n");

    write(sock_fd, encoded_username, strlen(encoded_username));
    sleep(1);
    read(sock_fd, response_buffer, sizeof(response_buffer));
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded password to server
    strncat(encoded_password, newline, strlen(newline));

    write(sock_fd, encoded_password, strlen(encoded_password));
    sleep(1);
    read(sock_fd, response_buffer, sizeof(response_buffer));
    strncpy(response_buffer, strtok(response_buffer, "\n"), sizeof(response_buffer));

    // checking server response for email and password authentication
    strncpy(check_status, strtok(response_buffer, " "), sizeof(check_status));
    if( strcmp(check_status, auth_success) )
    {
        printf("\nAuthentication failed\n\nEnsure username and password are entered correctly\n\nProgram ended\n\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    bzero(response_buffer, sizeof(response_buffer));

    printf("\nAuthentication successful\n\n");
    return;
}

// get email details from user
struct Email_info get_email_details()
{
    struct Email_info email;

    printf("\n\tProvide the details for the email you are sending\n\n\n");

    printf("Sender's name: ");
    fgets(email.senders_name, sizeof(email.senders_name), stdin);

    printf("\nSender's email address: ");
    fgets(email.senders_name, sizeof(email.senders_name), stdin);

    printf("\nRecipient's name: ");
    fgets(email.recipients_name, sizeof(email.recipients_name), stdin);

    printf("\nRecipient's email address: ");
    fgets(email.destination_email, sizeof(email.destination_email), stdin);

    printf("\nEmail subject: ");
    fgets(email.subject, sizeof(email.subject), stdin);

    printf("\nEmail body:\n");
    fgets(email.body, sizeof(email.body), stdin);

    return email;
}

// format email details into commands
struct Email_commands format_commands(struct Email_info info)
{
    struct Email_commands commands;

    strncpy(commands.from_email, "MAIL FROM: <", (sizeof(commands.from_email) -1));
    strncat(commands.from_email, info.source_email, (sizeof(commands.from_email) - strlen(commands.from_email) -1));
    strncat(commands.from_email, ">\n", (sizeof(commands.from_email) - strlen(commands.from_email) -1));

    strncpy(commands.to_email, "RCPT TO: <", (sizeof(commands.to_email) - 1));
    strncat(commands.to_email, info.destination_email, (sizeof(commands.to_email) - strlen(commands.to_email) -1));
    strncat(commands.to_email, ">\n", (sizeof(commands.to_email) - strlen(commands.to_email) -1));

    strncpy(commands.data, "DATA\n", sizeof(commands.data) -1);

    strncpy(commands.from_name, "FROM: ", sizeof(commands.from_name) -1);
    strncat(commands.from_name, info.senders_name, (sizeof(commands.from_name) - strlen(commands.from_name) -1));
    strncat(commands.from_name, " <", (sizeof(commands.from_name) - strlen(commands.from_name) -1));
    strncat(commands.from_name, info.source_email, (sizeof(commands.from_name) - strlen(commands.from_name) -1));
    strncat(commands.from_name, ">\n", (sizeof(commands.from_name) - strlen(commands.from_name) -1));

    strncpy(commands.to_name, "TO: ", (sizeof(commands.to_name) -1));
    strncat(commands.to_name, info.recipients_name, (sizeof(commands.to_name) - strlen(commands.to_name) -1));
    strncat(commands.to_name, " <", (sizeof(commands.to_name) - strlen(commands.to_name) -1));
    strncat(commands.to_name, info.destination_email, (sizeof(commands.to_name) - strlen(commands.to_name) -1));
    strncat(commands.to_name, ">\n", (sizeof(commands.to_name) - strlen(commands.to_name) -1));

    strncpy(commands.subject, "SUBJECT: ", (sizeof(commands.subject) -1));
    strncat(commands.subject, info.subject, (sizeof(commands.subject) - strlen(commands.subject) -1));
    strncat(commands.subject, "\n", (sizeof(commands.subject) - strlen(commands.subject) - 1));

    strncpy(commands.body, info.body, (sizeof(commands.body) -1));
    strncat(commands.body, "\n.\n", (sizeof(commands.body), strlen(commands.body -1)));

    return commands;
}

// send commands to server
void send_commands(int sock_fd, struct Email_commands commands)
{
    char server_response[BODY_SIZE] = { 0 };
    char compare_buffer[BODY_SIZE] = { 0 };
    char email_id[BODY_SIZE] = { 0 };

    printf("\n\nSending email through SMTP2GO.....\n");

    write(sock_fd, commands.from_email, strlen(commands.from_email));
    sleep(1);
    read(sock_fd, server_response, BODY_SIZE);

    strncpy(compare_buffer, strtok(server_response, " "), (sizeof(compare_buffer) -1));
    if( strcmp(compare_buffer, "250") )
    {
        printf("\nError when sending \"MAIL FROM:\" command\n\n");
        printf("Server's response: %s\n\n", server_response);
        printf("Try entering a valid source email address\n\nCanceling email transmission.....");
        return;
    }
    bzero(server_response, BODY_SIZE);
    bzero(compare_buffer, BODY_SIZE);

    write(sock_fd, commands.to_email, strlen(commands.to_email));
    sleep(1);
    read(sock_fd, server_response, sizeof(server_response));

    strncpy(compare_buffer, strtok(server_response, " "), (sizeof(compare_buffer) -1));
    if( strcmp(compare_buffer, "250") )
    {
        printf("\nError when sending \"RCPT TO:\" command\n\n");
        printf("Server's response: %s\n\n", server_response);
        printf("Try entering a valid destination email address\n\nCanceling email transmission.....");
        return;
    }
    bzero(server_response, sizeof(server_response));
    bzero(compare_buffer, sizeof(compare_buffer));

    write(sock_fd, commands.data, strlen(commands.data));
    sleep(1);
    read(sock_fd, server_response, sizeof(server_response));

    strncpy(compare_buffer, strtok(server_response, " "), (sizeof(compare_buffer) -1));
    if( strcmp(compare_buffer, "354") )
    {
        printf("\nError when sending \"DATA\" command\n\n");
        printf("Server's response: %s\n\n", server_response);
        printf("Try entering the email details again\n\nCanceling email transmission.....");
        return;
    }
    bzero(server_response, sizeof(server_response));
    bzero(compare_buffer, sizeof(compare_buffer));

    write(sock_fd, commands.from_name, strlen(commands.from_name));
    sleep(1);

    write(sock_fd, commands.to_name, strlen(commands.to_name));
    sleep(1);

    write(sock_fd, commands.subject, strlen(commands.subject));
    sleep(1);

    write(sock_fd, commands.body, strlen(commands.body));
    sleep(1);

    read(sock_fd, server_response, sizeof(server_response));

    strncpy(compare_buffer, strtok(server_response, " "), (sizeof(compare_buffer) -1));
    if( strcmp(compare_buffer, "250") )
    {
        printf("\nError when sending email's body\n\n");
        printf("Server's response: %s\n\n", server_response);
        printf("Try entering the email details again\n\nCanceling email transmission.....");
        return;
    }

    // get the transmission ID from server's response
    strncpy(compare_buffer, strtok(NULL, "="), (sizeof(compare_buffer) -1));
    strncpy(email_id, strtok(NULL, "\n"), (sizeof(email_id) -1));

    printf("\n\nEmail has been successfully sent\n");
    printf("\n\nYour email's transmission ID is: %s\n\n", email_id);

    bzero(server_response, sizeof(server_response));
    bzero(compare_buffer, sizeof(compare_buffer));
    bzero(email_id, sizeof(email_id));
    return;
}

// close tcp socket with smtp2go
void close_connection(int sock_fd)
{
    const char quit_command[] = "quit\n";
    char response_buffer[BODY_SIZE] = { 0 };

    printf("\nClosing connection with smtp2go.....\n");

    // send quit command to server
    write(sock_fd, quit_command, strlen(quit_command));

    // wait and read response
    sleep(1);
    read(sock_fd, response_buffer, sizeof(response_buffer));

    // close connection to smtp2go
    close(sock_fd);

    printf("\n\nConnection closed\n\n");
    return;
}
