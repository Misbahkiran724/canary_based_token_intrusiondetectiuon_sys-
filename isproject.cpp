#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <conio.h>

using namespace std;

enum Color {
    BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, DARK_CYAN = 3,
    DARK_RED = 4, DARK_MAGENTA = 5, DARK_YELLOW = 6, LIGHT_GRAY = 7,
    DARK_GRAY = 8, BLUE = 9, GREEN = 10, CYAN = 11,
    RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(Color text, Color bg = BLACK) {
    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
}

void resetColor() { setColor(WHITE, BLACK); }

void setCursorPosition(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}



void printBox(int x, int y, int width, int height, Color borderColor = CYAN, Color bgColor = BLACK, bool doubleLine = false) {
    setColor(borderColor, bgColor);

    string TL = doubleLine ? "╔" : "┌";
    string TR = doubleLine ? "╗" : "┐";
    string BL = doubleLine ? "╚" : "└";
    string BR = doubleLine ? "╝" : "┘";
    string H  = doubleLine ? "═" : "─";
    string V  = doubleLine ? "║" : "│";

    // Top border
    setCursorPosition(x, y);
    cout << TL;
    for (int i = 0; i < width - 2; i++) cout << H;
    cout << TR;

    // Side borders
    for (int i = 1; i < height - 1; i++) {
        setCursorPosition(x, y + i);
        cout << V;
        setColor(WHITE, bgColor);
        for (int j = 0; j < width - 2; j++) cout << " ";
        setColor(borderColor, bgColor);
        cout << V;
    }

    // Bottom border
    setCursorPosition(x, y + height - 1);
    cout << BL;
    for (int i = 0; i < width - 2; i++) cout << H;
    cout << BR;

    resetColor();
}

void printCentered(int y, const string& text, int consoleWidth = 80, Color textColor = WHITE, Color bgColor = BLACK) {
    int x = (consoleWidth - (int)text.length()) / 2;
    if (x < 0) x = 0;
    setCursorPosition(x, y);
    setColor(textColor, bgColor);
    cout << text;
    resetColor();
}

void printBoxedText(int x, int y, int width, const string& label, const string& text, Color labelColor = YELLOW, Color textColor = GREEN) {
    setCursorPosition(x, y);
    setColor(labelColor, BLACK);
    cout << label;
    setColor(textColor, BLACK);
    cout << text;
    resetColor();
}

// UI COMPONENTS

void clearScreen() {
    system("cls");
}

void printSeparator(int y, int width = 78, Color c = DARK_CYAN) {
    setCursorPosition(1, y);
    setColor(c, BLACK);
    for (int i = 0; i < width; i++) cout << "─";
    resetColor();
}

void printBanner() {
    clearScreen();
    hideCursor();

    // Outer double-line box
    printBox(0, 0, 80, 9, CYAN, BLACK, true);

    // ASCII Art Title
    setColor(GREEN, BLACK);
    printCentered(1, "███████╗███████╗ ██████╗██╗   ██╗██████╗ ███████╗", 80, GREEN);
    printCentered(2, "██╔════╝██╔════╝██╔════╝██║   ██║██╔══██╗██╔════╝", 80, GREEN);
    printCentered(3, "███████╗█████╗  ██║     ██║   ██║██████╔╝█████╗  ", 80, GREEN);
    printCentered(4, "╚════██║██╔══╝  ██║     ██║   ██║██╔══██╗██╔══╝  ", 80, DARK_GREEN);
    printCentered(5, "███████║███████╗╚██████╗╚██████╔╝██║  ██║███████╗", 80, DARK_GREEN);
    printCentered(6, "╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝", 80, DARK_GREEN);

    setColor(YELLOW, BLACK);
    printCentered(7, "◄  A U T H E N T I C A T I O N   S Y S T E M  ►", 80, YELLOW);
    printCentered(8, "       [ Multi-Layer Security v1.0 ]", 80, DARK_YELLOW);

    // Info bar
    printBox(0, 9, 80, 3, DARK_CYAN, BLACK, false);
    time_t now = time(0);
    char* dt = ctime(&now);
    string ts(dt); ts.pop_back();
    setCursorPosition(2, 10);
    setColor(DARK_CYAN, BLACK); cout << "  ◉ ";
    setColor(WHITE, BLACK); cout << "System Online";
    setCursorPosition(30, 10);
    setColor(DARK_CYAN, BLACK); cout << "  ◈ ";
    setColor(DARK_GRAY, BLACK); cout << ts;
    resetColor();
}

void printMenuBox(const string& title, const vector<string>& options, int startY = 12) {
    int boxW = 40;
    int boxX = 20;
    int boxH = (int)options.size() + 4;

    printBox(boxX, startY, boxW, boxH, CYAN, BLACK, true);

    // Title bar
    setCursorPosition(boxX + 1, startY);
    setColor(CYAN, BLACK); cout << "╡ ";
    setColor(YELLOW, BLACK); cout << title;
    setColor(CYAN, BLACK); cout << " ╞";
    resetColor();

    for (int i = 0; i < (int)options.size(); i++) {
        setCursorPosition(boxX + 3, startY + 2 + i);
        setColor(GREEN, BLACK); cout << "[" << (i + 1) << "] ";
        setColor(WHITE, BLACK); cout << options[i];
    }

    setCursorPosition(boxX + 3, startY + boxH - 1);
    setColor(YELLOW, BLACK); cout << "  Option: ";
    setColor(GREEN, BLACK);
    showCursor();
    resetColor();
}

void printStatusMsg(const string& msg, const string& type = "INFO", int y = 23) {
    // Clear the line first
    setCursorPosition(0, y);
    setColor(BLACK, BLACK);
    for (int i = 0; i < 80; i++) cout << " ";

    setCursorPosition(2, y);
    if (type == "SUCCESS") {
        printBox(1, y - 1, 78, 3, GREEN, BLACK, false);
        setCursorPosition(3, y);
        setColor(GREEN, BLACK); cout << " ✔  ";
        setColor(WHITE, BLACK); cout << msg;
    } else if (type == "ERROR") {
        printBox(1, y - 1, 78, 3, RED, BLACK, false);
        setCursorPosition(3, y);
        setColor(RED, BLACK); cout << " ✖  ";
        setColor(WHITE, BLACK); cout << msg;
    } else if (type == "WARN") {
        printBox(1, y - 1, 78, 3, YELLOW, BLACK, false);
        setCursorPosition(3, y);
        setColor(YELLOW, BLACK); cout << " ⚠  ";
        setColor(WHITE, BLACK); cout << msg;
    } else if (type == "ALERT") {
        printBox(1, y - 1, 78, 3, RED, BLACK, false);
        setCursorPosition(3, y);
        setColor(RED, BLACK); cout << " !!! SECURITY ALERT: ";
        setColor(YELLOW, BLACK); cout << msg;
    } else {
        setCursorPosition(3, y);
        setColor(CYAN, BLACK); cout << " ◈  ";
        setColor(LIGHT_GRAY, BLACK); cout << msg;
    }
    resetColor();
}

void printInputPrompt(int x, int y, const string& label) {
    setCursorPosition(x, y);
    setColor(YELLOW, BLACK); cout << label;
    setColor(GREEN, BLACK);
    showCursor();
    resetColor();
}

void printDivider(const string& title = "", int y = 11) {
    setCursorPosition(1, y);
    setColor(DARK_CYAN, BLACK);
    cout << "╠";
    for (int i = 0; i < 77; i++) cout << "═";
    cout << "╣";
    if (!title.empty()) {
        int x = (80 - (int)title.length() - 4) / 2;
        setCursorPosition(x, y);
        setColor(CYAN, BLACK); cout << "╡ ";
        setColor(YELLOW, BLACK); cout << title;
        setColor(CYAN, BLACK); cout << " ╞";
    }
    resetColor();
}


// DATA STRUCTURES

struct User {
    string username;
    string passwordHash;
    string salt;
    string role;
    int attempts = 0;
    bool isLocked = false;
    time_t lockTime = 0;
};

vector<User> userDatabase;
const string FILENAME = "users.txt";
const string LOG_FILENAME = "security_audit.txt";
const int UNLOCK_TIME_SECONDS = 30;

struct AdminLock {
    int attempts = 0;
    time_t lockTime = 0;
    bool isLocked = false;
};
AdminLock adminLock;
const int ADMIN_MAX_ATTEMPTS = 3;
const int ADMIN_UNLOCK_SECONDS = 60;

// ============================================================================
// SECURE PASSWORD INPUT (SHOWS STARS)
// ============================================================================
string getPasswordInput(const string& prompt, int inputX = 3, int inputY = 18) {
    string password = "";
    char ch;
    setCursorPosition(inputX, inputY);
    setColor(YELLOW, BLACK); cout << prompt;
    setColor(GREEN, BLACK);
    showCursor();

    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !password.empty()) {
            password.pop_back();
            cout << "\b \b";
        } else if (isprint(ch) && ch != '\t') {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    resetColor();
    return password;
}

// SECURITY & LOGGING

void logSecurityEvent(string user, string action, string status, string details) {
    ofstream logFile(LOG_FILENAME, ios::app);
    time_t now = time(0);
    char* dt = ctime(&now);
    string ts(dt); ts.pop_back();
    logFile << "[" << ts << "] | USER: " << setw(10) << left << user
            << " | ACTION: " << setw(15) << left << action
            << " | STATUS: " << setw(8) << left << status
            << " | DETAILS: " << details << endl;
    logFile.close();
}

bool isSQLInjection(string input) {
    vector<string> blacklist = {"' OR '", "1=1", "DROP ", "UNION ", "--", ";","SELECT", "INSERT", "DELETE", "UPDATE"};
    string upperInput = input;
    transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
    for (const string &pattern : blacklist) {
        if (upperInput.find(pattern) != string::npos) return true;
    }
    return false;
}


string generateSalt() {
    string salt = "";
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 16; i++) salt += chars[rand() % chars.length()];
    return salt;
}

