//
// Created by Lucas Liu on 2/13/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the max number of patient to be added to the record
#define INITIAL_CAPACITY 50

int     patientId[INITIAL_CAPACITY];
char    name[INITIAL_CAPACITY][50];
int     age[INITIAL_CAPACITY];
char    diagnosis[INITIAL_CAPACITY][100];
int     roomNumber[INITIAL_CAPACITY];

//array to keep track of number of position available in the hospital
int     avail[INITIAL_CAPACITY];
int     availNum = 0;

//keep track of the number of the patient
int     totalPatients = 0;


void initializedHospital();
void addPatient();
void displayPatient();
void dischargePatient();
void searchPatient();

int main() {
    initializedHospital();

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
                //displayPatient() ;
            break;
            case 3:
                //dischargePatient();
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

void addPatient() {
    if (availNum >= INITIAL_CAPACITY - 1) {
        printf("Hospital Full\n");
        return;
    }
    printf("Please enter patient ID:\n");
    scanf("%d", );
    getchar();
}


//function to pupulate the array of available spots in the hospital
void initializedHospital() {
    for (int i = 0; i < INITIAL_CAPACITY; i++) {
        avail[i] = i;
    }
}

