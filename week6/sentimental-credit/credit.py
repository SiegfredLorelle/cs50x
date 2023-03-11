# Prompt for a credit card number,
# and it'll print what card it is, assuming it is valid according to Luhn's Algo

# TODO
import cs50
import sys


def main():
    # Get the card number
    card = cs50.get_int("Number:  ")
    # Apply Luhn's Algo, return the card's validity, no. of its digits, first and first 2 digits of the card
    isValid, no_digits, first_digit, first_2digits = Luhns_Algo(card)

    if isValid:
        # Check what card it is
        check_card(no_digits, first_digit, first_2digits)
    else:
        print("INVALID")
        sys.exit(0)


def Luhns_Algo(number):
    current_digit = number
    counter = 0
    sum = 0

    # Loop until all digits were passed
    while number > 0:
        # Get the last digit as current digit
        current_digit = number % 10
        # Step1 or 2 of Luhn's Algo depending on the position of the digit
        if (counter % 2) == 0:
            sum = Luhns_step2(current_digit, sum)
        else:
            sum = Luhns_step1(current_digit, sum)

        # Get the first and first 2 digits of the card
        if number < 10:
            first_digit = number
        elif number < 100:
            first_2digits = number

        # Go to the next digit, and increment the digit counter
        number //= 10
        counter += 1

    # Step3 of Luhn's Algo, return needed values
    if sum % 10 == 0:
        return True, counter, first_digit, first_2digits
    else:
        return False, counter, first_digit, first_2digits


def Luhns_step1(digit, sum):
    # Multiply the digit by 2
    digit *= 2
    # If the product has 2 digits, break them down and add to the sum
    if (digit >= 10):
        sum += (digit % 10) + (digit // 10)
    else:
        sum += digit
    return sum


def Luhns_step2(digit, sum):
    # Add the digit to the sum
    sum += digit
    return sum


def check_card(no_digits, first_digit, first_2digits):
    # Determine what card it is
    if no_digits == 15 and (first_2digits in [34, 37]):
        print("AMEX")

    elif no_digits == 16 and (first_2digits in [51, 52, 53, 54, 55]):
        print("MASTERCARD")

    elif no_digits in [13, 16] and first_digit == 4:
        print("VISA")
    else:
        print("INVALID")
        sys.exit(0)


if __name__ == "__main__":
    main()