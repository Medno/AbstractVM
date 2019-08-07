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

std::string	readStdin( void ) {
	std::string			line;
	std::stringstream	ss;

	while ( std::getline(std::cin, line) && line != ";;")
		ss << line << std::endl;
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

	std::string	str;
	str = ( index == ac ) ? readStdin() : readFile( av[index] );
	if (str == "")
		return (1);
	Lexer	lexer(str, options);
	Parser	parser(lexer);
	if ( parser.getError() ) {
		std::cout << "AVM: Cannot assemble " << (index == ac ? "in stdin" : av[index]) << std::endl;
		return (1);
	}
	Execution	execution(lexer);
	return (0);
}
