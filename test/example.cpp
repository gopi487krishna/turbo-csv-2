#include<iostream>
#include<turbo-csv2/turbo-csv2.hpp>
#include<fstream>
#include<cstring>
#include<sstream>

std::string get_examples_dir(){
    return std::string(EXAMPLES);
}


std::string get_data(const std::string& filename){
    std::fstream file(filename);
    std::stringstream data_stream;
    data_stream<<file.rdbuf();
    return data_stream.str();
}


int main(){
    std::string data=get_data(get_examples_dir()+"cars.csv");
    char* raw_buf=(char*)(data.c_str());

    turbo_csv::csv_parser csv_reader;
    csv_reader.put_buffer(raw_buf,std::strlen(raw_buf));
    csv_reader.close_stream();

    turbo_csv::event current_event;
    int row_count = 0;
    
    while (!current_event.is_active(turbo_csv::ERRORED|turbo_csv::END_DOCUMENT)) {
       
        if (current_event.is_active(turbo_csv::END_RECORD)) {
            row_count++;
        }
        current_event = csv_reader.next();
    
    }

    std::cout << "\n Total Rows: " << row_count << '\n';


}