#ifndef MEMSTREAM_HPP
#define MEMSTREAM_HPP

#include<boost/circular_buffer.hpp>

namespace turbo_csv {
    class memory_stream {
        bool stream_open;
        boost::circular_buffer<std::uint8_t> data_buffer;
    public:
        /**
         * @brief Construct a new memory stream object
         *
         * @param buffer_size Internal buffer size
         */
        memory_stream(std::size_t buffer_size = 1024) :stream_open(true), data_buffer(buffer_size) { }

        /**
         * @brief Returns whether stream is open or has been closed
         *
         * @return true
         * @return false
         */
        bool is_open() {
            return stream_open;
        }

        /**
         * @brief  Closes the stream for reading
         *
         */
        void close() { stream_open = false; }

        /**
         * @brief Inserts the byte into the buffer at last position
         *
         * @param byte byte to be inserted into the buffer
         * @note This function may overwrite the unread buffer data if the buffer is full
         */
        void push_back(std::uint8_t byte) {
            data_buffer.push_back(byte);
        }

        /**
         * @brief Get the byte object
         *
         * @return std::optional<std::uint8_t>
         */
        std::optional<std::uint8_t> get_byte() {
            if (!data_buffer.empty()) {
                auto element = data_buffer.front();
                data_buffer.pop_front();
                return element;
            }
            return {};
        }

        /**
         * @brief Get the freespace avaiable inside buffer
         *
         * @return std::size_t
         */
        std::size_t get_freespace() {
            return data_buffer.capacity() - data_buffer.size();
        }

        /**
         * @brief  Returns true if the buffer is open but is not recieving any data
         *
         * @return true
         * @return false
         */
        bool is_errored() {
            if (!stream_open) { return false; }
            else if (data_buffer.empty()) { return true; }
            else {
                return false;
            }
        }

        /**
         * @brief  Inserts the data from begin to end(exclusive) into the buffer
         *
         * @tparam Iter Iterator type
         * @param begin Starting position
         * @param end   Ending position
         * @note Insert must be done according to the freespace requirements or the buffer will be overwritten
         *       (which is desirable sometimes)
         */
        template<typename Iter>
        void insert(Iter begin, Iter end) {
            data_buffer.insert(data_buffer.end(), begin, end);
        }

    };
}

#endif