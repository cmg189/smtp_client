#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define EXE_ARGS 3
#define EMAIL_SIZE 30
#define SUBJECT_SIZE 50
#define BODY_SIZE 10000
#define _5BYTES 5120
#define _1BYTE 1024
#define _1KB 8192

// holds details of email from user
struct Email_info{
    char senders_name[EMAIL_SIZE];
    char recipients_name[EMAIL_SIZE];
    char source[EMAIL_SIZE];
    char destination[EMAIL_SIZE];
    char subject[SUBJECT_SIZE];
    char body[BODY_SIZE];
};

// check if program is executed with email and password
void check_exe(int num_args);

// get email details from user 
struct Email_info get_email_details();

// connect to smtp2go server
int connect_to_server(char smtp_server[], int smtp_port);