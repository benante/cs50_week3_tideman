#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

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
    // Loop through candidates names and cross check them with names voted. If name exist, update rank array
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // EXPLANATION: A, B, and C. If a voter ranks them as A > C > B, the ranks array for this voter would be [0, 2, 1], meaning candidate 0 (A) is ranked first, candidate 2 (C) is ranked second, and candidate 1 (B) is ranked third.
    // Now, we update the preferences array based on this voter’s ranks. We increase preferences[0][2] and preferences[0][1] by 1 because this voter prefers candidate 0 over candidates 2 and 1. Similarly, we increase preferences[2][1] by 1 because the voter prefers candidate 2 over candidate 1.

    // For each candidate loop through the rank array and add 1 for each candidate that come after them (position j > i) in the array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j > i)
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {

            if (preferences[i][j] - preferences[j][i] > 0)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] - preferences[j][i] < 0)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Loop through pairs array
    for (int i = 0; i < pair_count; i++)
    {
        // Set temporary score and pair
        int highestScore = 0;
        pair highest;
        // Compare each pairs value (taken from preferences array) with pairs[i]
        for (int j = i + 1; j < pair_count; j++)
        {
            // If value is higher then highestScore, set highestScore to that value
            if (preferences[pairs[i].winner][pairs[i].loser] > highestScore)
            {
                highestScore = preferences[pairs[i].winner][pairs[i].loser];
                // Change highest value to current [j] pairs element (with higher value)
                highest = pairs[j];
                // Set highest value [j] pair with (lower value) [i] pair
                pairs[j] = pairs[i];
            }
            // Set [i] pair value with highest value
            pairs[i] = highest;
        }
    }
    return;
}

bool isCycle(int winner_candidate, int current_candidate)
{
    // Base case: if the winner candidate is also current candidate (loser), there's a cycle ==> TRUE
    if (winner_candidate == current_candidate)
        return true;
    for (int i = 0; i < candidate_count; i++)
    {
        // Investigate further down to check if loser has a link to winner in its path comparing it to other candidates

        // If there's a connection between current candidate (loser) and i (th) candidate
        if (locked[current_candidate][i])
        {
            // Repeat action with winner and i(th) candidate
            if (isCycle(winner_candidate, i))
                return true;
        }
    }
    return false;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Iterate through each pair and check whether there' s a cycle. If so, set lock to false
    for (int i = 0; i < pair_count; i++)
    {
        if (!isCycle(pairs[i].winner, pairs[i].loser))
        {

            locked[pairs[i].winner][pairs[i].loser] = true;
        }

        else
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO

    return;
}
