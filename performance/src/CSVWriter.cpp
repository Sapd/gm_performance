#include "CSVWriter.h"

#include <iostream>
#include <algorithm>

CSVWriter::CSVWriter()
{
    file.open("benchmark.csv");
    file << "Time;FPS;DiffMS;DiffUS\n";
    file.flush();
}

CSVWriter::~CSVWriter()
{
    file.flush();
    file.close();
}

void CSVWriter::writeLine(std::string line)
{
    std::replace(line.begin(), line.end(), '.', ',');
    file << line.c_str() << "\n";
}
