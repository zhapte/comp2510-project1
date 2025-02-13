#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the max number of patient to be added to the record
#define MAX_PATIENT 50

// the number of days in a week: used for the doctors schedule
#define DAYS 7
// the number of shifts in a day: morning, afternoon, evening
#define SHIFTS 3

char doctorSchedule[DAYS][SHIFTS][50];

// structure of the patient record
typedef struct{
    int     patientId;
    char    name[50];
    int     age;
    char    diagnosis[100];
    int     roomNumber;
}Patient;

//create the patient record in the global scope
Patient patient[MAX_PATIENT];

//global scope counter for the number of patient to be added
int patientCounter = 0;

void addPatient();
void displayPatient();
void dischargePatient();
void searchPatient();

void initializeSchedule();
void displaySchedule();
void assignDoctor();
void doctorScheduleMenu();

int main(void) {
    //the choice the user would make for the system
    int choice;
    //while loop to keep the user selecting different option
    do {

        //prompt th user to choices
        printf("Welcome to the Patient Management System.\n");
        printf("1. Add Patient\n");
        printf("2. Display Patient\n");
        printf("3. Discharge Patient\n");
        printf("4. Search Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Exit\n");
        scanf("%d", &choice);
        getchar();

        // switch case to cycle through the choices
        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                displayPatient() ;
                break;
            case 3:
                dischargePatient();
                break;
            case 4:
                searchPatient();
                break;
            case 5:
                doctorScheduleMenu();
                break;
            case 6:
                printf("Exiting.");
                return 0;
            default:
                printf("invalid Choice try again!\n");
                break;
        }
    }while(choice != 5);
    return 0;
}

// function to add a patiet to the record
void addPatient() {
    if (patientCounter >= MAX_PATIENT) {
        printf("Hospital is full is full!\n");
        return;
    }

    printf("Please enter patient ID:\n");
    scanf("%d", &patient[patientCounter].patientId);
    getchar();

    printf("Please enter patient name:\n");
    scanf("%s", &patient[patientCounter].name);
    getchar();

    printf("Please enter patient age:\n");
    scanf("%d", &patient[patientCounter].age);
    getchar();

    printf("Please enter patient diagnosis:\n");
    scanf("%s", &patient[patientCounter].diagnosis);
    getchar();

    printf("Please enter patient room number:\n");
    scanf("%d", &patient[patientCounter].roomNumber);
    getchar();

    //increase the patient count
    patientCounter++;
    return;

}

void displayPatient() {
    for (int i = 0; i < patientCounter; i++) {
        printf("%-10d %-20s %-5d %-20s %-10d\n",
               patient[i].patientId, patient[i].name, patient[i].age,
               patient[i].diagnosis, patient[i].roomNumber);
    }
}

void dischargePatient() {
    printf("discharge\n");
}

void searchPatient() {
    printf("Please enter patient ID:\n");
}

/* This function initializes the schedule,
 * the function sets all shifts as 'unassigned' to begin
 */
void initializeSchedule() {
    for (int i = 0; i < DAYS; i++) {
        for (int j = 0; j < SHIFTS; j++) {
            strcpy(doctorSchedule[i][j], "Unassigned");
        }
    }
}

// This function displays the doctor's schedule
void displaySchedule() {
    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                    "Friday", "Saturday", "Sunday"};

    char *shifts[] = {"Morning", "Afternoon", "Evening"};

    printf("\nDoctor Weekly Schedule: \n");
    for (int i = 0; i < DAYS; i++) {
        printf("\n%s\n", days[i]);
        for (int j = 0; j < SHIFTS; j++) {
            printf("%s: %s\n", shifts[j], doctorSchedule[i][j]);
        }
    }
}

// Function to assign a doctor to a shift
void assignDoctor() {
    int day;
    int shift;
    char doctorName[50];

    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                    "Friday", "Saturday", "Sunday"};
    char *shifts[] = {"Morning", "Afternoon", "Evening"};

    printf("\nSelect a day (1-Monday to 7-Sunday): ");
    scanf("%d", &day);
    if (day < 1 || day > 7) {
        printf("Invalid day!\n");
        return;
    }

    printf("Select a shift (1-Morning, 2-Afternoon, 3-Evening): ");
    scanf("%d", &shift);
    if (shift < 1 || shift > 3) {
        printf("Invalid shift!\n");
        return;
    }
    printf("Enter a doctor's name: ");
    scanf("%s", &doctorName);

    strcpy(doctorSchedule[day - 1][shift - 1], doctorName);

    printf("Doctor %s assigned to %s shift on %s.\n", doctorName, shifts[shift - 1], days[day - 1]);
}

// The menu for the doctor schedule management
void doctorScheduleMenu() {
    int choice;

    do {
        printf("\n Doctor Schedule Management\n");
        printf("1. Assign a doctor to a shift.\n");
        printf("2. View Weekly Schedule\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                assignDoctor();
                break;
            case 2:
                displaySchedule();
                break;
            case 3:
                printf("Exiting Doctor Schedule Management.\n");
                return;
            default:
                printf("Invalid Choice try again!\n");
        }
    } while (choice != 3);
}


