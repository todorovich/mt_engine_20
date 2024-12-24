export module PriorityQueue;

import std;
import Error;

using mt::error::ErrorCode;

namespace mt::queue {

    export template <size_t CAPACITY, typename Compare = std::less<int>>
    class PriorityQueue {
        size_t _size = size_t{ 0ull };
        int _data[CAPACITY]; 

        void siftUp(size_t index) {
            Compare compare;
            while (index > 0) {
                size_t parentIndex = (index - 1) / 2;
                if (!compare(_data[parentIndex], _data[index])) break;
                std::swap(_data[index], _data[parentIndex]);
                index = parentIndex;
            }
        }

        void siftDown(size_t index) { 
            Compare compare;
            size_t leftChild, rightChild, largest;
            while ((leftChild = 2 * index + 1) < _size) {
                rightChild = leftChild + 1;
                largest = (rightChild < _size && compare(_data[leftChild], _data[rightChild])) ? rightChild : leftChild;
                
                if (!compare(_data[index], _data[largest])) 
                    break;
                std::swap(_data[index], _data[largest]);
                index = largest;
            }
        }

    public:

        std::expected<void, std::error_condition> push(int value)
        {
            if (_size == CAPACITY) {
                return std::unexpected(MakeErrorCondition(ErrorCode::OUT_OF_MEMORY));
            }
            _data[_size] = value;
            siftUp(_size);
            ++_size;
            return {};
        }

        std::expected<void, std::error_condition> pop() 
        {
            if (_size == 0) {
                return std::unexpected(MakeErrorCondition(ErrorCode::NO_SUCH_ELEMENT));
            }
            std::swap(_data[0], _data[--_size]);
            siftDown(0);
            return {};
        }

        std::expected<int, std::error_condition> top() const 
        { 
            if (_size == 0) {
                return std::unexpected(MakeErrorCondition(ErrorCode::NO_SUCH_ELEMENT));
            }
            return _data[0];
        }

        constexpr size_t size() const {
            return _size;
        }

        constexpr bool empty() const {
            return _size == 0;
        }
    };

    // Type aliases for convenience
    export template <size_t CAPACITY>
    using MaxPriorityQueue = PriorityQueue<CAPACITY, std::less<int>>;

    export template <size_t CAPACITY>
    using MinPriorityQueue = PriorityQueue<CAPACITY, std::greater<int>>;
};
