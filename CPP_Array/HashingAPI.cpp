#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int SIZE = 23;
const string DELETED = "__DELETED__";
string hashTable[SIZE];

int hashFunction(const string& key) {
    unsigned int sum = 0;
    for (unsigned char ch : key)
        sum += ch;
    return sum % SIZE;
}

void loadTable() {
    ifstream in("passenger_hash_state.txt");
    int index;
    string value;

    while (in >> index >> ws && getline(in, value)) {
        if (index >= 0 && index < SIZE)
            hashTable[index] = value;
    }
}

void saveTable() {
    ofstream out("passenger_hash_state.txt");
    for (int i = 0; i < SIZE; i++) {
        if (!hashTable[i].empty())
            out << i << " " << hashTable[i] << "\n";
    }
}

int main() {
    loadTable();

    string action, key;
    cin >> action;

    if (action == "reset") {
        for (int i = 0; i < SIZE; i++)
            hashTable[i].clear();

        saveTable();
        cout << "Hash Table Reset";
        return 0;
    }

    if (action == "display") {
        for (int i = 0; i < SIZE; i++) {
            cout << i << ": ";
            cout << (hashTable[i].empty() ? "Empty" : hashTable[i]);
            cout << "\n";
        }
        return 0;
    }

    cin >> ws;
    getline(cin, key);

    if (key.empty()) {
        cout << "Invalid ID";
        return 0;
    }

    int start = hashFunction(key);

    if (action == "insert") {
        int firstDeleted = -1;

        for (int step = 0; step < SIZE; step++) {
            int index = (start + step) % SIZE;

            if (hashTable[index] == key) {
                cout << "Already exists";
                return 0;
            }

            if (hashTable[index] == DELETED && firstDeleted == -1)
                firstDeleted = index;

            if (hashTable[index].empty()) {
                int target = firstDeleted == -1 ? index : firstDeleted;
                hashTable[target] = key;
                saveTable();
                cout << "Inserted: " << key << " at index " << target;
                return 0;
            }
        }

        if (firstDeleted != -1) {
            hashTable[firstDeleted] = key;
            saveTable();
            cout << "Inserted: " << key << " at index " << firstDeleted;
        } else {
            cout << "Hash Table Full";
        }
    }
    else if (action == "search") {
        for (int step = 0; step < SIZE; step++) {
            int index = (start + step) % SIZE;

            if (hashTable[index].empty())
                break;

            if (hashTable[index] == key) {
                cout << "Found at index " << index;
                return 0;
            }
        }

        cout << "Not Found";
    }
    else if (action == "delete") {
        for (int step = 0; step < SIZE; step++) {
            int index = (start + step) % SIZE;

            if (hashTable[index].empty())
                break;

            if (hashTable[index] == key) {
                hashTable[index] = DELETED;
                saveTable();
                cout << "Deleted: " << key;
                return 0;
            }
        }

        cout << "Not Found";
    }
    else {
        cout << "Invalid Action";
    }

    return 0;
}