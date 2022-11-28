#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define EMAIL_SIZE 30
#define SUBJECT_SIZE 50
#define BODY_SIZE 10000
#define _5BYTES 5120
#define _1BYTE 1024
#define _1KB 8192

// holds smtp2go username and password
struct Account_info{
    char username[_1BYTE];
    char password[_1KB];
};

// holds details of email from user
struct Email_info{
    char senders_name[EMAIL_SIZE];
    char recipients_name[EMAIL_SIZE];
    char source[EMAIL_SIZE];
    char destination[EMAIL_SIZE];
    char subject[SUBJECT_SIZE];
    char body[BODY_SIZE];
};

// holds email commands to send to server
struct Email_commands{
    char from_email[EMAIL_SIZE];
    char to_email[EMAIL_SIZE];
    char data[EMAIL_SIZE];
    char from_name[EMAIL_SIZE];
    char to_name[EMAIL_SIZE];
    char subject_line[SUBJECT_SIZE];
    char body_message[BODY_SIZE];
};

// get account info user
struct Account_info get_account_info(char smtp_server[], int smtp_port);

// create tcp socket with smtp2go
int connect_to_server(char smtp_server[], int smtp_port);

// encode data to base64
char *base64_encode(char* data);

// check to see if smtp2go account is valid
void authenticate_account(int sock_fd, char* encoded_username, char* encoded_password);

// get email details from user
struct Email_info get_email_details();

// format email details into commands
struct Email_commands format_commands(struct Email_info info);

// send commands to server
void send_commands(struct Email_commands commands);

// close tcp socket with smtp2go
void close_connection(int sock_fd);
