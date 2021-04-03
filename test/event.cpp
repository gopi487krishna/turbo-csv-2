#define BOOST_TEST_MODULE event_test

#include<turbo-csv2/event.hpp>
#include<boost/test/unit_test.hpp>



// To make keep things clean and easily understandable
using namespace turbo_csv;

BOOST_AUTO_TEST_SUITE(event_methods)

BOOST_AUTO_TEST_CASE(get_value) {

    event test_event{ "2014",event::event_type::FIELD };

    BOOST_REQUIRE_EQUAL("2014", test_event.value());

}
BOOST_AUTO_TEST_CASE(get_value_t){
    event test_event{"2014",event::event_type::FIELD};

    BOOST_REQUIRE_EQUAL(2014, test_event.value<std::int32_t>());
}

BOOST_AUTO_TEST_CASE(get_event_mask){
    event test_event{"2014",event::event_type::FIELD|event::event_type::START_RECORD};

    BOOST_REQUIRE_EQUAL((event::event_type::FIELD|event::event_type::START_RECORD),test_event.events());

}


BOOST_AUTO_TEST_SUITE_END()