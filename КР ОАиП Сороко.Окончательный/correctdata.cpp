#include"Header.h"

void inputString(string& input) {
    bool hasLettersOrDigits, hasOnlySymbols, hasOnlyDigits;

    do {
        getline(cin, input);

        hasLettersOrDigits = false;
        hasOnlySymbols = true;
        hasOnlyDigits = true;

        for (char ch : input) {
            if (isalnum(ch)) hasLettersOrDigits = true;
            if (!ispunct(ch) && !isspace(ch)) hasOnlySymbols = false;
            if (!isdigit(ch)) hasOnlyDigits = false;
        }

        if (input.empty()) {
            cout << "\033[31mОшибка!\033[0m\n";
        }
        else if (hasOnlyDigits) {
            cout << "\033[31mОшибка!\033[0m\n";
        }
        else if (hasOnlySymbols) {
            cout << "\033[31mОшибка!\033[0m\n";
        }

    } while (input.empty() || (hasOnlyDigits) || hasOnlySymbols);
}

void failInt(int& value) {
    string input;
    bool valid = false;
    while (!valid) {
        getline(cin, input);
        if (!input.empty()) {
            input.erase(remove(input.begin(), input.end(), ' '), input.end());
            stringstream ss(input);
            if (ss >> value && ss.eof()) {
                valid = true;
            }
            else {
                cout << "\033[31mОшибка!\033[0m" << endl;
            }
        }
        else {
            cout << "\033[31mОшибка!\033[0m" << endl;
        }
    }
}
