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

#include "loki/pddl/function.hpp"

#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/term.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/hash.hpp"

namespace loki
{
FunctionImpl::FunctionImpl(int identifier, FunctionSkeleton function_skeleton, TermList terms) :
    Base(identifier),
    m_function_skeleton(std::move(function_skeleton)),
    m_terms(std::move(terms))
{
}

bool FunctionImpl::is_structurally_equivalent_to_impl(const FunctionImpl& other) const
{
    return (m_function_skeleton == other.m_function_skeleton) && (m_terms == other.m_terms);
}

size_t FunctionImpl::hash_impl() const { return hash_combine(m_function_skeleton, hash_container(m_terms)); }

void FunctionImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    if (m_terms.empty())
    {
        out << "(" << m_function_skeleton->get_name() << ")";
    }
    else
    {
        out << "(" << m_function_skeleton->get_name() << "(";
        for (size_t i = 0; i < m_terms.size(); ++i)
        {
            if (i != 0)
                out << " ";
            std::visit(StringifyVisitor(out, options), *m_terms[i]);
        }
        out << "))";
    }
}

const FunctionSkeleton& FunctionImpl::get_function_skeleton() const { return m_function_skeleton; }

const TermList& FunctionImpl::get_terms() const { return m_terms; }

}
