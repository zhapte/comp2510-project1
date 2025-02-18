#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

//simple stack structure to keep track of the patients
typedef struct {
    int position[MAX_PATIENT];
    int top;
}Stack;

//create the patient record in the global scope
Patient patient[MAX_PATIENT];

//create the stack for patient position at the global level
Stack position;

//id to keep track of the patient
int currentid;

//function prototype.
void addPatient();
void displayPatient();
void dischargePatient();
void searchPatient();
int findPosition(int input);
void initializeStack(Stack *stack);
void initializeSchedule();
void displaySchedule();
void assignDoctor();
void doctorScheduleMenu();
void menu();
int numberInput();

//main to display
int main(void) {
    //initialize the stack for the position of the hosipital
    initializeStack(&position);
    //start the hopital with id of 1 ensure that id will be unique.
    currentid = 1;
    //call the menu;
    menu();
    return 0;
}

void menu() {
    //choice for the user to enter
    int choice;
    do {

        //prompt th user to choices
        printf("Welcome to the Patient Management System.\n");
        printf("1. Add Patient\n");
        printf("2. Display Patient\n");
        printf("3. Discharge Patient\n");
        printf("4. Search Patient\n");
        printf("5. Doctor Scheduling\n");
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
                initializeSchedule();
                doctorScheduleMenu();
            break;
            case 6:
                printf("Exiting.");
                return;
            default:
                printf("invalid Choice try again!\n");
            break;
        }
    }while(choice != 6);
}

// function to add a patient to the record
void addPatient() {
    if (position.top < 0) {
        printf("Hospital is full is full!\n");
        return;
    }
    //pop out the top most position to put the patient in to the struct
    int current = position.position[position.top];
    position.top--;

    //auto increment patient id is assigned.
    printf("The Patient id is %d\n", currentid);
    patient[current].patientId = currentid;
    currentid++;

    //take patient name and store in the struct
    printf("Please enter patient name:\n");
    fgets(patient[current].name, 50,stdin);


    printf("Please enter patient age:\n");
    patient[current].age = numberInput();

    //take the patient diagnosis
    printf("Please enter patient diagnosis:\n");
    fgets(patient[current].diagnosis, 50,stdin);

    //take the roomnumber for the patient
    printf("Please enter patient room number:\n");
    patient[current].roomNumber = numberInput();

    printf("Patient added successfuly\n2");

}

void displayPatient() {
    //if the current ID is 1 it means that no patient is added print a meaningful message
    if (currentid == 1) {
        printf("No patient enrolled yet\n");
        return;
    }

    //loops through the entire array to print out every patient if the patient id is a positive number
    for (int i = 0; i < MAX_PATIENT; i++) {
        if (patient[i].patientId > 0) {
            printf("Patient ID: %-10d Patient Name: %-20s Age: %-5d Diagnosis: %-20s Room Number: %-10d\n",
               patient[i].patientId, patient[i].name, patient[i].age,
               patient[i].diagnosis, patient[i].roomNumber);
        }

    }
}

void dischargePatient() {
    //if the current ID is 1 it means that no patient is added print a meaningful message
    if (currentid == 1) {
        printf("No patient enrolled yet\n");
        return;
    }

    //choice of the user input and the index to look for in the array.
    int choice, index;

    //take user input
    printf("enter the patient id to discharge\n");
    choice = numberInput();

    //call the method to find the position in the hopital.
    index = findPosition(choice);

    //if index is negative number it means patient is not found print a message and return.
    if (index < 0) {
        printf("Patient ID is not found!\n");
        return;
    }

    //once patient is found set the id to -1 so that the system won't print or interact with it
    patient[index].patientId = -1;

    //push the position into the stack
    position.top++;
    position.position[position.top] = index;
    printf("Patient discharged successfully");
}

void searchPatient() {
    //if the current ID is 1 it means that no patient is added print a meaningful message
    if (currentid == 1) {
        printf("No patient enrolled yet\n");
        return;
    }

    //take user input via helper method
    int input, index;
    printf("Please enter patient ID:\n");
    input =  numberInput();

    //find the position in the struct
    index = findPosition(input);

    // if patient is not fund print a message
    if (index < 0) {
        printf("Patient ID not found!\n");
        return;
    }

    //print out the patient details
    printf("%-10d %-20s %-5d %-20s %-10d\n",
               patient[index].patientId, patient[index].name, patient[index].age,
               patient[index].diagnosis, patient[index].roomNumber);

}


int findPosition(int input) {
    //loop through the entire struct to find the patient position in the stuct.
    for (int i = 0; i < MAX_PATIENT; i++) {
        if (patient[i].patientId == input) {
            return i;
        }
    }
    //return -1 if patient is not found in the system.
    return -1;
}

//initialize the stack with all the available spots in the sturct
void initializeStack(Stack *stack) {
    stack->top = 49;
    int j = 0;
    for (int i = 49; i >= 0; i--) {
        stack->position[i] = j;
        j++;
    }
}

int numberInput() {
    int flag = 1;
    char input[50];
    int num;

    while (flag) {
        fgets(input, sizeof(input), stdin); // Read input as a string

        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';

        // Validate if input is a positive integer (only digits, no negatives or floating points)
        int validNum = 1;
        for (int i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                validNum = 0;
                break;
            }
        }

        if (validNum && strlen(input) > 0) {
            num = atoi(input); // Convert valid input to integer
            if (num > 0) {  // Ensure number is positive
                return num;
            }
        }

        printf("Invalid Entry! Try Again.\n");
    }

}

/*
 * This function initializes the schedule,
 * the function sets all shifts as 'unassigned' to begin
 */
void initializeSchedule() {
    for (int i = 0; i < DAYS; i++) {
        for (int j = 0; j < SHIFTS; j++) {
            if (doctorSchedule[i][j][0] == '\0') {
                strcpy(doctorSchedule[i][j], "Unassigned");
            }
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

    while (1) {
        printf("\nSelect a day (1-Monday to 7-Sunday): ");
        if (scanf("%d", &day) == 1 && day >= 1 && day <= 7) {
            break;
        } else {
            printf("Invalid day! Enter a number between 1 and 7.\n");
            while (getchar() != '\n');
        }
    }

    while (1) {
        printf("\nSelect a shift (1-Morning, 2-Afternoon, 3-Evening): ");
        if (scanf("%d", &shift) == 1 && shift >= 1 && shift <= 3) {
            break;
        } else {
            printf("Invalid shift! Enter a number between 1 and 3\n");
            while (getchar() != '\n');
        }

    }

    while (getchar() != '\n');
    printf("Enter a doctor's name: ");
    fgets(doctorName, 50, stdin);
    doctorName[strcspn(doctorName, "\n")] = 0;
   // scanf("%[^\n]s", &doctorName);

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


