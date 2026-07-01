#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────
//  Student Class
// ─────────────────────────────────────────
class Student {
public:
    int    rollNo;
    string name;
    float  marks;
    string grade;

    // Auto-calculate grade from marks
    void calculateGrade() {
        if      (marks >= 90) grade = "A+";
        else if (marks >= 80) grade = "A";
        else if (marks >= 70) grade = "B";
        else if (marks >= 60) grade = "C";
        else if (marks >= 50) grade = "D";
        else                  grade = "F";
    }

    void display() const {
        cout << left
             << setw(10) << rollNo
             << setw(25) << name
             << setw(10) << marks
             << setw(5)  << grade
             << "\n";
    }
};

// ─────────────────────────────────────────
//  File Operations
// ─────────────────────────────────────────
const string FILE_NAME = "students.dat";

void saveToFile(const vector<Student>& students) {
    ofstream fout(FILE_NAME);
    for (const auto& s : students) {
        fout << s.rollNo << "\n"
             << s.name   << "\n"
             << s.marks  << "\n"
             << s.grade  << "\n";
    }
    fout.close();
}

vector<Student> loadFromFile() {
    vector<Student> students;
    ifstream fin(FILE_NAME);
    if (!fin.is_open()) return students;

    Student s;
    while (fin >> s.rollNo) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.marks;
        fin.ignore();
        getline(fin, s.grade);
        students.push_back(s);
    }
    fin.close();
    return students;
}

// ─────────────────────────────────────────
//  Core Functions
// ─────────────────────────────────────────
void printHeader() {
    cout << "\n" << string(52, '=') << "\n";
    cout << left
         << setw(10) << "Roll No"
         << setw(25) << "Name"
         << setw(10) << "Marks"
         << setw(5)  << "Grade"
         << "\n";
    cout << string(52, '-') << "\n";
}

void addStudent(vector<Student>& students) {
    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll No   : "; cin >> s.rollNo;

    // Check duplicate roll number
    for (const auto& existing : students) {
        if (existing.rollNo == s.rollNo) {
            cout << "Error: Roll No already exists!\n";
            return;
        }
    }

    cin.ignore();
    cout << "Enter Name      : "; getline(cin, s.name);
    cout << "Enter Marks(100): "; cin >> s.marks;

    if (s.marks < 0 || s.marks > 100) {
        cout << "Error: Marks must be between 0 and 100!\n";
        return;
    }

    s.calculateGrade();
    students.push_back(s);
    saveToFile(students);
    cout << "Student added successfully! Grade assigned: " << s.grade << "\n";
}

void displayAll(const vector<Student>& students) {
    if (students.empty()) {
        cout << "\nNo student records found.\n";
        return;
    }
    cout << "\n--- All Students ---";
    printHeader();
    for (const auto& s : students) s.display();
    cout << string(52, '=') << "\n";
    cout << "Total Students: " << students.size() << "\n";
}

void searchStudent(const vector<Student>& students) {
    int roll;
    cout << "\nEnter Roll No to search: "; cin >> roll;
    for (const auto& s : students) {
        if (s.rollNo == roll) {
            cout << "\n--- Student Found ---";
            printHeader();
            s.display();
            cout << string(52, '=') << "\n";
            return;
        }
    }
    cout << "Student with Roll No " << roll << " not found.\n";
}

void updateStudent(vector<Student>& students) {
    int roll;
    cout << "\nEnter Roll No to update: "; cin >> roll;
    for (auto& s : students) {
        if (s.rollNo == roll) {
            cout << "Current Name  : " << s.name  << "\n";
            cout << "Current Marks : " << s.marks << "\n";
            cin.ignore();
            cout << "New Name (press Enter to keep): ";
            string newName; getline(cin, newName);
            if (!newName.empty()) s.name = newName;

            cout << "New Marks (-1 to keep): ";
            float newMarks; cin >> newMarks;
            if (newMarks >= 0 && newMarks <= 100) {
                s.marks = newMarks;
                s.calculateGrade();
            }

            saveToFile(students);
            cout << "Record updated successfully!\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void deleteStudent(vector<Student>& students) {
    int roll;
    cout << "\nEnter Roll No to delete: "; cin >> roll;
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->rollNo == roll) {
            cout << "Deleting: " << it->name << " (Roll No: " << roll << ")\n";
            students.erase(it);
            saveToFile(students);
            cout << "Student deleted successfully!\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void showStats(const vector<Student>& students) {
    if (students.empty()) { cout << "\nNo data to show.\n"; return; }
    float total = 0, highest = 0, lowest = 100;
    string topStudent;
    for (const auto& s : students) {
        total += s.marks;
        if (s.marks > highest) { highest = s.marks; topStudent = s.name; }
        if (s.marks < lowest)    lowest  = s.marks;
    }
    cout << "\n--- Class Statistics ---\n";
    cout << "Total Students : " << students.size()          << "\n";
    cout << "Average Marks  : " << fixed << setprecision(2)
                                << total / students.size()   << "\n";
    cout << "Highest Marks  : " << highest << " (" << topStudent << ")\n";
    cout << "Lowest Marks   : " << lowest                    << "\n";
}

// ─────────────────────────────────────────
//  Main Menu
// ─────────────────────────────────────────
int main() {
    vector<Student> students = loadFromFile();

    cout << "\n╔══════════════════════════════════════╗\n";
    cout <<   "║   STUDENT MANAGEMENT SYSTEM (C++)   ║\n";
    cout <<   "╚══════════════════════════════════════╝\n";

    int choice;
    do {
        cout << "\n┌─── MENU ───────────────────┐\n";
        cout <<   "│ 1. Add Student             │\n";
        cout <<   "│ 2. Display All Students    │\n";
        cout <<   "│ 3. Search Student          │\n";
        cout <<   "│ 4. Update Student          │\n";
        cout <<   "│ 5. Delete Student          │\n";
        cout <<   "│ 6. Class Statistics        │\n";
        cout <<   "│ 0. Exit                    │\n";
        cout <<   "└────────────────────────────┘\n";
        cout << "Enter choice: ";
        cin  >> choice;

        switch (choice) {
            case 1: addStudent(students);    break;
            case 2: displayAll(students);    break;
            case 3: searchStudent(students); break;
            case 4: updateStudent(students); break;
            case 5: deleteStudent(students); break;
            case 6: showStats(students);     break;
            case 0: cout << "\nGoodbye! Data saved.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}