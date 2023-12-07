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

#include "../../../include/loki/domain/pddl/effects.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
/* BaseCondition */
EffectImpl::EffectImpl(int identifier)
    : Base(identifier) { }

EffectImpl::~EffectImpl() = default;

/* Literal */
EffectLiteralImpl::EffectLiteralImpl(int identifier, const Literal& literal)
    : EffectImpl(identifier)
    , m_literal(literal) { }

bool EffectLiteralImpl::are_equal_impl(const EffectImpl& other) const {
    // https://stackoverflow.com/questions/11332075/comparing-polymorphic-base-types-in-c-without-rtti
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectLiteralImpl&>(other);
        return m_literal == other_derived.m_literal;
    }
    return false;
}

size_t EffectLiteralImpl::hash_impl() const {
    return std::hash<Literal>()(m_literal);
}

void EffectLiteralImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void EffectLiteralImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Literal& EffectLiteralImpl::get_literal() const {
    return m_literal;
}


EffectAndImpl::EffectAndImpl(int identifier, const EffectList& effects)
    : EffectImpl(identifier), m_effects(effects) { }

bool EffectAndImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectAndImpl&>(other);
        return get_sorted_vector(m_effects) == get_sorted_vector(other_derived.m_effects);
    }
    return false;
}

size_t EffectAndImpl::hash_impl() const {
    return hash_vector(get_sorted_vector(m_effects));
}

void EffectAndImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void EffectAndImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const EffectList& EffectAndImpl::get_effects() const {
    return m_effects;
}


EffectConditionalForallImpl::EffectConditionalForallImpl(int identifier, const ParameterList& parameters, const Effect& effect)
    : EffectImpl(identifier), m_parameters(parameters), m_effect(effect) { }

bool EffectConditionalForallImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectConditionalForallImpl&>(other);
        return m_parameters == other_derived.m_parameters
            && m_effect == other_derived.m_effect;
    }
    return false;
}

size_t EffectConditionalForallImpl::hash_impl() const {
    return hash_combine(hash_vector(m_parameters), m_effect);
}

void EffectConditionalForallImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void EffectConditionalForallImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const ParameterList& EffectConditionalForallImpl::get_parameters() const {
    return m_parameters;
}

const Effect& EffectConditionalForallImpl::get_effect() const {
    return m_effect;
}


EffectConditionalWhenImpl::EffectConditionalWhenImpl(int identifier, const Condition& condition, const Effect& effect)
    : EffectImpl(identifier), m_condition(condition), m_effect(effect) { }

bool EffectConditionalWhenImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectConditionalWhenImpl&>(other);
        return m_condition == other_derived.m_condition
            && m_effect == other_derived.m_effect;
    }
    return false;
}

size_t EffectConditionalWhenImpl::hash_impl() const {
    return hash_combine(m_condition, m_effect);
}

void EffectConditionalWhenImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void EffectConditionalWhenImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Condition& EffectConditionalWhenImpl::get_condition() const {
    return m_condition;
}

const Effect& EffectConditionalWhenImpl::get_effect() const {
    return m_effect;
}

}


namespace std {
    bool less<loki::pddl::Effect>::operator()(
        const loki::pddl::Effect& left_effect,
        const loki::pddl::Effect& right_effect) const {
        return *left_effect < *right_effect;
    }

    std::size_t hash<loki::pddl::EffectLiteralImpl>::operator()(const loki::pddl::EffectLiteralImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectAndImpl>::operator()(const loki::pddl::EffectAndImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectConditionalForallImpl>::operator()(const loki::pddl::EffectConditionalForallImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectConditionalWhenImpl>::operator()(const loki::pddl::EffectConditionalWhenImpl& effect) const {
        return effect.hash_impl();
    }
}
