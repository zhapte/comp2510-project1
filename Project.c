//
// Created by Lucas Liu on 2/13/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the max number of patient to be added to the record
#define MAX_PATIENT 50

int     patientId[MAX_PATIENT];
char    name[MAX_PATIENT][50];
int     age[MAX_PATIENT];
char    diagnosis[MAX_PATIENT][100];
int     roomNumber[MAX_PATIENT];

//array to keep track of number of position available in the hospital
int     avail[MAX_PATIENT];
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
    if (availNum >= MAX_PATIENT - 1) {
        printf("Hospital Full\n");
        return;
    }
    printf("Please enter patient ID:\n");
    scanf("%d", &patient[current].patientId);
    getchar();
}


//function to pupulate the array of available spots in the hospital
void initializedHospital() {
    for (int i = 0; i < MAX_PATIENT; i++) {
        avail[i] = i;
    }
}

