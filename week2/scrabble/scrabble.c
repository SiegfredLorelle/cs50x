// Scrabble: Two players are asked for a word, and checks for who has a higher score

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else
    {
        printf("Player 2 wins!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int i, length, score, total_score, ASCII_equi;

    // Loop through each letter
    for (i = 0, length = strlen(word), total_score = 0; i < length; i++)
    {
        // Lowercase all letters
        if (isupper(word[i]))
        {
            word[i] = tolower(word[i]);
        }

        // Get the ASCII equivalent
        ASCII_equi = word[i];

        // If valid letters then get the scores
        if (ASCII_equi  <= 122 && ASCII_equi  >= 97)
        {
            score = POINTS[ASCII_equi - 97];
            total_score += score;
        }
    }
    return total_score;
}
