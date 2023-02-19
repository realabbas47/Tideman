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
bool chkcycle(int win, int lose);

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
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    pair_count = (candidate_count * (candidate_count - 1)) / 2;
    int tie_flag = 0;
    int i = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        for (int k = j; k < candidate_count; k++)
        {
            if (k != j)
            {
                int a = preferences[j][k];
                int b = preferences[k][j];
                if (a > b)
                {
                    pairs[i].winner = j;
                    pairs[i].loser = k;
                    i++;
                }
                else if (a < b)
                {
                    pairs[i].winner = k;
                    pairs[i].loser = j;
                    i++;
                }
                else if (a == b)
                {
                    tie_flag++;
                }
            }
        }
    }
    pair_count -= tie_flag;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int t1, t2 = 0;
    int flag = 0;
    do
    {
        flag = 0;
        for (int i = 0; i < pair_count + 1; i++)
        {
            int ith = preferences[pairs[i].winner][pairs[i].loser] -
                      preferences[pairs[i].loser][pairs[i].winner];
            int ithh = preferences[pairs[i + 1].winner][pairs[i + 1].loser] -
                       preferences[pairs[i + 1].loser][pairs[i + 1].winner];
            if (ith < ithh)
            {
                t1 = pairs[i].winner;
                t2 = pairs[i].loser;

                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser = pairs[i + 1].loser;

                pairs[i + 1].winner = t1;
                pairs[i + 1].loser = t2;
                flag = 1;
            }
        }
    } while (flag == 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int win;
    int lose;
    for (int i = 0; i < pair_count; i++)
    {
        win = pairs[i].winner;
        lose = pairs[i].loser;

        if (chkcycle(win, lose) == false)
        {
            locked[win][lose] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int srcs[candidate_count];
    for (int x = 0; x < candidate_count; x++)
    {
        srcs[x] = 0;
    }

    for (int c = 0; c < candidate_count; c++)
    {
        for (int r = 0; r < candidate_count; r++)
        {
            if (r != c)
            {
                if (locked[r][c])
                {
                    srcs[c]++;
                }
            }
        }
    }

    int flag = 0;
    int vict = candidate_count;
    int winr;
    do
    {
        flag = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            if (srcs[i] < vict)
            {
                vict = srcs[i];
                winr = i;
                flag = 1;
            }
        }
    } while (flag == 1);

    printf("%s\n", candidates[winr]);
    return;
}

bool chkcycle(int win, int lose)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (i != lose && locked[lose][i])
        {
            if (i == win)
            {
                return true;
            }
            else if (chkcycle(win, i))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}
