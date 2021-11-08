#include "timeseries.h"

/**
 * read a file and fill a table with the info.
 * @param CSVfileName the file name.
 * @return a table of the csv file.
 */
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

/**
 * get a column in the table.
 * @param columnName the column title - name.
 * @return the column as vector.
 */
const std::vector<float> &TimeSeries::getColumn(const std::string &columnName) const {
    // searching for the column
    for (const std::pair<std::string, std::vector<float>> &pair: table) {
        if (pair.first == columnName) {
            return pair.second;
        }
    }
    throw std::runtime_error("column name wasn't found");
}

/**
 * get the amount of of columns.
 * @return unsigned long long that represent the amount of of columns.
 */
unsigned long long int TimeSeries::getColumnsSize() const {
    return table.size();
}

/**
 * get the amount of of rows.
 * @return unsigned long long that represent the amount of of rows.
 */
unsigned long long int TimeSeries::getRowsSize() const {
    if (getColumnsSize() <= 0) {
        return 0;
    }
    return table[0].second.size();
}

/**
 * get a column in the table.
 * @param id the column number.
 * @return the column as vector.
 */
const std::vector<float> &TimeSeries::getColumn(int id) const {
    return table[id].second;
}

/**
 * get the title of a column.
 * @param id the column id - number.
 * @return the name of the column.
 */
std::string TimeSeries::getColumnName(int id) const {
    return table[id].first;
}

