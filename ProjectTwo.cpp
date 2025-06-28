// --------------------
// Roger Fisher 6/19/2025
// CS-300 Project Two
//--------------------

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// --------------------
// Class: Course
// A container for course data. Holds course number, title, and prereqs
// --------------------
class Course {
public:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() = default;

    Course(string number, string title, vector<string> prereqs) {
        courseNumber = number;
        courseTitle = title;
        prerequisites = prereqs;
    }
};

// --------------------
// Class: Node
// Wraps a course with pointers to left and right kids, because trees like branching out
// --------------------
class Node {
public:
    Course course;
    Node* left;
    Node* right;

    Node(Course newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

// --------------------
// Class: BinarySearchTree
// Our trusty Binary Search Tree to store and retrieve course quickly. (no whiteboards needed or harmed in the process)
// --------------------
class BinarySearchTree {
private:
    Node* root;

    // Recursive insert - drops the new course in its aphabetical slot like a good librarian
    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    // Recursive in order traversal: left node right, just like how we read (unless your a BST)
    void printInOrder(Node* node) {
        if (node != nullptr) {
            printInOrder(node->left);
            cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
            printInOrder(node->right);
        }
    }

    // Searching- go hunting for that course like you're registering last minute
    Course* search(Node* node, string courseNumber) {
        if (node == nullptr) return nullptr;

        if (node->course.courseNumber == courseNumber) {
            return &node->course;
        }
        else if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course) {
        insert(root, course);
    }

    void PrintCourseList() {
        printInOrder(root);
    }

    Course* FindCourse(string courseNumber) {
        return search(root, courseNumber);
    }
};

// --------------------
// LoadCoursesFromCSV
// Opens the CSV file and unpacks it line by line, kinda like a loot box of course info (No EA, we haven't forgiven you)
// --------------------
void LoadCoursesFromCSV(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;

        // Split line on commas (may need updating if someone goes rogue with commas in titles)
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 2) {
            string courseNum = tokens[0];
            string title = tokens[1];
            vector<string> prereqs;

            for (size_t i = 2; i < tokens.size(); ++i) {
                prereqs.push_back(tokens[i]);
            }

            Course newCourse(courseNum, title, prereqs);
            bst.Insert(newCourse);
            // DEBUG: show whats being inserted into the BST. Confirms everything has loaded properly. Can easily be commented to turn the feature off
            cout << "Inserted: " << courseNum << " - " << title << endl;
        }
    }

    file.close();
    cout << "Courses successfully loaded into the Binary Search Tree.\n";
}

//--------------------
// Helper: Convert a string to uppercase (because not everyone types like a course catalog)
//--------------------
string ToUpper(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

// --------------------
// Function: main()
// Do you believe in magic? Because this is where it happens (unless you pick option 9)
// --------------------
int main() {
    int choice = 0;
    BinarySearchTree bst;

    cout << "Welcome to the course planner.\n" << endl;

    while (choice != 9) {
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit\n" << endl;

        cout << "What would you like to do? ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Reset cin from fail state
            cin.ignore(1000, '\n'); // Toss out the garbage input
            cout << "\nThat was not a number. Try again, preferably with digits.\n" << endl;
            continue; // Jump to the next loop iteration
        }

        switch (choice) {
        case 1:
            cout << "\nLoading data from CSV file...\n" << endl;
            LoadCoursesFromCSV("CS 300 ABCU_Advising_Program_Input.csv", bst);
            break;
        case 2:
            cout << "\nPrinting alphanumeric course list...\n" << endl;
            bst.PrintCourseList();
            break;
        case 3: {
            cout << "\nEnter the course number (Such as CSCI400): ";
            string searchCourse;
            cin >> searchCourse;
            searchCourse = ToUpper(searchCourse);  // Make it uppercase so it actually matches

            Course* result = bst.FindCourse(searchCourse);

            if (result) {
                cout << "\nCourse Number: " << result->courseNumber << endl;
                cout << "Course Title: " << result->courseTitle << endl;
                if (!result->prerequisites.empty()) {
                    cout << "Prerequisites: ";
                    for (size_t i = 0; i < result->prerequisites.size(); ++i) {
                        cout << result->prerequisites[i];
                        if (i != result->prerequisites.size() - 1) cout << ", ";
                    }
                    cout << endl;
                }
                else {
                    cout << "Prerequisites: None" << endl;
                }
            }
            else {
                cout << "\nCourse not found.\n";
            }
            break;
        }
        case 9:
            cout << "\nThank you for using the course planner! Have a nice day! :)" << endl;
            break;
        default:
            cout << "\n" << choice << " is not a valid option. Please try another number.\n" << endl;
        }
        cout << endl;
    }
    return 0;
}