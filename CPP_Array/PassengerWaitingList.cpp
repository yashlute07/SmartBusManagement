//Varad
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ============================================================
// NODE
// Each node stores one passenger in the waiting list
// ============================================================
struct Node {
    int passengerId;
    string passengerName;
    string destination;
    Node* next;

    Node(int id, string name, string dest) {
        passengerId = id;
        passengerName = name;
        destination = dest;
        next = nullptr;
    }
};


// ============================================================
// WAITING LIST CLASS
// Singly Linked List implementation
// ============================================================
class WaitingList {

private:
    Node* head;
    int passengerCount;

public:

    // Constructor
    WaitingList() {
        head = nullptr;
        passengerCount = 0;
    }


    // ========================================================
    // ADD PASSENGER
    // Adds a new passenger at the end of the linked list
    // ========================================================
    void addPassenger(int id, string name, string destination) {

        Node* newNode =
            new Node(id, name, destination);

        // If list is empty
        if (head == nullptr) {
            head = newNode;
        }

        else {
            Node* temp = head;

            // Move to the last node
            while (temp->next != nullptr) {
                temp = temp->next;
            }

            // Connect last node to new node
            temp->next = newNode;
        }

        passengerCount++;

        cout << "\n";
        cout << "  [SUCCESS] Passenger added to waiting list.\n";
        cout << "  Passenger ID : " << id << endl;
        cout << "  Name         : " << name << endl;
        cout << "  Destination  : " << destination << endl;
    }


    // ========================================================
    // DISPLAY WAITING LIST
    // ========================================================
    void displayWaitingList() {

        if (head == nullptr) {

            cout << "\n";
            cout << "  [INFO] Waiting list is currently empty.\n";
            return;
        }

        cout << "\n";
        cout << "============================================================\n";
        cout << "                 PASSENGER WAITING LIST\n";
        cout << "============================================================\n";

        cout << left
             << setw(8) << "No."
             << setw(15) << "Passenger ID"
             << setw(22) << "Passenger Name"
             << setw(20) << "Destination"
             << endl;

        cout << "------------------------------------------------------------\n";

        Node* temp = head;
        int position = 1;

        while (temp != nullptr) {

            cout << left
                 << setw(8) << position
                 << setw(15) << temp->passengerId
                 << setw(22) << temp->passengerName
                 << setw(20) << temp->destination
                 << endl;

            temp = temp->next;
            position++;
        }

        cout << "------------------------------------------------------------\n";
        cout << "  Total Waiting Passengers : " << passengerCount << endl;
        cout << "============================================================\n";
    }

//Yash

    // ========================================================
    // DISPLAY LINKED LIST STRUCTURE
    // Useful for PL CO2 viva demonstration
    // ========================================================
    void displayLinkedList() {

        if (head == nullptr) {

            cout << "\n  Waiting List: NULL\n";
            return;
        }

        cout << "\n";
        cout << "============================================================\n";
        cout << "              LINKED LIST VISUALIZATION\n";
        cout << "============================================================\n\n";

        Node* temp = head;

        while (temp != nullptr) {

            cout << "[ "
                 << temp->passengerId
                 << " | "
                 << temp->passengerName
                 << " ]";

            if (temp->next != nullptr) {
                cout << "  -->  ";
            }

            else {
                cout << "  -->  NULL";
            }

            temp = temp->next;
        }

        cout << "\n\n";
        cout << "  Each node points to the next passenger.\n";
        cout << "============================================================\n";
    }


    // ========================================================
    // REMOVE FIRST PASSENGER
    // Removes the first node from linked list
    // ========================================================
    void removeFirstPassenger() {

        if (head == nullptr) {

            cout << "\n";
            cout << "  [INFO] Waiting list is empty.\n";
            return;
        }

        Node* temp = head;

        cout << "\n";
        cout << "  Processing first passenger...\n";

        cout << "  Passenger ID   : "
             << temp->passengerId << endl;

        cout << "  Passenger Name : "
             << temp->passengerName << endl;

        // Move head to next node
        head = head->next;

        // Delete old first node
        delete temp;

        passengerCount--;

        cout << "\n";
        cout << "  [SUCCESS] Passenger removed from waiting list.\n";
    }


