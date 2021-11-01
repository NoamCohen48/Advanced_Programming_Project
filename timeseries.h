

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <fstream>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

class TimeSeries {
private:
    std::string CSVfileName;
    std::vector<std::pair<std::string, std::vector<float>>> table;

    static std::vector<std::pair<std::string, std::vector<float>>> readCsv(const std::string &CSVfileName);

public:

    explicit TimeSeries(const std::string &CSVfileName) {
        this->CSVfileName = CSVfileName;
        table = readCsv(CSVfileName);
    }


    const std::vector<float> &getColumn(int id) const;

    const std::vector<float> &getColumn(const std::string &columnName) const;

    int getValue(std::string &columnName, int row) const;

    std::string getColumnName(int id) const;

    unsigned long long int getColumnsSize() const;

    unsigned long long int getRowsSize() const;
};


#endif /* TIMESERIES_H_ */
