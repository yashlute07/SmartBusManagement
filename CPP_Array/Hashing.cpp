
#include <iostream>
using namespace std;

const int TABLE_SIZE = 10;

class HashTable {
    int keys[TABLE_SIZE];
    bool occupied[TABLE_SIZE];

    int hashFunction(int key) {
        int index = key % TABLE_SIZE;

        if (index < 0)
            index += TABLE_SIZE;

        return index;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            keys[i] = 0;
            occupied[i] = false;
        }
    }

    void insertKey(int key) {
        int start = hashFunction(key);

        for (int step = 0; step < TABLE_SIZE; step++) {
            int index = (start + step) % TABLE_SIZE;

            if (!occupied[index]) {
                keys[index] = key;
                occupied[index] = true;

                cout << "Passenger ID " << key
                     << " inserted at index "
                     << index << endl;
                return;
            }

            if (keys[index] == key) {
                cout << "Passenger ID already exists!\n";
                return;
            }
        }

        cout << "Hash Table is Full!\n";
    }

    int searchKey(int key) {
        int start = hashFunction(key);

        for (int step = 0; step < TABLE_SIZE; step++) {
            int index = (start + step) % TABLE_SIZE;

            if (!occupied[index])
                return -1;

            if (keys[index] == key)
                return index;
        }

        return -1;
    }

    void display() {
        cout << "\n--- HASH TABLE ---\n";

        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << i << " : ";

            if (occupied[i])
                cout << keys[i];
            else
                cout << "EMPTY";

            cout << endl;
        }
    }
};

int main() {
    HashTable table;
    int choice, id;

    do {
        cout << "\n--- SMART BUS HASHING ---\n";
        cout << "1. Insert Passenger ID\n";
        cout << "2. Search Passenger ID\n";
        cout << "3. Display Hash Table\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Passenger ID: ";
                cin >> id;
                table.insertKey(id);
                break;

            case 2: {
                cout << "Enter ID to Search: ";
                cin >> id;

                int result = table.searchKey(id);

                if (result == -1)
                    cout << "Passenger Not Found!\n";
                else
                    cout << "Passenger Found at Index "
                         << result << endl;
                break;
            }

            case 3:
                table.display();
                break;

            case 0:
                cout << "Exiting Hashing.\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
