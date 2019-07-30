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
	class	WrongNumberOfArgumentsException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InvalidNumberOfInstructionException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	OpenBracketMissingException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	ClosingBracketMissingException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InvalidNumberValueZException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InvalidNumberValueNException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InvalidInstructionException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	bool	getError( void ) const;

private:
	bool	_error;
	void	_parse( Lexer const & );
	void	handleSingleInstruction(std::vector<Lexer::tokens> const &);
	void	handlePairInstruction(std::vector<Lexer::tokens> const &);
	void	handleValueInstruction( std::vector<Lexer::tokens> const & );
	void	invalidInstruction( std::vector<Lexer::tokens> const & );
	void	handleException( int const & i );
};

#endif
