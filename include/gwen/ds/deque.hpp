#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief リングバッファベースの高速な両端キュー
 * 
 * 内部容量を常に2の冪乗サイズに保ち、ビット演算を用いた高速なインデックス計算を行う。
 * `std::deque` よりもメモリ確保のオーバーヘッドが小さく、連続したメモリに近い特性を持つ。
 * 
 * @tparam T 要素の型
 * @tparam Allocator 要素のメモリ割り当てを行うアロケータ
 */
template <class T, class Allocator = std::allocator<T>>
class Deque {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = usize;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    using traits = std::allocator_traits<allocator_type>;
    using pointer = typename traits::pointer;
    using const_pointer = typename traits::const_pointer;

    /**
     * @brief ランダムアクセス可能なイテレータ
     * @tparam IsConst 定数イテレータかどうか
     */
    template <bool IsConst>
    class DequeIterator {
        using DequePtr = std::conditional_t<IsConst, const Deque*, Deque*>;
        DequePtr deque_;
        usize idx_;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        DequeIterator() : deque_(nullptr), idx_(0) {}
        DequeIterator(DequePtr deque, usize idx) : deque_(deque), idx_(idx) {}

        template <bool C = IsConst, std::enable_if_t<C, int> = 0>
        DequeIterator(const DequeIterator<false>& other)
            : deque_(other.deque_), idx_(other.idx_) {}

        reference operator*() const { return (*deque_)[idx_]; }
        pointer operator->() const { return &(*deque_)[idx_]; }

        DequeIterator& operator++() {
            ++idx_;
            return *this;
        }
        DequeIterator operator++(int) {
            auto copy = *this;
            ++idx_;
            return copy;
        }
        DequeIterator& operator--() {
            --idx_;
            return *this;
        }
        DequeIterator operator--(int) {
            auto copy = *this;
            --idx_;
            return copy;
        }

        DequeIterator& operator+=(difference_type n) {
            idx_ += n;
            return *this;
        }
        DequeIterator operator+(difference_type n) const {
            return DequeIterator(deque_, idx_ + n);
        }
        friend DequeIterator operator+(difference_type n, const DequeIterator& it) {
            return it + n;
        }
        DequeIterator& operator-=(difference_type n) {
            idx_ -= n;
            return *this;
        }
        DequeIterator operator-(difference_type n) const {
            return DequeIterator(deque_, idx_ - n);
        }

        difference_type operator-(const DequeIterator& other) const {
            return idx_ - other.idx_;
        }
        reference operator[](difference_type n) const { return (*deque_)[idx_ + n]; }

#if __cplusplus >= 202002L
        auto operator<=>(const DequeIterator& other) const = default;
#else
        bool operator==(const DequeIterator& other) const { return idx_ == other.idx_; }
        bool operator!=(const DequeIterator& other) const { return idx_ != other.idx_; }
        bool operator<(const DequeIterator& other) const { return idx_ < other.idx_; }
        bool operator>(const DequeIterator& other) const { return idx_ > other.idx_; }
        bool operator<=(const DequeIterator& other) const { return idx_ <= other.idx_; }
        bool operator>=(const DequeIterator& other) const { return idx_ >= other.idx_; }
#endif

        friend class DequeIterator<!IsConst>;
        friend class Deque;
    };

    using iterator = DequeIterator<false>;
    using const_iterator = DequeIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    allocator_type alloc_;
    T* data_;
    usize cap_;
    usize head_;
    usize size_;

    static constexpr usize min_capacity = 16;

    usize mask() const { return cap_ - 1; }

    static constexpr usize bit_ceil(usize n) {
        if (n <= 1) return 1;
        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        if constexpr (sizeof(usize) >= 8) n |= n >> 32;
        return n + 1;
    }

    void allocate_buffer(usize cap) {
        if (cap == 0) {
            data_ = nullptr;
            cap_ = 0;
        } else {
            data_ = traits::allocate(alloc_, cap);
            cap_ = cap;
        }
    }

    void deallocate_buffer() {
        if (data_) {
            traits::deallocate(alloc_, data_, cap_);
        }
    }

    void expand_if_needed() {
        if (size_ == cap_) {
            usize new_cap = (cap_ == 0) ? min_capacity : cap_ * 2;
            reallocate(new_cap);
        }
    }

    void reallocate(usize new_cap) {
        T* new_data = traits::allocate(alloc_, new_cap);
        for (usize i = 0; i < size_; ++i) {
            usize src = (head_ + i) & mask();
            traits::construct(alloc_, &new_data[i], std::move_if_noexcept(data_[src]));
            traits::destroy(alloc_, &data_[src]);
        }
        deallocate_buffer();
        data_ = new_data;
        cap_ = new_cap;
        head_ = 0;
    }

