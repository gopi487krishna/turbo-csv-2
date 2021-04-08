#ifndef CUSTOM_ALLOCATOR_HPP
#define CUSTOM_ALLOCATOR_HPP

#include<memory>
#include<cstddef>
#include<cassert>

template<typename T>
class fixed_allocator:public std::allocator<T>{
    public:
    using value_type=T;
    private:
    value_type* buffer_handle;
    std::size_t capacity;
    bool in_use;
    public:



     ~fixed_allocator()                                  = default;
    fixed_allocator()                                   = delete;    
    fixed_allocator(fixed_allocator const&)             = default;  // Required by rebind.
    fixed_allocator(fixed_allocator &&)                 = default;
    fixed_allocator& operator=(fixed_allocator const&)  = default;
    fixed_allocator& operator=(fixed_allocator&&)       = default;


    template<class OT> struct rebind{
        using other=fixed_allocator<OT>;
    };

    fixed_allocator(value_type* buffer_handle, std::size_t capacity):
    buffer_handle(buffer_handle),
    capacity(capacity),
    in_use(false)
    {
        volatile int x=0;
    }

    value_type* allocate(std::size_t length){
        assert(length<=this->capacity);
        assert(!this->in_use);
        this->in_use=true;
        return this->buffer_handle;
    }

    void deallocate(value_type* buffer_handle, std::size_t length){
        assert(buffer_handle==this->buffer_handle);
        assert(this->in_use);
        this->in_use=false;
    }
};



#endif