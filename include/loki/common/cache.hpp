#ifndef LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_

#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>


namespace loki {
/**
 * A thread-safe reference-counted object cache.
 * Idea taken from Herb Sutter: https://channel9.msdn.com/Events/GoingNative/2013/My-Favorite-Cpp-10-Liner
 * Other sources: (1) https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
 */
template<typename T>
class ReferenceCountedObjectCache : public std::enable_shared_from_this<ReferenceCountedObjectCache<T>> {
private:
    std::unordered_map<std::string, std::weak_ptr<T>> m_cache;

    /**
     * For multi-threading purposes
     */
    mutable std::mutex m_mutex;

public:
    ReferenceCountedObjectCache() { }

    /**
     * Inserts a new value and derives the key from it.
     */
    std::pair<std::shared_ptr<T>, bool> insert(std::unique_ptr<T>&& element) {
        const std::string key = compute_key(*element);
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        std::lock_guard<std::mutex> hold(m_mutex);
        auto& cached = m_cache[key];
        sp = cached.lock();
        bool new_insertion = false;
        if (!sp) {
            new_insertion = true;
            cached = sp = std::shared_ptr<T>(
                element.get(),
                [parent=this->shared_from_this(), original_deleter=element.get_deleter()](T* x)
                {
                    {
                        std::lock_guard<std::mutex> hold(parent->m_mutex);
                        parent->m_cache.erase(compute_key(*x));
                    }
                    /* After cache removal, we can call the objects destructor
                       and recursively call the deleter of children if their ref count goes to 0 */
                    original_deleter(x);
                }
            );
            element.release();
        }
        return std::make_pair(sp, new_insertion);
    }
};

}


#endif