    void destroy_elements() {
        if (data_) {
            for (usize i = 0; i < size_; ++i) {
                traits::destroy(alloc_, &data_[(head_ + i) & mask()]);
            }
        }
    }

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    Deque() noexcept(noexcept(Allocator())) : data_(nullptr), cap_(0), head_(0), size_(0) {}

    /**
     * @brief アロケータを指定して初期化
     * @param alloc アロケータ
     */
    explicit Deque(const Allocator& alloc) noexcept
        : alloc_(alloc), data_(nullptr), cap_(0), head_(0), size_(0) {}

    /**
     * @brief 指定した要素数と初期値で初期化
     * @param count 要素数
     * @param value 初期値
     * @param alloc アロケータ
     */
    explicit Deque(usize count, const T& value = T(), const Allocator& alloc = Allocator())
        : alloc_(alloc), data_(nullptr), cap_(0), head_(0), size_(0) {
        if (count > 0) {
            usize new_cap = bit_ceil(std::max(count, min_capacity));
            allocate_buffer(new_cap);
            for (usize i = 0; i < count; ++i) {
                traits::construct(alloc_, &data_[i], value);
            }
            size_ = count;
        }
    }

    /**
     * @brief コピーコンストラクタ
     * @param other コピー元
     */
    Deque(const Deque& other)
        : alloc_(traits::select_on_container_copy_construction(other.alloc_)),
          data_(nullptr),
          cap_(0),
          head_(0),
          size_(0) {
        if (other.size_ > 0) {
            allocate_buffer(other.cap_);
            for (usize i = 0; i < other.size_; ++i) {
                traits::construct(alloc_, &data_[i], other[i]);
            }
            size_ = other.size_;
        }
    }

    /**
     * @brief ムーブコンストラクタ
     * @param other ムーブ元
     */
    Deque(Deque&& other) noexcept
        : alloc_(std::move(other.alloc_)),
          data_(other.data_),
          cap_(other.cap_),
          head_(other.head_),
          size_(other.size_) {
        other.data_ = nullptr;
        other.cap_ = 0;
        other.head_ = 0;
        other.size_ = 0;
    }

    /**
     * @brief コピー代入演算子
     * @param other コピー元
     * @return 自身の参照
     */
    Deque& operator=(const Deque& other) {
        if (this != &other) {
            clear();
            if constexpr (traits::propagate_on_container_copy_assignment::value) {
                if (alloc_ != other.alloc_) {
                    deallocate_buffer();
                    alloc_ = other.alloc_;
                    cap_ = 0;
                }
            }
            if (other.size_ > 0) {
                if (cap_ < other.size_) {
                    deallocate_buffer();
                    allocate_buffer(bit_ceil(std::max(other.size_, min_capacity)));
                }
                for (usize i = 0; i < other.size_; ++i) {
                    traits::construct(alloc_, &data_[i], other[i]);
                }
                head_ = 0;
                size_ = other.size_;
            }
        }
        return *this;
    }

