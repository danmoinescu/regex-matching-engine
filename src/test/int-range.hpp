struct IntRange
{
    const int range_start, range_end;

    class iterator
    {
        public:
            // Iterator traits for STL compliance
            using iterator_category = std::forward_iterator_tag;
            using value_type = int;
            using difference_type = int;
            using pointer = int*;
            using reference = int&;

        private:
            int v;
        public:
            iterator(const int v): v(v) {}
            iterator& operator++() { ++v; return *this; }
            iterator& operator++(int) { ++v; return *this; }
            iterator& operator+=(int x) { v+=x; return *this; }
            iterator& operator-=(int x) { v-=x; return *this; }
            iterator operator+(int x) { return iterator(v+x); }
            iterator operator-(int x) { return iterator(v-x); }
            bool operator==(const iterator &o) { return v == o.v; }
            bool operator!=(const iterator &o) { return v != o.v; }
            const int& operator*() { return v; }
    };

    iterator begin() { return iterator(range_start); }
    iterator end() { return iterator(range_end); }
};
