// File:   args.hpp Author: Vader Created on 28 August 2019, 01:33

#pragma once

// pro
#include <libv/arg/parser.hpp>


namespace libv {
namespace arg {

// -------------------------------------------------------------------------------------------------

//	// Argument parsing
//
//	- Missing: validator func(const T&):<bool, string>
//	- Missing: commands / sub commands / sub-sub commands
//	- Missing: allow / disable override of arguments
//
//	auto args = libv::arg::Parser("Program Name", "Program Description");
//
//	const auto input_file = args.required<std::string>
//			("input_file", "File path of the binary input file")
//			["-i"]["--input"];
//
//	const auto output_file = args.optional<std::string>
//			("output_file", "File path of the generated C++ output")
//			["-o"]["--output"]
//			= input_file;
//
//	const auto function_name = args.optional<std::string>
//			("-f", "--function")
//			("function_name", "Name of the generated function")
//			= input_file;
//
//	const auto indexes = args.multiple<std::string>(min_count, max_count)
//			("-i", "--index")
//			("indexes_name", "Indexes of something");
//
//	const auto flag = args.flag
//			("-f", "--flag")
//			("flag", "Flag for something")
//			= false;
//
//	const auto rest = args.rest
//			("--");
//
//	args.positional(input_file, output_file, function_name, indexes);
//
//	args.require(output_file && function_name || flag);
//	args.require_no_unused();
//
//	if (args.parse(argc, argv)) {
//		std::err << args.error_message(100) << args.usage(100);
//		std::exit(EXIT_FAILURE);
//	}
//
//	std::cout << args.report(100);
//
//	// API:
//
// S - skeleton done, F - functionality done, D - usage reporting done,  E - error reporting done
//	S	args.flag               (name, aliases...)(description)? = value|args
//	S	args.optional<T>        (name                      )[flag]+(description)? = value|args
//	S	args.require<T>         (name                      )[flag]+(description)?
//		args.multiple
//			args.multiple<T>        (name, min_count, max_count)[flag]+(description)? = value|args
//			args.alternate<K, V>    (name                      )[flag]+(description)? = values...|args...
//			args.composite<T...>    (name                      )[flag]+(description)? = values...|args...
//		args.command            (...)
//		args.sub_command        (...)
//	S	args.positional         (args...)
//		args.group              (name, description) : grouped_args_proxy // group argument for 'help' and 'usage'
//		args.mergable // example: 'ls -la' where -l and -a are grouped
//			args.mergable_1         (args...)                 // usage: args.mergable(arg_list, arg_all);                // uses aliases that are 1 letter or 2 with common first letter
//			args.mergable_2         (start)(alias, args)...   // usage: args.mergable('-')('l', arg_list)('a', arg_all); // explicit
//			args.mergable_3         (start, args...)          // usage: args.mergable('-', arg_list, arg_all);           // uses aliases that are
//	?	args.rest               (aliases...)
//	S	args.unused             ()
//		args.require            (arg0 && arg1 || arg2)     // Marks certain combination of arguments as required
//		args.deprecated         (arg0 && arg1 || arg2)     // Marks certain combination of arguments as deprecated
//		args.reserve            (arg0)                     // Marks a certain argument as reserved for compatibility, reserved arguments are "silently" "ignored"
//		args.require            (bool, "reason")
//		args.warning            (bool, "reason")
//		args.deprecated         (bool, "reason")
//	SF	args.require_no_unused  ()                         // Needs better naming
//		args.require_no_deprecated()                       // Needs better naming
//		args.warning_no_unused  ()                         // Needs better naming
//		args.warning_no_deprecated()                       // Needs better naming
//	S	args.parse              (argc, argv)
//		args.parse              (line)
//		args.exe                ()
//		args.exe_dir            ()
//		args.error_message      (width)                    // Generates the error report
//		args.error_message      (os, width)                // Generates the error report
//		args.debug              (width)                    // Provides information which index set via which flag, default values, includes overrides, original input, parsed value
//		args.debug              (os, width)                    // Provides information which index set via which flag, default values, includes overrides, original input, parsed value
//	S	args.report             (width)
//	S	args.report             (os, width)
//	S	args.usage              (width)
//	S	args.usage              (os, width)

// -------------------------------------------------------------------------------------------------

} // namespace arg
} // namespace libv
