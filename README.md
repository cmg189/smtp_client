## SMTP Client

An SMTP email client that utilizes [smtp2go](https://www.smtp2go.com/)


## Table of Contents

1. [Description](#description)
2. [Program Output](#program_output)
3. [Compile and Execute](#compile_and_execute)
4. [Function Headers](#function_headers)
5. [Structures](#structures)
6. [Other](#other)

## Description <a name="description"></a>

Simple Mail Transport Protocol (SMTP) is an email protocol used to send email messages.
This C based email client uses a TCP socket to connect to smtp2go.

Users can enter:
- Sender's name and email address
- Recipient's name and email address
- Email subject
- Email message

One email can be sent with each execution of this program.

**NOTE:**
You will need to make an account with smtp2go prior to using this program.

Once an account is made, add a SMTP User via the settings tab.

The username and password, used to create the user, will be the credentials you will use with the program.

## Program Output <a name="program_output"></a>

## Compile and Execute <a name="compile_and_execute"></a>

- Windows: 

    To compile run the command `g++ *.cpp -o email_client`

    To execute run the command `email_client.exe`

- Linux / Mac

    To compile run the command `g++ *.cpp -o email_client`

    To execute run the command `./email_client`


## Function Headers <a name="function_headers"></a>

``` c 
struct Account_info get_account_info(char smtp_server[], int smtp_port)
```

- Description:

    Get the smtp2go username and password from user

- Parameters:

    ``` c
    char smtp_server[] 
    ``` 
    represents hostname of smtp2go

    `int smtp_port` represents port number used to connect to smtp2go

- Return:

    `struct Account_info` struct containing username and password of smtp2go account

---

`int connect_to_server(char smtp_server[], int smtp_port)`

- Description:

    makes connection to the smtp2go server

- Parameters:

    `char smtp_server[]` represents the hostname of smtp2go

    `int smtp_port` represents the port number to connect to smtp2go

- Return:

    `int server_socket_fd` represents the file descriptor of the TCP socket made when connecting

---

`struct Email_info get_email_details()`

- Description:

    Gets all information required to send email

- Parameters:

    None

- Return:

    `struct Email_info` contains senders name and email address, recipients name and email address, email subject, and email body

---

`void close_connection(int sock_fd)`

- Description:

    Closes tcp socket to smtp2go

- Parameters:

    `int sock_fd` represents tcp socket file descriptor to smtp2go

- Return:

    None


## Structures <a name="structures"></a>

`struct Account_info`

- Description:

    Holds username and password for smtp2go account

- Variables:

    `char username[_1BYTE]` Username for smtp2go account

    `char password[_1KB]` Password associated with username for smtp2go account

---

`struct Email_info`

- Description:

    Holds information needed to send email

- Variables:

    `char senders_name[EMAIL_SIZE]` Name of whos sending the email

    `char recipients_name[EMAIL_SIZE]` Name of whos recieving the email

    `char source[EMAIL_SIZE]` Email address of sender

    `char destination[EMAIL_SIZE]` Email address of recipients

    `char subject[SUBJECT_SIZE]` Emails subject

    `char body[BODY_SIZE]]` Emails body

## Other <a name="other"></a>

`EXE_ARGS 3` number of arguments passed to program 

`EMAIL_SIZE 30` max number of characters for senders name and email address, recipients name and email address

`SUBJECT_SIZE 50` max number of characters for the subject of email

`BODY_SIZE 10000` max number of characters for the body of email