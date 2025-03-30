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


typedef struct PatientNode {
    Patient data;
    struct PatientNode *next;
} PatientNode;


int currentId = 1;
PatientNode *head = NULL;





//function prototype.
void addPatient(PatientNode **head, int id);
void displayPatient(PatientNode *head);
void dischargePatient(PatientNode **head);
void searchPatient(PatientNode *head);
int findPosition(int input);
void initializeSchedule();
void displaySchedule();
void assignDoctor();
void doctorScheduleMenu();
void menu();
int numberInput();
void loadPatientsFromFile(const char *filename, PatientNode **head);
void savePatientsToFile(const char *filename, PatientNode *head);
void loadDoctorScheduleFromFile(const char *filename);
void saveDoctorScheduleToFile(const char *filename);
PatientNode* createPatientNode(int id);
void freePatientList(PatientNode *head);
int getLastId(PatientNode *head) {
    if (head == NULL) {
        return 1;  // Or another value indicating an empty list.
    }
    int maxId = 1;
    PatientNode *current = head;
    while (current != NULL) {
        if (current->data.patientId > maxId) {
            maxId = current->data.patientId;
        }
        current = current->next;
    }
    return maxId + 1;
}



//main to display
int main(void) {

    //initialize the patient list
    //load doctor schedule from file
    loadDoctorScheduleFromFile("schedule.txt");
    //load patients from file
    loadPatientsFromFile("patients.txt", &head);
    currentId = getLastId(head);
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
                addPatient(&head, currentId);
                currentId++;
                break;
            case 2:
                displayPatient(head) ;
            break;
            case 3:
                dischargePatient(&head);
            break;
            case 4:
                searchPatient(head);
            break;
            case 5:
                initializeSchedule();
                doctorScheduleMenu();
                break;
            case 6:
                printf("Saving and exiting...\n");
                savePatientsToFile("patients.txt", head);
                saveDoctorScheduleToFile("schedule.txt");
                freePatientList(head);
                return;
            default:
                printf("invalid Choice try again!\n");
            break;
        }
    }while(choice != 6);
}

PatientNode* createPatientNode(int id) {
    PatientNode *newNode = malloc(sizeof(PatientNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new patient node");
        exit(EXIT_FAILURE);
    }
    newNode->data.patientId = id;
    newNode->next = NULL;
    return newNode;
}

// function to add a patient to the record
void addPatient(PatientNode **head, int id) {
    PatientNode *newNode = createPatientNode(id);

    printf("Adding Patient ID: %d\n", newNode->data.patientId);

    // Get patient details
    printf("Enter patient name: ");
    fgets(newNode->data.name, 50, stdin);
    newNode->data.name[strcspn(newNode->data.name, "\n")] = '\0';

    printf("Enter patient age: ");
    newNode->data.age = numberInput();


    printf("Enter patient diagnosis: ");
    fgets(newNode->data.diagnosis, 100, stdin);
    newNode->data.diagnosis[strcspn(newNode->data.diagnosis, "\n")] = '\0';

    printf("Enter patient room number: ");
    newNode->data.roomNumber = numberInput();

    // Insert the new node at the end of the list
    if (*head == NULL) {
        *head = newNode;
    } else {
        PatientNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    printf("Patient added successfully.\n");
}

void displayPatient(PatientNode *head) {
    if (head == NULL) {
        printf("No patients enrolled yet.\n");
        return;
    }

    PatientNode *current = head;
    while (current != NULL){
        Patient p = current->data;
        printf("Patient ID: %-10d\tName: %-20s\tAge: %-5d\tDiagnosis: %-20s\tRoom Number: %-10d\n",
                        p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);
        current = current->next;
    }
}

void dischargePatient(PatientNode **head) {
    if (head == NULL) {
        printf("No patients enrolled yet.\n");
        return;
    }

    PatientNode *current = *head;
    PatientNode *prev = NULL;

    printf("Please enter the id of the patient");
    int id = numberInput();

    while (current != NULL) {
        if (current->data.patientId == id) {
            // Patient found; adjust pointers to remove the node
            if (prev == NULL) {
                // Removing the head node
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Patient with ID %d has been discharged successfully.\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }

    // If we reach here, the patient was not found
    printf("Patient with ID %d not found.\n", id);
}

void searchPatient(PatientNode *head) {
    if (head == NULL) {
        printf("No patients enrolled yet.\n");
        return;
    }
    PatientNode *current = head;
    printf("Please enter the id of the patient");
    int id = numberInput();

    while (current != NULL) {
        if (current->data.patientId == id) {
            Patient p = current->data;
            printf("Patient ID: %-10d\tName: %-20s\tAge: %-5d\tDiagnosis: %-20s\tRoom Number: %-10d\n",
                            p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);
            return;
        }
        current = current->next;
    }
    printf("Patient with ID %d not found.\n", id);

}

void freePatientList(PatientNode *head) {
    PatientNode *current = head;
    while (current != NULL) {
        PatientNode *temp = current;
        current = current->next;
        free(temp);
    }
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

void savePatientsToFile(const char *filename, PatientNode *head) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing patient data.\n");
        return;
    }
    PatientNode *current = head;
    while (current != NULL) {
        Patient p = current->data;
        fprintf(file, "%d|%s|%d|%s|%d\n",
                p.patientId,
                p.name,
                p.age,
                p.diagnosis,
                p.roomNumber);
        current = current->next;
    }
    fclose(file);
    printf("Patient data saved to file.\n");
}


void loadPatientsFromFile(const char *filename, PatientNode **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No saved patient data found.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline if present.
        line[strcspn(line, "\n")] = '\0';

        // Allocate a new node.
        PatientNode *newNode = malloc(sizeof(PatientNode));
        if (newNode == NULL) {
            perror("Failed to allocate memory for new patient node");
            fclose(file);
            return;
        }
        newNode->next = NULL;

        // Parse the line using strtok.
        char *token = strtok(line, "|");
        if (token != NULL)
            newNode->data.patientId = atoi(token);

        token = strtok(NULL, "|");
        if (token != NULL)
            strcpy(newNode->data.name, token);

        token = strtok(NULL, "|");
        if (token != NULL)
            newNode->data.age = atoi(token);

        token = strtok(NULL, "|");
        if (token != NULL)
            strcpy(newNode->data.diagnosis, token);

        token = strtok(NULL, "|");
        if (token != NULL)
            newNode->data.roomNumber = atoi(token);

        // Insert the new node at the end of the linked list.
        if (*head == NULL) {
            *head = newNode;
        } else {
            PatientNode *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
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