    /**
     * @brief ムーブ代入演算子
     * @param other ムーブ元
     * @return 自身の参照
     */
    Deque& operator=(Deque&& other) noexcept(
        traits::propagate_on_container_move_assignment::value ||
        traits::is_always_equal::value) {
        if (this != &other) {
            clear();
            deallocate_buffer();
            if constexpr (traits::propagate_on_container_move_assignment::value) {
                alloc_ = std::move(other.alloc_);
            }
            data_ = other.data_;
            cap_ = other.cap_;
            head_ = other.head_;
            size_ = other.size_;

            other.data_ = nullptr;
            other.cap_ = 0;
            other.head_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief デストラクタ
     */
    ~Deque() {
        destroy_elements();
        deallocate_buffer();
    }

    /**
     * @brief アロケータを取得
     * @return allocator_type アロケータのコピー
     */
    allocator_type get_allocator() const noexcept { return alloc_; }

    /** 
     * @brief 要素へのアクセス
     * @param idx インデックス (0 <= idx < size())
     * @return reference 要素への参照
     */
    reference operator[](usize idx) {
        assert(idx < size_);
        return data_[(head_ + idx) & mask()];
    }

    /** 
     * @brief 要素への定数アクセス
     * @param idx インデックス (0 <= idx < size())
     * @return const_reference 要素への定数参照
     */
    const_reference operator[](usize idx) const {
        assert(idx < size_);
        return data_[(head_ + idx) & mask()];
    }

    /** 
     * @brief 最初の要素を取得
     * @return reference 先頭要素への参照
     */
    reference front() {
        assert(size_ > 0);
        return data_[head_];
    }

    /** 
     * @brief 最初の要素を定数で取得
     * @return const_reference 先頭要素への定数参照
     */
    const_reference front() const {
        assert(size_ > 0);
        return data_[head_];
    }

    /** 
     * @brief 最後の要素を取得
     * @return reference 末尾要素への参照
     */
    reference back() {
        assert(size_ > 0);
        return data_[(head_ + size_ - 1) & mask()];
    }

    /** 
     * @brief 最後の要素を定数で取得
     * @return const_reference 末尾要素への定数参照
     */
    const_reference back() const {
        assert(size_ > 0);
        return data_[(head_ + size_ - 1) & mask()];
    }

    /** 
     * @brief 先頭を指すイテレータ取得
     * @return iterator 先頭を指すイテレータ
     */
    iterator begin() { return iterator(this, 0); }
    const_iterator begin() const { return const_iterator(this, 0); }
    const_iterator cbegin() const { return const_iterator(this, 0); }

    /** 
     * @brief 末尾を指すイテレータ取得
     * @return iterator 末尾の次を指すイテレータ
     */
    iterator end() { return iterator(this, size_); }
    const_iterator end() const { return const_iterator(this, size_); }
    const_iterator cend() const { return const_iterator(this, size_); }

    /** 
     * @brief 末尾から逆向きに走査するリバースイテレータ取得
     * @return reverse_iterator 末尾の要素を指すリバースイテレータ
     */
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }

    /** 
     * @brief 先頭から逆向きに走査するリバースイテレータの終端取得
     * @return reverse_iterator 先頭の要素の次を指すリバースイテレータ
     */
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

    /** 
     * @brief 空かどうか
     * @return true 空の場合
     * @return false 要素がある場合
     */
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    /** 
     * @brief 要素数を取得
     * @return usize 現在の要素数
     */
    usize size() const noexcept { return size_; }

    /** 
     * @brief 現在確保されている容量を取得
     * @return usize 確保済みの最大要素数
     */
    usize capacity() const noexcept { return cap_; }

    /**
     * @brief 全要素を削除する
     */
    void clear() noexcept {
        destroy_elements();
        head_ = 0;
        size_ = 0;
    }

    /**
     * @brief 容量を確保する
     * @param new_cap 新しい最小容量
     */
    void reserve(usize new_cap) {
        if (new_cap > cap_) {
            reallocate(bit_ceil(std::max(new_cap, min_capacity)));
        }
    }

    /**
     * @brief 末尾に要素を追加する
     * @param value 追加する要素
     */
    void push_back(const T& value) {
        expand_if_needed();
        traits::construct(alloc_, &data_[(head_ + size_) & mask()], value);
        ++size_;
    }

    /**
     * @brief 末尾に要素をムーブして追加する
     * @param value 追加する要素
     */
    void push_back(T&& value) {
        expand_if_needed();
        traits::construct(alloc_, &data_[(head_ + size_) & mask()], std::move(value));
        ++size_;
    }

    /**
     * @brief 末尾に要素を直接構築する
     * @tparam Args 構築用引数の型
     * @param args 構築用引数
     */
    template <class... Args>
    void emplace_back(Args&&... args) {
        expand_if_needed();
        traits::construct(alloc_, &data_[(head_ + size_) & mask()], std::forward<Args>(args)...);
        ++size_;
    }

    /**
     * @brief 先頭に要素を追加する
     * @param value 追加する要素
     */
    void push_front(const T& value) {
        expand_if_needed();
        head_ = (head_ - 1) & mask();
        traits::construct(alloc_, &data_[head_], value);
        ++size_;
    }

    /**
     * @brief 先頭に要素をムーブして追加する
     * @param value 追加する要素
     */
    void push_front(T&& value) {
        expand_if_needed();
        head_ = (head_ - 1) & mask();
        traits::construct(alloc_, &data_[head_], std::move(value));
        ++size_;
    }

    /**
     * @brief 先頭に要素を直接構築する
     * @tparam Args 構築用引数の型
     * @param args 構築用引数
     */
    template <class... Args>
    void emplace_front(Args&&... args) {
        expand_if_needed();
        head_ = (head_ - 1) & mask();
        traits::construct(alloc_, &data_[head_], std::forward<Args>(args)...);
        ++size_;
    }

    /**
     * @brief 末尾の要素を削除する
     */
    void pop_back() {
        assert(size_ > 0);
        traits::destroy(alloc_, &data_[(head_ + size_ - 1) & mask()]);
        --size_;
    }

    /**
     * @brief 先頭の要素を削除する
     */
    void pop_front() {
        assert(size_ > 0);
        traits::destroy(alloc_, &data_[head_]);
        head_ = (head_ + 1) & mask();
        --size_;
    }
};

}  // namespace gwen
