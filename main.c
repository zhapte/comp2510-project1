#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #define MKDIR(dir) mkdir(dir, 0700)
#endif


//the max number of patient to be added to the record
#define INITIAL_CAPACITY 10
// the number of days in a week: used for the doctors schedule
#define DAYS 7
// the number of shifts in a day: morning, afternoon, evening
#define SHIFTS 3


// structure of the patient record
typedef struct{
    int     patientId;
    char    name[50];
    int     age;
    char    diagnosis[100];
    int     roomNumber;
    char    admissionDate[11];
}Patient;


//linked list Struct
typedef struct PatientNode {
    Patient data;
    struct PatientNode *next;
} PatientNode;


//global values to use through out the program.
int currentId = 1;
PatientNode *head = NULL;
char doctorSchedule[DAYS][SHIFTS][50];
int rooms[100] = {0};


//function prototype.
//patient management functions
void addPatient(PatientNode **head, int id);
void displayPatient(PatientNode *head);
void dischargePatient(PatientNode **head);
void searchPatient(PatientNode *head);
void freePatientList(PatientNode *head);
PatientNode* createPatientNode(int id);
int numberInput();
int isEmptyRoom(int roomnumber);


//doctors scheduling functinos
void initializeSchedule();
void displaySchedule();
void assignDoctor();


//menu fuctions
void menu();
void reportMenu();
void doctorScheduleMenu();


//functions to do file back up and restore fucntions
void loadPatientsFromFile(const char *filename, PatientNode **head);
void savePatientsToFile(const char *filename, PatientNode *head);
void loadDoctorScheduleFromFile(const char *filename);
void saveDoctorScheduleToFile(const char *filename);
void backupData();
void restorePatientsFromFile(const char *filename);
void restoreScheduleFromFile(const char *filename);
void listBackupFiles(const char *prefix);
void writeToDichargeReport(const char *patientName);
int getLastId(PatientNode *head);


//main function the entry point of the program
int main(void) {

    //load doctor schedule from file
    loadDoctorScheduleFromFile("schedule.txt");
    //load patients from file
    loadPatientsFromFile("patients.txt", &head);
    currentId = getLastId(head);
    //call the menu;
    menu();
    return 0;
}


//function to display the main menu
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
        printf("7. Backup Data\n");
        printf("8. Restore Patient Data\n");
        printf("9. Restore Doctor Schedule\n");
        printf("10. Reports and Analytics\n");
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
            case 7:
                backupData();
                break;
            case 8: {
                char filename[100];
                listBackupFiles("patients_");
                printf("Enter the exact filename to restore (e.g., patients_20250330_1430.txt): ");
                scanf("%s", filename);
                getchar();
                char fullPath[150];
                snprintf(fullPath, sizeof(fullPath), "backup/%s", filename);
                restorePatientsFromFile(fullPath);
            }
             break;
            case 9: {
                char filename[100];
                listBackupFiles("schedule_");
                printf("Enter the exact filename to restore (e.g., schedule_20250330_1430.txt): ");
                scanf("%s", filename);
                getchar();

                char fullPath[150];
                snprintf(fullPath, sizeof(fullPath), "backup/%s", filename);
                restoreScheduleFromFile(fullPath);
             }
                break;
            case 10:
                reportMenu();
                break;
            default:
                printf("invalid Choice try again!\n");
            break;
        }
    }while(choice != 6);
}


//returns a new node to add to the linked list
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

    //assign the global id to the patient
    printf("Adding Patient ID: %d\n", newNode->data.patientId);

    // Get patient details
    printf("Enter patient name: ");
    fgets(newNode->data.name, 50, stdin);
    newNode->data.name[strcspn(newNode->data.name, "\n")] = '\0';

    do {
        printf("Enter patient age 1 - 99: ");
        newNode->data.age = numberInput();
    }while (newNode->data.age >= 100);



    printf("Enter patient diagnosis: ");
    fgets(newNode->data.diagnosis, 100, stdin);
    newNode->data.diagnosis[strcspn(newNode->data.diagnosis, "\n")] = '\0';

    int roomnumber;

    do {
        printf("Enter patient room number 1 - 100: ");
        roomnumber = numberInput();
    }while (roomnumber > 100 || isEmptyRoom(roomnumber));

    rooms[roomnumber-1] = 1;

    newNode->data.roomNumber = roomnumber;
    // Set admission date to today
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(newNode->data.admissionDate, sizeof(newNode->data.admissionDate), "%Y-%m-%d", t);


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

