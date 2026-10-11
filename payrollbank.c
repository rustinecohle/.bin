
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_EMPLOYEES 150

typedef struct {
    int id;
    char name[51];
    char department[16];
    float basicSalary;
    float allowancePercent;
    int joinYear;
} Employee;

/* Function declarations */
void promptEmployeeDetails(Employee *empPtr);
void displayEmployee(const Employee *empPtr);
int findID(const Employee registry[], int currentSize, int id);
int removeEmployee(Employee registry[], int *currentSize, int id);
int findEmployeesByDept(Employee registry[], int currentSize,
                        const char *dept, Employee *results[],
                        int maxResults);
float calculateGross(const Employee *empPtr);
float calculateTax(float gross);
int getPayslip(const Employee registry[], int currentSize, int id,
               float *gross, float *tax, float *net);
int printDepartmentSummary(const Employee registry[], int currentSize);
int saveRegistryToFile(const Employee registry[], int currentSize,
                       const char *filename);
int loadRegistryFromFile(Employee registry[], int maxSize,
                         const char *filename);
void printPayslip(const Employee registry[], int currentSize, int id);

/* File encoding helpers */
void encodeText(const char *src, char *dest);
int decodeText(const char *src, char *dest, int capacity);
int validEmployee(const Employee *empPtr);

/* Find employee ID */
int findID(const Employee registry[], int currentSize, int id)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].id == id)
            return i;
    }

    return -1;
}

/* Read employee details */
void promptEmployeeDetails(Employee *empPtr)
{
    printf("Enter employee ID: ");
    scanf("%d", &empPtr->id);

    printf("Enter employee name: ");
    scanf(" %50[^\n]", empPtr->name);

    printf("Enter department: ");
    scanf(" %15[^\n]", empPtr->department);

    do {
        printf("Enter basic salary: ");
        scanf("%f", &empPtr->basicSalary);

        if (empPtr->basicSalary <= 0)
            printf("Basic salary must be greater than 0!\n");

    } while (empPtr->basicSalary <= 0);

    do {
        printf("Enter allowance percentage: ");
        scanf("%f", &empPtr->allowancePercent);

        if (empPtr->allowancePercent < 0 ||
            empPtr->allowancePercent > 100)
            printf("Allowance must be between 0 and 100!\n");

    } while (empPtr->allowancePercent < 0 ||
             empPtr->allowancePercent > 100);

    do {
        printf("Enter join year: ");
        scanf("%d", &empPtr->joinYear);

        if (empPtr->joinYear < 1990 ||
            empPtr->joinYear > 2100)
            printf("Join year must be 1990 to 2100!\n");

    } while (empPtr->joinYear < 1990 ||
             empPtr->joinYear > 2100);
}

/* Validate record data */
int validEmployee(const Employee *empPtr)
{
    return empPtr->id > 0 &&
           empPtr->name[0] != '\0' &&
           empPtr->department[0] != '\0' &&
           empPtr->basicSalary > 0 &&
           empPtr->allowancePercent >= 0 &&
           empPtr->allowancePercent <= 100 &&
           empPtr->joinYear >= 1990 &&
           empPtr->joinYear <= 2100;
}

/* Display one employee */
void displayEmployee(const Employee *empPtr)
{
    printf("\nID: %d\n", empPtr->id);
    printf("Name: %s\n", empPtr->name);
    printf("Department: %s\n", empPtr->department);
    printf("Basic Salary: %.2f\n", empPtr->basicSalary);
    printf("Allowance: %.2f%%\n", empPtr->allowancePercent);
    printf("Join Year: %d\n", empPtr->joinYear);
    printf("Gross Salary: %.2f\n", calculateGross(empPtr));
}

/* Calculate gross salary */
float calculateGross(const Employee *empPtr)
{
    return empPtr->basicSalary +
           (empPtr->basicSalary * empPtr->allowancePercent / 100.0f);
}

/* Add progressive tax slab by slab */
float calculateTax(float gross)
{
    float tax = 0.0f;

    if (gross < 0)
        return 0.0f;

    if (gross > 30000.0f) {
        if (gross < 50000.0f)
            tax += (gross - 30000.0f) * 0.05f;
        else
            tax += 20000.0f * 0.05f;
    }

    if (gross > 50000.0f)
        tax += (gross - 50000.0f) * 0.10f;

    return tax;
}

/* Get payslip using output parameters */
int getPayslip(const Employee registry[], int currentSize, int id,
               float *gross, float *tax, float *net)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].id == id) {
            float g = calculateGross(&registry[i]);
            float t = calculateTax(g);

            /* Change outputs only after finding the employee */
            *gross = g;
            *tax = t;
            *net = g - t;

            return 1;
        }
    }

    return 0;
}

