## SMTP Client

This program implements an email client in connection with [smtp2go](https://www.smtp2go.com/)

### Description

Simple Mail Transport Protocol (SMTP) is an email protocol used to send email messages.
This C based email client uses a TCP socket to connect to smtp2go.

Users can enter:
- Sender's name and email address
- Recipient's name and email address
- Email subject
- Email message

One email can be sent with each execution of this program.

You will need to make an account with smtp2go prior to using this program.

### Program Output

### Compile and Execute

- Windows: 

    To compile run the command `g++ *.cpp -o email_client`

    To execute run the command `email_client.exe your@email.com your_password`

- Linux / Mac

    To compile run the command `g++ *.cpp -o email_client`

    To execute run the command `./email_client your@email.com your_password`

    **NOTE:** `your@email.com` and `your_password`  are the email address and password associated with your smtp2go account.

### Function Headers

`void check_exe(int num_args)`
- Description:
    Checks number of arguments passed upon execution to ensure program is ran correctly. If user does not execute with 2 arguments (email and password) program will terminate

- Parameters:
    `int num_args` an int representing number of arguments passed when program is executed

- Return:

    None

---

`struct Email_info get_email_details()`
- Description:
    Gets all information required to send email

- Parameters:
    None

- Return:
    `struct Email_info` contains senders name and email address, recipients name and email address, email subject, and email body



### Structures

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

### Other

`EXE_ARGS 3` number of arguments passed to program 

`EMAIL_SIZE 30` max number of characters for senders name and email address, recipients name and email address

`SUBJECT_SIZE 50` max number of characters for the subject of email

`BODY_SIZE 10000` max number of characters for the body of email