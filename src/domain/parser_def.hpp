#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/error_handler.hpp"
#include "include/loki/domain/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace loki::domain::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;
    struct VariableClass;
    struct NumberClass;

    struct StripsRequirementClass;
    struct TypingRequirementClass;
    struct RequirementClass;

    struct FluentTypeClass;
    struct EitherTypeClass;
    struct TypeClass;
    struct TypedListOfNamesRecursivelyClass;
    struct TypedListOfNamesClass;
    struct TypedListOfVariablesRecursivelyClass;
    struct TypedListOfVariablesClass;

    struct AtomicFormulaSkeletonClass;

    struct FunctionSymbolClass;
    struct FunctionTypeClass;
    struct AtomicFunctionSkeletonClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass;

    struct DomainNameClass;
    struct RequirementsClass;
    struct TypesClass;
    struct ConstantsClass;
    struct PredicatesClass;
    struct FunctionsClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<NameClass, ast::Name> const
        name = "name";
    x3::rule<VariableClass, ast::Variable> const
        variable = "variable";
    x3::rule<NumberClass, ast::Number> const
        number = "number";

    x3::rule<StripsRequirementClass, ast::StripsRequirement> const
        strips_requirement = "strips_requirement";
    x3::rule<TypingRequirementClass, ast::TypingRequirement> const
        typing_requirement = "typing_requirement";
    x3::rule<RequirementClass, ast::Requirement> const
        requirement = "requirement";

    x3::rule<FluentTypeClass, ast::FluentType> const
        fluent_type = "fluent_type";
    x3::rule<EitherTypeClass, ast::EitherType> const
        either_type = "either_type";
    x3::rule<TypeClass, ast::Type> const
        type = "type";
    x3::rule<TypedListOfNamesRecursivelyClass, ast::TypedListOfNamesRecursively> const
        typed_list_of_names_recursively = "typed_list_of_names_recursively";
    x3::rule<TypedListOfNamesClass, ast::TypedListOfNames> const
        typed_list_of_names = "typed_list_of_names";

    x3::rule<TypedListOfVariablesRecursivelyClass, ast::TypedListOfVariablesRecursively> const
        typed_list_of_variables_recursively = "typed_list_of_variables_recursively";
    x3::rule<TypedListOfVariablesClass, ast::TypedListOfVariables> const
        typed_list_of_variables = "typed_list_of_variables";

    x3::rule<AtomicFormulaSkeletonClass, ast::AtomicFormulaSkeleton> const
        atomic_formula_skeleton = "atomic_formula_skeleton";

    x3::rule<FunctionSymbolClass, ast::FunctionSymbol> const
        function_symbol = "function_symbol";
    x3::rule<FunctionTypeClass, ast::FunctionType> const
        function_type = "function_type";
    x3::rule<AtomicFunctionSkeletonClass, ast::AtomicFunctionSkeleton> const
        atomic_function_skeleton = "atomic_function_skeleton";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass, ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively> const
        function_typed_list_of_atomic_function_skeletons_recursively = "function_typed_list_of_atomic_function_skeletons_recursively";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsClass, ast::FunctionTypedListOfAtomicFunctionSkeletons> const
        function_typed_list_of_atomic_function_skeletons = "function_typed_list_of_atomic_function_skeletons";

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";
    x3::rule<RequirementsClass, ast::Requirements> const
        requirements = "requirements";
    x3::rule<TypesClass, ast::Types> const
        types = "types";
    x3::rule<ConstantsClass, ast::Constants> const
        constants = "constants";
    x3::rule<PredicatesClass, ast::Predicates> const
        predicates = "predicates";
    x3::rule<FunctionsClass, ast::Functions> const
        functions = "functions";
    domain_description_type const domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];
    const auto variable_def = char_('?') > name;
    const auto number_def = int_;

    const auto strips_requirement_def = lit(":strips") >> x3::attr(ast::StripsRequirement{});
    const auto typing_requirement_def = lit(":typing") >> x3::attr(ast::TypingRequirement{});
    const auto requirement_def = strips_requirement | typing_requirement;

    const auto fluent_type_def = lit('(') >> lit("fluent") > type > lit(')');
    const auto either_type_def = lit('(') >> +type > lit(')');
    const auto type_def = name | fluent_type | either_type;
    const auto typed_list_of_names_recursively_def = +name > lit('-') > type >> typed_list_of_names_recursively;
    const auto typed_list_of_names_def = ((*name) | typed_list_of_names_recursively);
    const auto typed_list_of_variables_recursively_def = +variable > lit('-') > type >> typed_list_of_variables_recursively;
    const auto typed_list_of_variables_def = ((*variable) | typed_list_of_variables_recursively);

    const auto atomic_formula_skeleton_def = name > typed_list_of_variables;

    const auto function_symbol_def = name;
    const auto function_type_def = number;
    const auto atomic_function_skeleton_def = function_symbol > typed_list_of_variables;
    const auto function_typed_list_of_atomic_function_skeletons_recursively_def = +atomic_function_skeleton > lit('-') > function_type > function_typed_list_of_atomic_function_skeletons_recursively;
    const auto function_typed_list_of_atomic_function_skeletons_def = ((*atomic_function_skeleton_def) | function_typed_list_of_atomic_function_skeletons_recursively);

    const auto domain_name_def = lit('(') > lit("domain") > name > lit(')');
    const auto requirements_def = lit('(') >> lit(":requirements") >> *requirement >> lit(')');
    const auto types_def = lit('(') > lit(":types") >> typed_list_of_names > lit(')');
    const auto constants_def = lit('(') > lit(":constants") >> typed_list_of_names > lit(')');
    const auto predicates_def = lit('(') > lit(":predicates") >> *atomic_formula_skeleton > lit(')');
    const auto functions_def = lit('(') > lit(":functions") >> *function_typed_list_of_atomic_function_skeletons > lit(')');

    const auto domain_description_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirements
           > types
           > constants
           > predicates
           > functions
        > lit(')');

    BOOST_SPIRIT_DEFINE(
        name, variable, number,
        strips_requirement, typing_requirement, requirement, requirements,
        fluent_type, either_type, type, typed_list_of_names_recursively, typed_list_of_names, typed_list_of_variables_recursively, typed_list_of_variables,
        atomic_formula_skeleton,
        function_symbol, function_type, atomic_function_skeleton, function_typed_list_of_atomic_function_skeletons_recursively, function_typed_list_of_atomic_function_skeletons,
        domain_name, types, constants, predicates, functions, domain_description)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct VariableClass : x3::annotate_on_success {};
    struct NumberClass : x3::annotate_on_success {};

    struct StripsRequirementClass : x3::annotate_on_success {};
    struct TypingRequirementClass : x3::annotate_on_success {};
    struct RequirementClass : x3::annotate_on_success {};

    struct FluentTypeClass : x3::annotate_on_success {};
    struct EitherTypeClass : x3::annotate_on_success {};
    struct TypeClass : x3::annotate_on_success {};
    struct TypedListOfNamesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfNamesClass : x3::annotate_on_success {};
    struct TypedListOfVariablesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfVariablesClass : x3::annotate_on_success {};

    struct AtomicFormulaSkeletonClass : x3::annotate_on_success {};

    struct FunctionSymbolClass : x3::annotate_on_success {};
    struct FunctionTypeClass : x3::annotate_on_success {};
    struct AtomicFunctionSkeletonClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass : x3::annotate_on_success {};

    struct DomainNameClass : x3::annotate_on_success {};
    struct RequirementsClass : x3::annotate_on_success {};
    struct TypesClass : x3::annotate_on_success {};
    struct ConstantsClass : x3::annotate_on_success {};
    struct PredicatesClass : x3::annotate_on_success {};
    struct FunctionsClass : x3::annotate_on_success {};
    struct DomainDescriptionClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::domain_description_type const& domain_description() {
        return parser::domain_description;
    }
}

#endif