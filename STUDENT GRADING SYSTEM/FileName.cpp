using namespace std;
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


const int MAX_SCORES = 10;
const int MAX_STUDENTS = 100;
const string PASSWORD = "333";

struct Student {
    int id;
    string name;
    string subjects[MAX_SCORES];
    int scores[MAX_SCORES];
    int numScores;
    float average;
    char grade;
};

void addStudent(Student students[], int& numStudents);
void updateStudent(Student students[], int numStudents);
void calculateGrades(Student& student);
void displayStudents(const Student students[], int numStudents);
void saveToFile(const Student students[], int numStudents);
void loadFromFile(Student students[], int& numStudents);
void login();

int main()
{
    cout << "*******************************************************************************************************" << endl;
    cout << "****************************Student GRADING SYSTEM ****************************************************" << endl;
    cout << "*******************************************************************************************************" << endl;

    login();

    Student students[MAX_STUDENTS];
    int numStudents = 0;
    int choice;

    loadFromFile(students, numStudents);

    do
    {
        cout << "Student Grade Management System" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Update Student" << endl;
        cout << "3. Display Students" << endl;
        cout << "4. Save to File" << endl;
        cout << "5. Load from File" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addStudent(students, numStudents);
            break;
        case 2:
            updateStudent(students, numStudents);
            break;
        case 3:
            displayStudents(students, numStudents);
            break;
        case 4:
            saveToFile(students, numStudents);
            break;
        case 5:
            loadFromFile(students, numStudents);
            break;
        case 6:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}

void addStudent(Student students[], int& numStudents)
{
    if (numStudents >= MAX_STUDENTS) {
        cout << "Cannot add more students, storage full." << endl;
        return;
    }

    Student& newStudent = students[numStudents];
    cout << "Enter student ID: ";
    cin >> newStudent.id;
    cin.ignore();
    cout << "Enter student name: ";
    getline(cin, newStudent.name);

    cout << "Enter number of scores (max " << MAX_SCORES << "): ";
    cin >> newStudent.numScores;
    if (newStudent.numScores > MAX_SCORES)
    {
        cout << "Exceeded maximum number of scores. Setting to " << MAX_SCORES << endl;
        newStudent.numScores = MAX_SCORES;
    }

    for (int i = 0; i < newStudent.numScores; ++i)
    {
        cout << "Enter subject " << i + 1 << ": ";
        cin.ignore();
        getline(cin, newStudent.subjects[i]);
        cout << "Enter score for " << newStudent.subjects[i] << ": ";
        cin >> newStudent.scores[i];
    }

    calculateGrades(newStudent);
    numStudents++;
}

void updateStudent(Student students[], int numStudents)
{
    int id;
    cout << "Enter the ID of the student to update: ";
    cin >> id;

    for (int i = 0; i < numStudents; ++i)
    {
        if (students[i].id == id) {
            Student& student = students[i];
            cout << "Updating student with ID: " << id << endl;
            cout << "Enter new name (leave blank to keep current name): ";
            cin.ignore();
            string newName;
            getline(cin, newName);
            if (!newName.empty()) {
                student.name = newName;
            }

            cout << "Enter number of new scores (max " << MAX_SCORES << "): ";
            cin >> student.numScores;
            if (student.numScores > MAX_SCORES)
            {
                cout << "Exceeded maximum number of scores. Setting to " << MAX_SCORES << endl;
                student.numScores = MAX_SCORES;
            }

            for (int j = 0; j < student.numScores; ++j) {
                cout << "Enter new subject " << j + 1 << ": ";
                cin.ignore();
                getline(cin, student.subjects[j]);
                cout << "Enter new score for " << student.subjects[j] << ": ";
                cin >> student.scores[j];
            }

            calculateGrades(student);
            cout << "Student updated successfully." << endl;
            return;
        }
    }

    cout << "Student with ID " << id << " not found." << endl;
}

void calculateGrades(Student& student)
{
    int sum = 0;
    for (int i = 0; i < student.numScores; ++i)
    {
        sum += student.scores[i];
    }
    student.average = static_cast<float>(sum) / student.numScores;
    if (student.average >= 90) student.grade = 'A';
    else if (student.average >= 80) student.grade = 'B';
    else if (student.average >= 70) student.grade = 'C';
    else if (student.average >= 60) student.grade = 'D';
    else student.grade = 'F';
}

void displayStudents(const Student students[], int numStudents)
{
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Average" << "Grade" << endl;
    for (int i = 0; i < numStudents; ++i)
    {
        const Student& student = students[i];
        cout << left << setw(10) << student.id << setw(20) << student.name
            << setw(10) << fixed << setprecision(2) << student.average << student.grade << endl;
        for (int j = 0; j < student.numScores; ++j)
        {
            cout << "  " << setw(20) << student.subjects[j] << student.scores[j] << endl;
        }
    }
}

void saveToFile(const Student students[], int numStudents)
{
    ofstream outFile("students.dat");
    if (outFile.is_open())
    {
        outFile << numStudents << '\n';
        for (int i = 0; i < numStudents; ++i)
        {
            const Student& student = students[i];
            outFile << student.id << '\n' << student.name << '\n' << student.numScores << '\n';
            for (int j = 0; j < student.numScores; ++j) {
                outFile << student.subjects[j] << '\n' << student.scores[j] << '\n';
            }
            outFile << student.average << ' ' << student.grade << '\n';
        }
        outFile.close();
        cout << "Data saved successfully." << endl;
    }
    else {
        cout << "Error opening file for writing." << endl;
    }
}

void loadFromFile(Student students[], int& numStudents)
{
    ifstream inFile("students.dat");
    if (inFile.is_open())
    {
        inFile >> numStudents;
        inFile.ignore();
        for (int i = 0; i < numStudents; ++i)
        {
            Student& student = students[i];
            inFile >> student.id;
            inFile.ignore();
            getline(inFile, student.name);
            inFile >> student.numScores;
            inFile.ignore();
            for (int j = 0; j < student.numScores; ++j)
            {
                getline(inFile, student.subjects[j]);
                inFile >> student.scores[j];
                inFile.ignore();
            }
            inFile >> student.average >> student.grade;
            inFile.ignore();
        }
        inFile.close();
        cout << "Data loaded successfully." << endl;
    }
    else
    {
        cout << "Error opening file for reading." << endl;
    }
}

void login()
{
    string username, password;
    while (true)
    {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        if (password == PASSWORD)
        {
            cout << "Login successful" << endl;
            break;
        }
        else {
            cout << "Invalid credentials. Please try again." << endl;
        }
    }
}