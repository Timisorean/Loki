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

#include "loki/details/pddl/ground_literal.hpp"

#include "loki/details/pddl/ground_atom.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
GroundLiteralImpl::GroundLiteralImpl(int identifier, bool is_negated, GroundAtom atom) : Base(identifier), m_is_negated(is_negated), m_atom(std::move(atom)) {}

bool GroundLiteralImpl::is_structurally_equivalent_to_impl(const GroundLiteralImpl& other) const
{
    return (m_is_negated == other.m_is_negated) && (m_atom == other.m_atom);
}

size_t GroundLiteralImpl::hash_impl() const { return hash_combine(m_is_negated, m_atom); }

void GroundLiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    if (m_is_negated)
    {
        out << "(not ";
        m_atom->str(out, options);
        out << ")";
    }
    else
    {
        m_atom->str(out, options);
    }
}

bool GroundLiteralImpl::is_negated() const { return m_is_negated; }

const GroundAtom& GroundLiteralImpl::get_atom() const { return m_atom; }

}