string hashPassword(const string &password, const string &salt) {
    string combined = password + salt;
    unsigned long long hash = 5381;
    for (char c : combined) hash = ((hash << 5) + hash) + c;
    stringstream ss; ss << hex << hash; return ss.str();
}

void saveToFile() {
    ofstream outFile(FILENAME);
    for (const auto &u : userDatabase)
        outFile << u.username << " " << u.passwordHash << " " << u.salt << " "
                << u.role << " " << u.isLocked << " " << u.attempts << " " << u.lockTime << endl;
    outFile.close();
}

void loadFromFile() {
    ifstream inFile(FILENAME); if (!inFile) return;
    userDatabase.clear(); User t;
    while (inFile >> t.username >> t.passwordHash >> t.salt >> t.role >> t.isLocked >> t.attempts >> t.lockTime)
        userDatabase.push_back(t);
    inFile.close();
}


// ADMIN BRUTE FORCE PROTECTION

bool checkAdminUnlock() {
    if (!adminLock.isLocked) return true;
    time_t now = time(0);
    double passed = difftime(now, adminLock.lockTime);
    if (passed >= ADMIN_UNLOCK_SECONDS) {
        adminLock.isLocked = false;
        adminLock.attempts = 0;
        return true;
    }
    return false;
}


// CHANGE PASSWORD SCREEN

