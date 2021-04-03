#ifndef EVENT_HPP
#define EVENT_HPP

#include<string>
#include<utility>
#include<string_view>
#include<boost/lexical_cast.hpp>

namespace turbo_csv {
    struct event {

        enum event_type:std::uint8_t {
            END_DOCUMENT = 1,
            FIELD = 2,
            START_RECORD = 4,
            END_RECORD = 8,
            ERRORED = 16
        };

    private:

        std::string token;
        std::uint8_t event_mask;

    public:

        /**
         * @brief Construct a new event object
         *
         * @param token basic unit in turbo_csv is field
         * @param event_mask A mask that indicates various events associated with parsing this token
         */
        event(const std::string& token, std::uint8_t event_mask)
            noexcept(noexcept(std::declval<std::string::allocator_type>()))
            :token(token), event_mask(event_mask) {}

        /**
         * @brief Returns a view into the token(field value) read by the parser
         *
         * @return std::string_view
         * @note The field value can only be read if the FIELD event has occured i.e a field has been parsed
         */
        std::string_view value()const noexcept {
            return token;
        }

        /**
         * @brief Returns the token read by the parser in the desired type (deserialized)
         *
         * @tparam T  Type to convert the field data(token) to
         * @return T  The field data(token) deserialized to type T
         */
        template<typename T>
        T value()const noexcept(false) {
            return boost::lexical_cast<T>(token);
        }

        /**
         * @brief Returns the event mask that depicts all the events active after the token reading
         *
         * @return std::uint8_t Mask
         */
        std::uint8_t events()const noexcept {
            return event_mask;
        }

        /**
         * @brief Checks if a perticular event is active
         *
         * @param event Event to be checked for
         * @return true
         * @return false
         */
        bool event_active(event_type type)const noexcept {
            bool state = false;

            switch (type) {
            case event_type::FIELD:
                state = event_mask & 2;
                break;
            case event_type::START_RECORD:
                state = event_mask & 4;
                break;
            case event_type::END_RECORD:
                state = event_mask & 8;
                break;
            case event_type::END_DOCUMENT:
                state = event_mask & 1;
                break;
            case event_type::ERRORED:
                state = event_mask & 16;
                break;
            }
            return state;
        }

    };
}



#endif