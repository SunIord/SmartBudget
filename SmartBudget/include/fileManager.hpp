#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "transaction.hpp"

class FileManager {
private:
    static std::vector<std::string> parseCSVLine(const std::string& line);

public:
    static bool saveToFile(const std::vector<Transaction>& transactions, const std::string& filename);
    static bool loadFromFile(std::vector<Transaction>& transactions, const std::string& filename);
    static std::string ensureCSVExtension(const std::string& filename); 
    static bool fileExists(const std::string& filename);
    static bool deleteFile(const std::string& filename);
    static std::string generateUniqueFilename(const std::string& baseName); 
    static std::vector<std::string> listCSVFiles();
};

#endif