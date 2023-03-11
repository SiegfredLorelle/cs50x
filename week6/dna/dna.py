# Enter a database of DNA in csv file and a sequence of DNA in txt file
# The program will determine who the sequence of DNA is from by checking the database

import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("-ERROR- Enter in the command-line:\ndna.py <csv file> <txt file>")

    # TODO: Read database file into a variable
    csv_file = []
    STR_list = []
    is_first_row = True

    # Open the csv file
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        # Go to each line of the csv file
        for row in reader:
            # If currently on the first line, get the STRs
            if is_first_row:
                STR_list = list(row.keys())
                # remove first element because it is 'name' not an STR
                del STR_list[0]
                is_first_row = False
            # Append each person in a list called csv file
            csv_file.append(row)

    # TODO: Read DNA sequence file into a variable
    # Open the txt file with the DNA sequence and save to a variable called txt_file
    with open(sys.argv[2], "r") as file:
        txt_file = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    # Create a dictionary where the STRs are keys and the number of times an STR cosecutively repeats is the value
    count = {}
    # Find the longest sequence of all STRs
    for STR in STR_list:
        longest_seq = longest_match(txt_file, STR)
        count[STR] = longest_seq

    # TODO: Check database for matching profiles
    # Loop to each person to check if someone got the same number of sequences
    for person in csv_file:
        no_correct_STR = 0
        # Check all STRs per person
        for STR in STR_list:
            # If the database has a match update the counter
            if int(person[STR]) == int(count[STR]):
                no_correct_STR += 1
                # If the database has a complete match with all STRs, print the name
                if no_correct_STR == len(STR_list):
                    print(person["name"])
                    sys.exit(0)

    # If the data base did not show any matches, then print no match
    print("No Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()