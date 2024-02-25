#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

#define MAX_MSG_LENGTH 1000 //maximum length for a message
#define MAX_USERNAME_LENGTH 50 //maximum length for a username

#define SYSCALL_SEND_MSG 441 //Systemcall number for sending
#define SYSCALL_GET_MSG 442 //Systemcall number for receiving

//send a message using the syscall
int send_msg(const char *to, const char *msg, const char *from) {
    return syscall(SYSCALL_SEND_MSG, to, msg, from);
}

//receive  a message using the syscall
int get_msg(const char *to, char *msg, char *from) {
    return syscall(SYSCALL_GET_MSG, to, msg, from);
}

// Prints the usage instructions
void print_usage(void) {
    printf("Usage:\n");
    printf("  osmsg -s [recipient] [\"message\"] - Send a message\n");
    printf("  osmsg -r - Receive messages\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return -1;
    }

    //sending messages
    if (!strcmp(argv[1], "-s") && argc == 4) {
        if (strlen(argv[2]) > MAX_USERNAME_LENGTH || strlen(argv[3]) > MAX_MSG_LENGTH) {
            printf("Error: Username or message exceeds maximum allowed length.\n");
            return -1;
        }
        int status = send_msg(argv[2], argv[3], getenv("USER"));
        if (status == 0) {
            printf("Message sent successfully.\n");
        } else {
            printf("Failed to send message.\n");
        }
    }
    //receiving messages
    else if (!strcmp(argv[1], "-r") && argc == 2) {
        char *msg = malloc(MAX_MSG_LENGTH + 1); //memory for the message
        char *from = malloc(MAX_USERNAME_LENGTH + 1); //memory for the sender's username
        int status;
        while ((status = get_msg(getenv("USER"), msg, from)) != -1) { //attempt to receive messages
            if (status >= 0) {
                printf("%s said: \"%s\"\n", from, msg); 
            } else {
                printf("No messages found.\n");
            }
            break;
        }
        //free allocated memory
        free(msg); 
        free(from);
    } else {
        print_usage();
    }
    return 0;
}

