#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// ================= STRUCT =================
// This structure stores all request information
struct Request {
    int id;                 // unique request ID
    char name[50];         // requester name
    char address[100];     // address (for barangay cert)
    char type[30];         // type of request (Cedula / Barangay)
    char status[20];       // status (Pending, Approved, etc.)
};

// Array to store all requests
struct Request requests[MAX];
int count = 0;             // number of requests stored
int request_id = 1001;     // auto-generated ID

// ================= DSA =================
// STACK (used for undo feature - LIFO)
int stack[MAX];
int top = -1;

// QUEUE (used for processing requests - FIFO)
int queue[MAX];
int front = 0, rear = -1;

// ================= PROTOTYPES =================
void main_menu();
void request_cedula();
void request_barangay();
void view_status();
void admin_login();
void admin_panel();
void get_input(char *prompt, char *data);

// DSA functions
void undo_last_request();
void process_queue();
void sort_requests_by_name();

// ================= VALIDATION FUNCTIONS =================
// Checks if name is valid (letters only, min length)
int is_valid_name(char *str) {
    int letters = 0;
    if (strlen(str) < 5) return 0;

    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '.') return 0;
        if (isalpha(str[i])) letters++;
    }
    return letters >= 2;
}

// Checks if address is valid
int is_valid_address(char *str) {
    int letters = 0;
    if (strlen(str) < 5) return 0;

    for (int i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != ' ' && str[i] != ',' && str[i] != '.') return 0;
        if (isalpha(str[i])) letters++;
    }
    return letters >= 2;
}

// Checks date format (MM-DD-YYYY)
int is_valid_dob(char *str) {
    int m, d, y;
    return sscanf(str, "%d-%d-%d", &m, &d, &y) == 3;
}

// Checks gender input
int is_valid_gender(char *str) {
    return (strcmp(str, "Male") == 0 ||
            strcmp(str, "Female") == 0 ||
            strcmp(str, "male") == 0 ||
            strcmp(str, "female") == 0);
}

// ================= INPUT FUNCTION =================
// reusable input function with validation loop
void get_input(char *prompt, char *data) {
    while (1) {
        printf("%s", prompt);
        fgets(data, 100, stdin);
        data[strcspn(data, "\n")] = 0;

        if (strlen(data) == 0)
            printf("Invalid input. Try again.\n");
        else
            break;
    }
}

// ================= MAIN FUNCTION =================
int main() {
    main_menu();
    return 0;
}

// ================= MAIN MENU =================
void main_menu() {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("        QUICKCEDULA SYSTEM\n");
        printf("========================================\n");

        printf("\n+--------------------------------------+\n");
        printf("|              MAIN MENU               |\n");
        printf("+--------------------------------------+\n");
        printf("| 1. Request Cedula                   |\n");
        printf("| 2. Request Barangay Certificate     |\n");
        printf("| 3. View My Status                   |\n");
        printf("| 4. Admin Login                      |\n");
        printf("| 5. Exit System                      |\n");
        printf("+--------------------------------------+\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) request_cedula();
        else if (choice == 2) request_barangay();
        else if (choice == 3) view_status();
        else if (choice == 4) admin_login();

        else if (choice == 5) {
            // Exit message
            printf("\n========================================\n");
            printf("   THANK YOU FOR USING QUICKCEDULA\n");
            printf("   SYSTEM IS NOW CLOSING...\n");
            printf("   Goodbye! ??\n");
            printf("========================================\n\n");
            break;
        }

        else {
            printf("Invalid input.\n");
        }
    }
}

// ================= CEDULA REQUEST =================
// Handles Cedula request form input
void request_cedula() {
    char temp[50];

    printf("\n--- CEDULA REQUEST FORM ---\n");

    do {
        get_input("Enter Full Name: ", requests[count].name);
    } while (!is_valid_name(requests[count].name));

    do {
        get_input("Enter Date of Birth (MM-DD-YYYY): ", temp);
    } while (!is_valid_dob(temp));

    do {
        get_input("Enter Place of Birth: ", temp);
    } while (strlen(temp) < 3);

    do {
        get_input("Enter Civil Status: ", temp);
    } while (strlen(temp) < 3);

    do {
        get_input("Enter Citizenship: ", temp);
    } while (strlen(temp) < 3);

    do {
        get_input("Enter Gender: ", temp);
    } while (!is_valid_gender(temp));

    // assign request details
    strcpy(requests[count].type, "Cedula");
    strcpy(requests[count].status, "Pending");
    requests[count].id = request_id++;

    // STACK + QUEUE insertion
    stack[++top] = requests[count].id;
    queue[++rear] = requests[count].id;

    printf("\nCedula Request Submitted! ID: %d\n", requests[count].id);
    count++;
}

