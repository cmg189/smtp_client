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
