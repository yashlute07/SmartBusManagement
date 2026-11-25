
#include <iostream>
#include <string>
#include <iomanip>
#include <queue>
#include <stack>
#include <limits>
using namespace std;

const int MAX_PASSENGERS = 10;

// Passenger details
struct Passenger {
    int passengerId;
    string passengerName;
    string destination;
};

// Node for singly linked list
struct Node {
    Passenger data;
    Node* next;

    Node(int id, string name, string dest) {
        data.passengerId = id;
        data.passengerName = name;
        data.destination = dest;
        next = nullptr;
    }
};

// ==================================================
// LINKED LIST: EXISTING WAITING LIST
// ==================================================
class WaitingList {
private:
    Node* head;
    int passengerCount;

public:
    WaitingList() {
        head = nullptr;
        passengerCount = 0;
    }

    void addPassenger(int id, string name, string destination) {
        Node* newNode = new Node(id, name, destination);

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        passengerCount++;
        cout << "\nPassenger added successfully!\n";
    }

    void displayWaitingList() {
        if (head == nullptr) {
            cout << "\nWaiting list is empty.\n";
            return;
        }

        cout << "\n========== PASSENGER WAITING LIST ==========\n";
        cout << left << setw(10) << "ID"
             << setw(22) << "Name"
             << setw(20) << "Destination" << '\n';

        Node* temp = head;

        while (temp != nullptr) {
            cout << left << setw(10) << temp->data.passengerId
                 << setw(22) << temp->data.passengerName
                 << setw(20) << temp->data.destination << '\n';
            temp = temp->next;
        }

        cout << "Total passengers: " << passengerCount << '\n';
    }

    void displayLinkedList() {
        if (head == nullptr) {
            cout << "\nWaiting List: NULL\n";
            return;
        }

        Node* temp = head;
        cout << "\nLinked List:\n";

        while (temp != nullptr) {
            cout << "[ " << temp->data.passengerId
                 << " | " << temp->data.passengerName << " ]";
            cout << " -> ";
            temp = temp->next;
        }

        cout << "NULL\n";
    }

    void removeFirstPassenger() {
        if (head == nullptr) {
            cout << "\nCannot remove: waiting list is empty.\n";
            return;
        }

        Node* temp = head;

        cout << "\nRemoved passenger: "
             << temp->data.passengerName
             << " (ID: " << temp->data.passengerId << ")\n";

        head = head->next;
        delete temp;
        passengerCount--;
    }

    void searchPassenger(int id) {
        Node* temp = head;

        while (temp != nullptr) {
            if (temp->data.passengerId == id) {
                cout << "\nPassenger found!\n";
                cout << "ID: " << temp->data.passengerId << '\n';
                cout << "Name: " << temp->data.passengerName << '\n';
                cout << "Destination: " << temp->data.destination << '\n';
                return;
            }
            temp = temp->next;
        }

        cout << "\nPassenger not found.\n";
    }

    void displayCount() {
        cout << "\nWaiting passengers: " << passengerCount << '\n';
        cout << "Status: "
             << (passengerCount == 0 ? "EMPTY" : "ACTIVE")
             << '\n';
    }

    ~WaitingList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ==================================================
// INPUT HELPER
// ==================================================
int readInteger(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            return value;
        }

        cout << "Invalid input! Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Passenger readPassenger() {
    Passenger p;

    p.passengerId = readInteger("Enter Passenger ID: ");

    cout << "Enter Passenger Name (one word): ";
    cin >> p.passengerName;

    cout << "Enter Destination (one word): ";
    cin >> p.destination;

    return p;
}

void displayPassenger(const Passenger& p) {
    cout << "ID: " << p.passengerId
         << " | Name: " << p.passengerName
         << " | Destination: " << p.destination << '\n';
}

// ==================================================
// QUEUE: FIFO (FIRST IN, FIRST OUT)
// ==================================================
void enqueuePassenger(queue<Passenger>& q) {
    if (q.size() >= MAX_PASSENGERS) {
        cout << "\nQueue is full! Cannot add more passengers.\n";
        return;
    }

    Passenger p = readPassenger();
    q.push(p);

    cout << "Passenger added to Queue successfully.\n";
}

