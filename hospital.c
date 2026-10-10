
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char pname[41];
    char dname[31];
    int wno;
    int age;
    float bill;
} Patient;

void promptPatientDetails(Patient *patPtr)
{
    printf("enter id\n");
    scanf("%d", &patPtr->id);

    printf("enter patient name\n");
    scanf(" %40[^\n]", patPtr->pname);

    printf("enter doctors name\n");
    scanf(" %30[^\n]", patPtr->dname);

    printf("enter ward number\n");
    scanf("%d", &patPtr->wno);

    printf("enter age\n");
    scanf("%d", &patPtr->age);

    printf("enter due bill\n");
    scanf("%f", &patPtr->bill);
}

void displayPatient(const Patient *patPtr)
{
    printf("id is %d\n", patPtr->id);
    printf("name is %s\n", patPtr->pname);
    printf("appointed doctor is %s\n", patPtr->dname);
    printf("ward number is %d\n", patPtr->wno);
    printf("age is %d\n", patPtr->age);
    printf("due bill is %.2f\n", patPtr->bill);

    // WRONG:
    // printf("age is %d", patPtr->wno);
    // You printed wno instead of age.
    //marked
}

int duplicate(Patient registry[], int currentSize, const Patient *patPtr)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].id == patPtr->id) {
            return 1;
        }
    }

    return 0;
}

void displayAll(const Patient *patPtr)
{
    printf("id is %d\n", patPtr->id);
    printf("patient name is %s\n", patPtr->pname);
    printf("doctors name is %s\n", patPtr->dname);
    printf("ward number is %d\n", patPtr->wno);
    printf("age is %d\n", patPtr->age);
    printf("bill is %.2f\n", patPtr->bill);
}

int findPatientByDoctor(Patient registry[],
                        int currentSize,
                        const char *doctor,
                        Patient *results[],
                        int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {

        if (strcmp(registry[i].dname, doctor) == 0) {

            if (count < maxResults) {
                results[count] = &registry[i];
                count++;
            }
        }
    }

    return count;

    /*
    WRONG:

    if(currentSize<=maxResults){
        ...
    }

    The problem is that currentSize is NOT the number of
    matching doctors.

    Also, you must make sure count does not go beyond
    maxResults.
    */
}

int getUnpaidList(const Patient registry[],
                  int currentSize,
                  float minAmount,
                  float *totalDue)
{
    int count = 0;

    *totalDue = 0;

    // WRONG:
    // You forgot to initialize *totalDue = 0.
    // Without this, totalDue could contain an old value.

    for (int i = 0; i < currentSize; i++) {

        if (registry[i].bill > minAmount) {

            displayAll(&registry[i]);

            *totalDue += registry[i].bill;

            // WRONG:
            // *(totalDue) += registry[i].bill - minAmount;
            //
            // The question asks for the SUM OF THE DUES.
            // It does not ask for bill - minAmount.

            count++;
        }
    }

    return count;

    // WRONG:
    // printf("total due found %f\n", *(totalDue));
    //
    // This function should calculate/return information.
    // main should print the result.
}

int savePatientsToFile(const Patient registry[],
                       int currentSize,
                       const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return -1;
    }

    int line = 0;

    for (int i = 0; i < currentSize; i++) {

        fprintf(file,
                "%d|%s|%s|%d|%d|%.2f\n",
                registry[i].id,
                registry[i].pname,
                registry[i].dname,
                registry[i].wno,
                registry[i].age,
                registry[i].bill);

        line++;
    }

    fclose(file);

    return line;

    /*
    WRONG:

    fprintf(file,"%d|%s|%s|%d|%d|%.2f", ...);

    You forgot \n.

    Without \n, multiple patients are written onto
    the same line.
    */
}

