#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "transaction.hpp"

namespace smartbudget {

class FileManager {
private:
    static vector<string> parseCSVLine(const string& line);

public:
    static bool saveToFile(const vector<Transaction>& transactions, const string& filename);
    static bool loadFromFile(vector<Transaction>& transactions, const string& filename);
    static string ensureCSVExtension(const string& filename); 
    static bool fileExists(const string& filename);
    static bool deleteFile(const string& filename);
    static string generateUniqueFilename(const string& baseName); 
    static vector<string> listCSVFiles();
};

}

#endif