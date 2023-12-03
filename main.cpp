#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <ctime>
#include <cstdlib>

// Function to check if the date is valid
bool isValidDate(const std::string& date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
        return false;
    }
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2099) {
        return false;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31) {
        return false;
    }
    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !isLeapYear)) {
            return false;
        }
    }
    return true;
}

// Function to generate a random letter (excluding 'I' and 'Q')
char generateRandomLetter() {
    char letter;
    do {
        letter = 'A' + rand() % 26;
    } while (letter == 'I' || letter == 'Q');
    return letter;
}

// Function to get the age identifier from the date
std::string getAgeIdentifier(const std::string& date) {
    if (!isValidDate(date)) {
        throw std::invalid_argument("Invalid date format");
    }
    int year = std::stoi(date.substr(6, 4));
    int month = std::stoi(date.substr(3, 2));
 // Adjust the year for dates in January and February
    if (month < 3) {
        year--;
    }
    int identifier;
    if (month >= 3 && month <= 8) {
        identifier = year % 100;
    } else {
        identifier = (year % 100) + 50;
    }
 // Prepend a zero if the identifier is less than 10 for consistent formatting
    if (identifier < 10) {
        return "0" + std::to_string(identifier);
    } else {
        return std::to_string(identifier);
    }
}

// Function to load previously generated plates from a file
void loadPlatesFromFile(std::unordered_set<std::string>& platesGenerated, const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        platesGenerated.insert(line);
    }
    file.close();
}

// Function to save a new plate to a file
void savePlateToFile(const std::string& numberPlate, const std::string& fileName) {
    std::ofstream file(fileName, std::ios::app);
    file << numberPlate << std::endl;
    file.close();
}

// Function to generate a unique number plate
std::string generateNumberPlate(const std::string& memoryTag, const std::string& date, 
                                std::unordered_set<std::string>& platesGenerated, const std::string& fileName) {
    std::string numberPlate;
    do {
        numberPlate = memoryTag + getAgeIdentifier(date)+ " ";
        for (int i = 0; i < 3; ++i) {
            numberPlate += generateRandomLetter();
        }
    } while (platesGenerated.find(numberPlate) != platesGenerated.end());
    platesGenerated.insert(numberPlate);
    savePlateToFile(numberPlate, fileName);
    return numberPlate;
}

int main() {
    srand(time(0));
    std::unordered_set<std::string> platesGenerated;
    std::string fileName = "generated_plates.txt";
    loadPlatesFromFile(platesGenerated, fileName);
    std::string memoryTags[] = {"YC", "LT", "FF"};
    std::string dates[] = {"04/07/2019", "23/01/2003", "30/05/2032"};
    for (int i = 0; i < 3; ++i) {
        try {
            std::string plate = generateNumberPlate(memoryTags[i], dates[i], platesGenerated, fileName);
            std::cout << "Number Plate for " << memoryTags[i] << " on " << dates[i] << ": " << plate << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << " for " << memoryTags[i] << " on " << dates[i] << std::endl;
        }
    }
    return 0;
}