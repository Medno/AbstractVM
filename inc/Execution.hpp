#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <stack>
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include "Lexer.hpp"

class	Execution {
public:
/*
 * Canonical form
*/
	Execution( void );
	~Execution( void );
	Execution( Execution const & );
	Execution	& operator=( Execution const &);

	Execution( Lexer const & lexer );
	class	StackLessThanTwoException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	StackLessThanOneException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InvalidAssertException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	MissingExitInstruction: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	void	push( std::vector<Lexer::tokens> const & );
	void	pop( std::vector<Lexer::tokens> const & );
	void	dump( std::vector<Lexer::tokens> const & );
	void	m_assert( std::vector<Lexer::tokens> const &  );
	void	add( std::vector<Lexer::tokens> const & );
	void	sub( std::vector<Lexer::tokens> const & );
	void	mul( std::vector<Lexer::tokens> const & );
	void	div( std::vector<Lexer::tokens> const & );
	void	mod( std::vector<Lexer::tokens> const & );
	void	print( std::vector<Lexer::tokens> const & );
//	void	exit( std::vector<Lexer::tokens>const & );

	void	handleExecution( Lexer const & lexer );
private:
	typedef void ( Execution::*memberPtr )( std::vector<Lexer::tokens> const & );
	std::stack<IOperand const *>	stack;
	std::map<tokenLabel, memberPtr>	fMap;
	void	registerHandler( tokenLabel type, memberPtr ptr );

	void	handleException( std::string const & instruction );
	std::map<tokenLabel, eOperandType>	typeMap;
};

#endif
