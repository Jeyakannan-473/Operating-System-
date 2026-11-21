/* Lab-16: Random access file for processing employee details (fseek, fread, fwrite) */
#include <stdio.h>

#define MAX_EMP 100
#define FILE_NAME "employee.dat"

struct employee {
    int id;                 /* 0 means the record slot is empty */
    char name[30];
    float salary;
};

/* Read the record of employee 'id' directly with fseek; returns 1 if it exists */
static int read_record(FILE *fp, int id, struct employee *emp)
{
    fseek(fp, (long)(id - 1) * sizeof(struct employee), SEEK_SET);
    if (fread(emp, sizeof(struct employee), 1, fp) != 1)
        return 0;
    return emp->id == id;
}

static void write_record(FILE *fp, const struct employee *emp)
{
    fseek(fp, (long)(emp->id - 1) * sizeof(struct employee), SEEK_SET);
    fwrite(emp, sizeof(struct employee), 1, fp);
    fflush(fp);
}

int main(void)
{
    FILE *fp;
    struct employee emp, old;
    int choice;

    fp = fopen(FILE_NAME, "rb+");
    if (fp == NULL)
        fp = fopen(FILE_NAME, "wb+");
    if (fp == NULL) {
        printf("Error opening the file\n");
        return 1;
    }

    do {
        printf("\n1.Add 2.Display 3.Update 4.Exit - Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            break;

        if (choice >= 1 && choice <= 3) {
            printf("Enter Employee ID (1-%d): ", MAX_EMP);
            if (scanf("%d", &emp.id) != 1 || emp.id < 1 || emp.id > MAX_EMP) {
                printf("Invalid employee ID\n");
                continue;
            }
        }

        switch (choice) {
        case 1:
            if (read_record(fp, emp.id, &old)) {
                printf("Employee %d already exists, use Update\n", emp.id);
                break;
            }
            printf("Enter Employee Name: ");
            scanf("%29s", emp.name);
            printf("Enter Employee Salary: ");
            scanf("%f", &emp.salary);
            write_record(fp, &emp);
            printf("Employee details added successfully\n");
            break;
        case 2:
            if (!read_record(fp, emp.id, &old))
                printf("Employee %d not found\n", emp.id);
            else
                printf("ID: %d  Name: %s  Salary: %.2f\n", old.id, old.name, old.salary);
            break;
        case 3:
            if (!read_record(fp, emp.id, &old)) {
                printf("Employee %d not found\n", emp.id);
                break;
            }
            printf("Enter new Employee Name: ");
            scanf("%29s", emp.name);
            printf("Enter new Employee Salary: ");
            scanf("%f", &emp.salary);
            write_record(fp, &emp);
            printf("Employee details updated successfully\n");
            break;
        case 4:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 4);

    fclose(fp);
    return 0;
}
