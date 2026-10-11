
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define MAX 200

typedef struct {
    int number;
    char name[51];
    char uni[31];
    int solved;
    int penalty;
    int year;
} Team;

/* Function declarations */
void promptTeamDetails(Team *teamPtr);
void displayTeam(const Team *teamPtr);

int duplicate(int currentSize, const Team registry[],
              const Team *teamPtr);

int findTeamsByContest(const Team registry[], int currentSize,
                       int searchContest, Team *results[],
                       int maxResults);

int getQualifiedList(const Team registry[], int currentSize,
                     int contest, int minSolved);

int printScoreboard(const Team registry[], int currentSize,
                    int contest);

int saveRegistry(const char *filename, const Team registry[],
                 int currentSize);

int loadRegistry(const char *filename, Team registry[],
                 int *currentSize);

int validTeam(const Team *teamPtr);
int readInteger(const char *prompt);
void readText(const char *prompt, char text[], int maxLength);
int parseInteger(const char *text, int *value);
int encodeText(FILE *fp, const char *text);
int decodeText(const char *text, char output[], int maxLength);

/* Read an integer safely */
int readInteger(const char *prompt)
{
    char line[256];
    char *end;
    long value;

    while (1) {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            exit(0);
        }

        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Input is too long. Try again.\n");
            continue;
        }

        errno = 0;
        value = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r') {
            end++;
        }

        if (end == line || *end != '\0' ||
            errno == ERANGE || value < INT_MIN ||
            value > INT_MAX) {
            printf("Invalid integer. Try again.\n");
            continue;
        }

        return (int)value;
    }
}

/* Read a string containing spaces */
void readText(const char *prompt, char text[], int maxLength)
{
    char line[256];
    size_t length;

    while (1) {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            exit(0);
        }

        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Input is too long. Try again.\n");
            continue;
        }

        line[strcspn(line, "\n")] = '\0';
        length = strlen(line);

        if (length == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        if (length > (size_t)maxLength) {
            printf("Maximum length is %d characters.\n",
                   maxLength);
            continue;
        }

        strcpy(text, line);
        return;
    }
}

/* Validate a team record */
int validTeam(const Team *teamPtr)
{
    if (teamPtr->number <= 0)
        return 0;

    if (strlen(teamPtr->name) == 0 ||
        strlen(teamPtr->name) > 50)
        return 0;

    if (strlen(teamPtr->uni) == 0 ||
        strlen(teamPtr->uni) > 30)
        return 0;

    if (teamPtr->solved < 0 || teamPtr->solved > 13)
        return 0;

    if (teamPtr->penalty < 0)
        return 0;

    if (teamPtr->year < 2000 || teamPtr->year > 2100)
        return 0;

    return 1;
}

/* Input team details */
void promptTeamDetails(Team *teamPtr)
{
    printf("\n========== ENTER TEAM DETAILS ==========\n");

    do {
        teamPtr->number = readInteger("Contest number: ");

        if (teamPtr->number <= 0)
            printf("Contest number must be positive.\n");

    } while (teamPtr->number <= 0);

    readText("Team name: ", teamPtr->name, 50);
    readText("University: ", teamPtr->uni, 30);

    do {
        teamPtr->solved = readInteger("Problems solved (0-13): ");

        if (teamPtr->solved < 0 || teamPtr->solved > 13)
            printf("Problems solved must be between 0 and 13.\n");

    } while (teamPtr->solved < 0 || teamPtr->solved > 13);

    do {
        teamPtr->penalty = readInteger("Penalty minutes: ");

        if (teamPtr->penalty < 0)
            printf("Penalty cannot be negative.\n");

    } while (teamPtr->penalty < 0);

    do {
        teamPtr->year = readInteger("Contest year (2000-2100): ");

        if (teamPtr->year < 2000 || teamPtr->year > 2100)
            printf("Invalid contest year.\n");

    } while (teamPtr->year < 2000 || teamPtr->year > 2100);
}

/* Display one team */
void displayTeam(const Team *teamPtr)
{
    printf("\n========================\n");
    printf("Contest Number : %d\n", teamPtr->number);
    printf("Team Name      : %s\n", teamPtr->name);
    printf("University     : %s\n", teamPtr->uni);
    printf("Problems Solved: %d\n", teamPtr->solved);
    printf("Penalty Minutes: %d\n", teamPtr->penalty);
    printf("Contest Year   : %d\n", teamPtr->year);
    printf("========================\n");
}

/* Check duplicate contest number + exact team name */
int duplicate(int currentSize, const Team registry[],
              const Team *teamPtr)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].number == teamPtr->number &&
            strcmp(registry[i].name, teamPtr->name) == 0) {
            return 1;
        }
    }

    return 0;
}

