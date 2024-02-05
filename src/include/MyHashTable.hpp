#include <iostream>
#include <cstring>
#include <cstdint>
#include <utility>
#include <nmmintrin.h>

const int INITIAL_SIZE = 10;

template<typename KeyType, typename ValueType>
class UnorderedMultimap {
public:
    struct Entry {
        KeyType key;
        ValueType value;
        bool occupied;

        Entry() : occupied(false) {}
    };

    Entry* table;
    uint64_t size;

    // CRC32哈希函数
    uint32_t hash(KeyType key) {
         return __builtin_ia32_crc32di(0, key);
    }

    UnorderedMultimap() : table(nullptr), size(0) {}

    ~UnorderedMultimap() {
        delete[] table;
    }

    void insert(KeyType key, ValueType value) {

        uint32_t index = hash(key) % size;

        while (table[index].occupied) {
            index = (index + 1) % size;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].occupied = true;
    }

    void reserve(int newCapacity) {
        if(newCapacity<10)newCapacity=10;
        if (newCapacity <= size)
            return;

        Entry* newTable = new Entry[newCapacity];
        for (int i = 0; i < size; ++i) {
            if (table[i].occupied) {
                uint32_t index = hash(table[i].key) % newCapacity;
                while (newTable[index].occupied) {
                    index = (index + 1) % newCapacity;
                }
                newTable[index] = table[i];
            }
        }
        delete[] table;
        table = newTable;
        size = newCapacity;
    }

    std::pair<uint32_t, uint32_t> equal_range(KeyType key) {
        uint32_t index = hash(key) % size;
        int probeCount = 0;

        while (table[index].occupied && table[index].key != key) {
            index = (index + 1) % size;
            ++probeCount;
            if (probeCount > size || !table[index].occupied) {
                // 无法找到键对应的值，返回迭代器范围，均指向nullptr
                return { -1,-1 };
            }
        }

        if (!table[index].occupied) {
            // 无法找到键对应的值，返回迭代器范围，均指向nullptr
            return { -1,-1};
        }

        auto first = index;

        while (table[(index + 1) % size].occupied && table[(index + 1) % size].key == key) {
            index = (index + 1) % size;
        }

        auto last = (index+1)%size;

        return {first,last};
    }

};
