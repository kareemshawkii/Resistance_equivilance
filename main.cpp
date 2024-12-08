#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
using namespace std;

float Calc(string input, char type) {
    float sum = 0;
    string x = "";
    try {
        for (int i = 0; i < input.length(); i++) {
                       if (input[i] == ' ') {
                if (x.empty()) throw invalid_argument("Empty resistance value detected.");
                if (type == 'S') sum += stof(x);
                else {
                    float value = stof(x);
                    if (value == 0) throw invalid_argument("Division by zero detected in parallel calculation.");
                    sum += 1 / value;
                }
                x = "";
            } else if (input[i] == 'S' || input[i] == 'P') {
                int indexOfFirstE = input.find('e', i + 2);
                if (indexOfFirstE == string::npos) throw invalid_argument("Invalid nested expression syntax.");
                if (type == 'S')
                    sum += Calc(input.substr(i + 2, indexOfFirstE - i - 2), input[i]);
                else {
                    float nestedValue = Calc(input.substr(i + 2, indexOfFirstE - i - 2), input[i]);
                    if (nestedValue == 0) throw invalid_argument("Division by zero detected in nested parallel calculation.");
                    sum += 1 / nestedValue;
                }
                i = indexOfFirstE + 1;
            } else {
                x += input[i];
            }
        }
        if (!x.empty()) {
            if (type == 'S') sum += stof(x);
            else {
                float value = stof(x);
                if (value == 0) throw invalid_argument("Division by zero detected in final calculation.");
                sum += 1 / value;
            }
        }
    } catch (const exception &e) {
        cerr << "Error during calculation: " << e.what() << endl;
        throw;
    }
    return type == 'S' ? sum : 1 / sum;
}

int main() {
    try {
        string input;

        while (true) {
            cout << "Please enter the resistances values and types (end with 'E'):\n";
            getline(cin, input);

            if (input.empty() || input[input.length() - 1] != 'E') {
                cout << "Input must end with 'E' to indicate end of input.\n";
                continue;
            }

            bool valid = true;
            for (int u = 0; u < input.length() - 1; u++) {
                if (input[u] == 'L' || input[u] == 'Z') {
                    cout << "Invalid connection type detected. Use 'S' or 'P'.\n";
                    valid = false;
                    break;
                }
            }

            if (valid) {
                break;
            }
        }

        float volt;

        while (true) {
            cout << "Please enter the voltage value:\n";
            if (!(cin >> volt)) {
                cout << "Invalid voltage value entered. Please enter a numeric value.\n";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                continue;
            }
            break;
        }

        float totalResistance = Calc(input.substr(2, input.size() - 3), input[0]);
        cout << "Total resistance is: " << totalResistance << endl;
        cout << "Current value is: " << volt / totalResistance << endl;

    } catch (const exception &e) {
        cerr << "An error occurred: " << e.what() << endl;
    }

    return 0;
}