/* Find teams by contest number */
int findTeamsByContest(const Team registry[], int currentSize,
                       int searchContest, Team *results[],
                       int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].number == searchContest) {
            if (count < maxResults) {
                results[count] = (Team *)&registry[i];
                count++;
            }
        }
    }

    return count;
}

/* Display teams meeting the solved-problem requirement */
int getQualifiedList(const Team registry[], int currentSize,
                     int contest, int minSolved)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].number == contest &&
            registry[i].solved >= minSolved) {

            displayTeam(&registry[i]);
            count++;
        }
    }

    if (count == 0)
        printf("No qualified teams found.\n");

    return count;
}

/* Contest scoreboard using bubble sort */
int printScoreboard(const Team registry[], int currentSize,
                    int contest)
{
    int index[MAX];
    int count = 0;

    /* Collect matching team indexes */
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].number == contest) {
            index[count] = i;
            count++;
        }
    }

    if (count == 0) {
        printf("No teams found for this contest.\n");
        return 0;
    }

    /* Bubble sort */
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            int a = index[j];
            int b = index[j + 1];

            if (registry[a].solved < registry[b].solved ||
                (registry[a].solved == registry[b].solved &&
                 registry[a].penalty > registry[b].penalty) ||
                (registry[a].solved == registry[b].solved &&
                 registry[a].penalty == registry[b].penalty &&
                 strcmp(registry[a].name, registry[b].name) > 0)) {

                int temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
    }

    /* Print scoreboard and ranks */
    printf("\n========== SCOREBOARD ==========\n");
    printf("%-6s %-25s %-10s %-10s\n",
           "Rank", "Team", "Solved", "Penalty");

    int rank = 1;

    for (int i = 0; i < count; i++) {
        int current = index[i];

        if (i > 0) {
            int previous = index[i - 1];

            if (registry[current].solved !=
                    registry[previous].solved ||
                registry[current].penalty !=
                    registry[previous].penalty) {
                rank = i + 1;
            }
        }

        printf("%-6d %-25s %-10d %-10d\n",
               rank,
               registry[current].name,
               registry[current].solved,
               registry[current].penalty);
    }

    return count;
}

/*
   Encode text as hexadecimal in the file.
   This prevents spaces and delimiter characters such as '|'
   from breaking the record format.
*/
int encodeText(FILE *fp, const char *text)
{
    for (size_t i = 0; text[i] != '\0'; i++) {
        if (fprintf(fp, "%02X", (unsigned char)text[i]) < 0)
            return 0;
    }

    return 1;
}

/* Convert hexadecimal text back to a string */
int decodeText(const char *text, char output[], int maxLength)
{
    size_t length = strlen(text);

    if (length % 2 != 0 || length / 2 > (size_t)maxLength)
        return 0;

    for (size_t i = 0; i < length; i += 2) {
        char pair[3];
        char *end;
        long value;

        pair[0] = text[i];
        pair[1] = text[i + 1];
        pair[2] = '\0';

        errno = 0;
        value = strtol(pair, &end, 16);

        if (errno == ERANGE || end == pair ||
            *end != '\0' || value <= 0 || value > 255)
            return 0;

        output[i / 2] = (char)value;
    }

    output[length / 2] = '\0';
    return 1;
}

/* Save registry to file */
int saveRegistry(const char *filename, const Team registry[],
                 int currentSize)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Cannot open file for saving.\n");
        return 0;
    }

    for (int i = 0; i < currentSize; i++) {
        if (fprintf(fp, "%d|", registry[i].number) < 0 ||
            !encodeText(fp, registry[i].name) ||
            fprintf(fp, "|") < 0 ||
            !encodeText(fp, registry[i].uni) ||
            fprintf(fp, "|%d|%d|%d\n",
                    registry[i].solved,
                    registry[i].penalty,
                    registry[i].year) < 0) {

            printf("Error writing file.\n");
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0) {
        printf("Error closing file.\n");
        return 0;
    }

    printf("Registry saved successfully.\n");
    return 1;
}

/* Convert a text field to an integer */
int parseInteger(const char *text, int *value)
{
    char *end;
    long number;

    errno = 0;
    number = strtol(text, &end, 10);

    if (text == end || *end != '\0' ||
        errno == ERANGE || number < INT_MIN ||
        number > INT_MAX) {
        return 0;
    }

    *value = (int)number;
    return 1;
}