//checked if the patient can be admitted to the room they want
int isEmptyRoom(int roomnumber) {
    if (rooms[roomnumber-1] == 1) {
        printf("Room is full Try again\n");
    }
    return rooms[roomnumber-1];
}


//function to display all the patient
void displayPatient(PatientNode *head) {
    if (head == NULL) {
        printf("No patients enrolled yet.\n");
        return;
    }

    //print all the patient if the the list is not empty
    PatientNode *current = head;
    while (current != NULL){
        Patient p = current->data;
        printf("Patient ID: %-10d\tName: %-20s\tAge: %-5d\tDiagnosis: %-20s\tRoom Number: %-10d\n",
                        p.patientId, p.name, p.age, p.diagnosis, p.roomNumber);
        current = current->next;
    }
}


//discharge patient from the hospital
void dischargePatient(PatientNode **head) {
    if (head == NULL) {
        printf("No patients enrolled yet.\n");
        return;
    }

    PatientNode *current = *head;
    PatientNode *prev = NULL;

    //take id of the patient
    printf("Please enter the id of the patient");
    int id = numberInput();

    //if the list is not empty then search for the pateint
    while (current != NULL) {
        if (current->data.patientId == id) {
            // Patient found; adjust pointers to remove the node
            rooms[current->data.roomNumber-1] = 0;
            writeToDichargeReport(current->data.name);
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


//function to search for the patient in the list
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

//after all is finished free up the memoryf or the linked list so that memory wont leak
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

// Saves the patient linked list to a file in a pipe-separated format.
// Parameters:
//   - filename: The name of the file to save to.
//   - head: Pointer to the head of the patient linked list.

void savePatientsToFile(const char *filename, PatientNode *head) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing patient data.\n");
        return;
    }
    PatientNode *current = head;
    while (current != NULL) {
        Patient p = current->data;
        fprintf(file, "%d|%s|%d|%s|%d|%s\n",
                p.patientId,
                p.name,
                p.age,
                p.diagnosis,
                p.roomNumber,
                p.admissionDate);
        current = current->next;
    }
    fclose(file);
    printf("Patient data saved to file.\n");
}

// Loads patient data from a file and populates the patient linked list.
// Parameters:
//   - filename: The name of the file to read from.
//   - head: Double pointer to the head of the patient linked list.

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
        PatientNode *newNode;

        // Parse the line using strtok.
        char *token = strtok(line, "|");
        if (token != NULL)
            newNode =  createPatientNode(atoi(token));

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
            rooms[atoi(token) -1] = 1;

        token = strtok(NULL, "|");
        if (token != NULL)
            strcpy(newNode->data.admissionDate, token);

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

// Saves the current doctor schedule to a file in a pipe-separated format.
// Parameters:
//   - filename: The name of the file to save to.

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

// Loads the doctor schedule from a file and updates the doctorSchedule array.
// If the file is not found, it initializes a default schedule.
// Parameters:
//   - filename: The name of the file to load from.
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
        sscanf(line, "%[^|]|%[^|]|%[^\n]", day, shift, name);


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

// Generates a timestamp string in the format "YYYYMMDD_HHMM".
// Parameters:
//   - buffer: The character array to store the timestamp.
//   - size: The size of the buffer.
void getCurrentTimeStamp(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y%m%d_%H%M", t);
}

// Creates a timestamped backup of both the patient data and doctor schedule.
// Backup files are stored in a `backup` directory, which is created if missing.
void backupData() {
    char timestamp[20];
    getCurrentTimeStamp(timestamp, sizeof(timestamp));

    struct stat st = {0};
    if (stat("backup", &st) == -1) {
        mkdir("backup");
    }

    char patientBackupFile[100];
    char scheduleBackupFile[100];
    snprintf(patientBackupFile, sizeof(patientBackupFile), "backup/patients_%s.txt", timestamp);
    snprintf(scheduleBackupFile, sizeof(scheduleBackupFile), "backup/schedule_%s.txt", timestamp);

    savePatientsToFile(patientBackupFile, head);
    saveDoctorScheduleToFile(scheduleBackupFile);

    printf("Backup completed: %s and %s\n", patientBackupFile, scheduleBackupFile);
}

// Restores patient data from a given file.
// Frees existing list and repopulates it with the file content.
// Parameters:
//   - filename: The file to load the patient data from.

void restorePatientsFromFile(const char *filename) {

        freePatientList(head);
        head = NULL;

        loadPatientsFromFile(filename, &head);

        currentId = getLastId(head);
}

// Restores the doctor schedule from a given file.
// Initializes the schedule first before loading.
// Parameters:
//   - filename: The file to load the schedule from.

void restoreScheduleFromFile(const char *filename) {
    initializeSchedule();
    loadDoctorScheduleFromFile(filename);
}

// Lists all backup files that match a specific prefix (e.g., "patients_" or "schedule_").
// Parameters:
//   - prefix: The prefix used to filter matching backup files.

void listBackupFiles(const char *prefix) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("backup");
    if (dir == NULL) {
        printf("Error opening backup directory.\n");
        return;
    }

    printf("\nAvailable %s backups: \n");
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, prefix) == entry->d_name) {
            printf("%s\n", entry->d_name);
            count++;
        }
    }
    closedir(dir);
    if (count == 0) {
        printf("No backups found.\n");
    }
}

