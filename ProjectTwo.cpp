//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Rick Martinez
// Version     : 1.0
// Description : CS 300 Project Two Advising Assistance Program
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Basic course setup

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Set up one spot in the tree

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Set up the binary search tree

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void clearTree(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);
    void Clear();
};

// Clean up text from the file and user input

string trim(string text) {
    while (!text.empty() && isspace(text.front())) {
        text.erase(text.begin());
    }

    while (!text.empty() && isspace(text.back())) {
        text.pop_back();
    }

    return text;
}

// Make course numbers match even if the user types lowercase

string toUpperCase(string text) {
    for (char& ch : text) {
        ch = toupper(ch);
    }

    return text;
}

// Start with an empty tree.

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Clear out the tree when the program closes.

BinarySearchTree::~BinarySearchTree() {
    Clear();
}

// Add a course to the tree.

void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Print the courses in order.

void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Find one course by moving left or right through the tree.

Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }

        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Course emptyCourse;
    return emptyCourse;
}

// Clear the current tree before loading again.

void BinarySearchTree::Clear() {
    clearTree(root);
    root = nullptr;
}

// Put the course on the left or right side.

void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// Go left, print this course, then go right.

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);

        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << endl;

        inOrder(node->right);
    }
}

// Delete nodes from the bottom up.

void BinarySearchTree::clearTree(Node* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

// Split one CSV line into separate values.

vector<string> splitLine(string line) {
    vector<string> tokens;
    string token;
    stringstream lineStream(line);

    while (getline(lineStream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Check that each prerequisite exists as a course.

bool validatePrerequisites(vector<Course> courses) {
    for (Course course : courses) {
        for (string prerequisite : course.prerequisites) {
            if (prerequisite.empty()) {
                continue;
            }

            bool found = false;

            for (Course checkCourse : courses) {
                if (checkCourse.courseNumber == prerequisite) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Error: Prerequisite " << prerequisite
                     << " does not exist as a course." << endl;
                return false;
            }
        }
    }

    return true;
}

// Load course data from the file into the tree.

bool loadCourses(string fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << "." << endl;
        return false;
    }

    vector<Course> courses;
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> tokens = splitLine(line);

        if (tokens.size() < 2) {
            cout << "Error: Each course needs a course number and course title." << endl;
            inputFile.close();
            return false;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens.at(0));
        course.courseTitle = tokens.at(1);

        for (unsigned int i = 2; i < tokens.size(); ++i) {
            string prerequisite = toUpperCase(trim(tokens.at(i)));

            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        courses.push_back(course);
    }

    inputFile.close();

    if (!validatePrerequisites(courses)) {
        return false;
    }

    courseTree.Clear();

    for (Course course : courses) {
        courseTree.Insert(course);
    }

    return true;
}

// Print one course and its prerequisites.

void printCourse(BinarySearchTree& courseTree) {
    string courseNumber;

    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(trim(courseNumber));

    Course course = courseTree.Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites.at(i);

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

// Show the user menu.

void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

// Run the menu and control the program flow.

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        printMenu();

        cout << "What would you like to do? ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "That is not a valid option." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            string fileName;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter the file name: ";
            getline(cin, fileName);

            fileName = trim(fileName);

            if (loadCourses(fileName, courseTree)) {
                dataLoaded = true;
                cout << "Data loaded successfully." << endl;
            }
            else {
                dataLoaded = false;
            }

            break;
        }

        case 2:
            if (!dataLoaded) {
                cout << "Please load the data first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                cout << endl;
                courseTree.InOrder();
            }

            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load the data first." << endl;
            }
            else {
                printCourse(courseTree);
            }

            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}