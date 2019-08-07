#include "Options.hpp"

Options::Options( void ) : effective(0) {
	this->available = {
		{ OPT_INTERACTIVE, {"-i", "--interactive"} },
		{ OPT_VERBOSE, {"-v", "--verbose"} },
	};
}
Options::~Options( void ) {}
Options::Options( Options const & src ) { *this = src; }
Options &	Options::operator=( Options const & rhs ) {
	if (this->effective != rhs.effective)
		this->effective = rhs.effective;
	return *this;
}

Options::UnknownOptionException::UnknownOptionException( const char * str ) : invalid_argument(str) {}

int		Options::getEffective( void ) const { return this->effective; }

void	Options::displayUsage( void ) {
	std::ostringstream	out;
	out << "Usage : avm";
	for ( auto&& opt : this->available ) {
		out << " [" << opt.second.first;
		if (opt.second.second != "")
			out << " | " << opt.second.second;
		out << "]";
	}
	out << " [file]";
	std::cout << out.str() << '\n';
}

void	Options::handleOpt( int ac, char **av, int *i ) {
	*i = 1;
	uint8_t	match;
	int		eoo = 0;

	while ( *i < ac && (eoo = strcmp(av[*i], "--") != 0) && av[*i][0] == '-' ) {
		match = 0;
		std::string	arg(av[*i]);
		for ( auto&& opt : this->available ) {
			if ( opt.second.first == arg || opt.second.second == arg ) {
				this->effective |= opt.first;
				match = 1;
			}
		}
		if (!match)
			throw UnknownOptionException("Unknown option");
		(*i)++;
	}
	if ( !eoo && *i != ac )
		(*i)++;
}
