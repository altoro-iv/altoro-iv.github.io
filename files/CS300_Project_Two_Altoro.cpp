// Author: Miguel Altoro IV

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>

// Define a structure to hold course information
struct Course {
    std::string courseNumber; // Unique identifier for a course
    std::string title;        // Title of the course
    std::vector<std::string> prerequisites; // List of prerequisites for the course

    // Function to display course details simply
    void simpleDisplay() const {
        std::cout << courseNumber << ", " << title << std::endl;
    }
};

// Function to display the user interface menu
void displayMenu() {
    std::cout << "\nMenu:\n"
        << "1. Load Data Structure\n"
        << "2. Print Course List\n"
        << "3. Print Course\n"
        << "9. Exit\n"
        << "Enter choice: ";
}

// Function to load courses from a specified file into a vector
bool loadCourses(std::vector<Course>& courses, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return false;
    }
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string part;
        std::vector<std::string> parts;
        while (getline(iss, part, ',')) {
            parts.push_back(part);
        }
        if (parts.size() >= 2) {
            courses.emplace_back(Course{ parts[0], parts[1], std::vector<std::string>(parts.begin() + 2, parts.end()) });
        }
    }
    std::cout << "Courses loaded successfully. Total: " << courses.size() << " courses." << std::endl;
    return true;
}

// Function to print all courses in a simple list format
void printCourses(const std::vector<Course>& courses) {
    std::cout << "\nHere is a sample schedule:\n";
    for (const auto& course : courses) {
        course.simpleDisplay();
    }
}

// Function to find and display details of a specific course by course number
void findCourse(const std::vector<Course>& courses, const std::string& courseNumber) {
    auto it = std::find_if(courses.begin(), courses.end(), [&courseNumber](const Course& course) {
        return course.courseNumber == courseNumber;
        });
    if (it != courses.end()) {
        it->simpleDisplay();
        if (!it->prerequisites.empty()) {
            std::cout << "Prerequisites: ";
            for (const auto& prereq : it->prerequisites) {
                std::cout << prereq << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "No prerequisites.\n";
        }
    }
    else {
        std::cout << "Course not found." << std::endl;
    }
}

// Main function driving the program
int main() {
    std::vector<Course> courses; // Vector to hold all courses
    std::string filename; // Variable to store the filename input by user
    int choice; // Variable to store user's menu choice

    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore incorrect input
            std::cout << "Invalid input; please enter a number.\n";
            continue;
        }
        switch (choice) {
        case 1: // Load data structure
            std::cout << "Enter the filename: ";
            std::cin >> filename; // Assuming the filename has no spaces
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            if (!loadCourses(courses, filename)) {
                std::cout << "Failed to load courses. Please check the file name and try again.\n";
            }
            break;
        case 2: // Print course list
            if (courses.empty()) {
                std::cout << "No courses loaded. Please load data first.\n";
            }
            else {
                printCourses(courses);
            }
            break;
        case 3: // Print specific course information
            if (courses.empty()) {
                std::cout << "No courses available to search. Please load data first.\n";
            }
            else {
                std::cout << "Enter the course number to search: ";
                if (!(std::cin >> filename)) {  // Reading the course number as string
                    std::cin.clear(); // Clear error flags
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore incorrect input
                    std::cout << "Invalid course number. Please try again.\n";
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer after input
                findCourse(courses, filename);
            }
            break;
        case 9: // Exit the program
            std::cout << "Thank you for using the course planner!\n";
            return 0;
        default:
            std::cout << "Invalid option, please try again.\n";
        }
    }
    return 0;
}