void changePassword(User &user) {
    printBanner();
    printDivider("CHANGE PASSWORD");

    printBox(1, 12, 78, 10, CYAN, BLACK, false);
    setCursorPosition(3, 13);
    setColor(CYAN, BLACK); cout << "╡ ";
    setColor(YELLOW, BLACK); cout << "Update Credentials";
    setColor(CYAN, BLACK); cout << " ╞";

    setCursorPosition(3, 14);
    setColor(DARK_CYAN, BLACK); cout << "User: ";
    setColor(GREEN, BLACK); cout << user.username;

    string newPass = getPasswordInput("  New Password    : ", 3, 16);
    string confirm  = getPasswordInput("  Confirm Password: ", 3, 17);

    if (newPass != confirm) {
        printStatusMsg("Passwords do not match!", "ERROR", 23);
        Sleep(1500); return;
    }
    if (isSQLInjection(newPass)) {
        printStatusMsg("SQL Injection pattern detected — blocked!", "ALERT", 23);
        logSecurityEvent(user.username, "PWD_CHANGE", "BLOCKED", "SQLi detected");
        Sleep(2000); return;
    }

    user.salt = generateSalt();
    user.passwordHash = hashPassword(newPass, user.salt);
    saveToFile();
    logSecurityEvent(user.username, "PWD_CHANGE", "SUCCESS", "Password updated");
    printStatusMsg("Password changed successfully!", "SUCCESS", 23);
    Sleep(1500);
}


