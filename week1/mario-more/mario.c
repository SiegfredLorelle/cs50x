// Asks for height & makes pyramids similar to the ones in Super Mario Bros.

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // i,j,k,l are counter variables
    int height, i, j, k, l;

    // User prompt for height
    do
    {
        height = get_int("How tall should the pyramid be?   ");
    }
    while (height > 8 || height <= 0);


    // Loop per horiz line
    for (i = 0; i < height; i++)
    {
        // Blank space in the left pyramid
        for (j = (height - i - 1); j > 0;  j--)
        {
            printf(" ");
        }

        // Blocks in the left pyramid
        for (k = 0; k <= i; k++)
        {
            printf("#");
        }

        // Double space between pyramids
        printf("  ");

        // Blocks in the right pyramid
        for (l = 0; l <= i;  l++)
        {
            printf("#");
        }

        // Go to next line
        printf("\n");

    }
}