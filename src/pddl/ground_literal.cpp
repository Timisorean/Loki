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

#include "loki/pddl/ground_literal.hpp"

#include "loki/pddl/ground_atom.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
GroundLiteralImpl::GroundLiteralImpl(int identifier, bool is_negated, GroundAtom atom) : Base(identifier), m_is_negated(is_negated), m_atom(std::move(atom)) {}

bool GroundLiteralImpl::is_structurally_equivalent_to_impl(const GroundLiteralImpl& other) const
{
    return (m_is_negated == other.m_is_negated) && (m_atom == other.m_atom);
}

size_t GroundLiteralImpl::hash_impl() const { return hash_combine(m_is_negated, m_atom); }

void GroundLiteralImpl::str(std::ostream& out, const FormattingOptions& options, bool typing_enabled) const
{
    if (m_is_negated)
    {
        out << "(not ";
        m_atom->str(out, options, typing_enabled);
        out << ")";
    }
    else
    {
        m_atom->str(out, options, typing_enabled);
    }
}

bool GroundLiteralImpl::is_negated() const { return m_is_negated; }

const GroundAtom& GroundLiteralImpl::get_atom() const { return m_atom; }

}
