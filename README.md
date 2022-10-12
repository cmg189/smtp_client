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

    To execute run the command `./email_client your@email.com your_passward`

    **NOTE:** `your@email.com` and `your_password`  are the email address and password associated with your smtp2go account.

### Function Headers

### Structures 