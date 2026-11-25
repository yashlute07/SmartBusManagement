
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    const int MAX = 100;
    string stack[MAX];
    int top = -1;

    // Load previously saved stack
    ifstream in("stack_state.txt");
    string line;

    while (getline(in, line) && top < MAX - 1) {
        stack[++top] = line;
    }
    in.close();

    string action;
    cin >> action;

    if (action == "push") {
        string value;
        cin >> ws;
        getline(cin, value);

        if (top == MAX - 1) {
            cout << "Stack Overflow";
        } else {
            stack[++top] = value;
            cout << "Pushed: " << value;
        }
    }
    else if (action == "pop") {
        if (top == -1) {
            cout << "Stack Underflow";
        } else {
            cout << "Popped: " << stack[top--];
        }
    }
    else if (action == "peek") {
        if (top == -1)
            cout << "Stack is Empty";
        else
            cout << "Top: " << stack[top];
    }
    else if (action == "display") {
        if (top == -1) {
            cout << "Stack is Empty";
        } else {
            for (int i = top; i >= 0; i--)
                cout << stack[i] << "\n";
        }
    }
    else if (action == "reset") {
        top = -1;
        cout << "Stack Reset";
    }
    else {
        cout << "Invalid Action";
    }

    // Save updated stack
    ofstream out("stack_state.txt");
    for (int i = 0; i <= top; i++)
        out << stack[i] << "\n";

    out.close();

    return 0;
}
