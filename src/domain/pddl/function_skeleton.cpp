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

#include "../../../include/domain/pddl/function_skeleton.hpp"

#include "../../../include/domain/pddl/parameter.hpp"
#include "../../../include/common/hash.hpp"


namespace loki::pddl {
FunctionSkeletonImpl::FunctionSkeletonImpl(int identifier, std::string name, ParameterList parameters, Type type)
    : Base(identifier)
    , m_name(std::move(name))
    , m_parameters(std::move(parameters))
    , m_type(std::move(type))
{
}

bool FunctionSkeletonImpl::is_structurally_equivalent_to_impl(const FunctionSkeletonImpl& other) const {
    return (m_name == other.m_name) && (m_parameters == other.m_parameters) && (m_type == other.m_type);
}

size_t FunctionSkeletonImpl::hash_impl() const {
    return hash_combine(m_name, hash_container(m_parameters), m_type);
}

void FunctionSkeletonImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    str(out, options, true);
}

void FunctionSkeletonImpl::str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const {
    out << "(" << m_name;
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        out << " ";
        m_parameters[i]->str(out, options, typing_enabled);
    }
    out << ")";
}

const std::string& FunctionSkeletonImpl::get_name() const {
    return m_name;
}

const ParameterList& FunctionSkeletonImpl::get_parameters() const {
    return m_parameters;
}

const Type& FunctionSkeletonImpl::get_type() const {
    return m_type;
}

}

namespace std {
    bool less<loki::pddl::FunctionSkeleton>::operator()(
        const loki::pddl::FunctionSkeleton& left_function,
        const loki::pddl::FunctionSkeleton& right_function) const {
        return *left_function < *right_function;
    }

    std::size_t hash<loki::pddl::FunctionSkeletonImpl>::operator()(const loki::pddl::FunctionSkeletonImpl& function) const {
        return function.hash_impl();
    }

}
