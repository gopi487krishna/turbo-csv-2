#ifndef PARSER_HPP
#define PARSER_HPP

#include<turbo-csv2/event.hpp>

namespace turbo_csv {
    template<typename FileReader, typename Dialect>
    struct parser {
        FileReader file_reader;
    private:
        std::uint8_t event_mask = 0; // Holds current active events of the parser (state)
    public:

        /**
         * @brief Constructs a new parser object and opens file('filepath') to parse csv data
         * 
         * @param filepath Location of the file
         */
        parser(const std::string& filepath) :
            file_reader(filepath)
        {
            if (!file_reader.is_open()) {
                event_mask = event::event_type::ERRORED;
            }
        }

        /**
         * @brief Retrieves the current state of parser and parses the token if possible
         * 
         * @return event Event depicting the current state of parser
         */
        event next() noexcept(false) {

            if (event_mask & event::event_type::END_DOCUMENT || event_mask & event::event_type::ERRORED) {
                return event{ "", event_mask };
            }
            else {
                return parse_token();
            }
        }


    // Helper methods
    private:

       
        event parse_token() noexcept(false) {
            std::string raw_token; // Field value
            std::int32_t escape_count = 0; // Indicates the number of escape characters encountered

            while (true) {

                auto byte = file_reader.get_byte();

                if (is_no_data_event(byte, raw_token)) {return event{ "",event_mask };}
                if (is_last_record(byte)) {set_event(event::event_type::END_RECORD);return event{ raw_token,event_mask };}
                
                if (Dialect::is_escapecharacter(*byte)) {
                    escape_count += 1;
                    raw_token.push_back(*byte);
                }
                else {
                    if (membership_in_escape_set(escape_count)) {
                        raw_token.push_back(*byte);
                    }
                    else {

                        if (is_seperator(*byte)) {
                            process_seperators(*byte);
                            return event{ raw_token,event_mask };
                        }
                        else if (Dialect::is_ignorecharacter(*byte)) {/*ignore it*/}
                        else {raw_token.push_back(*byte); }

                    }

                }
            }

        }



        bool is_end_of_document(const std::optional<std::uint8_t>& byte, const std::string& raw_token) noexcept {
            return !byte.has_value() && raw_token.empty();
        }

        bool is_last_record(const std::optional<std::uint8_t>& byte)noexcept {
            return !byte.has_value();
        }

        bool is_no_data_event(const std::optional<std::uint8_t>& byte, const std::string& raw_token)noexcept {

            if (file_reader.is_errored()) {
                set_event(event::event_type::ERRORED);
                return true;
            }
            else if (is_end_of_document(byte, raw_token)) {
                set_event(event::event_type::END_DOCUMENT);
                return true;
            }

            return false;
        }

        void set_event(std::uint8_t new_mask)noexcept {
            event_mask = new_mask;
        }


        bool is_seperator(std::uint8_t byte)noexcept {
            return Dialect::is_recordseperator(byte) || Dialect::is_fieldseperator(byte);
        }

        void process_seperators(std::uint8_t byte)noexcept {
            if (Dialect::is_recordseperator(byte)) {
                set_record_seperator_event();
            }
            else if (Dialect::is_fieldseperator(byte)) {
                set_field_seperator_event();
            }
        }

        void set_record_seperator_event()noexcept {
            set_event(event::event_type::END_RECORD | event::event_type::FIELD);
        }

        void set_field_seperator_event() noexcept {
            if (event_mask == 0 || event_mask & event::event_type::END_RECORD) {
                set_event(event::event_type::START_RECORD);
            }
            else if (event_mask & event::event_type::START_RECORD) {
                event_mask = 0;
            }

            set_event(event_mask | event::event_type::FIELD);
        }

        bool membership_in_escape_set(std::int32_t escape_count) noexcept {return escape_count%2!=0;}

    };
}
#endif