// ================= BARANGAY REQUEST =================
// Handles Barangay Certificate request
void request_barangay() {
    char temp[50];

    printf("\n--- BARANGAY CERTIFICATE FORM ---\n");

    do {
        get_input("Enter Full Name: ", requests[count].name);
    } while (!is_valid_name(requests[count].name));

    do {
        get_input("Enter Address: ", requests[count].address);
    } while (!is_valid_address(requests[count].address));

    do {
        get_input("Enter Purpose: ", temp);
    } while (strlen(temp) < 3);

    do {
        get_input("Enter Date of Birth: ", temp);
    } while (!is_valid_dob(temp));

    do {
        get_input("Enter Gender: ", temp);
    } while (!is_valid_gender(temp));

    strcpy(requests[count].type, "Barangay Certificate");
    strcpy(requests[count].status, "Pending");
    requests[count].id = request_id++;

    // STACK + QUEUE insertion
    stack[++top] = requests[count].id;
    queue[++rear] = requests[count].id;

    printf("\nBarangay Certificate Submitted! ID: %d\n", requests[count].id);
    count++;
}

// ================= VIEW STATUS =================
// Search request using linear search
void view_status() {
    int id;
    printf("Enter Request ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (requests[i].id == id) {
            printf("\nName: %s\nType: %s\nStatus: %s\n",
                   requests[i].name,
                   requests[i].type,
                   requests[i].status);
            return;
        }
    }
    printf("Record not found.\n");
}

// ================= ADMIN LOGIN =================
// simple authentication system
void admin_login() {
    char user[20], pass[20];

    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0)
        admin_panel();
    else
        printf("Access denied.\n");
}

// ================= ADMIN PANEL =================
// manages all requests
void admin_panel() {
    int choice;

    while (1) {
        printf("\n=============================\n");
        printf("        ADMIN PANEL\n");
        printf("=============================\n");
        printf("1. View All\n");
        printf("2. Approve Request\n");
        printf("3. Issue Document\n");
        printf("4. Undo Last Request\n");
        printf("5. Process Queue\n");
        printf("6. Sort by Name\n");
        printf("7. Back\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            for (int i = 0; i < count; i++)
                printf("%d | %s | %s\n",
                       requests[i].id,
                       requests[i].name,
                       requests[i].status);
        }

        else if (choice == 2) {
            int id;
            printf("Enter ID to approve: ");
            scanf("%d", &id);

            for (int i = 0; i < count; i++) {
                if (requests[i].id == id) {
                    strcpy(requests[i].status, "Approved");
                    printf("Request approved.\n");
                    break;
                }
            }
        }

        else if (choice == 3) {
            int id;
            printf("Enter ID to issue: ");
            scanf("%d", &id);

            for (int i = 0; i < count; i++) {
                if (requests[i].id == id) {
                    strcpy(requests[i].status, "Released");
                    printf("Request issued.\n");
                    break;
                }
            }
        }

        else if (choice == 4) undo_last_request();
        else if (choice == 5) process_queue();
        else if (choice == 6) sort_requests_by_name();
        else if (choice == 7) break;
        else printf("Invalid choice.\n");
    }
}

// ================= STACK =================
// Undo last request (LIFO)
void undo_last_request() {
    if (top == -1) {
        printf("No request to undo.\n");
        return;
    }

    int id = stack[top--];

    for (int i = 0; i < count; i++) {
        if (requests[i].id == id) {
            for (int j = i; j < count - 1; j++)
                requests[j] = requests[j + 1];

            count--;
            printf("Request undone.\n");
            return;
        }
    }
}

// ================= QUEUE =================
// Process requests in FIFO order
void process_queue() {
    if (front > rear) {
        printf("No requests in queue.\n");
        return;
    }

    int id = queue[front++];

    for (int i = 0; i < count; i++) {
        if (requests[i].id == id) {
            strcpy(requests[i].status, "Processing");
            printf("Processing request ID: %d\n", id);
            return;
        }
    }
}

// ================= SORT =================
// Bubble Sort (A-Z by name)
void sort_requests_by_name() {
    struct Request temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(requests[j].name, requests[j + 1].name) > 0) {
                temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    printf("Sorted successfully.\n");
}