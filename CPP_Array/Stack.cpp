
#include <iostream>
#include <string>
using namespace std;

const int MAX_SIZE = 100;

class Stack {
    string items[MAX_SIZE];
    int topIndex;

public:
    Stack() {
        topIndex = -1;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == MAX_SIZE - 1;
    }

    void push(string value) {
        if (isFull()) {
            cout << "Stack Overflow!\n";
            return;
        }

        items[++topIndex] = value;
        cout << "Booking added successfully!\n";
    }

    void pop() {
        if (isEmpty()) {
            cout << "Stack Underflow!\n";
            return;
        }

        cout << "Removed booking: "
             << items[topIndex] << endl;

        topIndex--;
    }

    void peek() {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
        } else {
            cout << "Latest booking: "
                 << items[topIndex] << endl;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return;
        }

        cout << "\nRecent Bookings:\n";

        for (int i = topIndex; i >= 0; i--) {
            cout << items[i] << endl;
        }
    }
};

int main() {
    Stack bookings;
    int choice;
    string booking;

    do {
        cout << "\n--- SMART BUS STACK ---\n";
        cout << "1. Add Booking (Push)\n";
        cout << "2. Undo Booking (Pop)\n";
        cout << "3. Latest Booking (Peek)\n";
        cout << "4. Display Bookings\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        switch (choice) {
            case 1:
                cout << "Enter booking details: ";
                getline(cin, booking);
                bookings.push(booking);
                break;

            case 2:
                bookings.pop();
                break;

            case 3:
                bookings.peek();
                break;

            case 4:
                bookings.display();
                break;

            case 0:
                cout << "Exiting Stack.\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