void dequeuePassenger(queue<Passenger>& q) {
    if (q.empty()) {
        cout << "\nQueue is empty! No passenger to process.\n";
        return;
    }

    cout << "\nProcessing first passenger (FIFO):\n";
    displayPassenger(q.front());
    q.pop();
}

void displayQueue(queue<Passenger> q) {
    if (q.empty()) {
        cout << "\nQueue is empty.\n";
        return;
    }

    cout << "\nQueue (Front -> Rear):\n";

    while (!q.empty()) {
        displayPassenger(q.front());
        q.pop();
    }
}

// ==================================================
// STACK: LIFO (LAST IN, FIRST OUT)
// ==================================================
void pushPassenger(stack<Passenger>& s) {
    if (s.size() >= MAX_PASSENGERS) {
        cout << "\nStack is full! Cannot add more passengers.\n";
        return;
    }

    Passenger p = readPassenger();
    s.push(p);

    cout << "Passenger added to Stack successfully.\n";
}

void popPassenger(stack<Passenger>& s) {
    if (s.empty()) {
        cout << "\nStack is empty! No passenger to remove.\n";
        return;
    }

    cout << "\nProcessing most recently added passenger (LIFO):\n";
    displayPassenger(s.top());
    s.pop();
}

void displayStack(stack<Passenger> s) {
    if (s.empty()) {
        cout << "\nStack is empty.\n";
        return;
    }

    cout << "\nStack (Top -> Bottom):\n";

    while (!s.empty()) {
        displayPassenger(s.top());
        s.pop();
    }
}

// ==================================================
// MAIN MENU
// ==================================================
int main() {
    WaitingList waitingList;
    queue<Passenger> passengerQueue;
    stack<Passenger> passengerStack;

    int choice;

    do {
        cout << "\n============================================\n";
        cout << "       SMART BUS MANAGEMENT SYSTEM\n";
        cout << "============================================\n";
        cout << "LINKED LIST OPERATIONS\n";
        cout << "1. Add Passenger to Waiting List\n";
        cout << "2. View Waiting List\n";
        cout << "3. View Linked List Structure\n";
        cout << "4. Remove First Passenger\n";
        cout << "5. Search Passenger\n";
        cout << "6. View Waiting List Status\n";
        cout << "\nQUEUE OPERATIONS (FIFO)\n";
        cout << "7. Enqueue Passenger\n";
        cout << "8. Dequeue Passenger\n";
        cout << "9. Display Queue\n";
        cout << "\nSTACK OPERATIONS (LIFO)\n";
        cout << "10. Push Passenger\n";
        cout << "11. Pop Passenger\n";
        cout << "12. Display Stack\n";
        cout << "13. Exit\n";
        cout << "============================================\n";

        choice = readInteger("Enter your choice: ");

        switch (choice) {
            case 1: {
                Passenger p = readPassenger();
                waitingList.addPassenger(
                    p.passengerId, p.passengerName, p.destination
                );
                break;
            }

            case 2:
                waitingList.displayWaitingList();
                break;

            case 3:
                waitingList.displayLinkedList();
                break;

            case 4:
                waitingList.removeFirstPassenger();
                break;

            case 5: {
                int id = readInteger("Enter Passenger ID to search: ");
                waitingList.searchPassenger(id);
                break;
            }

            case 6:
                waitingList.displayCount();
                break;

            case 7:
                enqueuePassenger(passengerQueue);
                break;

            case 8:
                dequeuePassenger(passengerQueue);
                break;

            case 9:
                displayQueue(passengerQueue);
                break;

            case 10:
                pushPassenger(passengerStack);
                break;

            case 11:
                popPassenger(passengerStack);
                break;

            case 12:
                displayStack(passengerStack);
                break;

            case 13:
                cout << "\nExiting Smart Bus Management System.\n";
                break;

            default:
                cout << "\nInvalid choice! Select an option from 1 to 13.\n";
        }

    } while (choice != 13);

    return 0;
}

