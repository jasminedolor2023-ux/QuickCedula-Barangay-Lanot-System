#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// ================= STRUCT =================
typedef struct Request {
    int id;
    char name[50];
    char document[50];
    char status[20];
    struct Request* next;
} Request;

// ================= GLOBAL =================
int idCounter = 1;

// Queue (Pending)
Request *front = NULL, *rear = NULL;

// Linked List (Approved/Issued)
Request *approvedHead = NULL;

// Stack (Undo)
Request* stack[MAX];
int top = -1;

// ================= STACK =================
void push(Request* r) {
    if (top < MAX - 1) stack[++top] = r;
}

Request* pop() {
    if (top >= 0) return stack[top--];
    return NULL;
}

// ================= QUEUE =================
void enqueue(char name[], char doc[]) {
    Request* r = (Request*)malloc(sizeof(Request));

    r->id = idCounter++;
    strcpy(r->name, name);
    strcpy(r->document, doc);
    strcpy(r->status, "Pending");
    r->next = NULL;

    if (!rear) front = rear = r;
    else {
        rear->next = r;
        rear = r;
    }

    printf("\nRequest submitted successfully! (Status: Pending)\n");
}

Request* dequeue() {
    if (!front) return NULL;

    Request* temp = front;
    front = front->next;

    if (!front) rear = NULL;
    return temp;
}

// ================= LINKED LIST =================
void addApproved(Request* r, char status[]) {
    strcpy(r->status, status);
    r->next = approvedHead;
    approvedHead = r;
}

// ================= USER: VIEW STATUS =================
void viewMyStatus(char name[]) {
    Request* temp;
    int found = 0;

    printf("\n===== MY REQUEST STATUS =====\n");

    // Pending
    temp = front;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("[ID: %d] %s ? %s\n",
                   temp->id, temp->document, temp->status);
            found = 1;
        }
        temp = temp->next;
    }

    // Approved / Issued
    temp = approvedHead;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            if (strcmp(temp->status, "Issued") == 0) {
                printf("[ID: %d] %s ? READY FOR PICKUP\n",
                       temp->id, temp->document);
            } else {
                printf("[ID: %d] %s ? %s\n",
                       temp->id, temp->document, temp->status);
            }
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("You have no requests yet.\n");
    }
}

// ================= ADMIN =================
void viewPending() {
    Request* temp = front;

    printf("\n--- PENDING REQUESTS ---\n");
    if (!temp) {
        printf("No pending requests.\n");
        return;
    }

    while (temp) {
        printf("ID:%d | %s | %s\n", temp->id, temp->name, temp->document);
        temp = temp->next;
    }
}

void viewApproved() {
    Request* temp = approvedHead;

    printf("\n--- APPROVED / ISSUED ---\n");
    if (!temp) {
        printf("No records.\n");
        return;
    }

    while (temp) {
        printf("ID:%d | %s | %s | %s\n",
               temp->id, temp->name, temp->document, temp->status);
        temp = temp->next;
    }
}

// ================= SEARCH =================
void searchRequest(int id) {
    Request* temp = approvedHead;

    while (temp) {
        if (temp->id == id) {
            printf("\nFOUND: %s | %s | %s\n",
                   temp->name, temp->document, temp->status);
            return;
        }
        temp = temp->next;
    }

    printf("Request not found.\n");
}

// ================= SORT =================
void sortApproved() {
    Request *i, *j;
    int tempID;
    char name[50], doc[50], status[20];

    for (i = approvedHead; i; i = i->next) {
        for (j = i->next; j; j = j->next) {
            if (i->id > j->id) {
                tempID = i->id;
                i->id = j->id;
                j->id = tempID;

                strcpy(name, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, name);

                strcpy(doc, i->document);
                strcpy(i->document, j->document);
                strcpy(j->document, doc);

                strcpy(status, i->status);
                strcpy(i->status, j->status);
                strcpy(j->status, status);
            }
        }
    }

    printf("Records sorted by ID.\n");
}

// ================= UNDO =================
void undo() {
    Request* r = pop();

    if (!r) {
        printf("Nothing to undo.\n");
        return;
    }

    r->next = front;
    front = r;

    printf("Undo successful! Returned to pending.\n");
}

// ================= MAIN =================
int main() {
    int choice, subChoice;
    char name[50], doc[50];
    int id;

    while (1) {
        printf("\n===== QUICKCEDULA SYSTEM =====\n");
        printf("1. Request Document\n");
        printf("2. View My Request Status\n");
        printf("3. Admin Panel\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Enter Name: ");
            scanf(" %[^\n]", name);

            if (strlen(name) == 0) {
                printf("Name cannot be empty!\n");
                break;
            }

            printf("\nSelect Document:\n");
            printf("1. Cedula\n");
            printf("2. Barangay Certificate\n");
            printf("Choice: ");
            scanf("%d", &subChoice);

            if (subChoice == 1) strcpy(doc, "Cedula");
            else if (subChoice == 2) strcpy(doc, "Barangay Certificate");
            else {
                printf("Invalid choice!\n");
                break;
            }

            enqueue(name, doc);
            break;

        case 2:
            printf("Enter your name: ");
            scanf(" %[^\n]", name);
            viewMyStatus(name);
            break;

        case 3:
            printf("\n--- ADMIN PANEL ---\n");
            printf("1. View Pending\n");
            printf("2. Approve Request\n");
            printf("3. Issue Document\n");
            printf("4. View All Records\n");
            printf("5. Search Request\n");
            printf("6. Sort Records\n");
            printf("7. Undo Last Action\n");
            printf("Choice: ");
            scanf("%d", &subChoice);

            if (subChoice == 1) viewPending();

            else if (subChoice == 2) {
                Request* r = dequeue();
                if (r) {
                    addApproved(r, "Approved");
                    push(r);
                    printf("Request Approved.\n");
                } else printf("No pending.\n");
            }

            else if (subChoice == 3) {
                Request* r = dequeue();
                if (r) {
                    addApproved(r, "Issued");
                    push(r);
                    printf("Document Issued.\n");
                } else printf("No pending.\n");
            }

            else if (subChoice == 4) viewApproved();

            else if (subChoice == 5) {
                printf("Enter ID: ");
                scanf("%d", &id);
                searchRequest(id);
            }

            else if (subChoice == 6) sortApproved();

            else if (subChoice == 7) undo();

            else printf("Invalid choice.\n");

            break;

        case 4:
            printf("Exiting program...\n");
            exit(0);

        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}