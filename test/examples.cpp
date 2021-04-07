#include<iostream>
#include<sstream>
#include<chrono>
#include<turbo-csv2/memstream.hpp>
#include<turbo-csv2/turbo-csv2.hpp>

auto get_examples_dir() {
    return std::string(EXAMPLES);
}

void fstream_based_row_count() {
    turbo_csv::adapted_fstream file_stream(get_examples_dir() + "cars.csv");

    turbo_csv::csv_parser csv_reader(file_stream);

    turbo_csv::event current_event;
    int row_count = 0;
    while (!current_event.is_active(turbo_csv::ERRORED|turbo_csv::END_DOCUMENT)) {

        if (current_event.is_active(turbo_csv::END_RECORD)) {
            row_count++;
        }
        current_event = csv_reader.next();

    }
    std::cout << "\nRow Count: " << row_count << '\n';
}




std::string get_file_data(const std::string& filepath) {
    std::fstream file(filepath);
    std::stringstream file_stream;
    file_stream << file.rdbuf();
    return file_stream.str();
}

// This just simulates a user controlled memory stream ( for example from network) 
void memory_based_row_count() {

    std::string data = get_file_data(get_examples_dir() + "business-price-index.csv");

    // 10 byte internal circular queue buffer
    turbo_csv::csv_stream memory_stream(10);

    // Should have probably used a coroutine to do this
    auto data_start_pos=data.begin();


    int row_count = 0;
    turbo_csv::event current_event;
    turbo_csv::parser<turbo_csv::csv_stream, turbo_csv::dialect> csv_reader(memory_stream);

    
     auto fill_buffer = [&]() {
        if (data.end() - data_start_pos < memory_stream.get_freespace()) {
            memory_stream.insert(data_start_pos, data.end());
            memory_stream.close();
        }
        else {
            auto free_space = memory_stream.get_freespace();
            memory_stream.insert(data_start_pos, data_start_pos + free_space);
            data_start_pos = data_start_pos + free_space;
        }

    };



    // Main loop to count rows in csv file
    while (!current_event.is_active(turbo_csv::END_DOCUMENT)) {

        if (current_event.is_active(turbo_csv::ERRORED)) {
            fill_buffer();
        }
        else if (current_event.is_active(turbo_csv::END_RECORD)) {
            row_count++;
        }

        current_event = csv_reader.next();

    }   

    std::cout << row_count;

}

int main(){
    memory_based_row_count();
    return 0;
}