// Generates a report showing how many shifts each doctor is scheduled for in a week.
// Parameters:
//   - filename: The name of the file to write the report to.

void generateDoctorUtilizationReport(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error creating doctor utilization report.\n");
        return;
    }

    fprintf(file, "Doctor Utilization Report (Weekly)\n\n");
    fprintf(file, "%-20s | Total Shifts\n", "Doctor");
    fprintf(file, "----------------------|-------------\n");

    char doctors[100][50];
    int counts[100] = {0};
    int doctorCount = 0;

    for (int i = 0; i < DAYS; i++) {
        for (int j = 0; j < SHIFTS; j++) {
            char *name = doctorSchedule[i][j];
            if (strcmp(name, "Unassigned") == 0) continue;
            int found = 0;
            for (int k = 0; k < doctorCount; k++) {
                if (strcmp(doctors[k], name) == 0) {
                    counts[k]++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(doctors[doctorCount], name);
                counts[doctorCount] = 1;
                doctorCount++;
            }
        }
    }

    for (int i = 0; i < doctorCount; i++) {
        fprintf(file, "%-20s | %-5d\n", doctors[i], counts[i]);
    }

    fclose(file);
    printf("Doctor Utilization Report saved to %s\n", filename);
}

// Generates a report summarizing the number of patients assigned to each room.
// Parameters:
//   - filename: The name of the file to write the report to.

void generateRoomUsageReport(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error creating room usage report.\n");
        return;
    }

    fprintf(file, "Room Usage Report\n\n");
    fprintf(file, "%-10s | Patients\n", "Room No.");
    fprintf(file, "-----------|----------\n");

    int rooms[100] = {0};
    int roomCounts[100] = {0};
    int roomTotal = 0;

    PatientNode *current = head;
    while (current != NULL) {
        int room = current->data.roomNumber;
        int found = 0;
        for (int i = 0; i < roomTotal; i++) {
            if (rooms[i] == room) {
                roomCounts[i]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            rooms[roomTotal] = room;
            roomCounts[roomTotal] = 1;
            roomTotal++;
        }
        current = current->next;
    }

    for (int i = 0; i < roomTotal; i++) {
        fprintf(file, "%-10d | %-5d\n", rooms[i], roomCounts[i]);
    }

    fclose(file);
    printf("Room Usage Report saved to %s\n", filename);
}

