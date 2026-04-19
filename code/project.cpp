#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>

using namespace std;

// current time function
string getTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

// log save function
void writeLog(string fileName, string action) {
    ofstream logFile("log.txt", ios::app);

    logFile << "File: " << fileName
            << " | Action: " << action
            << " | Time: " << getTime();

    logFile.close();
}

// alert function
void showAlert(string fileName, string action) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(h, 12);

    cout << "\n=====================================\n";
    cout << "        🚨 SECURITY ALERT 🚨\n";
    cout << "=====================================\n";
    cout << "File   : " << fileName << endl;
    cout << "Action : " << action << endl;
    cout << "=====================================\n";

    // 🔊 SAFE SOUND
    MessageBeep(MB_ICONHAND);
    MessageBeep(MB_ICONEXCLAMATION);

    SetConsoleTextAttribute(h, 7);
}

// canary files create
void createCanaryFiles() {
    ofstream f1("passwords.txt");
    f1 << "username: admin\npassword: 123456";
    f1.close();

    ofstream f2("config.txt");
    f2 << "DB_HOST=localhost\nDB_PASS=admin123";
    f2.close();

    ofstream f3("secret_keys.txt");
    f3 << "API_KEY: ABC123XYZ\nPRIVATE_KEY: SECRET";
    f3.close();

    cout << "Canary files created successfully!\n";
}

// view logs
void viewLogs() {
    string pass;
    cout << "Enter admin password: ";
    cin >> pass;

    if (pass != "admin123") {
        cout << "Access Denied!\n";
        return;
    }

    ifstream logFile("log.txt");

    if (!logFile) {
        cout << "Log file not found!\n";
        return;
    }

    cout << "\n===== LOG DATA =====\n";

    string line;
    while (getline(logFile, line)) {
        cout << line << endl;
    }

    logFile.close();
}

int main() {
    int choice;
    string fileName;

    createCanaryFiles();

    do {
        cout << "\n===== Canary Token IDS =====\n";
        cout << "1. Access File\n";
        cout << "2. Modify File\n";
        cout << "3. Delete File\n";
        cout << "4. View Logs (Admin)\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        // 🔥 INPUT FIX (IMPORTANT)
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Try again.\n";
            continue;
        }

        if (choice >= 1 && choice <= 3) {
            cout << "Enter file name: ";
            cin >> fileName;

            if (choice == 1) {
                ifstream file(fileName);
                if (file) {
                    showAlert(fileName, "Accessed");
                    writeLog(fileName, "Accessed");
                } else {
                    cout << "File not found!\n";
                }
                file.close();
            }

            else if (choice == 2) {
                ofstream file(fileName, ios::app);
                if (file) {
                    file << "\nModified data";
                    showAlert(fileName, "Modified");
                    writeLog(fileName, "Modified");
                } else {
                    cout << "File not found!\n";
                }
                file.close();
            }

            else if (choice == 3) {
                if (remove(fileName.c_str()) == 0) {
                    showAlert(fileName, "Deleted");
                    writeLog(fileName, "Deleted");
                } else {
                    cout << "File not found or cannot delete!\n";
                }
            }
        }

        else if (choice == 4) {
            viewLogs();
        }

    } while (choice != 5);

    cout << "Program Ended.\n";
    return 0;
}