#include "headers.h"


// check if program is executed with email and password
void check_exe(int num_args){

    if(num_args != EXE_ARGS){
        printf("\nUsage: email_client your@email.com password\n\nProgram ended\n\n");
        exit(EXIT_FAILURE);
    }

    return;
}

// get email details from user
struct Email_info get_email_details(){
    struct Email_info email; 

    printf("\nProvide the details for the email you are sending\n\n");

    printf("Sender's name: ");
    scanf("%[^\n]%*c", email.senders_name);

    printf("Sender's email address: ");
    scanf("%[^\n]%*c", email.source);

    printf("\nRecipient's name: ");
    scanf("%[^\n]%*c", email.recipients_name);

    printf("Recipient's email address: ");
    scanf("%[^\n]%*c", email.destination);

    printf("\nEmail subject: ");
    scanf("%[^\n]%*c", email.subject);

    printf("Email body:\n");
    scanf("%[^\n]%*c", email.body);

    return email;
}

int connect_to_server(char smtp_server[], int smtp_port){
    // get IP address of smtp server
	struct hostent *server_info;
	server_info = gethostbyname(smtp_server);
	if(server_info == NULL){
		printf("\nServer address not found\n\n");
		exit(0);
	}

	// convert IP address into string
	char *server_ip = inet_ntoa( *((struct in_addr*)server_info->h_addr_list[0]) );
    // create TCP socket
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	// initalize server socket
	struct sockaddr_in server_socket;
	bzero(&server_socket, sizeof(server_socket));
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons((unsigned short)smtp_port);

	// convert server socket to binary
	inet_pton(AF_INET, server_ip, &(server_socket.sin_addr));

	// connect to server
	connect(sock_fd, (struct sockaddr *)&server_socket, sizeof(server_socket));

	// read and output response from server
	char response_buffer[_5BYTES];
	read(sock_fd, response_buffer, _5BYTES);
	printf("\nResponse: %s", response_buffer);

	// clear string to reuse
	bzero(response_buffer, sizeof(response_buffer));

	// send EHLO command to server
	char server_command[] = "EHLO\n";
	write(sock_fd, server_command, strlen(server_command));

	// wait, read and output response from server
	sleep(1);
	read(sock_fd, response_buffer, _5BYTES);
	printf("\nResponse: %s", response_buffer);

	// clear string to reuse
	bzero(response_buffer, sizeof(response_buffer));

	return sock_fd;
}