// USER DASHBOARD
void userDashboard(User &user) {
    while (true) {
        printBanner();
        printDivider("USER DASHBOARD");

        // User info panel
        printBox(1, 12, 40, 5, CYAN, BLACK, false);
        setCursorPosition(3, 13);
        setColor(CYAN, BLACK); cout << "╡ ";
        setColor(YELLOW, BLACK); cout << "Session Info";
        setColor(CYAN, BLACK); cout << " ╞";
        setCursorPosition(3, 14);
        setColor(DARK_CYAN, BLACK); cout << "  Username : "; setColor(GREEN, BLACK); cout << user.username;
        setCursorPosition(3, 15);
        setColor(DARK_CYAN, BLACK); cout << "  Role     : "; setColor(YELLOW, BLACK); cout << user.role;

        vector<string> options = {"Change Password", "Logout"};
        printMenuBox("USER MENU", options, 18);

        int choice;
        setCursorPosition(30, 22);
        cin >> choice;

        if (choice == 1) changePassword(user);
        else if (choice == 2) {
            printStatusMsg("Logged out successfully.", "SUCCESS", 25);
            Sleep(1000); break;
        } else {
            printStatusMsg("Invalid option. Try again.", "ERROR", 25);
            Sleep(1000);
        }
    }
}


// ADMIN DASHBOARD

void adminDashboard() {
    while (true) {
        printBanner();
        printDivider("ADMIN DASHBOARD");

        // Admin badge
        printBox(1, 12, 40, 4, YELLOW, BLACK, false);
        setCursorPosition(3, 13);
        setColor(YELLOW, BLACK); cout << "  ★ ADMINISTRATOR ACCESS GRANTED ★";
        setCursorPosition(3, 14);
        setColor(DARK_YELLOW, BLACK); cout << "  Full system privileges active.";

        vector<string> options = {"Change Admin Password", "View Audit Logs", "Logout"};
        printMenuBox("ADMIN MENU", options, 17);

        int choice;
        setCursorPosition(30, 21);
        cin >> choice;

        if (choice == 1) {
            printBanner();
            printDivider("CHANGE ADMIN PASSWORD");
            printBox(1, 12, 78, 5, YELLOW, BLACK, false);
            string adminUser;
            setCursorPosition(3, 14);
            setColor(YELLOW, BLACK); cout << "  Admin Username: ";
            setColor(GREEN, BLACK);
            showCursor();
            cin >> adminUser;

            User* adminUserPtr = nullptr;
            for (auto& u : userDatabase) {
                if (u.username == adminUser && u.role == "ADMIN") {
                    adminUserPtr = &u; break;
                }
            }
            if (adminUserPtr) {
                changePassword(*adminUserPtr);
            } else {
                printStatusMsg("Admin user not found.", "ERROR", 20);
                Sleep(1500);
            }

        } else if (choice == 2) {
            if (!checkAdminUnlock()) {
                double remaining = ADMIN_UNLOCK_SECONDS - difftime(time(0), adminLock.lockTime);
                printStatusMsg("Admin locked. Wait " + to_string((int)remaining) + "s.", "ALERT", 23);
                Sleep(2000); continue;
            }

            string pass = getPasswordInput("  Admin Master Key: ", 3, 16);
            if (pass != "admin123") {
                adminLock.attempts++;
                logSecurityEvent("ADMIN", "LOGS_ACCESS", "FAILED", "Attempt " + to_string(adminLock.attempts));
                if (adminLock.attempts >= ADMIN_MAX_ATTEMPTS) {
                    adminLock.isLocked = true;
                    adminLock.lockTime = time(0);
                    printStatusMsg("Admin Access LOCKED for 60s!", "ALERT", 23);
                } else {
                    printStatusMsg("Access Denied. Attempt " + to_string(adminLock.attempts) + " of 3.", "ERROR", 23);
                }
                Sleep(2000); continue;
            }

            adminLock.attempts = 0;
            logSecurityEvent("ADMIN", "LOGS_ACCESS", "SUCCESS", "Audit logs viewed");

            // Display logs in a styled box
            clearScreen();
            printBox(0, 0, 80, 3, YELLOW, BLACK, true);
            printCentered(1, "  ★  SYSTEM SECURITY AUDIT LOG  ★", 80, YELLOW);

            ifstream f(LOG_FILENAME); string l; int lineY = 4;
            printBox(0, 3, 80, 20, DARK_CYAN, BLACK, false);
            while (getline(f, l) && lineY < 22) {
                setCursorPosition(2, lineY++);
                setColor(DARK_CYAN, BLACK); cout << "▸ ";
                setColor(LIGHT_GRAY, BLACK); cout << l.substr(0, 76);
            }
            f.close();
            printBox(0, 23, 80, 3, DARK_GRAY, BLACK, false);
            setCursorPosition(3, 24);
            setColor(DARK_GRAY, BLACK); cout << "  Press any key to return...";
            resetColor();
            _getch();

        } else if (choice == 3) {
            printStatusMsg("Admin session terminated.", "SUCCESS", 23);
            Sleep(1000); break;
        } else {
            printStatusMsg("Invalid option. Try again.", "ERROR", 23);
            Sleep(1000);
        }
    }
}


