#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <stack>
#include "IOperand.hpp"
#include "Operand.hpp"
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

	Execution( Lexer const & , Options const & );
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
	class	MissingExitInstructionException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	class	InstructionOnIntException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};
	void	push( std::vector<Lexer::token> const & );
	void	pop( std::vector<Lexer::token> const & );
	void	dump( std::vector<Lexer::token> const & );
	void	m_assert( std::vector<Lexer::token> const & );
	void	add( std::vector<Lexer::token> const & );
	void	sub( std::vector<Lexer::token> const & );
	void	mul( std::vector<Lexer::token> const & );
	void	div( std::vector<Lexer::token> const & );
	void	mod( std::vector<Lexer::token> const & );
	void	print( std::vector<Lexer::token> const & );

	void	m_and( std::vector<Lexer::token> const & );
	void	m_or( std::vector<Lexer::token> const & );
	void	m_xor( std::vector<Lexer::token> const & );
	void	m_not( std::vector<Lexer::token> const & );

	void	printStack( void ) const;
	void	handleExecution( Lexer const & lexer );
private:
	typedef void ( Execution::*memberPtr )( std::vector<Lexer::token> const & );
	std::stack<IOperand const *>	stack;
	std::map<tokenLabel, memberPtr>	fMap;
	void	registerHandler( tokenLabel type, memberPtr ptr );
	void	printInstruction( std::string const & instr ) const;
	static bool	isInt( IOperand const * check );

	void	handleException( std::string const & instruction );
	std::map<tokenLabel, eOperandType>	typeMap;
	int	opt;
};

#endif
