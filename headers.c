#include "headers.h"

// get account info from user
struct Account_info get_account_info(char smtp_server[], int smtp_port){
    struct Account_info info;

    printf("\n\n\t\t\t\tSMTP Client\n\n\n");
    printf("smtp2go's server can be found at: %s on port: %d\n\n", smtp_server, smtp_port);
    printf("\nEnter your smtp2go username: ");
    scanf("%[^\n]%*c", info.username);
    printf("\n\nEnter the password associated with your username: ");
    scanf("%[^\n]%*c", info.password);

    return info;
}

// establish connection to server
int connect_to_server(char smtp_server[], int smtp_port){

  printf("\n\nAttempting to connect to the smtp2go server.....\n\n");

  // get IP address of smtp server
	struct hostent *server_info;
	server_info = gethostbyname(smtp_server);
	if(server_info == NULL){
		printf("\nServer address not found\n\n");
    printf("Program ended\n\n");
		exit(EXIT_FAILURE);
	}

	// convert IP address into string and create TCP socket
	char *server_ip = inet_ntoa( *((struct in_addr*)server_info->h_addr_list[0]) );
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	// initalize TCP socket
	struct sockaddr_in server_socket;
	bzero(&server_socket, sizeof(server_socket));
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons((unsigned short)smtp_port);

	// convert TCP socket to binary
	inet_pton(AF_INET, server_ip, &(server_socket.sin_addr));

	// connect to server
	connect(sock_fd, (struct sockaddr *)&server_socket, sizeof(server_socket));

	// read response from server
	char response_buffer[_5BYTES];
	read(sock_fd, response_buffer, _5BYTES);
	bzero(response_buffer, sizeof(response_buffer));

	// send EHLO command to server
	char server_command[] = "EHLO\n";
	write(sock_fd, server_command, strlen(server_command));

	// wait and read response from server
	sleep(1);
	read(sock_fd, response_buffer, _5BYTES);

  // checking server response status for 250
  char check_status[5];
  char success[] = "250";
  strcpy(check_status, strtok(response_buffer, "-"));
  if( strcmp(check_status, success) == 0){
    printf("\nConnection to the smtp2go server: successful\n\n");
  }else{
    printf("\nConnection to the smtp2go server: failed\n\nProgram ended\n\n");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }
	bzero(response_buffer, sizeof(response_buffer));

	return sock_fd;
}

