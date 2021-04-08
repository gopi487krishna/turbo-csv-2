#define BOOST_TEST_MODULE csv_parser_test

#include<turbo-csv2/dialect.hpp>
#include<turbo-csv2/parser.hpp>
#include<turbo-csv2/fstream_adaptor.hpp>
#include<sstream>

#include<boost/test/unit_test.hpp>



using namespace turbo_csv;

auto get_examples_dir() {
    return std::string(EXAMPLES);
}


BOOST_AUTO_TEST_SUITE(parser_methods)

BOOST_AUTO_TEST_CASE(row_count) {

    std::fstream file(get_examples_dir()+"cars.csv");    
    std::stringstream raw_buffer;
    raw_buffer<<file.rdbuf();
    std::string data=raw_buffer.str();


    std::uint8_t* raw_buf= new std::uint8_t[data.size()];


    parser<dialect> csv_parser(raw_buf,data.size());
    csv_parser.get_memory_stream().insert(data.begin(),data.end());
    csv_parser.get_memory_stream().close();


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
    
    std::fstream file(get_examples_dir()+"business-price-index.csv");    
    std::stringstream raw_buffer;
    raw_buffer<<file.rdbuf();
    std::string data=raw_buffer.str();

    std::uint8_t* raw_buf= new std::uint8_t[data.size()];

    parser<dialect> csv_parser(raw_buf,data.size());
    csv_parser.get_memory_stream().insert(data.begin(),data.end());
   csv_parser.get_memory_stream().close();

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

    std::fstream file(get_examples_dir()+"business-price-index.csv");    
    std::stringstream raw_buffer;
    raw_buffer<<file.rdbuf();
    std::string data=raw_buffer.str();

    std::uint8_t* raw_buf= new std::uint8_t[data.size()];

    parser<dialect> csv_parser(raw_buf,data.size());
    csv_parser.get_memory_stream().insert(data.begin(),data.end());
    csv_parser.get_memory_stream().close();
    
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
  
    std::uint8_t raw_buf[5];
    parser<dialect> csv_parser(raw_buf,5);


    auto current_token = csv_parser.next();

    BOOST_ASSERT(current_token.is_active(event_type::ERRORED));

}
BOOST_AUTO_TEST_SUITE_END()