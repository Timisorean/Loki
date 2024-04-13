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

#include "loki/details/pddl/atom.hpp"

#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/visitors.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
AtomImpl::AtomImpl(int identifier, Predicate predicate, TermList terms) : Base(identifier), m_predicate(std::move(predicate)), m_terms(std::move(terms)) {}

bool AtomImpl::is_structurally_equivalent_to_impl(const AtomImpl& other) const { return (m_predicate == other.m_predicate) && (m_terms == other.m_terms); }

size_t AtomImpl::hash_impl() const { return hash_combine(m_predicate, hash_container(m_terms)); }

void AtomImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << m_predicate->get_name();
    for (size_t i = 0; i < m_terms.size(); ++i)
    {
        out << " ";
        std::visit(StringifyVisitor(out, options), *m_terms[i]);
    }
    out << ")";
}

const Predicate& AtomImpl::get_predicate() const { return m_predicate; }

const TermList& AtomImpl::get_terms() const { return m_terms; }

}
