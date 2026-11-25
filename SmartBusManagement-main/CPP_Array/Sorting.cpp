
#include <iostream>
#include <vector>
using namespace std;

void display(vector<int> a) {
    for (int value : a) {
        cout << value << " ";
    }
    cout << endl;
}

void bubbleSort(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

void selectionSort(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIndex]) {
                minIndex = j;
            }
        }

        swap(a[i], a[minIndex]);
    }
}

void insertionSort(vector<int>& a) {
    int n = a.size();

    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = key;
    }
}

int main() {
    int n, choice;

    cout << "--- SMART BUS SORTING ---\n";
    cout << "Enter number of values: ";
    cin >> n;

    if (n <= 0 || n > 1000) {
        cout << "Enter size between 1 and 1000.\n";
        return 0;
    }

    vector<int> values(n);

    cout << "Enter Passenger IDs or Fares:\n";

    for (int i = 0; i < n; i++) {
        cin >> values[i];
    }

    cout << "\nChoose Sorting Method:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Selection Sort\n";
    cout << "3. Insertion Sort\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            bubbleSort(values);
            cout << "Bubble Sort Result: ";
            break;

        case 2:
            selectionSort(values);
            cout << "Selection Sort Result: ";
            break;

        case 3:
            insertionSort(values);
            cout << "Insertion Sort Result: ";
            break;

        default:
            cout << "Invalid Choice!\n";
            return 0;
    }

    display(values);

    return 0;
}
