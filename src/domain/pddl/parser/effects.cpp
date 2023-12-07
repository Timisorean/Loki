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

#include "effects.hpp"

#include "conditions.hpp"
#include "literal.hpp"
#include "parameters.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorAssign& node, domain::Context& context) {
    return pddl::AssignOperatorEnum::ASSIGN;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleUp& node, domain::Context& context) {
    return pddl::AssignOperatorEnum::SCALE_UP;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleDown& node, domain::Context& context) {
    return pddl::AssignOperatorEnum::SCALE_DOWN;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorIncrease& node, domain::Context& context) {
    return pddl::AssignOperatorEnum::INCREASE;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorDecrease& node, domain::Context& context) {
    return pddl::AssignOperatorEnum::DECREASE;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperator& node, domain::Context& context) {
    return boost::apply_visitor(AssignOperatorVisitor(context), node);
}


AssignOperatorVisitor::AssignOperatorVisitor(domain::Context& context_)
    : context(context_) { }


pddl::Effect parse(const domain::ast::Effect& node, domain::Context& context) {
    return boost::apply_visitor(EffectVisitor(context), node);
}

pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, domain::Context& context) {
    auto literal = parse(node.literal, context);
    return context.cache.get_or_create<pddl::EffectLiteralImpl>(literal);
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionNumericFluent");
}

pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, domain::Context& context) {
    context.error_handler(node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, context.error_stream->str());
}

pddl::Effect parse(const domain::ast::EffectProduction& node, domain::Context& context) {
    return boost::apply_visitor(EffectProductionVisitor(context), node);
}

pddl::Effect parse(const domain::ast::EffectConditionalForall& node, domain::Context& context) {
    context.open_scope();
    pddl::ParameterList parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    pddl::Effect effect = parse(node.effect, context);
    context.close_scope();
    return context.cache.get_or_create<pddl::EffectConditionalForallImpl>(parameter_list, effect);
}

pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, domain::Context& context) {
    context.open_scope();
    pddl::Condition condition = parse(node.goal_descriptor, context);
    pddl::Effect effect = parse(node.effect, context);
    context.close_scope();
    return context.cache.get_or_create<pddl::EffectConditionalWhenImpl>(condition, effect);
}

pddl::Effect parse(const domain::ast::EffectConditional& node, domain::Context& context) {
    // requires :conditional-effects
    if (!context.requirements->test(pddl::RequirementEnum::CONDITIONAL_EFFECTS)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::CONDITIONAL_EFFECTS, context.error_stream->str());
    }
    return boost::apply_visitor(EffectConditionalVisitor(context), node);
}


EffectProductionVisitor::EffectProductionVisitor(domain::Context& context_)
    : context(context_) { }


EffectConditionalVisitor::EffectConditionalVisitor(domain::Context& context_)
    : context(context_) { }


EffectVisitor::EffectVisitor(domain::Context& context_)
    : context(context_) { }

pddl::Effect EffectVisitor::operator()(const std::vector<domain::ast::Effect>& effect_nodes) const {
    pddl::EffectList effect_list;
    for (const auto& effect_node : effect_nodes) {
        effect_list.push_back(parse(effect_node, context));
    }
    return context.cache.get_or_create<pddl::EffectAndImpl>(effect_list);
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectConditional& effect_node) const {
    return boost::apply_visitor(EffectConditionalVisitor(context), effect_node);
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectProduction& effect_node) const {
    return boost::apply_visitor(EffectProductionVisitor(context), effect_node);
}

}