// Asks for credit card number, checks its validity and brand

#include <cs50.h>
#include <stdio.h>

int multiply_every_other_digits(long current_number);
int break_into_digits(long current_number);
int get_unused_digits(long current_number);
long get_1st_2digits(long cc_number);
void find_type(int first_2_numbers, int first_number, int i);


int main(void)
{
    // i = num of digits
    long cc_number, first_number, last_number, current_number, first_2_numbers;
    int i, j, step1, step2, step3;

    // Prompt
    cc_number = get_long("Enter your credit card number:  ");

    // Counter variables
    step1 = 0;
    step2 = 0;

    // Loop through each digit
    first_number = cc_number;
    for (i = 1; first_number > 0; i++)
    {
        // Step1
        // Get every other value (later digit)
        if (0 == ((i) % 2))
        {
            // Save to diff var
            current_number = first_number;

            // Get every other digit then x2
            current_number = multiply_every_other_digits(current_number);

            // Break into digits in case more than 1 digit
            current_number = break_into_digits(current_number);

            // Summation of the digits
            step1 = step1 + current_number;
        }

        // Step2
        // Get unused value (later digit)
        else
        {
            // Save to diff var
            current_number = first_number;

            // Get unused digits
            current_number = get_unused_digits(current_number);

            // Summation of unused digits
            step2 = step2 + current_number;
        }

        // Limit excess dividing
        if (first_number < 10)
        {
            break;
        }

        // Go to next digit
        first_number = first_number / 10;
    }

    // Add the sum from step1 and step2
    step3 = step1 + step2;

    // Valid
    if (step3 % 10 == 0)
    {
        // Get 1st 2 digits
        first_2_numbers = get_1st_2digits(cc_number);

        // Find type of credit card
        find_type(first_2_numbers, first_number, i);
    }

    // Invalid
    else
    {
        printf("INVALID\n");
    }
}











// Functions

int multiply_every_other_digits(long current_number)
{
    current_number = (current_number % 10) * 2;
    return current_number;
}


int break_into_digits(long current_number)
{
    if (current_number >= 10)
    {
        if (current_number == 10)
        {
            current_number = 1;
        }

        else
        {
            // Take 1st digit
            int digit1 = current_number;
            for (int k = 0; digit1 > 10; k++)
            {
                digit1 = digit1 / 10;
            }

            // Take 2nd digit
            int digit2 = (current_number % 10);

            // Add both digits
            current_number = digit1 + digit2;
        }
    }
    return current_number;
}


int get_unused_digits(long current_number)
{
    current_number = (current_number % 10);
    return current_number;
}


long get_1st_2digits(long cc_number)
{
    long first_2_numbers = cc_number;
    while (first_2_numbers > 100)
    {
        first_2_numbers = first_2_numbers / 10;
    }
    return first_2_numbers;
}


void find_type(int first_2_numbers, int first_number, int i)
{

    if (i == 15)
    {
        if (first_2_numbers == 34 || first_2_numbers == 37)
        {
            printf("AMEX\n");
        }


        else
        {
            printf("INVALID\n");
        }
    }

    else if (i == 16)
    {
        if (first_2_numbers == 51 || first_2_numbers == 52 ||
            first_2_numbers == 53 || first_2_numbers == 54 || first_2_numbers == 55)
        {
            printf("MASTERCARD\n");
        }
        else if (first_number == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }

    }
    else if (i == 13)
    {
        if (first_number == 4)
        {
            printf("VISA\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}
