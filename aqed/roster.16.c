#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 150

typedef struct {
    int id;
    char name[101];
    float midMark;
    float finalMark;
    float totalMark;
} Student;


void promptStudentDetails(Student *studentPtr)
{
    printf("Enter ID: ");
    scanf("%d", &studentPtr->id);

    printf("Enter name: ");
    scanf(" %100[^\n]", studentPtr->name);

    printf("Enter mid marks: ");
    scanf("%f", &studentPtr->midMark);

    printf("Enter final marks: ");
    scanf("%f", &studentPtr->finalMark);

    studentPtr->totalMark =
        studentPtr->midMark + studentPtr->finalMark;
}


int checkDuplicate(const Student roster[],
                   const Student *studentPtr,
                   int rosterSize)
{
    for (int i = 0; i < rosterSize; i++)
    {
        if (roster[i].id == studentPtr->id)
        {
            printf("Duplicate found at position %d\n", i + 1);
            return 1;
        }
    }

    return 0;
}


void displayFullRoster(const Student registry[], int rosterSize)
{
    for (int i = 0; i < rosterSize; i++)
    {
        /*
        YOUR WRONG CODE:

        printf("id is %d\n", registy[1].id);
        printf("name is %s\n", registy[1].name);

        PROBLEM:
        You used [1], so you always access the second student.

        Since you are inside:

        for(int i = 0; i < rosterSize; i++)

        you should use [i].
        */

        printf("ID: %d\n", registry[i].id);
        printf("Name: %s\n", registry[i].name);

        /*
        YOUR WRONG CODE:

        printf("mid mark is %.f\n", registy[i].midMark);

        %.f is not a serious error, but it removes the
        decimal part.

        Better:

        %.2f
        */

        printf("Mid Mark: %.2f\n", registry[i].midMark);

        printf("Final Mark: %.2f\n", registry[i].finalMark);
        printf("Total Mark: %.2f\n", registry[i].totalMark);
    }
}


void displayStudent(const Student *studentPtr)
{
    printf("ID: %d\n", studentPtr->id);
    printf("Name: %s\n", studentPtr->name);
    printf("Mid Mark: %.2f\n", studentPtr->midMark);
    printf("Final Mark: %.2f\n", studentPtr->finalMark);
    printf("Total Mark: %.2f\n", studentPtr->totalMark);
}


int findStudentIndexByID(const Student registry[],
                         int rosterSize,
                         int id)
{
    for (int i = 0; i < rosterSize; i++)
    {
        if (registry[i].id == id)
        {
            return i;
        }
    }

    return -1;
}


int findTopStudentIndex(const Student registry[], int rosterSize)
{
    if (rosterSize == 0)
    {
        /*
        YOUR CODE:

        printf("size is empty\n");

        This isn't really wrong.
        But printing can be handled in main().
        */

        return -1;
    }


    /*
    YOUR WRONG CODE:

    int top = registry[0].totalMark;

    PROBLEM:

    top should contain an INDEX.

    But registry[0].totalMark contains a MARK.

    Example:

    registry[0].totalMark = 175

    Then:

    top = 175

    That means you would later try:

    registry[175]

    which is wrong.

    CORRECT:
    */

    int top = 0;


    /*
    YOUR CODE:

    for(int i=0; i<rosterSize; i++)

    This can work, but since student 0 is already
    our initial top student, we can start from 1.
    */

    for (int i = 1; i < rosterSize; i++)
    {
        /*
        YOUR WRONG CODE:

        if(registry[i].totalMark > top)

        PROBLEM:

        registry[i].totalMark = MARK
        top = INDEX

        So you are comparing:

        MARK > INDEX

        That is logically wrong.

        CORRECT:
        */

        if (registry[i].totalMark > registry[top].totalMark)
        {
            /*
            YOUR WRONG CODE:

            top = registry[i].totalMark;

            PROBLEM:

            Again, top should contain an INDEX,
            not a mark.

            CORRECT:
            */

            top = i;
        }
    }

    /*
    YOUR WRONG CODE:

    return -1;

    PROBLEM:

    -1 means "not found".

    If we successfully find the top student,
    we need to return its index.

    CORRECT:
    */

    return top;
}


