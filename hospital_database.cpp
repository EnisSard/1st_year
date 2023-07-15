#include <stdio.h>
#include <string.h>

struct Patient {
    char name[100];
    int age;
    char illnesses[100][100];
    int numIllnesses;
};

void addPatient(FILE* file);
void printPatients(FILE* file);
void deletePatient(FILE* file, const char* name);
void modifyPatient(FILE* file, const char* name);
void searchPatients(FILE* file, const char* keyword);

void addPatient(FILE* file) {
    Patient newPatient;
    char name[100];
    int age;
    char illnesses[100][100];
    int numIllnesses = 0;

    printf("Enter patient name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter patient age: ");
    scanf("%d", &age);
    getchar();

    printf("Enter patient illnesses (one per line, leave empty to finish):\n");
    char illness[100];
    while (fgets(illness, sizeof(illness), stdin) && illness[0] != '\n') {
        illness[strcspn(illness, "\n")] = '\0';
        strncpy(illnesses[numIllnesses], illness, sizeof(illnesses[numIllnesses])-1);
        illnesses[numIllnesses][sizeof(illnesses[numIllnesses])-1] = '\0';
        numIllnesses++;
    }

    strncpy(newPatient.name, name, sizeof(newPatient.name)-1);
    newPatient.name[sizeof(newPatient.name)-1] = '\0';
    newPatient.age = age;
    newPatient.numIllnesses = numIllnesses;
    for (int i = 0; i < numIllnesses; i++) {
        strncpy(newPatient.illnesses[i], illnesses[i], sizeof(newPatient.illnesses[i])-1);
        newPatient.illnesses[i][sizeof(newPatient.illnesses[i])-1] = '\0';
    }

    fseek(file, 0, SEEK_END);
    fwrite(&newPatient, sizeof(newPatient), 1, file);
}

void printPatients(FILE* file) {
    Patient patient;
    fseek(file, 0, SEEK_SET);

    while (fread(&patient, sizeof(patient), 1, file) == 1) {
        printf("Patient Name: %s\n", patient.name);
        printf("Age: %d\n", patient.age);
        printf("Illnesses: ");
        for (int i = 0; i < patient.numIllnesses; i++) {
            printf("%s, ", patient.illnesses[i]);
        }
        printf("\n\n");
    }
}

void deletePatient(FILE* file, const char* name) {
    FILE* tempFile = fopen("temp.txt", "wb+");
    Patient patient;
    fseek(file, 0, SEEK_SET);
    while (fread(&patient, sizeof(patient), 1, file) == 1) {
        if (strcmp(patient.name, name) != 0) {
            fwrite(&patient, sizeof(patient), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("data.txt");
    rename("temp.txt", "data.txt");
    file = fopen("data.txt", "rb+");
}

void modifyPatient(FILE* file, const char* name) {
    Patient patient;
    fseek(file, 0, SEEK_SET);
    while (fread(&patient, sizeof(patient), 1, file) == 1) {
        if (strcmp(patient.name, name) == 0) {
            printf("Enter new patient name: ");
            fgets(patient.name, sizeof(patient.name), stdin);
            patient.name[strcspn(patient.name, "\n")] = '\0';

            printf("Enter new patient age: ");
            scanf("%d", &patient.age);
            getchar();

            printf("Enter new patient illnesses (one per line, leave empty to finish):\n");
            char illness[100];
            int numIllnesses = 0;
            while (fgets(illness, sizeof(illness), stdin) && illness[0] != '\n') {
                illness[strcspn(illness, "\n")] = '\0';
                strncpy(patient.illnesses[numIllnesses], illness, sizeof(patient.illnesses[numIllnesses])-1);
                patient.illnesses[numIllnesses][sizeof(patient.illnesses[numIllnesses])-1] = '\0';
                numIllnesses++;
            }
            patient.numIllnesses = numIllnesses;
            fseek(file, -sizeof(patient), SEEK_CUR);
            fwrite(&patient, sizeof(patient), 1, file);
            break;
        }
    }
}

void searchPatients(FILE* file, const char* keyword) {
    Patient patient;
    fseek(file, 0, SEEK_SET);

    while (fread(&patient, sizeof(patient), 1, file) == 1) {
        int found = 0;
        for (int i = 0; i < patient.numIllnesses; i++) {
            if (strstr(patient.illnesses[i], keyword) != NULL) {
                found = 1;
                break;
            }
        }

        if (found) {
            printf("Patient Name: %s\n", patient.name);
            printf("Age: %d\n", patient.age);
            printf("Illnesses: ");
            for (int i = 0; i < patient.numIllnesses; i++) {
                printf("%s, ", patient.illnesses[i]);
            }
            printf("\n\n");
        }
    }
}

int main() {
    FILE* file = fopen("data.txt", "rb+");

    if (!file) {
        printf("Error opening the file.\n");
        return 1;
    }
    int choice;
    do {
        printf("1. Add patient\n");
        printf("2. Print patients\n");
        printf("3. Edit patient data\n");
        printf("4. Search patients\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addPatient(file);
                break;
            case 2:
                printPatients(file);
                break;
            case 3: {
                char name[100];
                printf("Enter patient name to edit: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                int editChoice;
                printf("1. Delete patient record\n");
                printf("2. Modify patient record\n");
                printf("Enter your choice: ");
                scanf("%d", &editChoice);
                getchar();

                switch (editChoice) {
                    case 1:
                        deletePatient(file, name);
                        break;
                    case 2:
                        modifyPatient(file, name);
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                        break;
                }
                break;
            }
            case 4: {
                char keyword[100];
                printf("Enter keyword to search patients: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';

                searchPatients(file, keyword);
                break;
            }
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    } while (choice != 5);
    fclose(file);

    return 0;
}