// REGISTER SCREEN

void registerUser() {
    printBanner();
    printDivider("NEW USER REGISTRATION");
    printBox(1, 12, 78, 10, CYAN, BLACK, false);
    setCursorPosition(3, 13);
    setColor(CYAN, BLACK); cout << "╡ ";
    setColor(YELLOW, BLACK); cout << "Create Account";
    setColor(CYAN, BLACK); cout << " ╞";

    string u, p;
    setCursorPosition(3, 15);
    setColor(YELLOW, BLACK); cout << "  Username : ";
    setColor(GREEN, BLACK);
    showCursor();
    cin >> u;

    if (isSQLInjection(u)) {
        printStatusMsg("SQL Injection detected — registration blocked!", "ALERT", 23);
        Sleep(2000); return;
    }
    for (const auto &user : userDatabase) {
        if (user.username == u) {
            printStatusMsg("Username already exists. Try another.", "ERROR", 23);
            Sleep(2000); return;
        }
    }

    p = getPasswordInput("  Password : ", 3, 17);

    string role = (userDatabase.empty()) ? "ADMIN" : "USER";
    string salt = generateSalt();
    string hashed = hashPassword(p, salt);
    userDatabase.push_back({u, hashed, salt, role, 0, false, 0});
    saveToFile();
    logSecurityEvent(u, "REGISTRATION", "SUCCESS", "Role: " + role);

    printStatusMsg("Account created! Role assigned: " + role, "SUCCESS", 23);
    Sleep(2000);
}


// LOGIN AS USER

void loginAsUser() {
    printBanner();
    printDivider("USER LOGIN");
    printBox(1, 12, 78, 10, CYAN, BLACK, false);
    setCursorPosition(3, 13);
    setColor(CYAN, BLACK); cout << "╡ ";
    setColor(YELLOW, BLACK); cout << "Authenticate";
    setColor(CYAN, BLACK); cout << " ╞";

    string u;
    setCursorPosition(3, 15);
    setColor(YELLOW, BLACK); cout << "  Username : ";
    setColor(GREEN, BLACK);
    showCursor();
    cin >> u;

    for (auto &user : userDatabase) {
        if (user.username == u) {
            if (user.isLocked) {
                time_t now = time(0);
                double passed = difftime(now, user.lockTime);
                if (passed < UNLOCK_TIME_SECONDS) {
                    while (passed < UNLOCK_TIME_SECONDS) {
                        setCursorPosition(3, 19);
                        setColor(YELLOW, BLACK);
                        cout << "  ⚠  Account LOCKED. Auto-unlock in: ";
                        setColor(RED, BLACK);
                        cout << (int)(UNLOCK_TIME_SECONDS - passed) << "s...   " << flush;
                        Sleep(1000);
                        now = time(0); passed = difftime(now, user.lockTime);
                    }
                    user.isLocked = false; user.attempts = 0; saveToFile();
                    setCursorPosition(3, 19);
                    setColor(GREEN, BLACK);
                    cout << "  ✔  Account unlocked! Re-enter credentials.    " << endl;
                    Sleep(1000);
                } else {
                    user.isLocked = false; user.attempts = 0;
                }
            }

            string p = getPasswordInput("  Password : ", 3, 17);
            if (hashPassword(p, user.salt) == user.passwordHash) {
                user.attempts = 0; saveToFile();
                logSecurityEvent(u, "LOGIN", "SUCCESS", "User session active");
                printStatusMsg("Login successful! Welcome, " + u, "SUCCESS", 23);
                Sleep(1200);
                userDashboard(user);
                return;
            } else {
                user.attempts++;
                logSecurityEvent(u, "LOGIN", "FAILED", "Attempt " + to_string(user.attempts));
                if (user.attempts >= 3) {
                    user.isLocked = true; user.lockTime = time(0);
                    printStatusMsg("Account LOCKED after 3 failed attempts!", "ALERT", 23);
                } else {
                    printStatusMsg("Incorrect password. Attempt " + to_string(user.attempts) + " of 3.", "ERROR", 23);
                }
                saveToFile();
                Sleep(2000); return;
            }
        }
    }
    printStatusMsg("Username not found in system.", "ERROR", 23);
    Sleep(2000);
}


