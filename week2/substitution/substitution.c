#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// Checks for validity of key
int validity_check(int n_words, string key);

// Asks for a key and a text, encrypts the text to the its equivalent in the key
int main(int argc, string argv[])
{
    // Save to var for simplier name
    int n_words = argc;
    string key = argv[1];

    // Checks validity of key
    int validity = validity_check(n_words, key);

    // Valid key
    if (validity == 1)
    {
        // Prompt for text
        string plaintext = get_string("plaintext:  ");
        printf("ciphertext: ");

        // Loop through each char of text
        int i, length;
        for (i = 0, length = strlen(plaintext); i < length; i++)
        {
            // Valid letter
            if ((plaintext[i] >= 65 && plaintext[i] <= 90) || (plaintext[i] >= 97 && plaintext[i] <= 122))
            {
                // Encrypt based on key
                if (islower(plaintext[i]))
                {
                    printf("%c", tolower(key[plaintext[i] - 'a']));
                }
                else if (isupper(plaintext[i]))
                {
                    printf("%c", toupper(key[plaintext[i] - 'A']));
                }

            }
            // Not valid letter
            else
            {
                printf("%c", plaintext[i]);
            }

        }
        printf("\n");
        return 0;
    }

    // Invalid key
    else
    {
        return 1;
    }
}



// Checks for the validity of key
int validity_check(int n_words, string key)
{
    // Correct # command-line arguement
    if (n_words == 2)
    {
        // Loop through each char of key
        int i = 0;
        while (key[i])
        {
            int j = 0, k = 0;

            // Valid letter
            if (tolower(key[i]) >= 97 && tolower(key[i]) <= 122)
            {
                // Loop through each char of key
                while (key[j])
                {
                    // Checks for repeat letters in key
                    if (tolower(key[i]) == tolower(key[j]))
                    {
                        k++;
                        // Repeat letters
                        if (k > 1)
                        {
                            printf("Each letter of the alphabet must be used once.\n");
                            return 0;
                        }
                    }
                    j++;
                }
                i++;
            }
            // Not a letter
            else
            {
                printf("All letters of keys must be from the English alphabet\n");
                return 0;
            }
        }

        // Wrong # of char
        if (i != 26)
        {
            printf("Key must contain 26 characters\n");
            return 0;
        }
        // 26 chars
        else
        {
            return 1;
        }

    }
    // Incorrect # of command-line arguement
    else
    {
        printf("USAGE: ./substitution key\n");
        return 0;
    }
    return 0;
}