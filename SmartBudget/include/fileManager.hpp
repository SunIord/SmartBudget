#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <vector>

class Transaction;

class FileManager {
public:
    static bool loadFromFile(std::vector<Transaction>& transactions, const std::string& filename);
};

#endif // FILEMANAGER_HPP