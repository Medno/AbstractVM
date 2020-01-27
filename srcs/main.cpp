#include "Options.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Execution.hpp"
#include "Operand.hpp"
#include "OperandFactory.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

std::string	readStdin( int opt ) {
	std::string			line;
	std::stringstream	ss;

	do {
		if ( opt & OPT_INTERACTIVE )
			std::cout << "> ";
		std::getline(std::cin, line);
		if ( !std::cin ) {
			std::cout << "\033[1;31mError:\033[0m \033[1;37mCannot read on standard input\033[0m" << '\n';
			return "\n";
		}
		ss << line << std::endl;
	} while ( line != ";;" && !(opt & OPT_INTERACTIVE));
	return ( ss.str() );
}

std::string	readFile( char *file ) {
	std::ifstream	ifs(file);

	if (ifs) {
		std::stringstream	ss;

		ss << ifs.rdbuf();
		ifs.close();
		return ( ss.str() );
	}
	else
		std::cout << "AVM: Cannot open " << file << std::endl;
	return "";
}

int	main( int ac, char **av ) {
	int		index;
	Options	options;

	try {
		options.handleOpt( ac, av, &index );
	} catch ( Options::UnknownOptionException & e ) {
		std::cout << "AVM : " << e.what() << ": " << av[index] << '\n';
		options.displayUsage();
		return (1);
	}

	if ( index < ac - 1 ) {
		std::cout << "AVM: Wrong number of input" << '\n';
		options.displayUsage();
		return (1);
	}

	Execution	execution( options );

	std::string	str;
	int	opt = options.getEffective();

	if (opt & OPT_INTERACTIVE && index != ac) {
		std::cout << "AVM: Error: Interactive mode can only handle stdin"
			<< std::endl;
		return (1);
	}
	do {
		str.clear();
		str = ( index == ac ) ? readStdin( opt ) : readFile( av[index] );
		if ( (str == "\n" && !(opt & OPT_INTERACTIVE)) || str == "")
			return (1);
		Lexer	lexer( str, options );
		Parser	parser( lexer );
		int		errors = parser.getError();
		if ( errors && !( opt & OPT_INTERACTIVE )) {
			std::cout << errors << " error" << (errors > 1 ? "s" : "") << " generated." << '\n'
				<< "AVM: Cannot assemble " << (index == ac ? "in stdin" : av[index]) << std::endl;
			return (1);
		}
		if ( !errors )
			execution.handleExecution( lexer );
	} while ( std::cin && opt & OPT_INTERACTIVE && (str != "exit\n" && str != ";;\n") );
	return (0);
}
