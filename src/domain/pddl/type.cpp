/*
 * Copyright (C) 2023 Simon Stahlberg
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


#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"


#include <memory>

namespace loki::pddl
{
    TypeImpl::TypeImpl(const std::string& name, const TypeList& bases)
        : name(name), bases(bases) { }

    Type create_type(const std::string& name, const TypeList& bases) {
        return std::make_shared<TypeImpl>(name, bases);
    }
}

