#include "fileManager.hpp"

// Classe responsável por gerenciar a leitura e escrita de arquivos CSV
bool FileManager::saveToFile(const std::vector<Transaction>& transactions, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return false;
    }

    for (const auto& transaction : transactions) {
        outFile << transaction.toCSV() << "\n";
    }

    outFile.close();
    // Para a versão em terminal
    std::cout << "Transacoes salvas com sucesso em '" << filename << "'." << std::endl;
    return true;
}

// Carrega transações de um arquivo CSV e preenche um vetor com os dados lidos 
bool FileManager::loadFromFile(std::vector<Transaction>& transactions, const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Erro ao abrir o arquivo para leitura.\n";
        return false;
    }

    std::string line;
    while (getline(infile, line)) {
        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() != 5) continue;

        double amount = stod(fields[0]);
        std::string type = fields[1];
        std::string category = fields[2];
        std::string date = fields[3];
        std::string description = fields[4];

        transactions.push_back(Transaction(amount, type, category, date, description));
    }
    return true;
}

// Converte uma linha CSV em um vetor de campos, respeitando aspas e vírgulas
std::vector<std::string> FileManager::parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
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

// Garante que o nome do arquivo termine com a extensão .csv
std::string FileManager::ensureCSVExtension(const std::string& filename) {
    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv") {
        return filename;
    } else {
        return filename + ".csv";
    }
}

// Verifica se um arquivo com o nome especificado existe
bool FileManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Exclui o arquivo selecionado do sistema de arquivos
bool FileManager::deleteFile(const std::string& filename) {
    if (remove(filename.c_str()) == 0) {
        return true; 
    } else {
        return false;
    }
}

// Adiciona um número caso já exista um outro arquivo com o mesmo nome de algum outro
std::string FileManager::generateUniqueFilename(const std::string& baseName) {
    std::string filename = ensureCSVExtension(baseName);
    int counter = 1;

    while (fileExists(filename)) {
        filename = baseName + std::to_string(counter) + ".csv";
        counter++;
    }

    return filename;
}

// Lista todos os arquivos CSV no diretório atual
std::vector<std::string> FileManager::listCSVFiles() {
    std::vector<std::string> csvFiles;
    DIR* dir;
    struct dirent* ent;

    dir = opendir(".");
    if (dir != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string filename(ent->d_name);
            if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".csv") {
                csvFiles.push_back(filename);
            }
        }
        closedir(dir);
    }
    return csvFiles;
}