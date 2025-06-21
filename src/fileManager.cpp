#include "fileManager.hpp"
using namespace smartbudget;

bool FileManager::saveToFile(const vector<Transaction>& transactions, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Erro ao abrir o arquivo para escrita: " << filename << endl;
        return false;
    }

    for (const auto& transaction : transactions) {
        outFile << transaction.toCSV() << "\n";
    }

    outFile.close();
    cout << "Transacoes salvas com sucesso em '" << filename << "'." << endl;
    return true;
}

bool FileManager::loadFromFile(vector<Transaction>& transactions, const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Erro ao abrir o arquivo para leitura.\n";
        return false;
    }

    string line;
    while (getline(infile, line)) {
        vector<string> fields = parseCSVLine(line);
        if (fields.size() != 5) continue;

        double amount = stod(fields[0]);
        string type = fields[1];
        string category = fields[2];
        string date = fields[3];
        string description = fields[4];

        transactions.push_back(Transaction(amount, type, category, date, description));
    }
    return true;
}

vector<string> FileManager::parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

string FileManager::ensureCSVExtension(const string& filename) {
    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv") {
        return filename;
    } else {
        return filename + ".csv";
    }
}

bool FileManager::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

bool FileManager::deleteFile(const string& filename) {
    if (remove(filename.c_str()) == 0) {
        return true; 
    } else {
        return false;
    }
}

string FileManager::generateUniqueFilename(const string& baseName) {
    string filename = ensureCSVExtension(baseName);
    int counter = 1;

    while (fileExists(filename)) {
        filename = baseName + to_string(counter) + ".csv";
        counter++;
    }

    return filename;
}