// Parses a date string in "YYYY-MM-DD" format into a struct tm.
// Parameters:
//   - dateStr: The input date string.
//   - outDate: Output struct tm to be filled.
// Returns:
//   - 1 if successful, 0 otherwise.

int parseDate(const char *dateStr, struct tm *outDate) {
    int year, month, day;
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0; // error
    }

    outDate->tm_year = year - 1900; // tm_year is years since 1900
    outDate->tm_mon = month - 1;    // tm_mon is 0-11
    outDate->tm_mday = day;
    outDate->tm_hour = 0;
    outDate->tm_min = 0;
    outDate->tm_sec = 0;
    outDate->tm_isdst = -1;

    // Normalize the date and fill in tm_yday
    mktime(outDate);
    return 1;
}

// Generates a report summarizing patient admissions for today, this week, and this month.
// Parameters:
//   - filename: The name of the file to write the report to.

void generateAdmissionSummaryReport(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error creating admission summary report.\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    int dayCount = 0, weekCount = 0, monthCount = 0;

    PatientNode *current = head;
    while (current != NULL) {
        struct tm admission_tm = {0};
        if (!parseDate(current->data.admissionDate, &admission_tm)) {
            current = current->next;
            continue;
        }

        if (admission_tm.tm_year == now_tm->tm_year) {
            if (admission_tm.tm_yday == now_tm->tm_yday)
                dayCount++;
            if ((now_tm->tm_yday - admission_tm.tm_yday) < 7 && (now_tm->tm_yday - admission_tm.tm_yday) >= 0)
                weekCount++;
            if (admission_tm.tm_mon == now_tm->tm_mon)
                monthCount++;
        }

        current = current->next;
    }

    fprintf(file, "Patient Admission Summary Report\n\n");
    fprintf(file, "Total admitted today: %d\n", dayCount);
    fprintf(file, "Total admitted this week: %d\n", weekCount);
    fprintf(file, "Total admitted this month: %d\n", monthCount);

    fclose(file);
    printf("Admission Summary Report saved to %s\n", filename);
}

// Displays a menu to generate different types of reports.
// Loops until the user chooses to exit.
void reportMenu() {
    int choice;
    char filename[100];

    do {
        printf("\n--- Reporting & Analytics ---\n");
        printf("1. Doctor Utilization Report\n");
        printf("2. Room Usage Report\n");
        printf("3. Patient Admission Summary (Day/Week/Month)\n");
        printf("4. Exit Report Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter filename to save Doctor Report (e.g., doctor_report.txt): ");
            scanf("%s", filename);
            getchar();
            generateDoctorUtilizationReport(filename);
            break;
            case 2:
                printf("Enter filename to save Room Usage Report (e.g., room_report.txt): ");
            scanf("%s", filename);
            getchar();
            generateRoomUsageReport(filename);
            break;
            case 3:
                printf("Enter filename to save Admission Summary (e.g., admissions.txt): ");
            scanf("%s", filename);
            getchar();
            generateAdmissionSummaryReport(filename);
            break;
            case 4:
                printf("Exiting report menu...\n");
            return;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 4);
}

// Appends a patient's discharge event to a report file, including the date.
// Parameters:
//   - patientName: Name of the discharged patient.

void writeToDichargeReport(const char *patientName) {
    char timeToday[11];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(timeToday, 11, "%Y-%m-%d", tm_info);

    FILE *fp = fopen("discharge_report.txt", "a");
    if (fp == NULL) {
        perror("Failed to open discharge report file");
        return;
    }
    fprintf(fp, "%s was discharged on %s\n", patientName, timeToday);
    fclose(fp);
}

// Retrieves the next available patient ID based on the current list.
// Parameters:
//   - head: The head of the patient linked list.
// Returns:
//   - The next available ID (max ID + 1).

int getLastId(PatientNode *head){
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