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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <optional>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
class ProblemImpl : public Base<ProblemImpl> {
private:
    Domain m_domain;
    std::string m_name;
    Requirements m_requirements;
    ObjectList m_objects;
    GroundLiteralList m_initial_literals;
    NumericFluentList m_numeric_fluents;
    Condition m_goal_condition;
    std::optional<OptimizationMetric> m_optimization_metric;

    ProblemImpl(int identifier, Domain domain, std::string name, Requirements requirements, ObjectList objects, GroundLiteralList initial_literals, NumericFluentList numeric_fluents, Condition goal_condition, std::optional<OptimizationMetric> optimization_metric);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const ProblemImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ProblemImpl>;

public:
    const Domain& get_domain() const;
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const ObjectList& get_objects() const;
    const GroundLiteralList& get_initial_literals() const;
    const NumericFluentList& numeric_fluents() const;
    const Condition& get_goal_condition() const;
    const std::optional<OptimizationMetric>& get_optimization_metric() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Problem>
    {
        bool operator()(const loki::pddl::Problem& left_problem, const loki::pddl::Problem& right_problem) const;
    };

    template<>
    struct hash<loki::pddl::ProblemImpl>
    {
        std::size_t operator()(const loki::pddl::ProblemImpl& problem) const;
    };
}

#endif
