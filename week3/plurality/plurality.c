#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}



// Update vote totals given a new vote
bool vote(string name)
{
    // Loop through each candidate
    int j = 0;
    while (j < candidate_count)
    {

        //  Is a candidate
        if (strcasecmp(name, candidates[j].name) == 0)
        {
            // add the vote to his/her name
            candidates[j].votes++;
            return true;
        }
        j++;
    }

    // Is not a candidate
    return false;
}


// Print the winner (or winners) of the election
void print_winner(void)
{
    // k is the index for the leading candidates
    int highest_votes[candidate_count];
    int k = 0;

    // Is first candidate winning against second canditate
    if (candidates[0].votes > candidates[1].votes)
    {
        highest_votes[0] = 0;
    }
    // Is second candidate winning against first canditate
    else if (candidates[0].votes < candidates[1].votes)
    {
        highest_votes[0] = 1;
    }
    // Is first and second canditate in a tie
    else
    {
        highest_votes[k] = 0;
        highest_votes[k + 1] = 1;
        k++;
    }

    // Loop throughout the rest of the candidates
    for (int l = 2; l <= (candidate_count - 1); l++)
    {
        // Is the leading candidate surpassed by a new candidate
        if (candidates[highest_votes[0]].votes < candidates[l].votes)
        {
            highest_votes[0] = l;
        }
        // Is the leading candidate in a tie
        else if (candidates[highest_votes[0]].votes == candidates[l].votes)
        {
            highest_votes[k + 1] = l;
            k++;
        }

        // If leading candidates not in a tie
        if (candidates[highest_votes[0]].votes != candidates[highest_votes[1]].votes)
        {
            highest_votes[candidate_count] = highest_votes[0];
            k = 0;
        }
    }


    int m;
    // Loop throught the leading candidate(s)
    for (m = 0; m <= k; m++)
    {
        printf("%s\n", candidates[highest_votes[m]].name);
    }

    return;
}

