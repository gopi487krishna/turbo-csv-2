#define BOOST_TEST_MODULE CSV_STREAM_TEST

#include<turbo-csv2/memstream.hpp>
#include<boost/test/unit_test.hpp>


using namespace turbo_csv;
BOOST_AUTO_TEST_SUITE(csv_stream_methods)

BOOST_AUTO_TEST_CASE(stream_closed){
    csv_stream test_stream;
    test_stream.close();
    BOOST_ASSERT(!test_stream.is_open());
}

BOOST_AUTO_TEST_CASE(insert_data){
    csv_stream test_stream;
    test_stream.push_back('A');
    BOOST_REQUIRE_EQUAL('A',test_stream.get_byte().value());
}

BOOST_AUTO_TEST_CASE(empty_buffer){
    csv_stream test_stream;
    BOOST_ASSERT(!test_stream.get_byte().has_value());
}

BOOST_AUTO_TEST_CASE(fully_consumed_buffer){
    csv_stream test_stream;
    test_stream.push_back('A');
    test_stream.push_back('B');
    test_stream.push_back('C');
    
    test_stream.get_byte();
    test_stream.get_byte();
    test_stream.get_byte();
    

    BOOST_ASSERT(!test_stream.get_byte().has_value());

}

BOOST_AUTO_TEST_CASE(free_space){
    csv_stream test_stream(3);
    BOOST_REQUIRE_EQUAL(3,test_stream.get_freespace());
    
    test_stream.push_back('A');
    test_stream.push_back('B');
    BOOST_REQUIRE_EQUAL(1,test_stream.get_freespace());
    
    test_stream.push_back('C');
    BOOST_REQUIRE_EQUAL(0,test_stream.get_freespace());
    
}
BOOST_AUTO_TEST_CASE(insert_x_bytes){
    csv_stream test_stream(3);
    
    std::string test_data;
    test_data.push_back('A');
    test_data.push_back('B');
    test_data.push_back('C');

    test_stream.insert(test_data.begin(),test_data.end());

    BOOST_REQUIRE_EQUAL('A',test_stream.get_byte().value());
    BOOST_REQUIRE_EQUAL('B',test_stream.get_byte().value());
    BOOST_REQUIRE_EQUAL('C',test_stream.get_byte().value());
   
    test_stream.insert(test_data.begin(),test_data.end());
    
    test_stream.get_byte();
    test_stream.get_byte();

    std::string new_test_data;
    new_test_data.push_back('M');
    new_test_data.push_back('N');
    
    test_stream.insert(new_test_data.begin(),new_test_data.end());

    BOOST_REQUIRE_EQUAL('C',test_stream.get_byte().value());
    BOOST_REQUIRE_EQUAL('M',test_stream.get_byte().value());
    BOOST_REQUIRE_EQUAL('N',test_stream.get_byte().value());
}

BOOST_AUTO_TEST_CASE(is_errored){
    csv_stream test_stream(3);
    std::string data{'A','B','C'};

    test_stream.insert(data.begin(),data.end());

    test_stream.get_byte();
    test_stream.get_byte();
    test_stream.get_byte();

    BOOST_ASSERT(test_stream.is_errored());
}

BOOST_AUTO_TEST_CASE(is_not_errored){
    csv_stream test_stream(3);
    std::string data{'A','B','C'};

    test_stream.insert(data.begin(),data.end());

    // Simulate consuming a byte
    test_stream.get_byte();

    BOOST_ASSERT(!test_stream.is_errored());

}


BOOST_AUTO_TEST_SUITE_END()