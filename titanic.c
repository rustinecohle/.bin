#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *fp = fopen("titanic.txt", "r");

    if (fp == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    char line[500];
    int id, survived, pclass;
    char name[150], sex[10];
    int count = 0;

    // Skip the header line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        int n = sscanf(line,
            "%d,%d,%d,\"%149[^\"]\",%9[^,]",
            &id, &survived, &pclass, name, sex);

        if (n == 5) {
            if (survived == 1 && strcmp(sex, "male") == 0) {
                count++;
            }
        }
    }

    fclose(fp);

    printf("Number of male survivors: %d\n", count);

    return 0;
}
