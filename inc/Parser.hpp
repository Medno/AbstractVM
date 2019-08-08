#ifndef PARSER_HPP
# define PARSER_HPP

#include "Lexer.hpp"
#include "Options.hpp"

class Parser {
public:
	~Parser( void );

	Parser( Lexer const & );
	int	getError( void ) const;

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

private:
	Parser( void );
	Parser( Parser const & );
	Parser	& operator=( Parser const & );

	typedef void ( Parser::*memberPtr )( std::vector<Lexer::token> const & );
	int		error;
	std::map<tokenLabel, memberPtr>	handler;

	void	registerTokenHandling( tokenLabel, memberPtr );
	void	parse( Lexer const & );
	void	handleSingleInstruction(std::vector<Lexer::token> const &);
	void	handlePairInstruction(std::vector<Lexer::token> const &);
	void	handleValueInstruction( std::vector<Lexer::token> const & );
	void	invalidInstruction( std::vector<Lexer::token> const & );
	void	handleException( int const & i );
};

#endif
