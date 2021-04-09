#define BOOST_TEST_MODULE csv_parser_test

#include<turbo-csv2/dialect.hpp>
#include<turbo-csv2/parser.hpp>
#include<sstream>
#include<fstream>
#include<boost/test/unit_test.hpp>



using namespace turbo_csv;

auto get_examples_dir() {
    return std::string(EXAMPLES);
}

std::string get_data(const std::string& filename){
    std::fstream file(filename);
    std::stringstream data_stream;
    data_stream<<file.rdbuf();
    return data_stream.str();
}


BOOST_AUTO_TEST_SUITE(parser_methods)

BOOST_AUTO_TEST_CASE(row_count) {

    std::string data=get_data(get_examples_dir()+"cars.csv");
    char* raw_buf=(char*)(data.c_str());

    parser<dialect> csv_parser;
    csv_parser.put_buffer(raw_buf,std::strlen(raw_buf));
    csv_parser.close_stream();



    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();

        if (current_token.is_active(event_type::END_DOCUMENT) || current_token.is_active(event_type::ERRORED)) {
            break;
        }

        if (current_token.is_active(event_type::END_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(2, row_count);
}

BOOST_AUTO_TEST_CASE(row_count_large_file) {
    
    std::string data=get_data(get_examples_dir()+"business-price-index.csv");
    char* raw_buf=(char*)(data.c_str());

    parser<dialect> csv_parser;
    csv_parser.put_buffer(raw_buf,std::strlen(raw_buf));
    csv_parser.close_stream();


    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();

        if (current_token.is_active(event_type::END_DOCUMENT) || current_token.is_active(event_type::ERRORED)) {
            break;
        }

        if (current_token.is_active(event_type::END_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(68855, row_count);
}

BOOST_AUTO_TEST_CASE(row_count_by_counting_start_of_records) {

    std::string data=get_data(get_examples_dir()+"business-price-index.csv");
    char* raw_buf=(char*)(data.c_str());

    parser<dialect> csv_parser;
    csv_parser.put_buffer(raw_buf,std::strlen(raw_buf));
    csv_parser.close_stream();

    
    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();
        if (current_token.is_active(event_type::END_DOCUMENT) || current_token.is_active(event_type::ERRORED)) {
            break;
        }

        if (current_token.is_active(event_type::START_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(68855, row_count);

}

BOOST_AUTO_TEST_CASE(errored_stream) {
  
    parser<dialect> csv_parser;


    auto current_token = csv_parser.next();

    BOOST_ASSERT(current_token.is_active(event_type::ERRORED));

}
BOOST_AUTO_TEST_SUITE_END()