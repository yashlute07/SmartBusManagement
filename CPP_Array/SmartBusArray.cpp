//Hitesh
#include <iostream>
using namespace std;

struct Bus {
    int busNumber;
    string source;
    string destination;
    int totalSeats;
};

Bus buses[10];
int busCount = 0;

void addBus() {

    if (busCount >= 10) {
        cout << "Bus limit reached." << endl;
        return;
    }

    cout << "Enter Bus Number: ";
    cin >> buses[busCount].busNumber;

    cout << "Enter Source: ";
    cin >> buses[busCount].source;

    cout << "Enter Destination: ";
    cin >> buses[busCount].destination;

    cout << "Enter Total Seats: ";
    cin >> buses[busCount].totalSeats;

    busCount++;

    cout << "Bus added successfully." << endl;
}

//Sarthak

void displayBuses() {

    if (busCount == 0) {
        cout << "No buses available." << endl;
        return;
    }

    cout << "\n===== BUS DETAILS =====" << endl;

    for (int i = 0; i < busCount; i++) {

        cout << "\nBus Number: "
             << buses[i].busNumber << endl;

        cout << "Source: "
             << buses[i].source << endl;

        cout << "Destination: "
             << buses[i].destination << endl;

        cout << "Total Seats: "
             << buses[i].totalSeats << endl;
    }
}

void searchBus() {

    int number;
    bool found = false;

    cout << "Enter Bus Number to search: ";
    cin >> number;

    for (int i = 0; i < busCount; i++) {

        if (buses[i].busNumber == number) {

            cout << "\nBus Found!" << endl;
            cout << "Bus Number: " << buses[i].busNumber << endl;
            cout << "Source: " << buses[i].source << endl;
            cout << "Destination: " << buses[i].destination << endl;
            cout << "Total Seats: " << buses[i].totalSeats << endl;

            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Bus not found." << endl;
    }
}

int main() {

    int choice;

    do {

        cout << "\n===== SMART BUS MANAGEMENT SYSTEM =====" << endl;
        cout << "1. Add Bus" << endl;
        cout << "2. Display Buses" << endl;
        cout << "3. Search Bus" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {

            case 1:
                addBus();
                break;

            case 2:
                displayBuses();
                break;

            case 3:
                searchBus();
                break;

            case 4:
                cout << "Exiting program..." << endl;
                break;

            default:
                cout << "Invalid choice." << endl;
        }

    } while (choice != 4);

    return 0;
}