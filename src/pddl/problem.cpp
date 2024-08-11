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

#include "loki/details/pddl/problem.hpp"

#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/numeric_fluent.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/visitors.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

#include <iostream>

using namespace std;

namespace loki
{
ProblemImpl::ProblemImpl(size_t identifier,
                         std::optional<fs::path> filepath,
                         Domain domain,
                         std::string name,
                         Requirements requirements,
                         ObjectList objects,
                         PredicateList derived_predicates,
                         LiteralList initial_literals,
                         NumericFluentList numeric_fluents,
                         std::optional<Condition> goal_condition,
                         std::optional<OptimizationMetric> optimization_metric,
                         AxiomList axioms) :
    Base(identifier),
    m_filepath(std::move(filepath)),
    m_domain(std::move(domain)),
    m_name(std::move(name)),
    m_requirements(std::move(requirements)),
    m_objects(std::move(objects)),
    m_derived_predicates(std::move(derived_predicates)),
    m_initial_literals(std::move(initial_literals)),
    m_numeric_fluents(std::move(numeric_fluents)),
    m_goal_condition(std::move(goal_condition)),
    m_optimization_metric(std::move(optimization_metric)),
    m_axioms(std::move(axioms))
{
}

bool ProblemImpl::is_structurally_equivalent_to_impl(const ProblemImpl& other) const
{
    if (this != &other)
    {
        return (m_domain == other.m_domain) && (m_name == other.m_name) && (m_requirements == other.m_requirements)
               && (get_sorted_vector(m_objects) == get_sorted_vector(other.m_objects))
               && (get_sorted_vector(m_initial_literals)) == get_sorted_vector(other.m_initial_literals) && (m_goal_condition == other.m_goal_condition)
               && (m_optimization_metric == other.m_optimization_metric)
               && (get_sorted_vector(m_derived_predicates) == get_sorted_vector(other.m_derived_predicates))
               && (get_sorted_vector(m_axioms) == get_sorted_vector(other.m_axioms));
    }
    return true;
}

size_t ProblemImpl::hash_impl() const
{
    size_t goal_hash = (m_goal_condition.has_value()) ? HashCombiner()(m_goal_condition.value()) : 0;
    size_t optimization_hash = (m_optimization_metric.has_value()) ? HashCombiner()(m_optimization_metric.value()) : 0;
    return HashCombiner()(m_domain,
                        m_name,
                        m_requirements,
                        get_sorted_vector(m_objects),
                        get_sorted_vector(m_initial_literals),
                        goal_hash,
                        optimization_hash,
                        get_sorted_vector(m_derived_predicates),
                        get_sorted_vector(m_axioms));
}

void ProblemImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << string(options.indent, ' ') << "(define (problem " << m_name << ")\n";
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    out << string(nested_options.indent, ' ') << "(:domain " << m_domain->get_name() << ")\n";
    if (!m_requirements->get_requirements().empty())
    {
        out << string(nested_options.indent, ' ');
        m_requirements->str(out, nested_options);
        out << "\n";
    }

    if (!m_objects.empty())
    {
        out << string(nested_options.indent, ' ') << "(:objects ";
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> objects_by_types;
        for (const auto& object : m_objects)
        {
            objects_by_types[object->get_bases()].push_back(object);
        }
        size_t i = 0;
        for (const auto& [types, objects] : objects_by_types)
        {
            if (i != 0)
                out << "\n" << string(nested_options.indent, ' ');
            for (size_t i = 0; i < objects.size(); ++i)
            {
                if (i != 0)
                {
                    out << " ";
                }
                out << objects[i]->get_name();
            }
            if (m_requirements->test(RequirementEnum::TYPING))
            {
                out << " - ";
                if (types.size() > 1)
                {
                    out << "(either ";
                    for (size_t i = 0; i < types.size(); ++i)
                    {
                        if (i != 0)
                            out << " ";
                        types[i]->get_name();
                    }
                    out << ")";
                }
                else if (types.size() == 1)
                {
                    out << types.front()->get_name();
                }
            }
            ++i;
        }
        out << ")\n";
    }

    if (!m_derived_predicates.empty())
    {
        out << string(nested_options.indent, ' ') << "(:derived-predicates ";
        for (size_t i = 0; i < m_derived_predicates.size(); ++i)
        {
            if (i != 0)
                out << " ";
            m_derived_predicates[i]->str(out, nested_options);
        }
        out << ")\n";
    }

    if (!(m_initial_literals.empty() && m_numeric_fluents.empty()))
    {
        out << string(nested_options.indent, ' ') << "(:init ";
        for (size_t i = 0; i < m_initial_literals.size(); ++i)
        {
            if (i != 0)
                out << " ";
            m_initial_literals[i]->str(out, nested_options);
        }
        for (size_t i = 0; i < m_numeric_fluents.size(); ++i)
        {
            out << " ";
            m_numeric_fluents[i]->str(out, nested_options);
        }
    }
    out << ")\n";

    if (m_goal_condition.has_value())
    {
        out << string(nested_options.indent, ' ') << "(:goal ";
        std::visit(StringifyVisitor(out, options), *m_goal_condition.value());
        out << ")\n";
    }

    if (m_optimization_metric.has_value())
    {
        out << string(nested_options.indent, ' ') << "(:metric ";
        m_optimization_metric.value()->str(out, nested_options);
        out << ")\n";
    }

    for (const auto& axiom : m_axioms)
    {
        axiom->str(out, nested_options);
    }

    out << string(options.indent, ' ') << ")";
}

std::ostream& operator<<(std::ostream& os, const ProblemImpl& problem)
{
    problem.str(os, FormattingOptions { 0, 4 });
    return os;
}

const std::optional<fs::path>& ProblemImpl::get_filepath() const { return m_filepath; }

const Domain& ProblemImpl::get_domain() const { return m_domain; }

const std::string& ProblemImpl::get_name() const { return m_name; }

const Requirements& ProblemImpl::get_requirements() const { return m_requirements; }

const ObjectList& ProblemImpl::get_objects() const { return m_objects; }

const PredicateList& ProblemImpl::get_derived_predicates() const { return m_derived_predicates; }

const LiteralList& ProblemImpl::get_initial_literals() const { return m_initial_literals; }

const NumericFluentList& ProblemImpl::get_numeric_fluents() const { return m_numeric_fluents; }

const std::optional<Condition>& ProblemImpl::get_goal_condition() const { return m_goal_condition; }

const std::optional<OptimizationMetric>& ProblemImpl::get_optimization_metric() const { return m_optimization_metric; }

const AxiomList& ProblemImpl::get_axioms() const { return m_axioms; }

}
