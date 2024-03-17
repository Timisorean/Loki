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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"

#include <string>

namespace loki
{
template<typename HolderType, typename Hash, typename EqualTo>
class PDDLFactory;
}

namespace loki::pddl
{

// An alternative name is AtomSkeleton
class PredicateImpl : public Base<PredicateImpl>
{
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(int identifier, std::string name, ParameterList parameters);

    // Give access to the constructor.
    friend class loki::PDDLFactory<PredicateImpl, loki::Hash<PredicateImpl*>, loki::EqualTo<PredicateImpl*>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const PredicateImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<PredicateImpl>;

public:
    /// @brief Returns a parseable string representation in the context of a domain.
    void str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}

#endif
