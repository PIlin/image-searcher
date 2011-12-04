
#pragma once

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

void conflicting_options(const bpo::variables_map& vm, 
                         const char* opt1, const char* opt2);

void option_dependency(const bpo::variables_map& vm,
                        const char* for_what, const char* required_option);