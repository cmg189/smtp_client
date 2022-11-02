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

    printf("\n\nAttempting to connect to the smtp2go server.....\n\n");

    // get IP address of smtp server
	struct hostent *server_info;
	server_info = gethostbyname(smtp_server);
	if(server_info == NULL){
		printf("\nServer address not found\n\n");
        printf("Program ended\n\n");
		exit(EXIT_FAILURE);
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

	// read response from server
	char response_buffer[_5BYTES];
	read(sock_fd, response_buffer, _5BYTES);
	//printf("\nResponse: %s", response_buffer);

	// clear string to reuse
	bzero(response_buffer, sizeof(response_buffer));

	// send EHLO command to server
	char server_command[] = "EHLO\n";
	write(sock_fd, server_command, strlen(server_command));

	// wait and read response from server
	sleep(1);
	read(sock_fd, response_buffer, _5BYTES);
	//printf("\nResponse: %s", response_buffer);

    // checking server response status for 250
    char check_status[5];
    char success[] = "250"; 
    strcpy(check_status, strtok(response_buffer, "-"));
    if( strcmp(check_status, success) == 0){
        printf("\nConnection to the smtp2go server: successful\n\n");
    }else{
        printf("\nConnection to the smtp2go server: failed\n\nProgram ended\n\n");
        exit(EXIT_FAILURE);
    }

	// clear string to reuse
	bzero(response_buffer, sizeof(response_buffer));

	return sock_fd;
}

char *base64_encode(char *data){
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

void authenticate_account(int sock_fd, char* encoded_username, char* encoded_password){
    char newline[] = "\n";

    // send AUTH LOGIN command to server
    char server_command[] = "AUTH LOGIN\n";
    write(sock_fd, server_command, strlen(server_command));

    // wait, read and output response from server
    char response_buffer[_5BYTES];
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    strcpy(response_buffer, strtok(response_buffer, "\n"));

    printf("\nResponse for AUTH: %s\n", response_buffer);

    // clear string to reuse
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded username to server
    strncat(encoded_username, newline, strlen(newline));

    write(sock_fd, encoded_username, strlen(encoded_username));
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    strcpy(response_buffer, strtok(response_buffer, "\n"));
    printf("\nResponse for username: %s\n", response_buffer);

    // clear string to reuse
    bzero(response_buffer, sizeof(response_buffer));

    // send encoded password to server
    strncat(encoded_password, newline, strlen(newline));

    write(sock_fd, encoded_password, strlen(encoded_password));
    sleep(1);
    read(sock_fd, response_buffer, _5BYTES);
    strcpy(response_buffer, strtok(response_buffer, "\n"));
    printf("\nResponse for password: %s\n", response_buffer);

    // clear string to reuse
    bzero(response_buffer, sizeof(response_buffer));
    return;
}