#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 20     // Maximum characters for student name
#define MAX_STUDENTS 30     // Maximum number of students
#define MAX_COURSES 6       // Maximum number of courses

typedef struct {
    long id;                    // Student ID (4 digits)
    char name[MAX_NAME_LEN];    // Student name
    float scores[MAX_COURSES];  // Course scores
    float total;                // Total score
    float average;              // Average score
} Student;

// Function declarations
int showMenu(void);
void showHelp();
int inputStudentData(Student students[], int studentCount, int courseCount);
void showCourseStatistics(Student students[], int studentCount, int courseCount);
void showStudentStatistics(Student students[], int studentCount, int courseCount);
void sortByTotalScore(Student students[], int studentCount, int courseCount, int descending);
void sortByStudentId(Student students[], int studentCount, int courseCount);
void sortByStudentName(Student students[], int studentCount, int courseCount);
void searchByStudentId(Student students[], int studentCount, int courseCount);
void searchByStudentName(Student students[], int studentCount, int courseCount);
void showGradeStatistics(Student students[], int studentCount, int courseCount);
void displayAllStudents(Student students[], int studentCount, int courseCount);
void clearInputBuffer();
int isValidStudentName(const char *name);
int hasDataBeenEntered = 0;  // Global flag to track if data exists

// Function 1: Display main menu
int showMenu(void) {
    int choice;

    printf("\n============================================\n");
    printf("        STUDENT MANAGEMENT SYSTEM\n");
    printf("============================================\n");
    printf("1. Show Instructions\n");
    printf("2. Enter Student Data\n");
    printf("3. Show Course Statistics\n");
    printf("4. Show Student Statistics\n");
    printf("5. Sort: Highest to Lowest Scores\n");
    printf("6. Sort: Lowest to Highest Scores\n");
    printf("7. Sort by Student ID\n");
    printf("8. Sort by Student Name\n");
    printf("9. Search by Student ID\n");
    printf("10. Search by Student Name\n");
    printf("11. Show Grade Distribution\n");
    printf("12. Display All Students\n");
    printf("0. Exit Program\n");
    printf("============================================\n");
    printf("Enter your choice (0-12): ");

    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        return -1;
    }
    return choice;
}

// Function 2: Show instructions
void showHelp() {
    printf("\n============================================\n");
    printf("                INSTRUCTIONS\n");
    printf("============================================\n");
    printf("1. Maximum 30 students and 6 courses\n");
    printf("2. Student ID must be 4 digits (1000-9999)\n");
    printf("3. Student name: letters and spaces only\n");
    printf("4. Scores must be between 0 and 100\n");
    printf("5. Data is saved until program ends\n");
    printf("6. You can use menu options in any order\n");
    printf("============================================\n");
}

// Function 3: Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function 4: Validate student name
int isValidStudentName(const char *name) {
    int i;

    // Check if name is empty
    if (strlen(name) == 0) {
        return 0;
    }

    // Check each character
    for (i = 0; name[i] != '\0'; i++) {
        // Allow only letters, spaces, and hyphens
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-') {
            return 0;
        }

        // Check for consecutive spaces
        if (name[i] == ' ' && name[i + 1] == ' ') {
            return 0;
        }
    }

    // Name must start with a letter
    if (!isalpha(name[0])) {
        return 0;
    }

    return 1;
}

