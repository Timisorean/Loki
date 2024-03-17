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

#include "loki/pddl/ground_atom.hpp"

#include "loki/pddl/object.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
GroundAtomImpl::GroundAtomImpl(int identifier, Predicate predicate, ObjectList objects) :
    Base(identifier),
    m_predicate(std::move(predicate)),
    m_objects(std::move(objects))
{
}

bool GroundAtomImpl::is_structurally_equivalent_to_impl(const GroundAtomImpl& other) const
{
    return (m_predicate == other.m_predicate) && (m_objects == other.m_objects);
}

size_t GroundAtomImpl::hash_impl() const { return hash_combine(m_predicate, hash_container(m_objects)); }

void GroundAtomImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const
{
    out << "(" << m_predicate->get_name();
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        out << " " << *m_objects[i];
    }
    out << ")";
}

const Predicate& GroundAtomImpl::get_predicate() const { return m_predicate; }

const ObjectList& GroundAtomImpl::get_objects() const { return m_objects; }

}