int main(void)
{
    Student courseRoster[MAX];

    int c;
    int rosterSize = 0;

    do
    {
        printf("\n===== MENU =====\n");
        printf("1. Add students\n");
        printf("2. Display full roster\n");
        printf("3. Find student by ID\n");
        printf("4. Show top student\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &c);


        switch (c)
        {
            case 1:
            {
                if (rosterSize >= MAX)
                {
                    printf("Maximum roster size reached!\n");
                    break;
                }

                Student temp;

                promptStudentDetails(&temp);

                int value =
                    checkDuplicate(courseRoster, &temp, rosterSize);

                if (!value)
                {
                    courseRoster[rosterSize] = temp;
                    rosterSize++;

                    printf("Student added successfully!\n");
                }

                break;
            }


            case 2:
            {
                /*
                YOUR WRONG CODE:

                if(rosterSize==0){
                    prinf("no data add data first\n");
                    break;
                }

                PROBLEM:

                prinf() does not exist.

                It should be:

                printf()
                */

                if (rosterSize == 0)
                {
                    printf("No data. Add data first.\n");
                    break;
                }

                displayFullRoster(courseRoster, rosterSize);

                break;
            }


            case 3:
            {
                int uid;

                /*
                YOUR WRONG CODE:

                prinf("enter id to search \n");

                PROBLEM:

                prinf -> printf
                */

                printf("Enter ID to search: ");
                scanf("%d", &uid);


                if (rosterSize != 0)
                {
                    /*
                    YOUR WRONG CODE:

                    int value=findStudentIndexByID(
                        courseRoster,
                        rosterSize
                    );

                    PROBLEM:

                    Your function requires:

                    roster
                    rosterSize
                    id

                    You forgot uid.

                    CORRECT:
                    */

                    int value =
                        findStudentIndexByID(
                            courseRoster,
                            rosterSize,
                            uid
                        );


                    /*
                    YOUR WRONG CODE:

                    if(value=-1)

                    PROBLEM:

                    = means ASSIGNMENT.

                    You are assigning -1 to value.

                    For comparison use:

                    ==
                    */

                    if (value == -1)
                    {
                        printf("No results found.\n");
                    }
                    else
                    {
                        /*
                        YOUR WRONG CODE:

                        displpayFullRoster(courseRoster, value);

                        PROBLEM:

                        value is the INDEX of the student.

                        If value = 3, then you want:

                        courseRoster[3]

                        But your function treats value as
                        the number of students to display.

                        So use displayStudent().
                        */

                        displayStudent(&courseRoster[value]);
                    }

                    break;
                }

                printf("Enter student data first.\n");

                break;
            }


            case 4:
            {
                /*
                YOUR WRONG CODE:

                int vlue=findTopStudentIndex(registry, rosterSize);

                PROBLEMS:

                1. Your array is called courseRoster,
                   not registry.

                2. You named the variable vlue.

                Later you wrote value.

                vlue and value are different variables.

                CORRECT:
                */

                int value =
                    findTopStudentIndex(courseRoster, rosterSize);


                /*
                YOUR CODE:

                if(value<0)

                This is actually okay.

                Since findTopStudentIndex() returns -1
                when the roster is empty, value < 0 works.
                */

                if (value < 0)
                {
                    printf("No top student. Roster is empty.\n");
                }
                else
                {
                    /*
                    YOUR WRONG CODE:

                    prinf("index is %d", value);

                    PROBLEM:

                    prinf -> printf

                    Also, the question wants you to show
                    the top student's information, not just
                    its index.

                    So we use displayStudent().
                    */

                    printf("\n===== Top Student =====\n");
                    displayStudent(&courseRoster[value]);
                }

                break;
            }


            case 5:
                printf("Goodbye!\n");
                break;


            default:
                printf("Invalid choice!\n");
        }

    } while (c != 5);

    return 0;
}
