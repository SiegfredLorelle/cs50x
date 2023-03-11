// The program prompts the user for a name and greets them

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompts for a name
    string so_and_so = get_string("Please tell me your name:   ");
    // Prints hello, <name>
    printf("hello, %s\n", so_and_so);
}