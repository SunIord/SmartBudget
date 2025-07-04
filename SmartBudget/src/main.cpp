#include <map>
#include "appUI.hpp"

int main() {
    // Other code...

    std::vector<Transaction>& transactions = manager.getTransactions(); // Ensure this returns a non-const reference

    switch (option) {
        // Other cases...
        case 5: {
            std::map<std::string, double> categoryTotals; // Declare before use
            // Other code...
            for (const auto& t : transactions) {
                categoryTotals[t.getCategory()] += t.getAmount();
            }
            // Other code...
            break;
        }
        // Other cases...
    }

    // Other code...
}