// encode username and password
char *base64_encode(char *data){
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

    for (int i = 0, j = 0; i < input_length;) {

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
        encoded_data[output_length - 1 - i] = '=';

    return encoded_data;
}

// send username and password to server
void authenticate_account(int sock_fd, char* encoded_username, char* encoded_password){
    char newline[] = "\n";

    // send AUTH LOGIN command to server
    char server_command[] = "AUTH LOGIN\n";
    write(sock_fd, server_command, strlen(server_command));

    // wait and read response from server
    char response_buffer[_5BYTES];
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded username to server
    strncat(encoded_username, newline, strlen(newline));

    printf("\nAuthenticating username and password.....\n\n");

    write(sock_fd, encoded_username, strlen(encoded_username));
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded password to server
    strncat(encoded_password, newline, strlen(newline));

    write(sock_fd, encoded_password, strlen(encoded_password));
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    strcpy(response_buffer, strtok(response_buffer, "\n"));

    // checking server response for email and password authentication
    char check_status[5];
    char auth_success[] = "235";
    strcpy(check_status, strtok(response_buffer, " "));
    if( strcmp(check_status, auth_success) == 0){
        printf("\nAuthentication successful\n\n");
    }else{
        printf("\nAuthentication failed\n\nEnsure username and password are entered correctly\n\nProgram ended\n\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    bzero(response_buffer, sizeof(response_buffer));

    return;
}

// get email details from user
struct Email_info get_email_details(){
    struct Email_info email;

    printf("\n\tProvide the details for the email you are sending\n\n\n");

    printf("Sender's name: ");
    scanf("%[^\n]%*c", email.senders_name);

    printf("\nSender's email address: ");
    scanf("%[^\n]%*c", email.source_email);

    printf("\nRecipient's name: ");
    scanf("%[^\n]%*c", email.recipients_name);

    printf("\nRecipient's email address: ");
    scanf("%[^\n]%*c", email.destination_email);

    printf("\nEmail subject: ");
    scanf("%[^\n]%*c", email.subject);

    printf("\nEmail body:\n");
    scanf("%[^\n]%*c", email.body);

    return email;
}

// format email details into commands
struct Email_commands format_commands(struct Email_info info){
  struct Email_commands commands;

	strcat(commands.from_email, "MAIL FROM: <");
	strcat(commands.from_email, info.source_email);
	strcat(commands.from_email, ">\n");

	strcat(commands.to_email, "RCPT TO: <");
	strcat(commands.to_email, info.destination_email);
	strcat(commands.to_email, ">\n");

	strcat(commands.data, "DATA\n");

	strcat(commands.from_name, "FROM: ");
	strcat(commands.from_name, info.senders_name);
	strcat(commands.from_name, " <");
	strcat(commands.from_name, info.source_email);
	strcat(commands.from_name, ">\n");

	strcat(commands.to_name, "TO: ");
	strcat(commands.to_name, info.recipients_name);
	strcat(commands.to_name, " <");
	strcat(commands.to_name, info.destination_email);
	strcat(commands.to_name, ">\n");

	strcat(commands.subject, "SUBJECT: ");
	strcat(commands.subject, info.subject);
	strcat(commands.subject, "\n");

	strcat(commands.body, info.body);
	strcat(commands.body, "\n.\n");

  return commands;
}

// send commands to server
void send_commands(int sock_fd, struct Email_commands commands){
	printf("\n\nSending email through SMTP2GO.....\n");
	char server_response[_1BYTE];
	char compare_buffer[_1BYTE];
	char email_id[_1BYTE];

	write(sock_fd, commands.from_email, strlen(commands.from_email));
	sleep(1);
	read(sock_fd, server_response, _1BYTE);

	strcpy(compare_buffer, strtok(server_response, " "));
	if(strcmp(compare_buffer, "250") != 0){
		printf("\nError when sending \"MAIL FROM:\" command\n\n");
		printf("Server's response: %s\n\n", server_response);
		printf("Try entering a valid source email address\n\nCanceling email transmission.....");
		return;
	}
	bzero(server_response, _1BYTE);
	bzero(compare_buffer, _1BYTE);

	write(sock_fd, commands.to_email, strlen(commands.to_email));
	sleep(1);
	read(sock_fd, server_response, _1BYTE);

	strcpy(compare_buffer, strtok(server_response, " "));
	if(strcmp(compare_buffer, "250") != 0){
		printf("\nError when sending \"RCPT TO:\" command\n\n");
		printf("Server's response: %s\n\n", server_response);
		printf("Try entering a valid destination email address\n\nCanceling email transmission.....");
		return;
	}
	bzero(server_response, _1BYTE);
	bzero(compare_buffer, _1BYTE);

	write(sock_fd, commands.data, strlen(commands.data));
	sleep(1);
	read(sock_fd, server_response, _1BYTE);

	strcpy(compare_buffer, strtok(server_response, " "));
	if(strcmp(compare_buffer, "354") != 0){
		printf("\nError when sending \"DATA\" command\n\n");
		printf("Server's response: %s\n\n", server_response);
		printf("Try entering the email details again\n\nCanceling email transmission.....");
		return;
	}
	bzero(server_response, _1BYTE);
	bzero(compare_buffer, _1BYTE);

	write(sock_fd, commands.from_name, strlen(commands.from_name));
	sleep(1);

	write(sock_fd, commands.to_name, strlen(commands.to_name));
	sleep(1);

	write(sock_fd, commands.subject, strlen(commands.subject));
	sleep(1);

	write(sock_fd, commands.body, strlen(commands.body));
	sleep(1);
	read(sock_fd, server_response, _1BYTE);

	strcpy(compare_buffer, strtok(server_response, " "));
	if(strcmp(compare_buffer, "250") != 0){
		printf("\nError when sending email's body\n\n");
		printf("Server's response: %s\n\n", server_response);
		printf("Try entering the email details again\n\nCanceling email transmission.....");
		return;
	}
	// get the transmission ID from server's response
	strcpy(compare_buffer, strtok(NULL, "="));
	strcpy(email_id, strtok(NULL, "\n"));

	printf("\n\nEmail has been successfully sent\n");
	printf("\n\nYour email's transmission ID is: %s\n\n", email_id);
	bzero(server_response, _1BYTE);
	bzero(compare_buffer, _1BYTE);
	bzero(email_id, _1BYTE);

  return;
}

// close tcp socket with smtp2go
void close_connection(int sock_fd){
	printf("\nClosing connection with smtp2go.....\n");

	// send quit command to server
	char quit_command[] = "quit\n";
	write(sock_fd, quit_command, strlen(quit_command));

	// wait and read response
	char response_buffer[_1BYTE];
	sleep(1);
	read(sock_fd, response_buffer, _1BYTE);

	// close connection to smtp2go
	close(sock_fd);

	printf("\n\nConnection closed\n\n");

	return;
}
