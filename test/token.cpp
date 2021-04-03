#define BOOST_TEST_MODULE token_test

#include<turbo-csv2/token.hpp>
#include<boost/test/unit_test.hpp>



// To make keep things clean and easily understandable
using namespace turbo_csv;

BOOST_AUTO_TEST_SUITE(token_methods)

BOOST_AUTO_TEST_CASE(get_value) {

    token test_token{ "2014",token::event::FIELD };

    BOOST_REQUIRE_EQUAL("2014", test_token.value());

}
BOOST_AUTO_TEST_CASE(get_value_t){
    token test_token{"2014",token::event::FIELD};

    BOOST_REQUIRE_EQUAL(2014, test_token.value<std::int32_t>());
}

BOOST_AUTO_TEST_CASE(get_event_mask){
    token test_token{"2014",token::event::FIELD|token::event::START_RECORD};

    BOOST_REQUIRE_EQUAL((token::event::FIELD|token::event::START_RECORD),test_token.events());

}


BOOST_AUTO_TEST_SUITE_END()