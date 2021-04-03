#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<string>
#include<utility>
#include<string_view>
#include<boost/lexical_cast.hpp>

namespace turbo_csv {
    struct token {

        enum event {
            END_DOCUMENT = 1,
            FIELD = 2,
            START_RECORD = 4,
            END_RECORD = 8,
            ERRORED = 16
        };

    private:

        std::string raw_field;
        std::int32_t event_mask;

    public:


        token(const std::string& raw_field, std::int32_t event_mask)
            noexcept(noexcept(std::declval<std::string::allocator_type>()))
            :raw_field(raw_field), event_mask(event_mask) {}

        std::string_view value()const noexcept {
            return raw_field;
        }

        template<typename T>
        T value()const noexcept(false) {
            return boost::lexical_cast<T>(raw_field);
        }

        std::int32_t events()const noexcept {
            return event_mask;
        }

        bool is_event(event event)const noexcept {
            bool result = false;

            switch (event) {
            case event::FIELD:
                result = event_mask & 2;
                break;
            case event::START_RECORD:
                result = event_mask & 4;
                break;
            case event::END_RECORD:
                result = event_mask & 8;
                break;
            case event::END_DOCUMENT:
                result = event_mask & 1;
                break;
            case event::ERRORED:
                result = event_mask & 16;
                break;
            }
            return result;
        }




    };
}



#endif