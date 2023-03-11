// Program that recovers deleted jpg files of a memory
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Check if command-line arguements (user input) are correct
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open the memory
    FILE *file = fopen(argv[1], "r");

    // Check if the memory is a empty
    if (file == NULL)
    {
        printf("File could not be opened\n");
        return 2;
    }

    // Block size in the memory
    const int BLOCK_SIZE = 512;

    // Memory location array (pointer)
    uint8_t buffer[BLOCK_SIZE];

    // Variables
    int jpg_counter = 0;
    char filename[8];
    FILE *img;

    // Loop until the whole memory is read
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If the header represents the start of a JPG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            // Close the jpg file if this is not the 1st jpg file
            if (jpg_counter != 0)
            {
                fclose(img);
            }

            // Name the new jpg file then open it
            sprintf(filename, "%03i.jpg", jpg_counter);
            img = fopen(filename, "w");

            // Copy the bytes in the memory to the new jpg
            fwrite(buffer, 1, BLOCK_SIZE, img);
            jpg_counter++;

        }
        // If it is not the header file of a JPG
        else if (jpg_counter != 0)
        {
            // Copy the bytes in the memory to the new jpg
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }
    // Close the last jpg file
    fclose(img);
    return 0;
}