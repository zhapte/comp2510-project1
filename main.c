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

//simple stack structure to keep track of the patients
typedef struct {
    int position[MAX_PATIENT];
    int top;
}Stack;

//create the patient record in the global scope
Patient patient[MAX_PATIENT];

//create the stack for patient position at the global level
Stack position;


void addPatient();
void displayPatient();
void dischargePatient();
void searchPatient();
void initializeStack(Stack *stack);


int main(void) {
    initializeStack(&position);
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
                searchPatient();
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
    if (position.top < 0) {
        printf("Hospital is full is full!\n");
        return;
    }
    int current = position.position[position.top];
    position.top--;
    printf("Please enter patient ID:\n");
    scanf("%d", &patient[current].patientId);
    getchar();

    printf("Please enter patient name:\n");
    scanf("%s", &patient[current].name);
    getchar();

    printf("Please enter patient age:\n");
    scanf("%d", &patient[current].age);
    getchar();

    printf("Please enter patient diagnosis:\n");
    scanf("%s", &patient[current].diagnosis);
    getchar();

    printf("Please enter patient room number:\n");
    scanf("%d", &patient[current].roomNumber);
    getchar();

    //increase the patient count
    return;

}

void displayPatient() {
    for (int i = 0; i < MAX_PATIENT; i++) {
        if (patient[i].patientId > 0) {
            printf("%-10d %-20s %-5d %-20s %-10d\n",
               patient[i].patientId, patient[i].name, patient[i].age,
               patient[i].diagnosis, patient[i].roomNumber);
        }

    }
}

void dischargePatient() {
    printf("discharge\n");
}

void searchPatient() {
    printf("Please enter patient ID:\n");
}

void initializeStack(Stack *stack) {
    stack->top = 49;
    int j = 0;
    for (int i = 49; i >= 0; i--) {
        stack->position[i] = j;
        j++;
    }
}