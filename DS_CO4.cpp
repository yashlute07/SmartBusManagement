#include <iostream>
using namespace std;

#define SIZE 5

class PassengerQueue {
    int queue[SIZE];
    int front, rear;

public:
    PassengerQueue() {
        front = rear = -1;
    }

    void addPassenger(int id) {
        if ((rear + 1) % SIZE == front) {
            cout << "Queue is Full!\n";
            return;
        }

        if (front == -1)
            front = 0;

        rear = (rear + 1) % SIZE;
        queue[rear] = id;

        cout << "Passenger Added Successfully!\n";
    }

    void servePassenger() {
        if (front == -1) {
            cout << "Queue is Empty!\n";
            return;
        }

        cout << "Passenger Served: " << queue[front] << endl;

        if (front == rear)
            front = rear = -1;
        else
            front = (front + 1) % SIZE;
    }

    void displayQueue() {
        if (front == -1) {
            cout << "No Waiting Passengers!\n";
            return;
        }

        int i = front;

        cout << "\nWaiting Passengers:\n";

        while (true) {
            cout << "Passenger ID: " << queue[i] << endl;

            if (i == rear)
                break;

            i = (i + 1) % SIZE;
        }
    }
};

int main() {
    PassengerQueue q;
    int choice, id;

    do {
        cout << "\n===== SMART BUS QUEUE =====\n";
        cout << "1. Add Passenger\n";
        cout << "2. Serve Passenger\n";
        cout << "3. Display Queue\n";
        cout << "4. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Passenger ID: ";
                cin >> id;
                q.addPassenger(id);
                break;

            case 2:
                q.servePassenger();
                break;

            case 3:
                q.displayQueue();
                break;

            case 4:
                cout << "Thank You!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 4);

    return 0;
}