/* Print payslip */
void printPayslip(const Employee registry[], int currentSize, int id)
{
    float gross, tax, net;

    if (getPayslip(registry, currentSize, id,
                   &gross, &tax, &net)) {
        printf("\n----- PAYSLIP -----\n");
        printf("Employee ID: %d\n", id);
        printf("Gross Salary: %.2f\n", gross);
        printf("Tax: %.2f\n", tax);
        printf("Net Salary: %.2f\n", net);
    } else {
        printf("Employee not found!\n");
    }
}

/* Search employees by department */
int findEmployeesByDept(Employee registry[], int currentSize,
                        const char *dept, Employee *results[],
                        int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (strcmp(registry[i].department, dept) == 0) {
            if (count < maxResults) {
                results[count] = &registry[i];
                count++;
            }
        }
    }

    for (int i = 0; i < count; i++)
        displayEmployee(results[i]);

    return count;
}

/* Remove employee and preserve order */
int removeEmployee(Employee registry[], int *currentSize, int id)
{
    int index = findID(registry, *currentSize, id);

    if (index == -1)
        return 0;

    for (int i = index; i < *currentSize - 1; i++)
        registry[i] = registry[i + 1];

    (*currentSize)--;

    return 1;
}

/* Department summary without an extra array */
int printDepartmentSummary(const Employee registry[], int currentSize)
{
    int departments = 0;

    if (currentSize == 0) {
        printf("Registry is empty!\n");
        return 0;
    }

    for (int i = 0; i < currentSize; i++) {
        int alreadyPrinted = 0;

        /* Check whether this department appeared earlier */
        for (int j = 0; j < i; j++) {
            if (strcmp(registry[i].department,
                       registry[j].department) == 0) {
                alreadyPrinted = 1;
                break;
            }
        }

        if (alreadyPrinted)
            continue;

        int count = 0;
        float total = 0.0f;

        /* Count employees and calculate total gross salary */
        for (int j = 0; j < currentSize; j++) {
            if (strcmp(registry[i].department,
                       registry[j].department) == 0) {
                count++;
                total += calculateGross(&registry[j]);
            }
        }

        printf("\nDepartment: %s\n", registry[i].department);
        printf("Employees: %d\n", count);
        printf("Total Gross Salary: %.2f\n", total);
        printf("Average Gross Salary: %.2f\n", total / count);

        departments++;
    }

    return departments;
}

/* Convert text into hexadecimal */
void encodeText(const char *src, char *dest)
{
    const char hex[] = "0123456789ABCDEF";

    while (*src != '\0') {
        unsigned char ch = (unsigned char)*src++;

        *dest++ = hex[ch / 16];
        *dest++ = hex[ch % 16];
    }

    *dest = '\0';
}

/* Convert hexadecimal back into text */
int decodeText(const char *src, char *dest, int capacity)
{
    int len = (int)strlen(src);

    if (len % 2 != 0 || len / 2 >= capacity)
        return 0;

    for (int i = 0; i < len; i++) {
        if (!isxdigit((unsigned char)src[i]))
            return 0;
    }

    for (int i = 0; i < len; i += 2) {
        int high = isdigit((unsigned char)src[i])
                 ? src[i] - '0'
                 : toupper((unsigned char)src[i]) - 'A' + 10;

        int low = isdigit((unsigned char)src[i + 1])
                ? src[i + 1] - '0'
                : toupper((unsigned char)src[i + 1]) - 'A' + 10;

        int value = high * 16 + low;

        /* Names and departments must not contain embedded nulls */
        if (value == 0)
            return 0;

        dest[i / 2] = (char)value;
    }

    dest[len / 2] = '\0';
    return 1;
}

/* Save all employee records */
int saveRegistryToFile(const Employee registry[], int currentSize,
                       const char *filename)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
        return -1;

    int saved = 0;

    for (int i = 0; i < currentSize; i++) {
        char nameHex[101];
        char deptHex[31];

        encodeText(registry[i].name, nameHex);
        encodeText(registry[i].department, deptHex);

        if (fprintf(fp, "%d|%.9g|%.9g|%d|%s|%s\n",
                    registry[i].id,
                    (double)registry[i].basicSalary,
                    (double)registry[i].allowancePercent,
                    registry[i].joinYear,
                    nameHex, deptHex) < 0) {
            fclose(fp);
            return -1;
        }

        saved++;
    }

    if (fclose(fp) != 0)
        return -1;

    return saved;
}

