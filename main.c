#include <stdio.h>
#include <stdlib.h>

//the max number of patient to be added to the record
#define MAX_PATIENT 50

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
//void searchPatient();


int main(void) {
    //the choice the user would make for the system
    int choice;
    do {

        //prompt th user to choices
        printf("Welcome to the Patient Management System.");
        printf("1. Add Patient\n");
        printf("2. Display Patient\n");
        printf("3. Discharge Patient\n");
        printf("4. Search Patient\n");
        printf("5. Exit\n");
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
                //searchPatient();
                break;
            case 5:
                printf("Bye");
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
    printf("discharge");
    return;
}