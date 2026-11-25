#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;

string lowerText(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return (char)tolower(c); });
    return s;
}

bool comesAfter(const string& a, const string& b) {
    return lowerText(a) > lowerText(b);
}

int main() {
    string algorithm;
    getline(cin, algorithm);

    int count;
    if (!(cin >> count) || count <= 0) {
        cout << "No passengers provided";
        return 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> names(count);
    vector<int> order(count);

    for (int i = 0; i < count; i++) {
        if (!getline(cin, names[i])) {
            cout << "Invalid passenger data";
            return 0;
        }
        order[i] = i;
    }

    if (algorithm == "bubble") {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (comesAfter(names[order[j]], names[order[j + 1]]))
                    swap(order[j], order[j + 1]);
            }
        }
    }
    else if (algorithm == "selection") {
        for (int i = 0; i < count - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < count; j++) {
                if (comesAfter(names[order[minIndex]], names[order[j]]))
                    minIndex = j;
            }
            swap(order[i], order[minIndex]);
        }
    }
    else if (algorithm == "insertion") {
        for (int i = 1; i < count; i++) {
            int key = order[i];
            int j = i - 1;
            while (j >= 0 && comesAfter(names[order[j]], names[key])) {
                order[j + 1] = order[j];
                j--;
            }
            order[j + 1] = key;
        }
    }
    else {
        cout << "Invalid Algorithm";
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (i) cout << " ";
        cout << order[i];
    }

    return 0;
}