int loadPatientsFromFile(Patient registry[],
                         int maxSize,
                         const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return -1;
    }

    char line[400];
    int i = 0;
    int lineno = 0;

    while (fgets(line, sizeof(line), file) != NULL && i < maxSize) {

        lineno++;

        Patient temp;

        int n = sscanf(line,
                       "%d|%40[^|]|%30[^|]|%d|%d|%f",
                       &temp.id,
                       temp.pname,
                       temp.dname,
                       &temp.wno,
                       &temp.age,
                       &temp.bill);

        if (n == 6) {

            registry[i] = temp;
            i++;

        } else {

            printf("warning: skipping bad line %d\n", lineno);
        }
    }

    fclose(file);

    return i;

    /*
    WRONG:

    while(fgets(line, sizeof(line), file)!=NULL && i<=maxSize)

    You used <=.

    If maxSize is 100, valid indexes are:

    0 ... 99

    But i <= 100 allows:

    registry[100]

    which is outside the array.

    Correct:

    i < maxSize


    WRONG:

    if(n==6) i++;
    printf("warning skipping bad lines %d\n", lineno);

    This prints the warning for EVERY line,
    including good lines.

    The warning belongs inside the else.


    I also used a temporary Patient.

    That way, a bad line does not partially overwrite
    registry[i].
    */
}

int recordPayment(Patient registry[],
                  int currentSize,
                  int id,
                  float amount)
{
    for (int i = 0; i < currentSize; i++) {

        if (registry[i].id == id) {

            if (amount <= 0) {
                return 0;
            }

            if (amount > registry[i].bill) {
                return 0;
            }

            registry[i].bill -= amount;

            if (registry[i].bill < 0.005f) {
                registry[i].bill = 0;
            }

            return 1;
        }
    }

    return 0;

    /*
    WRONG:

    if(registry[i].id==id && registry[i].bill>=amount)

    This allows:

    amount = -50

    because:

    500 >= -50

    is true.

    Then:

    500 - (-50) = 550

    The patient's bill would INCREASE.

    So amount must be checked first.
    */
}

int findPatientIndex(const Patient registry[],
                     int currentSize,
                     int id)
{
    for (int i = 0; i < currentSize; i++) {

        if (registry[i].id == id) {
            return i;
        }
    }

    return -1;

    /*
    Your original version was basically correct.

    You don't actually need:

    if (currentSize > 0)

    because the for loop automatically runs zero times
    when currentSize is 0.
    */
}

int dischargePatient(Patient registry[],
                     int *currentSize,
                     int id)
{
    int index = findPatientIndex(registry, *currentSize, id);

    if (index == -1) {
        return 0;
    }

    if (registry[index].bill >= 0.005f) {
        return -1;
    }

    for (int i = index; i < *currentSize - 1; i++) {
        registry[i] = registry[i + 1];
    }

    (*currentSize)--;//aladavabe komabe loop er moddhe na kheyal kore lekhe lekhe pora

    return 1;

    /*
    WRONG:

    else {
        int starting = *(currentSize);

        for(int i=index; i<starting; i++){

            if(registry[i].bill==0 ){

                registry[i]=registry[i+1];

                currentSize--;

                return 1;
            }
        }
    }

    There were several problems here.

    1. You already found the patient's index.
       You should check:

       registry[index].bill

       NOT keep searching through registry[i].

    2. You only shift one patient.

       Deleting:

       A B C D
         ↑

       should become:

       A C D

       So C and D both need to move.

    3. Your loop could access:

       registry[starting]

       which is outside the active records.

    4. You wrote:

       currentSize--;

       But currentSize is a POINTER.

       That changes the pointer itself.

       You need:

       (*currentSize)--;

    5. We use:

       bill < 0.005

       as the "paid" check because floating-point
       values should not normally be compared with == 0.
    */
}

