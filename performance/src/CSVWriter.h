#pragma once

#include <fstream>

class CSVWriter
{
    std::ofstream file;
public:
    CSVWriter();
    ~CSVWriter();

    void writeLine(std::string line);
};