/* Load and validate employee records */
int loadRegistryFromFile(Employee registry[], int maxSize,
                         const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    /* Temporary registry keeps old memory unchanged until success */
    Employee temp[MAX_EMPLOYEES];
    int count = 0;
    int lineNo = 0;
    int failed = 0;
    char line[512];

    if (maxSize < 0 || maxSize > MAX_EMPLOYEES) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNo++;

        /* Reject an overlong line rather than parsing a partial record */
        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int ch;

            while ((ch = fgetc(fp)) != '\n' && ch != EOF)
                ;

            printf("Warning: invalid line %d\n", lineNo);
            continue;
        }

        int id, year, pos = 0;
        float basic, allowance;
        char nameHex[101], deptHex[31];
        char extra;

        int fields = sscanf(line,
                            "%d|%f|%f|%d|%100[0-9A-Fa-f]|%30[0-9A-Fa-f]%n",
                            &id, &basic, &allowance, &year,
                            nameHex, deptHex, &pos);

        /* %n gives the position after the parsed record */
        if (fields != 6) {
            printf("Warning: invalid line %d\n", lineNo);
            continue;
        }

        /* Only whitespace may remain after a complete record */
        if (sscanf(line + pos, " %c", &extra) == 1) {
            printf("Warning: invalid line %d\n", lineNo);
            continue;
        }

        Employee emp;
        emp.id = id;
        emp.basicSalary = basic;
        emp.allowancePercent = allowance;
        emp.joinYear = year;

        /* Hex encoding protects names/departments containing | */
        if (!decodeText(nameHex, emp.name, sizeof(emp.name)) ||
            !decodeText(deptHex, emp.department,
                        sizeof(emp.department)) ||
            !validEmployee(&emp)) {
            printf("Warning: invalid line %d\n", lineNo);
            continue;
        }

        int duplicate = 0;

        for (int i = 0; i < count; i++) {
            if (temp[i].id == emp.id) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate) {
            printf("Warning: duplicate ID on line %d\n", lineNo);
            continue;
        }

        if (count >= maxSize) {
            printf("Warning: registry full at line %d\n", lineNo);
            continue;
        }

        temp[count] = emp;
        count++;
    }

    if (ferror(fp))
        failed = 1;

    fclose(fp);

    if (failed)
        return -1;

    /* Replace existing registry only after the load completes */
    for (int i = 0; i < count; i++)
        registry[i] = temp[i];

    return count;
}

/* Main menu */
int main(void)
{
    Employee registry[MAX_EMPLOYEES];
    Employee *results[MAX_EMPLOYEES];

    int currentSize = 0;
    int choice, id, count;
    char dept[16];

    do {
        printf("\n===== EMPLOYEE PAYROLL =====\n");
        printf("1. Add Employee\n");
        printf("2. Search by Department\n");
        printf("3. Display All\n");
        printf("4. Payslip for an Employee\n");
        printf("5. Department Summary\n");
        printf("6. Remove Employee\n");
        printf("7. Save to File\n");
        printf("8. Load from File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            if (currentSize >= MAX_EMPLOYEES) {
                printf("Registry is full!\n");
                break;
            }

            Employee temp;
            promptEmployeeDetails(&temp);

            if (temp.id <= 0) {
                printf("ID must be positive!\n");
                break;
            }

            if (findID(registry, currentSize, temp.id) != -1) {
                printf("Duplicate employee ID!\n");
                break;
            }

            registry[currentSize] = temp;
            currentSize++;

            printf("Employee added successfully!\n");
            break;
        }

        case 2:
            printf("Enter department: ");
            scanf(" %15[^\n]", dept);

            count = findEmployeesByDept(registry, currentSize,
                                        dept, results, MAX_EMPLOYEES);

            if (count == 0)
                printf("Department not found!\n");
            else
                printf("Total found: %d\n", count);
            break;

        case 3:
            if (currentSize == 0) {
                printf("Registry is empty!\n");
            } else {
                for (int i = 0; i < currentSize; i++)
                    displayEmployee(&registry[i]);
            }
            break;

        case 4:
            printf("Enter employee ID: ");
            scanf("%d", &id);
            printPayslip(registry, currentSize, id);
            break;

        case 5:
            count = printDepartmentSummary(registry, currentSize);
            printf("Different departments: %d\n", count);
            break;

        case 6:
            printf("Enter employee ID to remove: ");
            scanf("%d", &id);

            if (removeEmployee(registry, &currentSize, id))
                printf("Employee removed successfully!\n");
            else
                printf("Employee not found!\n");
            break;

        case 7:
            count = saveRegistryToFile(registry, currentSize,
                                       "employees.txt");

            if (count == -1)
                printf("Could not save file!\n");
            else
                printf("%d records saved.\n", count);
            break;

        case 8:
            count = loadRegistryFromFile(registry, MAX_EMPLOYEES,
                                         "employees.txt");

            if (count == -1)
                printf("Could not load file. Existing data unchanged.\n");
            else {
                currentSize = count;
                printf("%d records loaded.\n", count);
            }
            break;

        case 0:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
