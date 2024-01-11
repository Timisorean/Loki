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

#include <loki/domain/pddl/conditions.hpp>

#include <loki/domain/pddl/literal.hpp>
#include <loki/domain/pddl/parameter.hpp>
#include <loki/common/hash.hpp>
#include <loki/common/collections.hpp>
#include <loki/common/pddl/visitors.hpp>


namespace loki::pddl {

/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(int identifier, Literal literal)
    : Base(identifier)
    , m_literal(std::move(literal)) { }

bool ConditionLiteralImpl::is_structurally_equivalent_to_impl(const ConditionLiteralImpl& other) const {
    if (this != &other) {
        return m_literal == other.m_literal;
    }
    return true;
}

size_t ConditionLiteralImpl::hash_impl() const {
    return std::hash<Literal>()(m_literal);
}

void ConditionLiteralImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_literal;
}

const Literal& ConditionLiteralImpl::get_literal() const {
    return m_literal;
}


/* And */
ConditionAndImpl::ConditionAndImpl(int identifier, ConditionList conditions)
    : Base(identifier)
    , m_conditions(std::move(conditions)) { }

bool ConditionAndImpl::is_structurally_equivalent_to_impl(const ConditionAndImpl& other) const {
    if (this != &other) {
        return get_sorted_vector(m_conditions) == get_sorted_vector(other.m_conditions);
    }
    return true;
}

size_t ConditionAndImpl::hash_impl() const {
    return hash_container(get_sorted_vector(m_conditions));
}

void ConditionAndImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(and ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const ConditionList& ConditionAndImpl::get_conditions() const {
    return m_conditions;
}


/* Or */
ConditionOrImpl::ConditionOrImpl(int identifier, ConditionList conditions)
    : Base(identifier)
    , m_conditions(std::move(conditions)) { }

bool ConditionOrImpl::is_structurally_equivalent_to_impl(const ConditionOrImpl& other) const {
    if (this != &other) {
        return get_sorted_vector(m_conditions) == get_sorted_vector(other.m_conditions);
    }
    return true;
}

size_t ConditionOrImpl::hash_impl() const {
    return hash_container(get_sorted_vector(m_conditions));
}

void ConditionOrImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(or ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const ConditionList& ConditionOrImpl::get_conditions() const {
    return m_conditions;
}


/* Not */
ConditionNotImpl::ConditionNotImpl(int identifier, Condition condition)
    : Base(identifier)
    , m_condition(std::move(condition)) { }

bool ConditionNotImpl::is_structurally_equivalent_to_impl(const ConditionNotImpl& other) const {
    if (this != &other) {
        return m_condition == other.m_condition;
    }
    return true;
}

size_t ConditionNotImpl::hash_impl() const {
    return hash_combine(m_condition);
}

void ConditionNotImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(not ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const Condition& ConditionNotImpl::get_condition() const {
    return m_condition;
}


/* Imply */
ConditionImplyImpl::ConditionImplyImpl(int identifier, Condition condition_left, Condition condition_right)
    : Base(identifier), m_condition_left(std::move(condition_left)), m_condition_right(std::move(condition_right)) { }

bool ConditionImplyImpl::is_structurally_equivalent_to_impl(const ConditionImplyImpl& other) const {
    if (this != &other) {
        return (m_condition_left == other.m_condition_left)
            && (m_condition_right == other.m_condition_right);
    }
    return true;
}

size_t ConditionImplyImpl::hash_impl() const {
    return hash_combine(m_condition_left, m_condition_right);
}

void ConditionImplyImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(imply ";
    std::visit(StringifyVisitor(out, options), *m_condition_left);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_condition_right);
    out << ")";
}

const Condition& ConditionImplyImpl::get_condition_left() const {
    return m_condition_left;
}

const Condition& ConditionImplyImpl::get_condition_right() const {
    return m_condition_right;
}


/* Exists */
ConditionExistsImpl::ConditionExistsImpl(int identifier, ParameterList parameters, Condition condition)
    : Base(identifier), m_parameters(std::move(parameters)), m_condition(std::move(condition)) { }

bool ConditionExistsImpl::is_structurally_equivalent_to_impl(const ConditionExistsImpl& other) const {
    if (this != &other) {
        return (m_parameters == other.m_parameters)
            && (m_condition == other.m_condition);
    }
    return true;
}

size_t ConditionExistsImpl::hash_impl() const {
    return hash_combine(hash_container(m_parameters), m_condition);
}

void ConditionExistsImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(exists (";
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_parameters[i];
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& ConditionExistsImpl::get_parameters() const {
    return m_parameters;
}

const Condition& ConditionExistsImpl::get_condition() const {
    return m_condition;
}


/* Forall */
ConditionForallImpl::ConditionForallImpl(int identifier, ParameterList parameters, Condition condition)
    : Base(identifier), m_parameters(std::move(parameters)), m_condition(std::move(condition)) { }

bool ConditionForallImpl::is_structurally_equivalent_to_impl(const ConditionForallImpl& other) const {
    if (this != &other) {
        return (m_parameters == other.m_parameters)
            && (m_condition == other.m_condition);
    }
    return true;
}

size_t ConditionForallImpl::hash_impl() const {
    return hash_combine(hash_container(m_parameters), m_condition);
}

void ConditionForallImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(forall (";
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_parameters[i];
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& ConditionForallImpl::get_parameters() const {
    return m_parameters;
}

const Condition& ConditionForallImpl::get_condition() const {
    return m_condition;
}

}


namespace std {
    bool less<loki::pddl::Condition>::operator()(
        const loki::pddl::Condition& left_condition,
        const loki::pddl::Condition& right_condition) const {
        return std::visit(loki::pddl::LessComparatorVisitor(), *left_condition, *right_condition);
    }

    std::size_t hash<loki::pddl::ConditionLiteralImpl>::operator()(const loki::pddl::ConditionLiteralImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionAndImpl>::operator()(const loki::pddl::ConditionAndImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionOrImpl>::operator()(const loki::pddl::ConditionOrImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionNotImpl>::operator()(const loki::pddl::ConditionNotImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionImplyImpl>::operator()(const loki::pddl::ConditionImplyImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionExistsImpl>::operator()(const loki::pddl::ConditionExistsImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionForallImpl>::operator()(const loki::pddl::ConditionForallImpl& condition) const {
        return condition.hash_impl();
    }
}
