// Author: Miguel Altoro IV
// CS-300 Project Two (Enhanced for CS 499 Milestone Three)
// Enhancement focus: Algorithms & Data Structures
// - Added unordered_map for O(1) average-time lookups by course number
// - Kept vector to preserve a simple list for printing (sorted alphabetically)
// - Added small robustness: trim tokens, normalize course codes to uppercase

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <unordered_map>
#include <cctype>

// ---------------------- Utilities ----------------------

static inline std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

static inline std::string toUpper(const std::string& s) {
    std::string out = s;
    for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

// ---------------------- Data Model ----------------------

struct Course {
    std::string courseNumber;                 // "CS300"
    std::string title;                        // "Data Structures"
    std::vector<std::string> prerequisites;   // {"CS200", "MATH200"}

    void simpleDisplay() const {
        std::cout << courseNumber << ", " << title << '\n';
    }
};

// ---------------------- Menus/IO ----------------------

void displayMenu() {
    std::cout << "\nMenu:\n"
        << "1. Load Data Structure\n"
        << "2. Print Course List\n"
        << "3. Print Course\n"
        << "9. Exit\n"
        << "Enter choice: ";
}

// ---------------------- Core Logic ----------------------

// Loads courses from CSV-like file:
// Format per line: courseNumber,title,prereq1,prereq2,...
// Populates both `courses` (vector) and `courseTable` (hash map).
bool loadCourses(std::vector<Course>& courses,
    std::unordered_map<std::string, Course>& courseTable,
    const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << '\n';
        return false;
    }

    courses.clear();
    courseTable.clear();

    std::string line;
    size_t lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string part;
        std::vector<std::string> parts;
        while (std::getline(iss, part, ',')) {
            parts.push_back(trim(part));
        }

        if (parts.size() < 2) {
            std::cerr << "Warning: Skipping malformed line " << lineNum
                << " (needs at least courseNumber,title)\n";
            continue;
        }

        // Normalize course number and prereqs to uppercase for consistent lookups
        std::string courseNum = toUpper(parts[0]);
        std::string title = parts[1];

        std::vector<std::string> prereqs;
        for (size_t i = 2; i < parts.size(); ++i) {
            if (!parts[i].empty()) prereqs.push_back(toUpper(parts[i]));
        }

        courses.emplace_back(Course{ courseNum, title, prereqs });
        courseTable[courseNum] = courses.back(); // copy into hash map
    }

    std::cout << "Courses loaded successfully. Total: " << courses.size() << " courses.\n";
    return true;
}

// Prints courses sorted alphabetically by course number (CS200, CS300, etc)
void printCourses(const std::vector<Course>& courses) {
    if (courses.empty()) {
        std::cout << "No courses loaded. Please load data first.\n";
        return;
    }
    std::vector<Course> sorted = courses;
    std::sort(sorted.begin(), sorted.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    std::cout << "\nCourse List (alphabetical by course number):\n";
    for (const auto& c : sorted) {
        c.simpleDisplay();
    }
}

// Uses the hash table for O(1) average lookup.
void findCourse(const std::unordered_map<std::string, Course>& courseTable,
    const std::string& rawCourseNumber) {
    if (courseTable.empty()) {
        std::cout << "No courses available to search. Please load data first.\n";
        return;
    }

    std::string courseNumber = toUpper(trim(rawCourseNumber));
    auto it = courseTable.find(courseNumber);
    if (it == courseTable.end()) {
        std::cout << "Course not found.\n";
        return;
    }

    const Course& c = it->second;
    c.simpleDisplay();
    if (!c.prerequisites.empty()) {
        std::cout << "Prerequisites: ";
        for (size_t i = 0; i < c.prerequisites.size(); ++i) {
            std::cout << c.prerequisites[i];
            if (i + 1 < c.prerequisites.size()) std::cout << ' ';
        }
        std::cout << '\n';
    }
    else {
        std::cout << "No prerequisites.\n";
    }
}

// ---------------------- Main ----------------------

int main() {
    std::vector<Course> courses;
    std::unordered_map<std::string, Course> courseTable;

    std::string filename;
    int choice;

    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input; please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: { // Load data structure
            std::cout << "Enter the filename: ";
            std::cin >> filename; // assumes no spaces
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!loadCourses(courses, courseTable, filename)) {
                std::cout << "Failed to load courses. Please check the file name and try again.\n";
            }
            break;
        }
        case 2: { // Print course list
            printCourses(courses);
            break;
        }
        case 3: { // Print specific course information
            std::cout << "Enter the course number to search: ";
            std::string query;
            if (!(std::cin >> query)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid course number. Please try again.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            findCourse(courseTable, query);
            break;
        }
        case 9: {
            std::cout << "Thank you for using the course planner!\n";
            return 0;
        }
        default:
            std::cout << "Invalid option, please try again.\n";
        }
    }
    return 0;
}
