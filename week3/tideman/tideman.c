#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // Loop through each candidate
    for (int k = 0; k < candidate_count; k++)
    {
        // Check if voted name is a candidate
        if (strcasecmp(name, candidates[k]) == 0)
        {
            // ranks[i] where lowest i is most preferred
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // Loop through each row
    for (int l = 0; l < candidate_count; l++)
    {
        // Loop through each col
        for (int m = 0; m < candidate_count; m++)
        {
            // Check for all head2head
            if (l < m)
            {
                // Tally preferred candidate where preferences[preferred][preferred over]
                preferences[ranks[l]][ranks[m]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // q is counter for pair_count
    int q = 0;
    // Loop through each preferred candidate
    for (int o = 0; o < candidate_count; o++)
    {
        // Loop through each preferred over candidate
        for (int p = 0; p < candidate_count; p++)
        {
            // Compare to find winner and loser
            if (o < p)
            {
                if (preferences[o][p] > preferences[p][o])
                {
                    pairs[q].winner = o;
                    pairs[q].loser = p;
                    q++;
                }
                else if (preferences[o][p] < preferences[p][o])
                {
                    pairs[q].winner = p;
                    pairs[q].loser = o;
                    q++;
                }
            }
        }
    }
    // Save to global var pair_count
    pair_count = q;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Loop through each pair
    for (int r = 0, s = r + 1; r < pair_count - 1; r++)
    {
        // Compare current pair to next pair
        if ((preferences[pairs[r].winner][pairs[r].loser] - preferences[pairs[r].loser][pairs[r].winner]) <
            (preferences[pairs[s].winner][pairs[s].loser] - preferences[pairs[s].loser][pairs[s].winner]))
        {
            // Swap if next pair is bigger than current pair
            int temporary_variable = pairs[r].winner;
            int temporary_variable2 = pairs[r].loser;
            pairs[r].winner = pairs[s].winner;
            pairs[r].loser = pairs[s].loser;
            pairs[s].winner = temporary_variable;
            pairs[s].loser = temporary_variable2;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Lock all pairs
    for (int t = 0; t < pair_count; t++)
    {
        locked[pairs[t].winner][pairs[t].loser] = true;
    }

    // Check if cycle
    // Loop through each col
    for (int u = 0; u < candidate_count; u++)
    {
        // w is counter variable for unlocked pairs
        int w = 0;
        // Loop through each row
        for (int v = 0; v < candidate_count; v++)
        {
            // if unlocked pair
            if (!locked[v][u])
            {
                w++;
                // if not cycle
                if (w == candidate_count)
                {
                    // Exit
                    return;
                }
            }
        }
    }
    // If cycle
    locked[pairs[pair_count - 1].winner][pairs[pair_count - 1].loser] = false;
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // Loop through each col
    for (int x = 0; x < candidate_count; x++)
    {
        // z is counter var for unlocked pairs
        int z = 0;
        // Loop through each row
        for (int y = 0; y < candidate_count; y++)
        {
            // If unlocked pair
            if (!locked[y][x])
            {
                z++;
                // If no one is locked onto the candidate thus winner
                if (z >= candidate_count)
                {
                    printf("%s\n", candidates[x]);
                    return;
                }
            }
        }
    }
    return;
}



