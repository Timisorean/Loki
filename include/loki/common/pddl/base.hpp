#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_

#include <memory>

namespace loki::pddl {

/// @brief Implements a common base class for PDDL objects.
///
///        Each PDDL object has an identifier.
///        Identifiers are used to describe and detect semantically equivalent PDDL object.
///        Detecting semantic equivalence is important for
///            - reducing the required memory by detecting duplicates
///            - reducing comparison and hashing to pointer level instead of traversing the whole structure
///            - caching evaluation results
///        For the following type of PDDL objects, loki detects semantic equivalence:
///            * Type
///            * Object
///            * Atom
///            * Literal
///            * Parameter
///            * Function
///            * GroundAtom
///            * GroundLiteral
///            * Requirements
///        For the remaining type of PDDL objects, loki approximates semantic equivalence
///        with structural equivalence where collections of objects are sorted by the identifier.
///        For example, loki detects semantic equivalence of a conjunction of atoms
///        but loki does not detect semantic equivalence of an arbitrary formula of atoms.
class Base {
protected:
    int m_identifier;

    Base(int identifier);
    virtual ~Base() = 0;

public:
    virtual bool operator<(const Base& other) const;
    virtual bool operator>(const Base& other) const;
};

}

#endif
