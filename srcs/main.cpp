#include "Lexer.hpp"
#include "Parser.hpp"
#include "Operand.hpp"

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
		std::cout << "Cannot open " << file << std::endl;
	return "";
}


int	main( int ac, char **av ) {
	if (ac > 2) {
		std::cout << "Wrong number of input" << std::endl;
		return ( 1 );
	}

	std::string	str;
	str = ( ac == 1 ) ? readStdin() : readFile( av[1] );
	if (str == "")
		return (1);
	Lexer	lexer(str);
	Parser	parser(lexer);
	if ( parser.getError() ) {
		std::cout << "Cannot assemble " << (ac == 1 ? "in stdin" : av[1]) << std::endl;
		return (1);
	}
	Operand<int>	a(5, "5");
	Operand<float>	b(5.2, "5.2");
	const IOperand	*res = a + b;
	std::cout << res->toString() << std::endl;
	return ( 0 );
}
