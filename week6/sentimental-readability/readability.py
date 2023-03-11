# Prompt a text and determine the grade level needed to comprehend the text
# TODO

import string
import re


def main():
    # Prompt for text
    text = input("Text:  ")
    # Get necessary values for the computation
    # (might be better to use a single loop with multiple ifs for faster run time)
    no_letters = count_letters(text)
    no_words = count_words(text)
    no_sentences = count_sentences(text)
    # perform Coleman Liau index to determine grade level
    index = Coleman_Liau_index(no_letters, no_words, no_sentences)
    # Print out grade level based on index
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def count_letters(text_f):
    counter = 0
    for character in text_f:
        if character.lower() in string.ascii_lowercase:
            counter += 1
    return counter


def count_words(text_f):
    # Start with 1 because whitespace is used to determine no. words
    counter = 1
    # Remove multiple adjacent whitespaces, remove whitespace in the start and end of the text
    text_f = re.sub(' +', ' ', text_f).strip()
    for character in text_f:
        if character == " ":
            counter += 1
    return counter


def count_sentences(text_f):
    counter = 0
    for character in text_f:
        if character in [".", "?", "!"]:
            counter += 1
    return counter


def Coleman_Liau_index(letters, words, sentences):
    index_f = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8
    return round(index_f)


if __name__ == "__main__":
    main()