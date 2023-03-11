#include <cs50.h>
#include <stdio.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void giving_grade(int n_letters, int n_words, int n_sentences);

// Ask for text, and grade its readability level
int main(void)
{
    // Prompt
    string text = get_string("Text:  ");

    // Get number of letters, words, sentences
    int n_letters = count_letters(text);
    int n_words = count_words(text);
    int n_sentences = count_sentences(text);

    // Calculate grade & print it
    giving_grade(n_letters, n_words, n_sentences);
}

// Count letters
int count_letters(string text)
{
    int i = 0, n_letters = 0;
    // Loop through each letter of the text
    while (text[i])
    {
        // If valid letter
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            n_letters++;
        }
        i++;
    }
    return n_letters;
}


// Count words
int count_words(string text)
{
    int i = 0, n_words = 1;
    // Loop through each letter of the text
    while (text[i])
    {
        // Words seperator
        if (text[i] == ' ')
        {
            n_words++;
        }
        i++;
    }
    return n_words;
}

// Count sentences
int count_sentences(string text)
{
    int i = 0, n_sentences = 0;
    // Loop through each letter of the text
    while (text[i])
    {
        // End of sentence
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            n_sentences++;
        }
        i++;
    }
    return n_sentences;
}

// Calculate grade and print it
void giving_grade(int n_letters, int n_words, int n_sentences)
{
    // Solution
    float L = ((n_letters + 0.0) / (n_words + 0.0)) * 100.0;
    float S = ((n_sentences + 0.0) / (n_words + 0.0)) * 100.0;

    float index = 0.0588 * L - 0.296 * S - 15.8;

    // Print result
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %0.f\n", index);
    }
}