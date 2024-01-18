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


#include <loki/problem/pddl/ground_function.hpp>

#include <loki/common/hash.hpp>
#include <loki/common/pddl/visitors.hpp>
#include <loki/domain/pddl/object.hpp>
#include <loki/domain/pddl/function_skeleton.hpp>


namespace loki::pddl {
GroundFunctionImpl::GroundFunctionImpl(int identifier, FunctionSkeleton function_skeleton, ObjectList objects)
    : Base(identifier)
    , m_function_skeleton(std::move(function_skeleton))
    , m_objects(std::move(objects))
{
}

bool GroundFunctionImpl::is_structurally_equivalent_to_impl(const GroundFunctionImpl& other) const {
    return (m_function_skeleton == other.m_function_skeleton) && (m_objects == other.m_objects);
}

size_t GroundFunctionImpl::hash_impl() const {
    return hash_combine(m_function_skeleton, hash_container(m_objects));
}

void GroundFunctionImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    if (m_objects.empty()) {
        out << "(" << m_function_skeleton->get_name() << ")";
    } else {
        out << "(" << m_function_skeleton->get_name() << "(";
        for (size_t i = 0; i < m_objects.size(); ++i) {
            if (i != 0) out << " ";
            out << *m_objects[i];
        }
        out << "))";
    }
}

const FunctionSkeleton& GroundFunctionImpl::get_function_skeleton() const {
    return m_function_skeleton;
}

const ObjectList& GroundFunctionImpl::get_objects() const {
    return m_objects;
}

}

namespace std {
    bool less<loki::pddl::GroundFunction>::operator()(
        const loki::pddl::GroundFunction& left_function,
        const loki::pddl::GroundFunction& right_function) const {
        return *left_function < *right_function;
    }

    std::size_t hash<loki::pddl::GroundFunctionImpl>::operator()(const loki::pddl::GroundFunctionImpl& function) const {
        return function.hash();
    }
}