// LOGIN AS ADMIN

void loginAsAdmin() {
    if (!checkAdminUnlock()) {
        double remaining = ADMIN_UNLOCK_SECONDS - difftime(time(0), adminLock.lockTime);
        printBanner();
        printDivider("ADMIN LOGIN");
        printStatusMsg("Admin locked. Wait " + to_string((int)remaining) + "s.", "ALERT", 15);
        Sleep(2500); return;
    }

    printBanner();
    printDivider("ADMIN LOGIN");
    printBox(1, 12, 78, 8, YELLOW, BLACK, false);
    setCursorPosition(3, 13);
    setColor(YELLOW, BLACK); cout << "╡ ";
    setColor(WHITE, BLACK); cout << "Administrator Authentication";
    setColor(YELLOW, BLACK); cout << " ╞";

    setCursorPosition(3, 15);
    setColor(DARK_YELLOW, BLACK); cout << "  ★ Restricted Access — Authorized Personnel Only ★";

    string pass = getPasswordInput("  Master Key : ", 3, 17);
    if (pass != "admin123") {
        adminLock.attempts++;
        logSecurityEvent("ADMIN_MASTER", "LOGIN", "FAILED", "Attempt " + to_string(adminLock.attempts));
        if (adminLock.attempts >= ADMIN_MAX_ATTEMPTS) {
            adminLock.isLocked = true;
            adminLock.lockTime = time(0);
            printStatusMsg("Admin Access LOCKED for 60s!", "ALERT", 22);
        } else {
            printStatusMsg("Invalid key. Attempt " + to_string(adminLock.attempts) + " of 3.", "ERROR", 22);
        }
        Sleep(2000); return;
    }

    adminLock.attempts = 0;
    logSecurityEvent("ADMIN_MASTER", "LOGIN", "SUCCESS", "Admin session active");
    printStatusMsg("Admin authenticated successfully!", "SUCCESS", 22);
    Sleep(1200);
    adminDashboard();
}


// MAIN MENU

int main() {
    srand(time(0));
    loadFromFile();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitle("Secure Auth System v1.0");
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Resize console window for better display
    system("mode con cols=80 lines=30");
    system("color 07");

    int choice;
    while (true) {
        printBanner();
        printDivider("MAIN MENU");

        vector<string> options = {"Register New Account", "Login", "Exit System"};
        printMenuBox("MAIN MENU", options, 13);

        int menuY = 13 + (int)options.size() + 3;
        setCursorPosition(30, menuY);
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n'); continue;
        }

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            printBanner();
            printDivider("SELECT LOGIN TYPE");

            vector<string> loginOpts = {"Login as User", "Login as Administrator"};
            printMenuBox("LOGIN TYPE", loginOpts, 13);

            int loginType;
            setCursorPosition(30, 13 + (int)loginOpts.size() + 3);
            cin >> loginType;

            if (loginType == 1) loginAsUser();
            else if (loginType == 2) loginAsAdmin();
            else {
                printStatusMsg("Invalid option selected.", "ERROR", 22);
                Sleep(1000);
            }
        } else if (choice == 3) {
            printBanner();
            printCentered(15, "Thank you for using Secure Auth System.", 80, CYAN);
            printCentered(16, "Session terminated. Goodbye.", 80, DARK_CYAN);
            Sleep(1500);
            break;
        } else {
            printStatusMsg("Invalid option. Please enter 1, 2, or 3.", "ERROR", 22);
            Sleep(1000);
        }
    }

    showCursor();
    resetColor();
    return 0;
}