#ifndef PARSER_HPP
# define PARSER_HPP

#include "Lexer.hpp"

class Parser {
public:
	Parser( Lexer const & );
	~Parser( void );
	Parser( Parser const & );
	Parser	& operator=( Parser const & );
	class	IntructionTooLongException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
private:
	void	_parse( Lexer const & );
	void	handleSingleInstruction(std::vector<Lexer::tokens> const &);
	void	notYet(std::vector<Lexer::tokens> const &);
	void	handleException( int const & i );
};

#endif
