/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_INCLUDE_LOKI_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_ATOM_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/value_type_factory.hpp"

#include <string>

namespace loki
{
class AtomImpl : public Base<AtomImpl>
{
private:
    Predicate m_predicate;
    TermList m_terms;

    AtomImpl(size_t identifier, Predicate predicate, TermList terms);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<AtomImpl, Hash<const AtomImpl*, true>, EqualTo<const AtomImpl*, true>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const AtomImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<AtomImpl>;

public:
    const Predicate& get_predicate() const;
    const TermList& get_terms() const;
};

}

#endif
