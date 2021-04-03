#define BOOST_TEST_MODULE csv_parser_test

#include<turbo-csv2/dialect.hpp>
#include<turbo-csv2/parser.hpp>
#include<turbo-csv2/fstream_adaptor.hpp>

#include<boost/test/unit_test.hpp>



using namespace turbo_csv;

auto get_examples_dir() {
    return std::string(EXAMPLES);
}


BOOST_AUTO_TEST_SUITE(parser_methods)

BOOST_AUTO_TEST_CASE(row_count) {
    parser<adapted_fstream, dialect> csv_parser(get_examples_dir() + "cars.csv");
    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();

        if (current_token.event_active(event::event_type::END_DOCUMENT) || current_token.event_active(event::event_type::ERRORED)) {
            break;
        }

        if (current_token.event_active(event::event_type::END_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(2, row_count);
}

BOOST_AUTO_TEST_CASE(row_count_large_file) {
    parser<adapted_fstream, dialect> csv_parser(get_examples_dir() + "business-price-index.csv");
    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();

        if (current_token.event_active(event::event_type::END_DOCUMENT) || current_token.event_active(event::event_type::ERRORED)) {
            break;
        }

        if (current_token.event_active(event::event_type::END_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(68855, row_count);
}

BOOST_AUTO_TEST_CASE(row_count_by_counting_start_of_records) {
    parser<adapted_fstream, dialect> csv_parser(get_examples_dir() + "business-price-index.csv");
    std::int32_t row_count = 0;

    while (true) {
        auto current_token = csv_parser.next();
        if (current_token.event_active(event::event_type::END_DOCUMENT) || current_token.event_active(event::event_type::ERRORED)) {
            break;
        }

        if (current_token.event_active(event::event_type::START_RECORD)) {
            row_count++;
        }
    }

    BOOST_REQUIRE_EQUAL(68855, row_count);

}

BOOST_AUTO_TEST_CASE(errored_stream) {
    parser<adapted_fstream, dialect> csv_parser("meow");

    auto current_token = csv_parser.next();

    BOOST_ASSERT(current_token.event_active(event::event_type::ERRORED));

}
BOOST_AUTO_TEST_SUITE_END()