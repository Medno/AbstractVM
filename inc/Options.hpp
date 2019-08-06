#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <iostream>
#include <vector>
#include <sstream>

class	Options {
public:
	Options( void );
	~Options( void );
	void	handleOpt( int, char **, int * );
	void	displayUsage( void );
	int		getEffective( void );
	class	UnknownOptionException: public std::invalid_argument {
		public:
			UnknownOptionException(const char *str);
	};

private:
	Options( Options const & );
	Options &	operator=( Options const & );
	int	effective;
	std::vector<std::pair<std::string, std::string> >	available;
};

#endif
