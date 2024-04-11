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

#ifndef LOKI_INCLUDE_LOKI_PDDL_SCOPE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_SCOPE_HPP_

#include "loki/ast/config.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/error_reporting.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <cassert>
#include <deque>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>

namespace loki
{

/// @brief Encapsulates binding related information of a type T.
///        The object is the entity bound to the name.
///        The position points to the matched location
///        in the input stream and is used for error reporting.
template<typename T>
using BindingValueType = std::tuple<T, std::optional<Position>>;

/// @brief Datastructure to store bindings of a type T.
template<typename T>
using Bindings = std::unordered_map<std::string, BindingValueType<T>>;

/// @brief Wraps bindings in a scope with reference to a parent scope.
class Scope
{
private:
    const Scope* m_parent_scope;

    Bindings<pddl::Type> m_types;
    Bindings<pddl::Object> m_objects;
    Bindings<pddl::FunctionSkeleton> m_function_skeletons;
    Bindings<pddl::Variable> m_variables;
    Bindings<pddl::Predicate> m_predicates;
    Bindings<pddl::Predicate> m_derived_predicates;

public:
    explicit Scope(const Scope* parent_scope = nullptr);

    // delete copy and move to avoid dangling references.
    Scope(const Scope& other) = delete;
    Scope& operator=(const Scope& other) = delete;
    Scope(Scope&& other) = delete;
    Scope& operator=(Scope&& other) = delete;

    /// @brief Return a binding if it exists.
    std::optional<BindingValueType<pddl::Type>> get_type(const std::string& name) const;
    std::optional<BindingValueType<pddl::Object>> get_object(const std::string& name) const;
    std::optional<BindingValueType<pddl::FunctionSkeleton>> get_function_skeleton(const std::string& name) const;
    std::optional<BindingValueType<pddl::Variable>> get_variable(const std::string& name) const;
    std::optional<BindingValueType<pddl::Predicate>> get_predicate(const std::string& name) const;
    std::optional<BindingValueType<pddl::Predicate>> get_derived_predicate(const std::string& name) const;

    /// @brief Insert a binding.
    void insert_type(const std::string& name, const pddl::Type& type, const std::optional<Position>& position);
    void insert_object(const std::string& name, const pddl::Object& object, const std::optional<Position>& position);
    void insert_function_skeleton(const std::string& name, const pddl::FunctionSkeleton& function_skeleton, const std::optional<Position>& position);
    void insert_variable(const std::string& name, const pddl::Variable& variable, const std::optional<Position>& position);
    void insert_predicate(const std::string& name, const pddl::Predicate& predicate, const std::optional<Position>& position);
    void insert_derived_predicate(const std::string& name, const pddl::Predicate& derived_predicate, const std::optional<Position>& position);
};

/// @brief Encapsulates the result of search for a binding with the corresponding ErrorHandler.
template<typename T>
using ScopeStackSearchResult = std::tuple<T, const std::optional<Position>, const PDDLErrorHandler&>;

/// @brief Implements a scoping mechanism to store bindings which are mappings from name to a pointer to a PDDL object
///        type and a position in the input stream that can be used to construct error messages with the given ErrorHandler.
///
///        We use ScopeStacks indicated with surrounding "[.]" to stack scopes indicated with surrounding "(.)" to store bindings.
///        We initialize the parsing step by creating a ScopeStack with a single scope that we call the global scope.
///        During parsing we can open and close new scopes but we must ensure that only the single global scope remains.
///        We can access bindings by calling the get method and the matching binding in the nearest scope is returned.
///        We can also insert new bindings in the current scope.
///
///        During domain file parsing, we have a single scope stack structured as follows:
///        [ (Domain Scope Global), (Domain Scope Child 1), (Domain Scope Child 2), ... ]
///
///        During problem file parsing, we get access to bindings from the domain through additional composition as follows:
///        [ (Domain Scope Global) ], [ (Problem Scope Global), (Problem Scope Child 1), (Problem Scope Child 2), ... ]
class ScopeStack
{
private:
    std::deque<std::unique_ptr<Scope>> m_stack;

    const PDDLErrorHandler& m_error_handler;

    const ScopeStack* m_parent;

public:
    ScopeStack(const PDDLErrorHandler& error_handler, const ScopeStack* parent = nullptr);

    // delete copy and move to avoid dangling references.
    ScopeStack(const ScopeStack& other) = delete;
    ScopeStack& operator=(const ScopeStack& other) = delete;
    ScopeStack(ScopeStack&& other) = delete;
    ScopeStack& operator=(ScopeStack&& other) = delete;

    /// @brief Inserts a new scope on the top of the stack.
    void open_scope();

    /// @brief Deletes the topmost scope from the stack.
    void close_scope();

    /// @brief Return a binding if it exists.
    std::optional<ScopeStackSearchResult<pddl::Type>> get_type(const std::string& name) const;
    std::optional<ScopeStackSearchResult<pddl::Object>> get_object(const std::string& name) const;
    std::optional<ScopeStackSearchResult<pddl::FunctionSkeleton>> get_function_skeleton(const std::string& name) const;
    std::optional<ScopeStackSearchResult<pddl::Variable>> get_variable(const std::string& name) const;
    std::optional<ScopeStackSearchResult<pddl::Predicate>> get_predicate(const std::string& name) const;
    std::optional<ScopeStackSearchResult<pddl::Predicate>> get_derived_predicate(const std::string& name) const;

    /// @brief Insert a binding.
    void insert_type(const std::string& name, const pddl::Type& type, const std::optional<Position>& position);
    void insert_object(const std::string& name, const pddl::Object& object, const std::optional<Position>& position);
    void insert_function_skeleton(const std::string& name, const pddl::FunctionSkeleton& function_skeleton, const std::optional<Position>& position);
    void insert_variable(const std::string& name, const pddl::Variable& variable, const std::optional<Position>& position);
    void insert_predicate(const std::string& name, const pddl::Predicate& predicate, const std::optional<Position>& position);
    void insert_derived_predicate(const std::string& name, const pddl::Predicate& derived_predicate, const std::optional<Position>& position);

    /// @brief Get the error handler to print an error message.
    const PDDLErrorHandler& get_error_handler() const;

    // For testing purposes only.
    const std::deque<std::unique_ptr<Scope>>& get_stack() const;
};

}

#endif