int main()
{
    Patient info[MAX];

    int c;
    int currentSize = 0;

    do {

        printf(".....................HOSPITAL........................\n");

        printf("1. Admit Patient\n"
               "2. Search Patients by Doctor\n"
               "3. Display All\n"
               "4. Unpaid Bills Report\n"
               "5. Record a Payment\n"
               "6. Discharge Patient\n"
               "7. Save to File\n"
               "8. Load from File\n"
               "0. Exit\n");

        scanf("%d", &c);

        switch (c) {

            case 1: {

                if (currentSize >= MAX) {
                    printf("registry is full\n");
                    break;
                }

                Patient temp;

                promptPatientDetails(&temp);

                int value = duplicate(info, currentSize, &temp);

                if (value != 1) {

                    info[currentSize] = temp;
                    currentSize++;

                } else {

                    printf("duplicate id found!\n");
                }

                break;

                /*
                WRONG:

                Patient temp;
                promptPatientDetails(&temp);

                int value=duplicate(...)

                ...

                info[currentSize]=temp;

                There was no check that currentSize < MAX.

                If currentSize == 100, you would write to:

                info[100]

                which is outside the array.

                The full-registry check must happen BEFORE
                asking for patient details.
                */
            }

            case 2: {

                Patient *results[MAX];

                char doct[31];

                printf("enter doctor\n");
                scanf(" %30[^\n]", doct);

                int value = findPatientByDoctor(info,
                                                currentSize,
                                                doct,
                                                results,
                                                MAX);

                if (value == 0) {

                    printf("no match found\n");
                    break;
                }

                printf("found %d matches\n", value);

                for (int i = 0; i < value; i++) {

                    displayAll(results[i]);
                }

                break;

                /*
                WRONG:

                Patient *results;

                You need an ARRAY of pointers:

                Patient *results[MAX];

                The function expects:

                Patient *results[]

                which means an array of Patient pointers.


                WRONG:

                &results

                You don't need & here.

                results already represents the array.


                WRONG:

                displayAll(&results[i]);

                results[i] is already a Patient pointer.

                Therefore:

                displayAll(results[i]);
                */
            }

            case 3: {

                if (currentSize > 0) {

                    for (int i = 0; i < currentSize; i++) {

                        printf("number %d\n", i);

                        displayAll(&info[i]);

                        printf("**************************\n");
                    }

                    break;

                } else {

                    printf("no data to show\n");
                }

                break;
            }

            case 4: {

                float min;

                printf("enter minimum amount\n");
                scanf("%f", &min);

                float totalDue;

                int value = getUnpaidList(info,
                                          currentSize,
                                          min,
                                          &totalDue);

                if (value == 0) {

                    printf("no due found\n");

                } else {

                    printf("%d patients found with due\n", value);
                    printf("Total due: %.2f\n", totalDue);
                }

                break;
            }

            case 5: {

                int uid;
                float tk;

                printf("enter id to search\n");
                scanf("%d", &uid);

                printf("enter amount to pay\n");
                scanf("%f", &tk);

                int value = recordPayment(info,
                                          currentSize,
                                          uid,
                                          tk);

                if (value == 1) {

                    printf("payment successful\n");

                } else {

                    printf("payment failed\n");
                }

                break;
            }

            case 6: {

                int uid;

                printf("enter id\n");
                scanf("%d", &uid);

                int value = dischargePatient(info,
                                             &currentSize,
                                             uid);

                if (value == -1) {

                    printf("patient still owes money\n");

                } else if (value == 0) {

                    printf("patient not found\n");

                } else {

                    printf("discharged\n");
                }

                break;
            }

            case 7: {

                int value = savePatientsToFile(info,
                                               currentSize,
                                               "saved.txt");

                if (value == -1) {

                    printf("could not save file\n");

                } else {

                    printf("file saved successfully named "
                           "saved.txt with %d lines\n", value);
                }

                break;
            }

            case 8: {

                int value = loadPatientsFromFile(info,
                                                 MAX,
                                                 "saved.txt");

                if (value == -1) {

                    printf("could not load from file\n");

                } else {

                    currentSize = value;

                    printf("loaded successfully from %d lines\n",
                           value);
                }

                break;

                /*
                WRONG:

                int value=loadPatientsFromFile(...);

                printf("loaded successfully...");

                You forgot:

                currentSize = value;

                Without this, the records may be loaded into
                the array but main doesn't know how many
                records are there.

                Example:

                file has 5 patients
                value = 5
                currentSize = 0

                Display All would still think the registry
                is empty.

                Also, if value == -1, DO NOT change
                currentSize. That satisfies the failed-load
                requirement.
                */
            }

            case 0:

                printf("goodbye!\n");
                break;

            default:

                printf("invalid keyword\n");
        }

    } while (c != 0);

    return 0;
}
