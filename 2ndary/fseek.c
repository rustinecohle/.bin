#include <stdio.h>

struct Student {
    int id;
    char name[50];
    float mark;
};

int main(void)
{
    FILE *fp;
    struct Student s;
    int position;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("File not found\n");
        return 1;
    }

    printf("Enter record number (0, 1, 2, ...): ");
    scanf("%d", &position);

    if (position < 0) {
        printf("Invalid position\n");
        fclose(fp);
        return 1;
    }

    fseek(fp, (long)position * sizeof(s), SEEK_SET);

    if (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("ID: %d\n", s.id);
        printf("Name: %s\n", s.name);
        printf("Mark: %.2f\n", s.mark);
    } else {
        printf("Record not found\n");
    }

    fclose(fp);
    return 0;
}
