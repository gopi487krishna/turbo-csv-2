#ifndef MEMSTREAM_HPP
#define MEMSTREAM_HPP

#include<boost/circular_buffer.hpp>

namespace turbo_csv{
    class csv_stream{
    bool stream_open;
    boost::circular_buffer<std::uint8_t> cb;
    public:
    csv_stream(std::size_t buffer_size=1024):stream_open(true),cb(buffer_size){ }
    bool is_open(){
        return stream_open;
    }
    void close(){stream_open=false;}
    void push_back(std::uint8_t character){
        cb.push_back(character);
    }
    std::optional<std::uint8_t> get_byte(){
       if(!cb.empty()){
           auto element=cb.front();
           cb.pop_front();
           return element;
       }
       return {};
    }
    std::size_t get_freespace(){
        return cb.capacity()-cb.size();
    }

    bool is_errored(){
        if(!stream_open){return false;}
        else if(cb.empty()){return true;}
        else{
           return false;
        }      
    }

    template<typename Iter>
    void insert(Iter begin, Iter end){
        cb.insert(cb.end(),begin,end);
    }

};
}

#endif