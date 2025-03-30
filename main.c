#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//the max number of patient to be added to the record
#define INITIAL_CAPACITY 10

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

//keep a list of records and keep track of the count and capcity to dynamically increase and decrese the size
typedef struct {
    Patient *records;
    int count;
    int capacity;
} PatientList;

Patient *patients = NULL;
int patientCount = 0;
int patientCapacity = INITIAL_CAPACITY; // Start with 10 or so


//create a patient list in the global level
PatientList patientList;



//function prototype.
void addPatient();
void displayPatient();
void dischargePatient();
void searchPatient();
int findPosition(int input);
void initializeSchedule();
void displaySchedule();
void assignDoctor();
void doctorScheduleMenu();
void menu();
int numberInput();
void initializePatientList(PatientList *list);
void loadPatientsFromFile(const char *filename);
void savePatientsToFile(const char *filename);
void loadDoctorScheduleFromFile(const char *filename);
void saveDoctorScheduleToFile(const char *filename);


//main to display
int main(void) {

    //initialize the patient list
    initializePatientList(&patientList);
    //load doctor schedule from file
    loadDoctorScheduleFromFile("schedule.txt");
    //load patients from file
    loadPatientsFromFile("patients.txt");
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
                printf("Saving and exiting...\n");
                savePatientsToFile("patients.txt");
                saveDoctorScheduleToFile("schedule.txt");
                return;
            default:
                printf("invalid Choice try again!\n");
            break;
        }
    }while(choice != 6);
    free(patientList.records);

}

// function to add a patient to the record
void addPatient() {
    if (patientList.count == patientList.capacity) {
        int newCapacity = patientList.capacity * 2;
        Patient *temp = realloc(patientList.records, newCapacity * sizeof(Patient));
        if (temp == NULL) {
            perror("Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
        patientList.records = temp;
        patientList.capacity = newCapacity;
    }

    Patient *newPatient = &patientList.records[patientList.count];
    newPatient->patientId = patientList.count + 1;

    printf("Adding Patient ID: %d\n", newPatient->patientId);

    printf("Enter patient name: ");
    fgets(newPatient->name, 50, stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = '\0';

    printf("Enter patient age: ");
    newPatient->age = numberInput();

    printf("Enter patient diagnosis: ");
    fgets(newPatient->diagnosis, 100, stdin);
    newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = '\0';

    printf("Enter patient room number: ");
    newPatient->roomNumber = numberInput();

    patientList.count++;
    printf("Patient added successfully.\n");
}

void displayPatient() {
    if (patientList.count == 0) {
        printf("No patients enrolled yet.\n");
        return;
    }

    for (int i = 0; i < patientList.count; i++) {
        Patient p = patientList.records[i];
        if (p.patientId != 0) {
            printf("Patient ID: %-10d\tName: %-20s\tAge: %-5d\tDiagnosis: %-20s\tRoom Number: %-10d\n",
                        p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);
        }
    }
}

void dischargePatient() {
    //if the current ID is 1 it means that no patient is added print a meaningful message
    if (patientList.count == 0) {
        printf("No patients enrolled yet.\n");
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

    Patient p = patientList.records[index];
    printf("Patient ID: %d Name: %s Age: %d Diagnosis: %s Room Number: %d is now discharged\n",
           p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);
    patientList.records[index].patientId = 0;


}

void searchPatient() {
    //if the current ID is 1 it means that no patient is added print a meaningful message
    if (patientList.count == 0) {
        printf("No patients enrolled yet.\n");
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

    Patient p = patientList.records[index];
    printf("Patient ID: %-10d\tName: %-20s\tAge: %-5d\tDiagnosis: %-20s\tRoom Number: %-10d\n",
           p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);

}


int findPosition(int input) {
    //loop through the entire struct to find the patient position in the stuct.
    for (int i = 0; i < patientList.count; i++) {
        if (patientList.records[i].patientId == input) {
            return i;
        }
    }
    //return -1 if patient is not found in the system.
    return -1;
}

void initializePatientList(PatientList *list)
{
    list->records = malloc(INITIAL_CAPACITY * sizeof(Patient));
    if (list->records == NULL) {
        perror("Failed to allocate memory for patient records");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

//helper method to take only positive number as input and reject all string and negative numbers or floating point
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

void savePatientsToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing patient data.\n");
        return;
    }

    for (int i = 0; i < patientList.count; i++) {
        Patient p = patientList.records[i];
        if (p.patientId != 0) {
            fprintf(file, "%d|%s|%d|%s|%d\n",
                    p.patientId,
                    p.name,
                    p.age,
                    p.diagnosis,
                    p.roomNumber);
        }
    }

    fclose(file);
    printf("Patient data saved to file.\n");
}


void loadPatientsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No saved patient data found.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (patientList.count >= patientList.capacity) {
            patientList.capacity *= 2;
            Patient *temp = realloc(patientList.records, patientList.capacity * sizeof(Patient));
            if (temp == NULL) {
                printf("Memory allocation failed while loading patients.\n");
                fclose(file);
                return;
            }
            patientList.records = temp;
        }

        Patient *p = &patientList.records[patientList.count];

        char *token = strtok(line, "|");
        p->patientId = atoi(token);

        token = strtok(NULL, "|");
        strcpy(p->name, token);

        token = strtok(NULL, "|");
        p->age = atoi(token);

        token = strtok(NULL, "|");
        strcpy(p->diagnosis, token);

        token = strtok(NULL, "|");
        p->roomNumber = atoi(token);

        patientList.count++;
    }

    fclose(file);
    printf("Patient data loaded from file.\n");
}

void saveDoctorScheduleToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing doctor schedule\n");
        return;
    }

    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                    "Saturday", "Sunday"};
    char *shifts[] = {"Morning", "Afternoon", "Evening"};

    for (int i = 0; i < DAYS; i++) {
        for (int j = 0; j < SHIFTS; j++) {
            fprintf(file, "%s|%s|%s\n",
                days[i], shifts[j],doctorSchedule[i][j]);
        }
    }

    fclose(file);
    printf("Doctor schedule saved to file.\n");
}

void loadDoctorScheduleFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No saved doctor schedule found.\n");
        initializeSchedule();
        return;
    }

    char line[150], day[20], shift[20], name[50];
    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                    "Saturday", "Sunday"};
    char *shifts[] = {"Morning", "Afternoon", "Evening"};

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]%[^\n]", day, shift, name);

        int dayIndex = -1;
        int shiftIndex = -1;
        for (int i = 0; i < DAYS; i++) {
            if (strcmp(day, days[i]) == 0) {
                dayIndex = i;
            }
        }
        for (int j = 0; j < SHIFTS; j++) {
            if (strcmp(shift, shifts[j]) == 0) {
                shiftIndex = j;
            }
        }
        if (dayIndex != -1 && shiftIndex != -1) {
            strncpy(doctorSchedule[dayIndex][shiftIndex], name, 50);
        }
    }
    fclose(file);
    printf("Doctor schedule loaded from file.\n");

}