/* Load registry from file */
int loadRegistry(const char *filename, Team registry[],
                 int *currentSize)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Cannot open file for loading. Registry unchanged.\n");
        return 0;
    }

    Team temp[MAX];
    int size = 0;
    int lineNo = 0;
    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNo++;

        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int ch;
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
            }

            printf("Skipping invalid line %d: line too long.\n",
                   lineNo);
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') {
            printf("Skipping invalid line %d: empty line.\n",
                   lineNo);
            continue;
        }

        char *field[6];
        char *start = line;
        int bad = 0;

        /* Split into exactly six fields */
        for (int i = 0; i < 5; i++) {
            field[i] = start;
            char *separator = strchr(start, '|');

            if (separator == NULL) {
                bad = 1;
                break;
            }

            *separator = '\0';
            start = separator + 1;
        }

        field[5] = start;

        if (bad || strchr(field[5], '|') != NULL) {
            printf("Skipping invalid line %d: wrong format.\n",
                   lineNo);
            continue;
        }

        Team t = {0};

        if (!parseInteger(field[0], &t.number) ||
            !decodeText(field[1], t.name, 50) ||
            !decodeText(field[2], t.uni, 30) ||
            !parseInteger(field[3], &t.solved) ||
            !parseInteger(field[4], &t.penalty) ||
            !parseInteger(field[5], &t.year) ||
            !validTeam(&t)) {

            printf("Skipping invalid line %d: invalid data.\n",
                   lineNo);
            continue;
        }

        if (duplicate(size, temp, &t)) {
            printf("Skipping line %d: duplicate team.\n", lineNo);
            continue;
        }

        if (size >= MAX) {
            printf("Skipping line %d: registry is full.\n",
                   lineNo);
            continue;
        }

        temp[size] = t;
        size++;
    }

    if (ferror(fp)) {
        printf("Error reading file. Registry unchanged.\n");
        fclose(fp);
        return 0;
    }

    if (fclose(fp) != 0) {
        printf("Error closing file. Registry unchanged.\n");
        return 0;
    }

    /* Replace registry only after reading completes successfully */
    for (int i = 0; i < size; i++) {
        registry[i] = temp[i];
    }

    *currentSize = size;

    printf("Registry loaded successfully. Records: %d\n", size);
    return 1;
}

/* Main menu */
int main(void)
{
    Team team[MAX];
    int currentSize = 0;
    int choice;

    do {
        printf("\n================================\n");
        printf("1. Add Team Entry\n");
        printf("2. Search Entries by Contest Number\n");
        printf("3. Display All Entries\n");
        printf("4. Qualified Teams List\n");
        printf("5. Scoreboard for a Contest\n");
        printf("6. Save Registry to File\n");
        printf("7. Load Registry from File\n");
        printf("8. Exit\n");
        printf("================================\n");

        choice = readInteger("Enter choice: ");

        switch (choice) {

            case 1: {
                if (currentSize >= MAX) {
                    printf("Maximum registry size reached.\n");
                    break;
                }

                Team temp;
                promptTeamDetails(&temp);

                if (duplicate(currentSize, team, &temp)) {
                    printf("Duplicate team found. Not added.\n");
                } else {
                    team[currentSize] = temp;
                    currentSize++;
                    printf("Team added successfully.\n");
                }

                break;
            }

            case 2: {
                int contest;
                Team *results[MAX];

                contest = readInteger("Enter contest number: ");

                int count = findTeamsByContest(
                    team, currentSize, contest, results, MAX
                );

                if (count == 0) {
                    printf("No teams found.\n");
                } else {
                    for (int i = 0; i < count; i++) {
                        displayTeam(results[i]);
                    }
                }

                break;
            }

            case 3: {
                if (currentSize == 0) {
                    printf("Registry is empty.\n");
                } else {
                    for (int i = 0; i < currentSize; i++) {
                        displayTeam(&team[i]);
                    }
                }

                break;
            }

            case 4: {
                int contest;
                int minSolved;

                contest = readInteger("Enter contest number: ");
                minSolved = readInteger(
                    "Minimum problems solved (0-13): "
                );

                if (minSolved < 0 || minSolved > 13) {
                    printf("Invalid minimum solved value.\n");
                } else {
                    getQualifiedList(
                        team, currentSize, contest, minSolved
                    );
                }

                break;
            }

            case 5: {
                int contest;

                contest = readInteger("Enter contest number: ");

                printScoreboard(team, currentSize, contest);
                break;
            }

            case 6:
                saveRegistry("scoreboard.txt", team, currentSize);
                break;

            case 7:
                loadRegistry("scoreboard.txt", team, &currentSize);
                break;

            case 8:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 8);

    return 0;
}
