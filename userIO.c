#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userIO.h"
#define MAXLEN 30
typedef char String[MAXLEN];
void print_menu()
{
    printf("What would you like to do \n");
    printf("\t1. Add a name(s) to the Secret Santa list\n");
    printf("\t2. Draw the names\n");
    printf("\t3. View the names\n");
    printf("\t4. Quit application\n");
}

int user_choice(int maxChoice)
{
    char *end = NULL;
    char buf[3];
    long n = 0;
    printf("Enter an integer:\n");
    while (fgets(buf, sizeof(buf), stdin))
    {
        n = strtol(buf, &end, 10);
        if (end == buf || *end != '\n')
        {
            printf("Not recognised as an integer. Please enter an integer:\n");
        }
        else if (n > maxChoice || n < 1)
        {
            printf("That number is not a option. Please enter one of the options:\n");
        }
        else
        {
            
            break;
        }
    }
    buf[strlen(buf) - 1] = '\0';
    // printf("You entered %d\n", n); //use for testing
    return n;
}

int get_user_string(String input)
{
        printf("Please enter a name: ");
        fgets(input, MAXLEN, stdin);

        // Ensures the last line is is set to a end lin
        input[strlen(input) - 1] = '\0';

        printf("I read this: '%s'\n", input);
    return 0;
}
