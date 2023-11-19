#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_ATOM_HPP_

#include "declarations.hpp"

#include "../../domain/pddl/declarations.hpp"


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundAtomImpl {
private:
    int m_identifier;
    Predicate m_predicate;
    ObjectList m_arguments;

    GroundAtomImpl(int identifier, const Predicate& predicate, const ObjectList& arguments);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool operator==(const GroundAtomImpl& other) const;
    bool operator!=(const GroundAtomImpl& other) const;

    bool operator<(const GroundAtomImpl& other) const;
    bool operator>(const GroundAtomImpl& other) const;

    size_t hash() const;

    const Predicate& get_predicate() const;
    const ObjectList& get_arguments() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundAtom>
    {
        bool operator()(const loki::pddl::GroundAtom& left_atom, const loki::pddl::GroundAtom& right_atom) const;
    };

    template<>
    struct hash<loki::pddl::GroundAtomImpl>
    {
        std::size_t operator()(const loki::pddl::GroundAtomImpl& type) const;
    };
}

#endif
