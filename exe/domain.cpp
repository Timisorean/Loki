#include "domain.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "include/loki/common/config.hpp"
#include "include/loki/domain/error_handler.hpp"
#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/parser.hpp"


namespace loki::domain {

ast::DomainDescription parse(
    common::iterator_type& iter, common::iterator_type end,
    const std::string& filename) {

    // Our AST
    ast::DomainDescription ast;

    // Our parser
    using boost::spirit::x3::with;
    using common::error_handler_tag;
    common::error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler))
        [
            domain_description()
        ];

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    std::cout << "parse" << std::endl;
    bool success = phrase_parse(iter, end, parser, space, ast);
    std::cout << "success: " << success << std::endl;
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }
    if (iter != end) {
        std::cout << "Warning: Did not consume whole input." << std::endl;
    }

    return ast;
}

}
