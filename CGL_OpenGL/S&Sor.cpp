
@'
#include <iostream>
using namespace std;

class PassengerSystem {
    int id[50];
    int n;

public:
    void input() {
        cout << "Enter number of passengers: ";
        cin >> n;

        cout << "Enter Passenger IDs:\n";
        for (int i = 0; i < n; i++) {
            cin >> id[i];
        }
    }

    void display() {
        cout << "\nPassenger IDs: ";
        for (int i = 0; i < n; i++) {
            cout << id[i] << " ";
        }
        cout << endl;
    }

    void linearSearch() {
        int key;
        cout << "Enter ID to search: ";
        cin >> key;

        for (int i = 0; i < n; i++) {
            if (id[i] == key) {
                cout << "Passenger Found at position "
                     << i + 1 << endl;
                return;
            }
        }

        cout << "Passenger Not Found!\n";
    }

    void bubbleSort() {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (id[j] > id[j + 1]) {
                    int temp = id[j];
                    id[j] = id[j + 1];
                    id[j + 1] = temp;
                }
            }
        }

        cout << "Passengers Sorted Successfully!\n";
        display();
    }

    void binarySearch() {
        int key;
        cout << "Enter ID to search: ";
        cin >> key;

        int low = 0;
        int high = n - 1;

        while (low <= high) {
            int mid = (low + high) / 2;

            if (id[mid] == key) {
                cout << "Passenger Found at position "
                     << mid + 1 << endl;
                return;
            }
            else if (id[mid] < key) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        cout << "Passenger Not Found!\n";
    }
};

int main() {
    PassengerSystem p;
    int choice;

    p.input();

    do {
        cout << "\n===== SMART BUS SEARCH & SORT =====\n";
        cout << "1. Display Passengers\n";
        cout << "2. Linear Search\n";
        cout << "3. Bubble Sort\n";
        cout << "4. Binary Search\n";
        cout << "5. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                p.display();
                break;

            case 2:
                p.linearSearch();
                break;

            case 3:
                p.bubbleSort();
                break;

            case 4:
                p.binarySearch();
                break;

            case 5:
                cout << "Thank You!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 5);

    return 0;
}
'@ | Set-Content -Encoding ASCII SearchingSorting.cpp
