
#include <iostream>
#include <string>
using namespace std;

const int MAX_SIZE = 100;

class CircularQueue {
    string passengers[MAX_SIZE];
    int frontIndex, rearIndex, count;

public:
    CircularQueue() {
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == MAX_SIZE;
    }

    void enqueue(string passenger) {
        if (isFull()) {
            cout << "Queue is Full!\n";
            return;
        }

        rearIndex = (rearIndex + 1) % MAX_SIZE;
        passengers[rearIndex] = passenger;
        count++;

        cout << "Passenger added to waiting list!\n";
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is Empty!\n";
            return;
        }

        cout << "Now Serving: "
             << passengers[frontIndex] << endl;

        frontIndex = (frontIndex + 1) % MAX_SIZE;
        count--;
    }

    void peek() {
        if (isEmpty()) {
            cout << "No passengers waiting.\n";
        } else {
            cout << "Next Passenger: "
                 << passengers[frontIndex] << endl;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Waiting list is empty.\n";
            return;
        }

        cout << "\nWaiting Passengers:\n";

        for (int i = 0; i < count; i++) {
            int index = (frontIndex + i) % MAX_SIZE;
            cout << passengers[index] << endl;
        }
    }
};

int main() {
    CircularQueue waitingList;
    int choice;
    string passenger;

    do {
        cout << "\n--- SMART BUS QUEUE ---\n";
        cout << "1. Add Passenger (Enqueue)\n";
        cout << "2. Serve Passenger (Dequeue)\n";
        cout << "3. Next Passenger (Peek)\n";
        cout << "4. Display Waiting List\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        switch (choice) {
            case 1:
                cout << "Enter Passenger ID and Name: ";
                getline(cin, passenger);
                waitingList.enqueue(passenger);
                break;

            case 2:
                waitingList.dequeue();
                break;

            case 3:
                waitingList.peek();
                break;

            case 4:
                waitingList.display();
                break;

            case 0:
                cout << "Exiting Queue.\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
