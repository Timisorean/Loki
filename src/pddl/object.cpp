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

#include "loki/details/pddl/object.hpp"

#include "loki/details/pddl/type.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
ObjectImpl::ObjectImpl(size_t identifier, std::string name, TypeList types) : Base(identifier), m_name(std::move(name)), m_types(std::move(types)) {}

bool ObjectImpl::is_structurally_equivalent_to_impl(const ObjectImpl& other) const
{
    return (m_name == other.m_name) && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types));
}

size_t ObjectImpl::hash_impl() const { return hash_combine(m_name, hash_container(get_sorted_vector(m_types))); }

void ObjectImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << m_name;
    if (!m_types.empty())
    {
        out << " - ";
        if (m_types.size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < m_types.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                m_types[i]->str(out, options);
            }
            out << ")";
        }
        else if (m_types.size() == 1)
        {
            m_types.front()->str(out, options);
        }
    }
}

const std::string& ObjectImpl::get_name() const { return m_name; }

const TypeList& ObjectImpl::get_bases() const { return m_types; }

}
