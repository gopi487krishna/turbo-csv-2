#include<iostream>
#include<turbo-csv2/turbo-csv2.hpp>

auto get_examples_dir() {
    return std::string(EXAMPLES);
}

int main() {
    turbo_csv::csv_parser csv_reader(get_examples_dir() + "cars.csv");
    turbo_csv::event current_event;
    int row_count = 0;
    while (!current_event.is_active(turbo_csv::ERRORED|turbo_csv::END_DOCUMENT)) {
       
        if (current_event.is_active(turbo_csv::END_RECORD)) {
            row_count++;
        }
        current_event = csv_reader.next();
    
    }
    std::cout << "\nRow Count: " << row_count << '\n';
    return 0;
}