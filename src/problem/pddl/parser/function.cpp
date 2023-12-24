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

#include "goal.hpp"

#include "objects.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/functions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

pddl::Function parse(const problem::ast::BasicFunctionTerm& node, Context& context) {
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    pddl::TermList term_list;
    for (const auto& name_node : node.names) {
        term_list.push_back(context.factories.terms.get_or_create<pddl::TermObjectImpl>(parse_object_reference(name_node, context)));
    }
    if (function_skeleton->get_parameters().size() != term_list.size()) {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, term_list);
    context.positions.push_back(function, node);
    return function;
}

}