    // ========================================================
    // SEARCH PASSENGER
    // ========================================================
    void searchPassenger(int id) {

        Node* temp = head;

        while (temp != nullptr) {

            if (temp->passengerId == id) {

                cout << "\n";
                cout << "============================================================\n";
                cout << "                  PASSENGER FOUND\n";
                cout << "============================================================\n";

                cout << "  Passenger ID   : "
                     << temp->passengerId << endl;

                cout << "  Passenger Name : "
                     << temp->passengerName << endl;

                cout << "  Destination    : "
                     << temp->destination << endl;

                cout << "============================================================\n";

                return;
            }

            temp = temp->next;
        }

        cout << "\n";
        cout << "  [NOT FOUND] Passenger is not in the waiting list.\n";
    }


    // ========================================================
    // DISPLAY NUMBER OF PASSENGERS
    // ========================================================
    void displayCount() {

        cout << "\n";
        cout << "============================================================\n";
        cout << "                  WAITING LIST STATUS\n";
        cout << "============================================================\n";

        cout << "  Current waiting passengers : "
             << passengerCount << endl;

        if (passengerCount == 0) {
            cout << "  Status                     : EMPTY\n";
        }
        else {
            cout << "  Status                     : ACTIVE\n";
        }

        cout << "============================================================\n";
    }


    // ========================================================
    // DESTRUCTOR
    // Frees remaining dynamically allocated nodes
    // ========================================================
    ~WaitingList() {

        Node* temp;

        while (head != nullptr) {

            temp = head;
            head = head->next;

            delete temp;
        }
    }
};


// ============================================================
// HEADER
// ============================================================
void displayHeader() {

    cout << "\n";
    cout << "============================================================\n";
    cout << "                                                            \n";
    cout << "              SMART BUS MANAGEMENT SYSTEM                   \n";
    cout << "              PASSENGER WAITING LIST                        \n";
    cout << "                                                            \n";
    cout << "============================================================\n";
}


// ============================================================
// MENU
// ============================================================
void displayMenu() {

    cout << "\n";
    cout << "+----------------------------------------------------------+\n";
    cout << "|                      MAIN MENU                           |\n";
    cout << "+----------------------------------------------------------+\n";
    cout << "|  1. Add Passenger to Waiting List                       |\n";
    cout << "|  2. View Waiting List                                   |\n";
    cout << "|  3. View Linked List Structure                          |\n";
    cout << "|  4. Remove First Passenger                              |\n";
    cout << "|  5. Search Passenger                                    |\n";
    cout << "|  6. View Waiting List Status                            |\n";
    cout << "|  7. Exit                                                |\n";
    cout << "+----------------------------------------------------------+\n";
}


// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {

    WaitingList waitingList;

    int choice;

    displayHeader();

    do {

        displayMenu();

        cout << "\n  Enter your choice: ";
        cin >> choice;

        switch (choice) {

            // ------------------------------------------------
            // ADD PASSENGER
            // ------------------------------------------------
            case 1: {

                int id;
                string name;
                string destination;

                cout << "\n------------------------------------------------------------\n";
                cout << "                  ADD PASSENGER\n";
                cout << "------------------------------------------------------------\n";

                cout << "  Enter Passenger ID  : ";
                cin >> id;

                cout << "  Enter Passenger Name: ";
                cin >> name;

                cout << "  Enter Destination   : ";
                cin >> destination;

                waitingList.addPassenger(
                    id,
                    name,
                    destination
                );

                break;
            }


            // ------------------------------------------------
            // DISPLAY WAITING LIST
            // ------------------------------------------------
            case 2:

                waitingList.displayWaitingList();
                break;


            // ------------------------------------------------
            // DISPLAY LINKED LIST
            // ------------------------------------------------
            case 3:

                waitingList.displayLinkedList();
                break;


            // ------------------------------------------------
            // REMOVE FIRST PASSENGER
            // ------------------------------------------------
            case 4:

                waitingList.removeFirstPassenger();
                break;


            // ------------------------------------------------
            // SEARCH PASSENGER
            // ------------------------------------------------
            case 5: {

                int id;

                cout << "\n";
                cout << "  Enter Passenger ID to search: ";
                cin >> id;

                waitingList.searchPassenger(id);

                break;
            }


            // ------------------------------------------------
            // STATUS
            // ------------------------------------------------
            case 6:

                waitingList.displayCount();
                break;


            // ------------------------------------------------
            // EXIT
            // ------------------------------------------------
            case 7:

                cout << "\n";
                cout << "============================================================\n";
                cout << "       Thank you for using Smart Bus System!\n";
                cout << "============================================================\n";

                break;


            default:

                cout << "\n";
                cout << "  [ERROR] Invalid choice. Please try again.\n";
        }

    } while (choice != 7);


    return 0;
}