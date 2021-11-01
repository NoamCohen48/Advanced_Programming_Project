#include "timeseries.h"


std::vector<std::pair<std::string, std::vector<float>>> TimeSeries::readCsv(const std::string &CSVfileName) {
    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<float>>> result;

    // Create an input filestream
    std::ifstream myFile(CSVfileName);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    float val;

    // Read the column names
    if (myFile.good()) {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ',')) {

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<float>{}});
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line)) {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while (ss >> val) {

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',') ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return std::move(result);
}

const std::vector<float> &TimeSeries::getColumn(const std::string &columnName) const {
    // searching for the column
    for (const std::pair<std::string, std::vector<float>> &pair: table) {
        if (pair.first == columnName) {
            return pair.second;
        }
    }
    throw std::runtime_error("column name wasn't found");
}

int TimeSeries::getValue(std::string &columnName, int row) const {
    std::vector<float> column = getColumn(columnName);

    return column[row];
}

unsigned long long int TimeSeries::getColumnsSize() const {
    return table.size();
}

unsigned long long int TimeSeries::getRowsSize() const {
    return table[0].second.size();
}

const std::vector<float> &TimeSeries::getColumn(int id) const {
    return table[id].second;
}

std::string TimeSeries::getColumnName(int id) const {
    return table[id].first;
}

