#include "Lexer.hpp"
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
	if ( ac == 1 )
		str = readStdin();
	else
		str = readFile( av[1] );
	if (str != "") {
		Lexer	lexer(str);
	}
	return ( 0 );
}
