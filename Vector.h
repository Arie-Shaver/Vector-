#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity,_size;

    // You may want to write a function that grows the vector
    void grow() {
        size_t i;
        if(_capacity==0){
            _capacity+=1;
        }
        else {
            _capacity*=2;
        }
        T* resize=new T[_capacity];
        for (i=0;i< _size;++i){
            resize[i]=std::move(array[i]);
        }
        delete [] array;
        array=std::move(resize);
    }

public:
    Vector() noexcept : _size{0}, _capacity{0}, array{nullptr}{}
    Vector(size_t count, const T& value) : array(new T[count]), _capacity(count), _size(count){
        for(size_t i=0;i<count;++i){
            array[i]=value;
        }
    }
    explicit Vector(size_t count) : _size{count}, _capacity{count}, array{new T[count]} {
        for(size_t i=0;i<_size;++i){
            array[i]=T();
        }
    }
    Vector(const Vector& other) : array {new T[other._size]}, _capacity{other._size}, _size{other._size}{
        for(size_t i=0;i<other._size;++i){
            array[i]=other.array[i];
        }
    }
    Vector(Vector&& other) noexcept : _size{other._size},_capacity{other._capacity},array{other.array}{
        for(size_t i=0;i<other._size;++i){
            array[i]=other.array[i];
        }
        other.array=nullptr;
        other._size=0;
        other._capacity=0;
    }
    ~Vector() { 
        delete [] array;
    }
    Vector& operator=(const Vector& other) { 
        size_t i;
        if (this!=&other){
            T* copy = new T[other._size];
            for(i=0;i<other._size;++i){
                copy[i]=other.array[i];
            }
            delete [] array;
            array=copy;
            _capacity=other._capacity;
            _size=other._size;
        }
        return *this;
     }
    Vector& operator=(Vector&& other) noexcept { 
        if (this!=&other){
            delete [] array;
            array=other.array;
            _capacity=other._capacity;
            _size=other._size;
            other.array=nullptr;
            other._capacity=0;
            other._size=0;
        }
        return *this;
     }

    iterator begin() noexcept { 
        return iterator(&array[0]);
    }
    iterator end() noexcept { 
        return iterator(&array[_size]);
    }

    [[nodiscard]] bool empty() const noexcept { 
        if (_size==0){
            return true;
        }
        else {
            return false;
        }
    }
    size_t size() const noexcept { 
        return _size;
    }
    size_t capacity() const noexcept { 
        return _capacity;
    }

    T& at(size_t pos) { 
        if (pos>=_size) {
            throw std::out_of_range("Out of bounds");
        }
        return array[pos];
     }
    const T& at(size_t pos) const { 
        if (pos>=_size) {
            throw std::out_of_range("Out of bounds");
        }
        return array[pos];
    }
    T& operator[](size_t pos) { 
        return array[pos];
     }
    const T& operator[](size_t pos) const { 
        return array[pos];
    }
    T& front() { 
        return array[0];
     }
    const T& front() const { 
        return array[0];
     }
    T& back() { 
        return array[_size-1];
     }
    const T& back() const { 
        return array[_size-1];
     }

    void push_back(const T& value) { 
        if(_size>=_capacity){
            grow();
        }
        array[_size]=value;
        _size+=1;
    }
    void push_back(T&& value) { 
        if(_size>=_capacity){
           grow();
        }
        array[_size]=std::move(value);
        _size+=1;
    }
    void pop_back() { 
        _size-=1;
     }

    iterator insert(iterator pos, const T& value) { 
        size_t position;
        size_t i=0;
        position = pos - begin();
        if (_size==_capacity) {
            grow();
        }
        for(i=_size-1;i>=position;--i){
            array[i+1]=std::move(array[i]);
        }
        array[position]=value;
        _size+=1;
        return iterator(&array[position]);
     }
    iterator insert(iterator pos, T&& value) { 
        size_t position;
        size_t i=0;
        position = pos - begin();
        if (_size==_capacity) {
            grow();
        }
        for(i=_size-1;i>=position;--i){
            array[i+1]=std::move(array[i]);
        }
        array[position]=std::move(value);
        _size+=1;
        return iterator(&array[position]);
    }
    iterator insert(iterator pos, size_t count, const T& value) {
        size_t position;
        size_t i=0;
        if (_size + count > _capacity){
            grow();
        }
        position=pos-begin();
        for (i=_size-1;i>=position;--i){
            array[i+count]=array[i];
        }
        for (i=position;i<(position+count);++i){
            array[i] = value;
        }
        _size=_size+count;
        return iterator(&array[position]);
    }
    iterator erase(iterator pos) { 
        size_t indice;
        size_t i;
        indice=pos-begin();
        for (i=indice;i<_size-1;++i)
        array[i]=array[i+1];
        _size=_size-1;
        return iterator(array+indice);
    }
    iterator erase(iterator first, iterator last) { 
        size_t indice;
        size_t num;
        size_t i;
        indice=first-begin();
        num=last-first;
        for (i=indice;i<(_size-num);++i){
            array[i]=array[i+num];
        }
        _size=_size-num;
        return iterator(array+indice);
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        T* v;
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() { 
            v = nullptr;
         }
        ~iterator() noexcept = default;
        explicit iterator(pointer vec) : v(vec) {}

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { 
            return *v;
        }
        [[nodiscard]] pointer operator->() const noexcept { 
            return v;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept { 
            v+=1;
            return *this;
         }
        // Postfix Increment: a++
        iterator operator++(int) noexcept { 
            iterator plus(*this);
            ++(*this);
            return plus;
         }
        // Prefix Decrement: --a
        iterator& operator--() noexcept { 
            v=v-1;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept { 
            iterator subtract(*this);
            --(*this);
            return subtract;
         }

        iterator& operator+=(difference_type offset) noexcept { 
            v+=offset;
            return *this;
         }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            return iterator (v+offset);
         }
        
        iterator& operator-=(difference_type offset) noexcept { 
            v=v-offset; 
            return *this;
         }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            return iterator(v-offset);
         }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            return (v-rhs.v);
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept { 
            return *(v+offset);
         }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            if(v==rhs.v){
                return true;
            }
            else {
                return false;
            }
        }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { 
            if (v!=rhs.v){
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { 
            if (v<rhs.v){
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { 
            if (v>rhs.v){
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { 
            if (v<=rhs.v){
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { 
            if (v>=rhs.v){
                return true;
            }
            else {
                return false;
            }
         }
    };


    void clear() noexcept { 
        _size=0;
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { 
     auto sum=iterator;
     sum=sum+offset;
     return sum;
 }

#endif