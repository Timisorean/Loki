/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PARSER_HPP_

#include "loki/common/pddl/context.hpp"
#include "loki/problem/ast/ast.hpp"
#include "loki/problem/pddl/declarations.hpp"

namespace loki
{

extern pddl::Problem parse(const problem::ast::Problem& problem_node, Context& context, const pddl::Domain& domain);

}

#endif
