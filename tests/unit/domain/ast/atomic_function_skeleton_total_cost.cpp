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

#include "../../../../src/domain/ast/parser.hpp"

#include <loki/common/ast/parser_wrapper.hpp>
#include <loki/domain/ast/ast.hpp>
#include <loki/domain/ast/printer.hpp>

#include <gtest/gtest.h>


namespace loki::domain::tests {

TEST(LokiTests, AtomicFunctionSkeletonTotalCostTest) {
    ast::AtomicFunctionSkeletonTotalCost ast;

    EXPECT_NO_THROW(parse_ast("(total-cost)", atomic_function_skeleton_total_cost(), ast));
    EXPECT_EQ(parse_text(ast), "(total-cost)");

    EXPECT_ANY_THROW(parse_ast("(loki)", atomic_function_skeleton_total_cost(), ast));
}

}
