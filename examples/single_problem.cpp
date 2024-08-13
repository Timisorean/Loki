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

#include <iostream>
#include <loki/loki.hpp>

int main()
{
    // Parse the domain
    auto domain_parser = loki::DomainParser(std::string(DATA_DIR) + "gripper/domain.pddl");
    const auto domain = domain_parser.get_domain();
    std::cout << loki::StreamWriter(*domain) << std::endl;

    // Parse the problem
    const auto problem_parser = loki::ProblemParser(std::string(DATA_DIR) + "gripper/p-2-0.pddl", domain_parser);
    const auto problem = problem_parser.get_problem();
    std::cout << loki::StreamWriter(*problem) << std::endl;

    return 0;
}
