#ifndef EX7_AMITHALBREICH_VL_VECTOR_H
#define EX7_AMITHALBREICH_VL_VECTOR_H

#include <iostream>

#define DEFAULT_STATIC_CAPACITY 16
#define INITIAL_SIZE 0
#define SINGLE_VALUE 1
#define IDX_OUT_OF_RANGE_ERROR "Error: Index out of bound!\n"

template<typename T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class vl_vector {
 public:
    // Iterator Typedefs:
    /**
     * Define the iterator of vl_vector as pointer to T Value.
     */
    typedef T* iterator;

    /**
    * Define the const_iterator of vl_vector as pointer to T Value.
    */
    typedef const T* const_iterator;

    /**
     * Define the reverse_iterator of vl_vector the standard library
     * reverse_iterator using the iterator typedef above (Pointer to T Value).
     */
    typedef std::reverse_iterator<iterator> reverse_iterator;

    /**
     * Define the const_reverse_iterator of vl_vector the standard library
     * reverse_iterator using the const_iterator typedef above
     * (Pointer to T Value).
     */
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Iterator functions:
    /**
    * Define vl_vector const or non-const beginnings and ends for
    * forward iterator.
    */
    iterator begin() { return data(); }

    const_iterator begin() const { return data(); }

    const_iterator cbegin() const { return data(); }

    iterator end() { return data() + size_; }

    const_iterator end() const { return data() + size_; }

    const_iterator cend() const { return data() + size_; }

    /**
    * Define vl_vector const or non-const beginnings and ends for
    * reverse_iterator and const_reverse_iterator.
    */

    reverse_iterator rbegin()
    {
        return reverse_iterator(data() + size_);
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(data() + size_);
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(data() + size_);
    }

    reverse_iterator rend()
    {
        return reverse_iterator(data());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(data());
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(data());
    }

    /**
     * Default constructor, constructs an empty vl_vector with given
     * Static Capacity.
     */
    vl_vector() : size_(INITIAL_SIZE), capacity_(StaticCapacity),
                  dynamic_container_(nullptr) {};

    /**
    * Copy Constructor for vl_vector
    * @param other_vl_vector vl_vector to copy from
    */
    vl_vector(const vl_vector &other_vl_vector) : size_(other_vl_vector.size_),
    capacity_(other_vl_vector.capacity_), dynamic_container_(nullptr)
    {
        copy_vl_vector_values(other_vl_vector);
    }

    /**
   * Sequence based Constructor - Gets a range of T Values as InputIterator
   * and stores the values in the vl_vector by the order they appeared in the
   * InputIterator from first to last (not including last) [first,last).
   * @param first InputIterator- first T Value in the range to insert.
   * @param last InputIterator- last- one after T Value in the range to insert.
   */
    template<class InputIterator>
    vl_vector(InputIterator first, InputIterator last)
    {
        capacity_ = StaticCapacity;
        size_t value_count = std::distance(first, last);
        if (value_count > StaticCapacity)
        {
            capacity_ = (3 * value_count) / 2;
            dynamic_container_ = new T[capacity_];
            std::copy(first, last, begin());
        }
        else
        {
            std::copy(first, last, begin());
        }
        size_ = value_count;
    }

    /**
    * Single-value initialized Constructor -
    * Gets a count - an non-negative integer (unsigned long) and a T Value.
    * This constructor initializes the vl_vector with 'count' times T Values as
    * vector members.
    * @param count number of times to insert the T Value to the vl_vector.
    * @param value T Value to insert 'count' times to the vl_vector.
    */
    vl_vector(size_t count, const T &value) : size_(INITIAL_SIZE),
                                              capacity_(StaticCapacity)
    {
        if (count > StaticCapacity)
        {
            capacity_ = (3 * count) / 2;
            dynamic_container_ = new T[capacity_];
            for (size_t i = 0; i < count; ++i)
            {
                dynamic_container_[i] = value;
            }
        }
        else
        {
            for (size_t i = 0; i < count; ++i)
            {
                static_container_[i] = value;
            }
        }
        size_ = count;
    }

    /**
  * Destructor for vl_vector Instance. Deletes the dynamic array iff the
  * vl_vector is currently using the Dynamic container of T Values -
  * e.g if the vl_vector's capacity is exceeding from the static capacity.
  */
    ~vl_vector()
    {
        if (capacity_ != StaticCapacity)
        {
            delete[] dynamic_container_;
        }
    }

    /**
     * @return Current size of vl_vector.
     */
    size_t size() const
    {
        return size_;
    }

    /**
     * @return Current capacity of vl_vector.
     */
    size_t capacity() const
    {
        return capacity_;
    }

    /**
     * @return true if vl_vector size is 0, false otherwise.
     */
    bool empty() const
    {
        return size_ == 0;
    }

    /**
     * Gets the data in a specific index in the vl_vector.
     * Throws an out_of_range exception if index is out of bounds of vl_vector.
     * @param index index to extract data from.
     * @return The data that is stored inside the vector in a specific provided
     * index.
     */
    T& at(size_t index)
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range(IDX_OUT_OF_RANGE_ERROR);
        }
        return data()[index];
    }

    /**
    * const version of at function.
    * Gets the data in a specific index in the vl_vector.
    * Throws an out_of_range exception if index is out of bounds of vl_vector.
    * @param index index to extract data from.
    * @return The data that is stored inside the vector in a specific provided
    * index.
    */
    const T& at(size_t index) const
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range(IDX_OUT_OF_RANGE_ERROR);
        }
        return data()[index];
    }

    /**
    * Adds an element with a given data (T Value)
    * to the end of the given vl_vector. Runtime is O(1) amortized.
    * @param value T type Value to add to the end of the vl_vector.
    */
    void push_back(const T &value)
    {
        size_t value_count = SINGLE_VALUE;
        if (capacity_ == StaticCapacity)
        {
            if (size_ + SINGLE_VALUE > capacity_) //switch to dynamic container
            {
                increase_capacity_static_to_dynamic(value_count);
            }
        }
        else
        {
            if (size_ + SINGLE_VALUE > capacity_)
            {
                increase_capacity_dynamic_to_dynamic(value_count);
            }
        }
        data()[size_++] = value;
    }

    /**
    * This method gets a const_iterator that point to a certain T value in the
    * vl_vector in place (position) and a new T Value. Inserts the value before
    * the given position and returns an iterator that points to the newly added
    * T Value.
    * @param position const_iterator that point to a certain T value in the
    * vl_vector.
    * @param value T type Value to add to vl_vector in the suitable position.
    * @return an iterator that points to the newly added T Value.
    */
    iterator insert(const_iterator position, const T &value)
    {
        size_t value_count = SINGLE_VALUE;
        size_t distance = position - begin();
        if (capacity_ == StaticCapacity)
        {
            if (size_ + SINGLE_VALUE > capacity_) //switch to dynamic container
            {
                increase_capacity_static_to_dynamic(value_count);
            }
        }
        else
        {
            if (size_ + SINGLE_VALUE > capacity_)
            {
                increase_capacity_dynamic_to_dynamic(value_count);
            }
        }
        std::copy_backward(begin() + distance, begin() + size_,
                           begin() + size_ + SINGLE_VALUE);
        data()[distance] = value;
        size_++;
        return begin() + distance;
    }

    /**
    * This method gets a const_iterator that point to a certain T value in the
    * vl_vector in place (position) and a 2 InputIterator that describes a
    * range of values from [first,last). Inserts the T values within the range
    * before the given position and returns an iterator that points to first
    * newly added T Value.
    * @param position const_iterator that point to a certain T value in the
    * vl_vector.
    * @param first first T Value to insert - beginning of the T values range.
    * @param last last T Value - all values from first till last - 1
    * will be inserted until 'last' value not including 'last'
    * @return an iterator that points to the first newly added T Value.
    */
    template<class InputIterator>
    iterator
    insert(const_iterator position, InputIterator first, InputIterator last)
    {
        size_t distance = position - begin();
        size_t tmp_idx = distance;
        size_t value_count = std::distance(first, last);
        if (capacity_ == StaticCapacity)
        {
            if (size_ + value_count > capacity_) // switch to dynamic container
            {
                increase_capacity_static_to_dynamic(value_count);
            }
        }
        else
        {
            if (size_ + value_count > capacity_)
            {
                increase_capacity_dynamic_to_dynamic(value_count);
            }
        }
        std::copy(begin() + distance, begin() + size_,
                  begin() + distance + value_count);
        size_ += value_count;
        while (first != last)
        {
            data()[tmp_idx++] = *first++;
        }
        return begin() + distance;
    }

    /**
    * Removes the last element from the vl_vector. Runtime is O(1) amortized.
    */
    void pop_back()
    {
        if (empty())
        {
            return;
        }
        --size_;
        if (capacity_ != StaticCapacity)
        {
            if (size_ <= StaticCapacity) // switch to static container
            {
                decrease_capacity_dynamic_to_static();
            }
        }
    }

    /**
   * This method gets a const_iterator that point to a certain T value in the
   * vl_vector in place (position) and removes it from the vl_vector.
   * Returns an iterator that points the T Value
   * that appears to the right of the removed T Value.
   * @param position const_iterator that point to a certain T value in the
   * vl_vector.
   * @return an iterator that points to a T Value
   * that appears to the right of the removed T Value.
   */
    iterator erase(const_iterator position)
    {
        size_t distance = position - begin();
        std::copy(begin() + distance + SINGLE_VALUE, begin() + size_,
                  begin() + distance);
        --size_;
        if (capacity_ != StaticCapacity)
        {
            if (size_ <= StaticCapacity) // switch to static container
            {
                decrease_capacity_dynamic_to_static();
            }
        }
        return begin() + distance;
    }

    /**
    * This method gets a const_iterator first that point to a certain T value
    * in the vl_vector and a const_iterator last and removes all the T Values
    * in the range [first,last) from the vl_vector.
    * Returns an iterator that points the T Value
    * that appears to the right of the removed T Values.
    * @param first first T Value to be erased -beginning of the T values range.
    * @param last last T Value - all values from first until last - 1
    * will be removed until 'last' value not including 'last'
    * @return an iterator that points to a T Value
    * that appears to the right of the removed T Values.
    */
    iterator erase(const_iterator first, const_iterator last)
    {
        size_t value_count = std::distance(first, last);
        size_t distance = first - begin();
        std::copy(begin() + distance + value_count, begin() + size_,
                  begin() + distance);
        size_ -= value_count;
        if (capacity_ != StaticCapacity)
        {
            if (size_ <= StaticCapacity) // switch to static container
            {
                decrease_capacity_dynamic_to_static();
            }
        }
        return begin() + distance;
    }

    /**
    * This method removes all T Values that are stored in the vl_vector and
    * initializes the vl_vector size to 0 as expected.
    * Runtime: O(n).
    */
    void clear()
    {
        if (capacity_ != StaticCapacity)
        {
            delete[] dynamic_container_;
        }
        size_ = 0;
        capacity_ = StaticCapacity;
    }

    /**
    * const version of data function.
    * @return The current data structure - dynamic/static container that
    * the data of the vector (T Values) are stored at the moment.
    */
    const_iterator data() const // const_iterator is defined as const T*
    {
        if (capacity_ == StaticCapacity)
        {
            return static_container_;
        }
        else
        {
            return dynamic_container_;
        }
    }

    /**
     * @return The current data structure - dynamic/static container that
     * the data of the vector (T Values) are stored at the moment.
     */
    iterator data() // iterator is defined as T* (Pointer to T)
    {
        if (capacity_ == StaticCapacity)
        {
            return static_container_;
        }
        else
        {
            return dynamic_container_;
        }
    }

    /**
    *
    * @param value value to check if is it contained in the vl_vector.
    * @return true if this vl_vector contains the T value false otherwise.
    */
    bool contains(const T &value) const
    {
        for (size_t i = 0; i < size_; ++i)
        {
            if (data()[i] == value)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Assignment operator for vl_vector instance.
     * Assigns other vl_vector to this vl_vector.
     * @param other_vl_vec The other vl_vector instance to assign to this.
     */
    vl_vector &operator=(const vl_vector &other_vl_vec)
    {
        if (this == &other_vl_vec)
        {
            return *this;
        }
        if (capacity_ != StaticCapacity)
        {
            delete[] dynamic_container_;
        }
        size_ = other_vl_vec.size_;
        capacity_ = other_vl_vec.capacity_;
        if (other_vl_vec.capacity_ != StaticCapacity)
        {
            dynamic_container_ = new T[capacity_];
        }
        for (size_t i = 0; i < other_vl_vec.size_; ++i)
        {
            data()[i] = other_vl_vec.data()[i];
        }
        return *this;
    }

    /**
     * Subscript operator [].
     * Gets the data in a specific index in the vl_vector.
     * Doesn't throw any exceptions.
     * @param index index to get T value from.
     * @return The T Value data that is
     * stored inside the vector in a specific provided index.
     */
    T& operator[](size_t index)
    {
        return data()[index];
    }

    /**
     * const version of Subscript operator [].
     * Gets the data in a specific index in the vl_vector.
     * Doesn't throw any exceptions.
     * @param index index to get T value from.
     * @return The T Value data that is
     * stored inside the vector in a specific provided index.
     */
    const T& operator[](size_t index) const
    {
        return data()[index];
    }

    /**
    * Operator== for vl_vector - checks if 2 vl_vectors are equal.
    * Two vectors are considered equal if they have exactly the same T values
    * in the exact indices.
    * @param other_vl_vec other vl_vector to compare with this.
    * @return true if this vl_vector and other vl_vector are equal to each
    * other ,false otherwise.
    */
    bool operator==(const vl_vector &other_vl_vec) const
    {
        if (size_ != other_vl_vec.size_)
        {
            return false;
        }
        for (size_t i = 0; i < size_; ++i)
        {
            if (data()[i] != other_vl_vec.data()[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
   * Operator!= for vl_vector - checks if 2 vl_vectors are unequal.
   * Two vectors are considered unequal if they differ in at least 1 T value
   * in the same index.
   * @param other_vl_vec other vl_vector to compare with this.
   * @return true if this vl_vector and other vl_vector are unequal to each
   * other ,false otherwise.
   */
    bool operator!=(const vl_vector &other_vl_vec) const
    {
        if (size_ != other_vl_vec.size_)
        {
            return true;
        }
        for (size_t i = 0; i < size_; ++i)
        {
            if (data()[i] != other_vl_vec.data()[i])
            {
                return true;
            }
        }
        return false;
    }
 private:
    size_t size_;
    size_t capacity_;
    T static_container_[StaticCapacity];
    T* dynamic_container_;

    /**
     * Helper function for copy constructor.
     * Copy values from other vl_vector to this vl_vector - if the data() is
     * dynamic it allocates new data according to the other vl_vector capacity.
     * @param other_vl_vec The other vl_vector to copy from.
     */
    void copy_vl_vector_values(const vl_vector &other_vl_vector)
    {
        if (other_vl_vector.capacity_ != StaticCapacity)
        {
            dynamic_container_= new T[other_vl_vector.capacity_];
        }
        for (size_t i = 0; i < other_vl_vector.size_; ++i)
        {
            data()[i] = other_vl_vector.data()[i];
        }
    }

    /**
    * Helper function.
    * Decrease capacity of vl_vector when alternating from dynamic to static.
    */
    void decrease_capacity_dynamic_to_static()
    {
        for (size_t i = 0; i < size_; ++i)
        {
            static_container_[i] = dynamic_container_[i];
        }
        capacity_ = StaticCapacity;
        delete[] dynamic_container_;
    }

    /**
    * Helper function.
    * Increase capacity of vl_vector from static capacity to dynamic capacity.
    * @param value_count number of T Values to add to the vl_vector.
    * e.g. - number of T Values to calculate the new capacity with.
    */
    void increase_capacity_static_to_dynamic(size_t value_count)
    {
        capacity_ = (3 * (size_ + value_count)) / 2;
        dynamic_container_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i)
        {
            dynamic_container_[i] = static_container_[i];
        }
    }

    /**
    * Helper function.
    * Increase capacity of vl_vector from dynamic to dynamic
    * (larger size dynamic vl_vector).
    * @param value_count number of T Values to add to the vl_vector.
    * e.g. - number of T Values to calculate the new capacity with.
    */
    void increase_capacity_dynamic_to_dynamic(size_t value_count)
    {
        capacity_ = (3 * (size_ + value_count)) / 2;
        T *new_dynamic_cont = new T[capacity_];
        for (size_t i = 0; i < size_; ++i)
        {
            new_dynamic_cont[i] = dynamic_container_[i];
        }
        delete[] dynamic_container_;
        dynamic_container_ = new_dynamic_cont;
    }
};


#endif //EX7_AMITHALBREICH_VL_VECTOR_H
