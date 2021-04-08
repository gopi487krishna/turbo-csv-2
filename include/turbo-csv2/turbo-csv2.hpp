#ifndef TURBO_CSV_2_HPP
#define TURBO_CSV_2_HPP

#include<turbo-csv2/parser.hpp>
#include<turbo-csv2/event.hpp>
#include<turbo-csv2/dialect.hpp>
#include<turbo-csv2/fstream_adaptor.hpp>


namespace turbo_csv{

   using csv_parser = parser<dialect>;

}

#endif