#ifndef RHEA_UTIL_DOWNCAST_HPP
#define RHEA_UTIL_DOWNCAST_HPP

#include <memory>
#include <type_traits>
#include <utility>

namespace rhea { namespace util {
    /*
     * Static downcast of std::unique_ptr. This is an idiomatic method
     * for casting a unique_ptr to a base class into a unique_ptr of
     * a derived class. It's safe *if and only if* the pointer to base
     * actually does point to an instance of the derived class. Otherwise,
     * it's considered undefined behavior. We can't check this at compile
     * time, but we at least check to make sure the derived class is derived
     * from the base.
     * 
     * Note also that this doesn't handle custom deleters, but we don't use
     * those at any point where we would use this.
     */
    template <typename Base, typename Derived>
    std::unique_ptr<Derived> unique_ptr_downcast(std::unique_ptr<Base>& ptr)
    {
        static_assert(std::is_convertible<Derived*, Base*>::value,
            "Attempted to downcast a unique_ptr to a class that is not derived from Base");

        // First, we release the raw pointer from the original unique_ptr,
        // and we cast it into a pointer to the derived type.
        auto d { static_cast<Derived*>(ptr.release()) };

        // Then, we create a new unique_ptr to the derived class and return it
        return std::unique_ptr<Derived>(d);

    }
}}

#endif /* RHEA_UTIL_DOWNCAST_HPP */