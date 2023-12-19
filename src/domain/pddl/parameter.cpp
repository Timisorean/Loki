/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "../../../include/loki/domain/pddl/parameter.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/domain/pddl/variable.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"

#include <cassert>


namespace loki::pddl {
ParameterImpl::ParameterImpl(int identifier, pddl::Variable variable, TypeList types)
    : Base(identifier)
    , m_variable(std::move(variable))
    , m_types(std::move(types))
{
}

bool ParameterImpl::are_equal_impl(const ParameterImpl& other) const {
    return (m_variable == other.m_variable) && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types));
}

size_t ParameterImpl::hash_impl() const {
    return hash_combine(m_variable, hash_vector(get_sorted_vector(m_types)));
}

void ParameterImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    str(out, options, true);
}

void ParameterImpl::str(std::ostringstream& out, const FormattingOptions& /*options*/, bool typing_enabled) const {
    out << *m_variable;
    if (typing_enabled) {
        assert(!m_types.empty());
        out << " - ";
        if (m_types.size() > 1) {
            out << "(either ";
            for (size_t i = 0; i < m_types.size(); ++i) {
                if (i != 0) out << " ";
                out << *m_types[i];
            }
            out << ")";
        } else if (m_types.size() == 1) {
            out << *m_types.front();
        }
    }
}

const Variable& ParameterImpl::get_variable() const {
    return m_variable;
}

const TypeList& ParameterImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    bool less<loki::pddl::Parameter>::operator()(
        const loki::pddl::Parameter& left_parameter,
        const loki::pddl::Parameter& right_parameter) const {
        return *left_parameter < *right_parameter;
    }

    std::size_t hash<loki::pddl::ParameterImpl>::operator()(const loki::pddl::ParameterImpl& parameter) const {
        return parameter.hash_impl();
    }
}