// Function 5: Input student data
int inputStudentData(Student students[], int studentCount, int courseCount) {
    int i, j;
    char confirm;

    // Check if data already exists
    if (hasDataBeenEntered) {
        printf("\nWARNING: Student data already exists!\n");
        printf("Entering new data will overwrite existing records.\n");
        printf("Do you want to continue? (y/n): ");
        clearInputBuffer();
        scanf("%c", &confirm);
        if (confirm != 'y' && confirm != 'Y') {
            printf("Data entry cancelled.\n");
            return 0;
        }
    }

    printf("\n============================================\n");
    printf("          ENTER STUDENT INFORMATION\n");
    printf("============================================\n");

    for (i = 0; i < studentCount; i++) {
        printf("\n--- Student %d ---\n", i + 1);

        // Input student ID with validation
        while (1) {
            printf("Student ID (4 digits, 1000-9999): ");
            if (scanf("%ld", &students[i].id) != 1) {
                printf("ERROR: Invalid input. Please enter numbers only.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            // Validate ID range
            if (students[i].id < 1000 || students[i].id > 9999) {
                printf("ERROR: ID must be 4 digits (1000-9999).\n");
                continue;
            }

            // Check for duplicate IDs
            int duplicate = 0;
            for (j = 0; j < i; j++) {
                if (students[j].id == students[i].id) {
                    printf("ERROR: This ID already exists. Please enter a unique ID.\n");
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) break;
        }

        // Input student name with validation
        while (1) {
            printf("Student Name (letters and spaces only): ");
            fgets(students[i].name, MAX_NAME_LEN, stdin);

            // Remove newline character
            students[i].name[strcspn(students[i].name, "\n")] = '\0';

            // Validate name
            if (strlen(students[i].name) == 0) {
                printf("ERROR: Name cannot be empty.\n");
                continue;
            }

            if (strlen(students[i].name) >= MAX_NAME_LEN) {
                printf("ERROR: Name is too long. Maximum %d characters.\n", MAX_NAME_LEN - 1);
                clearInputBuffer();
                continue;
            }

            if (!isValidStudentName(students[i].name)) {
                printf("ERROR: Invalid name. Use letters and spaces only.\n");
                continue;
            }
            break;
        }

        // Input scores for each course
        printf("Enter scores for %d courses:\n", courseCount);
        students[i].total = 0;

        for (j = 0; j < courseCount; j++) {
            while (1) {
                printf("  Course %d (0-100): ", j + 1);
                if (scanf("%f", &students[i].scores[j]) != 1) {
                    printf("    ERROR: Invalid input. Enter a number.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();

                if (students[i].scores[j] < 0 || students[i].scores[j] > 100) {
                    printf("    ERROR: Score must be between 0 and 100.\n");
                    continue;
                }
                break;
            }
            students[i].total += students[i].scores[j];
        }

        // Calculate average
        students[i].average = (courseCount > 0) ? students[i].total / courseCount : 0;
    }

    hasDataBeenEntered = 1;
    printf("\nSUCCESS: %d student records have been saved.\n", studentCount);
    return 1;
}

// Function 6: Display all students in a clean table
void displayAllStudents(Student students[], int studentCount, int courseCount) {
    int i, j;

    if (studentCount <= 0) {
        printf("\nERROR: No student data available.\n");
        printf("Please enter student data first (Option 2).\n");
        return;
    }

    printf("\n=========================================================================================================\n");
    printf("                                     STUDENT RECORDS\n");
    printf("=========================================================================================================\n");

    // Print table header
    printf("Student ID    Student Name         ");
    for (j = 0; j < courseCount; j++) {
        printf("Course %-2d  ", j + 1);
    }
    printf("   Total     Average\n");

    printf("------------  -------------------- ");
    for (j = 0; j < courseCount; j++) {
        printf("---------- ");
    }
    printf("  ---------  ---------\n");

    // Print student data
    for (i = 0; i < studentCount; i++) {
        printf("%-12ld  %-20s", students[i].id, students[i].name);

        for (j = 0; j < courseCount; j++) {
            printf("  %8.1f", students[i].scores[j]);
        }

        printf("  %9.1f  %8.2f\n", students[i].total, students[i].average);
    }

    printf("=========================================================================================================\n");
    printf("Total Students: %d, Total Courses: %d\n", studentCount, courseCount);
}

// Function 7: Calculate course statistics
void showCourseStatistics(Student students[], int studentCount, int courseCount) {
    int i, j;

    if (studentCount <= 0) {
        printf("\nERROR: No data available.\n");
        return;
    }

    printf("\n============================================\n");
    printf("            COURSE STATISTICS\n");
    printf("============================================\n");

    for (j = 0; j < courseCount; j++) {
        float total = 0, highest = 0, lowest = 100;

        for (i = 0; i < studentCount; i++) {
            float score = students[i].scores[j];
            total += score;
            if (score > highest) highest = score;
            if (score < lowest) lowest = score;
        }

        float average = total / studentCount;

        printf("\nCourse %d:\n", j + 1);
        printf("  Average Score: %.2f\n", average);
        printf("  Highest Score: %.2f\n", highest);
        printf("  Lowest Score:  %.2f\n", lowest);
        printf("  Total Score:   %.2f\n", total);
    }
}

// Function 8: Calculate student statistics
void showStudentStatistics(Student students[], int studentCount, int courseCount) {
    int i;

    if (studentCount <= 0) {
        printf("\nERROR: No data available.\n");
        return;
    }

    printf("\n============================================\n");
    printf("           STUDENT STATISTICS\n");
    printf("============================================\n");

    for (i = 0; i < studentCount; i++) {
        printf("\nID: %ld, Name: %s\n", students[i].id, students[i].name);
        printf("  Total Score: %.1f, Average: %.2f\n", students[i].total, students[i].average);
    }
}

// Function 9: Sort by total score
void sortByTotalScore(Student students[], int studentCount, int courseCount, int descending) {
    int i, j;

    for (i = 0; i < studentCount - 1; i++) {
        for (j = 0; j < studentCount - i - 1; j++) {
            int shouldSwap = 0;

            if (descending) {
                shouldSwap = students[j].total < students[j + 1].total;
            } else {
                shouldSwap = students[j].total > students[j + 1].total;
            }

            if (shouldSwap) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

// Function 10: Sort by student ID
void sortByStudentId(Student students[], int studentCount, int courseCount) {
    int i, j;

    for (i = 0; i < studentCount - 1; i++) {
        for (j = 0; j < studentCount - i - 1; j++) {
            if (students[j].id > students[j + 1].id) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

// Function 11: Sort by student name
void sortByStudentName(Student students[], int studentCount, int courseCount) {
    int i, j;

    for (i = 0; i < studentCount - 1; i++) {
        for (j = 0; j < studentCount - i - 1; j++) {
            if (strcasecmp(students[j].name, students[j + 1].name) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

// Function 12: Search by student ID
void searchByStudentId(Student students[], int studentCount, int courseCount) {
    long searchId;
    int i, j, found = 0;

    if (studentCount <= 0) {
        printf("\nERROR: No data available.\n");
        return;
    }

    printf("\nEnter Student ID to search: ");
    if (scanf("%ld", &searchId) != 1) {
        printf("ERROR: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (i = 0; i < studentCount; i++) {
        if (students[i].id == searchId) {
            printf("\nSTUDENT FOUND:\n");
            printf("  ID: %ld\n", students[i].id);
            printf("  Name: %s\n", students[i].name);
            printf("  Scores: ");
            for (j = 0; j < courseCount; j++) {
                printf("Course %d: %.1f  ", j + 1, students[i].scores[j]);
            }
            printf("\n  Total: %.1f, Average: %.2f\n", students[i].total, students[i].average);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nERROR: Student with ID %ld not found.\n", searchId);
    }
}

// Function 13: Search by student name
void searchByStudentName(Student students[], int studentCount, int courseCount) {
    char searchName[MAX_NAME_LEN];
    int i, j, found = 0;

    if (studentCount <= 0) {
        printf("\nERROR: No data available.\n");
        return;
    }

    printf("\nEnter Student Name to search: ");
    clearInputBuffer();
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    for (i = 0; i < studentCount; i++) {
        if (strcasecmp(students[i].name, searchName) == 0) {
            printf("\nSTUDENT FOUND:\n");
            printf("  ID: %ld\n", students[i].id);
            printf("  Name: %s\n", students[i].name);
            printf("  Scores: ");
            for (j = 0; j < courseCount; j++) {
                printf("Course %d: %.1f  ", j + 1, students[i].scores[j]);
            }
            printf("\n  Total: %.1f, Average: %.2f\n", students[i].total, students[i].average);
            found = 1;
        }
    }

    if (!found) {
        printf("\nERROR: Student '%s' not found.\n", searchName);
    }
}

// Function 14: Show grade distribution
void showGradeStatistics(Student students[], int studentCount, int courseCount) {
    int i, j;

    if (studentCount <= 0) {
        printf("\nERROR: No data available.\n");
        return;
    }

    printf("\n============================================\n");
    printf("          GRADE DISTRIBUTION\n");
    printf("============================================\n");

    for (j = 0; j < courseCount; j++) {
        int gradeCount[5] = {0}; // F, D, C, B, A

        for (i = 0; i < studentCount; i++) {
            float score = students[i].scores[j];
            if (score >= 90) gradeCount[4]++;      // A
            else if (score >= 80) gradeCount[3]++; // B
            else if (score >= 70) gradeCount[2]++; // C
            else if (score >= 60) gradeCount[1]++; // D
            else gradeCount[0]++;                  // F
        }

        printf("\nCourse %d:\n", j + 1);
        printf("  A (90-100):   %2d students\n", gradeCount[4]);
        printf("  B (80-89):    %2d students\n", gradeCount[3]);
        printf("  C (70-79):    %2d students\n", gradeCount[2]);
        printf("  D (60-69):    %2d students\n", gradeCount[1]);
        printf("  F (0-59):     %2d students\n", gradeCount[0]);
    }
}

// Main Function
int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0, courseCount = 0;
    int choice;

    printf("\n============================================\n");
    printf("     STUDENT MANAGEMENT SYSTEM\n");
    printf("============================================\n");

    // Get number of students
    while (studentCount <= 0 || studentCount > MAX_STUDENTS) {
        printf("\nEnter number of students (1-%d): ", MAX_STUDENTS);
        if (scanf("%d", &studentCount) != 1) {
            printf("ERROR: Invalid input.\n");
            clearInputBuffer();
            continue;
        }

        if (studentCount <= 0 || studentCount > MAX_STUDENTS) {
            printf("ERROR: Please enter between 1 and %d.\n", MAX_STUDENTS);
        }
    }

    // Get number of courses
    while (courseCount <= 0 || courseCount > MAX_COURSES) {
        printf("Enter number of courses (1-%d): ", MAX_COURSES);
        if (scanf("%d", &courseCount) != 1) {
            printf("ERROR: Invalid input.\n");
            clearInputBuffer();
            continue;
        }

        if (courseCount <= 0 || courseCount > MAX_COURSES) {
            printf("ERROR: Please enter between 1 and %d.\n", MAX_COURSES);
        }
    }

    clearInputBuffer();

    // Main program loop
    do {
        choice = showMenu();

        switch (choice) {
            case 1:
                showHelp();
                break;
            case 2:
                inputStudentData(students, studentCount, courseCount);
                break;
            case 3:
                if (hasDataBeenEntered) {
                    showCourseStatistics(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 4:
                if (hasDataBeenEntered) {
                    showStudentStatistics(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 5:
                if (hasDataBeenEntered) {
                    sortByTotalScore(students, studentCount, courseCount, 1);
                    printf("\nSORTED BY HIGHEST SCORES:\n");
                    displayAllStudents(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 6:
                if (hasDataBeenEntered) {
                    sortByTotalScore(students, studentCount, courseCount, 0);
                    printf("\nSORTED BY LOWEST SCORES:\n");
                    displayAllStudents(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 7:
                if (hasDataBeenEntered) {
                    sortByStudentId(students, studentCount, courseCount);
                    printf("\nSORTED BY STUDENT ID:\n");
                    displayAllStudents(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 8:
                if (hasDataBeenEntered) {
                    sortByStudentName(students, studentCount, courseCount);
                    printf("\nSORTED BY STUDENT NAME:\n");
                    displayAllStudents(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 9:
                if (hasDataBeenEntered) {
                    searchByStudentId(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 10:
                if (hasDataBeenEntered) {
                    searchByStudentName(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 11:
                if (hasDataBeenEntered) {
                    showGradeStatistics(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 12:
                if (hasDataBeenEntered) {
                    displayAllStudents(students, studentCount, courseCount);
                } else {
                    printf("\nERROR: Please enter student data first (Option 2).\n");
                }
                break;
            case 0:
                printf("\nThank you for using the Student Management System.\n");
                break;
            default:
                if (choice != -1) {
                    printf("\nERROR: Invalid choice. Please enter a number from 0 to 12.\n");
                }
                clearInputBuffer();
        }

        if (choice != 0) {
            printf("\nPress Enter to continue...");
            clearInputBuffer();
        }

    } while (choice != 0);

    return 0;
}
