
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX = 100;
string queueData[MAX];
int frontIndex = 0, rearIndex = -1;

void loadQueue() {
    ifstream in("queue_state.txt");
    string value;

    while (getline(in, value) && rearIndex < MAX - 1) {
        queueData[++rearIndex] = value;
    }
    in.close();
}

void saveQueue() {
    ofstream out("queue_state.txt");
    for (int i = frontIndex; i <= rearIndex; i++) {
        out << queueData[i] << "\n";
    }
    out.close();
}

int main() {
    loadQueue();

    string action;
    cin >> action;

    if (action == "enqueue") {
        string value;
        cin >> ws;
        getline(cin, value);

        if (rearIndex == MAX - 1) {
            cout << "Queue Overflow";
        } else {
            queueData[++rearIndex] = value;
            cout << "Enqueued: " << value;
        }
    }
    else if (action == "dequeue") {
        if (frontIndex > rearIndex) {
            cout << "Queue is Empty";
        } else {
            cout << "Dequeued: " << queueData[frontIndex++];
        }
    }
    else if (action == "peek") {
        if (frontIndex > rearIndex) {
            cout << "Queue is Empty";
        } else {
            cout << "Front: " << queueData[frontIndex];
        }
    }
    else if (action == "display") {
        if (frontIndex > rearIndex) {
            cout << "Queue is Empty";
        } else {
            for (int i = frontIndex; i <= rearIndex; i++) {
                cout << queueData[i] << "\n";
            }
        }
    }
    else if (action == "reset") {
        frontIndex = 0;
        rearIndex = -1;
        cout << "Queue Reset";
    }
    else {
        cout << "Invalid Action";
    }

    saveQueue();
    return 0;
}
