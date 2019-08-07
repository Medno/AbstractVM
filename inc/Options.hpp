#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <iostream>
#include <vector>
#include <sstream>

#define OPT_INTERACTIVE	1 << 0
#define OPT_VERBOSE		1 << 1

class	Options {
public:
	Options( void );
	~Options( void );
	void	handleOpt( int, char **, int * );
	void	displayUsage( void );
	int		getEffective( void ) const;
	class	UnknownOptionException: public std::invalid_argument {
		public:
			UnknownOptionException(const char *str);
	};
	typedef std::pair<int, std::pair<std::string, std::string> > opt;

private:
	Options( Options const & );
	Options &	operator=( Options const & );
	int	effective;
	std::vector<opt>	available;
};

#endif
