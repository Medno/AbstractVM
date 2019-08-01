#include "Execution.hpp"

Execution::Execution( void ) {}
Execution::Execution( Lexer const & lexer ) {
	this->registerHandler(PUSH, &Execution::push);
	this->registerHandler(POP, &Execution::pop);
	this->registerHandler(DUMP, &Execution::dump);
	this->registerHandler(ASSERT, &Execution::m_assert);
	this->registerHandler(ADD, &Execution::add);
	this->registerHandler(SUB, &Execution::sub);
	this->registerHandler(MUL, &Execution::mul);
	this->registerHandler(DIV, &Execution::div);
	this->registerHandler(PRINT, &Execution::print);
//	this->registerHandler(EXIT, &Execution::exit);

	this->typeMap[INT8] = Int8;
	this->typeMap[INT16] = Int16;
	this->typeMap[INT32] = Int32;
	this->typeMap[FLOAT] = Float;
	this->typeMap[DOUBLE] = Double;
	this->handleExecution( lexer );
}
Execution::~Execution( void ) {}
Execution::Execution( Execution const & src ) { *this = src; }
Execution &	Execution::operator=( Execution const & ) { return *this; }

/*
 * Exception classes
*/
const char*	Execution::StackLessThanTwoException::what( void ) const throw() {
	return "Stack must have at least two operands";
}

const char*	Execution::StackLessThanOneException::what( void ) const throw() {
	return "Stack must have at least one operand";
}

const char*	Execution::InvalidAssertException::what( void ) const throw() {
	return "Mismatch between values... abort execution";
}

const char*	Execution::MissingExitInstruction::what( void ) const throw() {
	return "Missing exit instruction... abort execution";
}

void	Execution::handleException( std::string const & instruction ) {
	try {
		std::cout << "Runtime error: " << instruction << ": ";
		throw;
	} catch ( std::exception & e ) {
		std::cout << e.what() << std::endl;
	}
}

void	Execution::registerHandler( tokenLabel type, Execution::memberPtr ptr ) {
	this->fMap[type] = ptr;
}

void	Execution::push( std::vector<Lexer::tokens> const & instr ) {
	try {
		IOperand const * newOperand =
		OperandFactory::getOp()->createOperand(this->typeMap[instr[1].first], instr[3].second);
		this->stack.push( newOperand );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::pop( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 1 )
		throw StackLessThanOneException();
	this->stack.pop();
}

void	Execution::dump( std::vector<Lexer::tokens> const & ) {
	std::stack<IOperand const *>	copy(this->stack);
	while ( !copy.empty() ) {
		IOperand const *	popped = copy.top();
		std::cout << popped->toString() << '\n';
		copy.pop();
	}
}

void	Execution::m_assert( std::vector<Lexer::tokens> const & instr ) {
	if ( this->stack.size() < 1 )
		throw StackLessThanOneException();
	std::stack<IOperand const *>	copy(this->stack);
	IOperand const *	popped = copy.top();
	if ( popped->toString() != instr[3].second )
		throw InvalidAssertException();
}

void	Execution::add( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *first + *second;
		this->stack.push( res );
	} catch ( std::out_of_range &e ) {
		throw;
	}
}

void	Execution::sub( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *first - *second;
		this->stack.push( res );
	} catch ( std::out_of_range &e ) {
		throw;
	}
}

void	Execution::mul( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *first * *second;
		this->stack.push( res );
	} catch ( std::out_of_range &e ) {
		throw;
	}
}

void	Execution::div( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *first / *second;
		this->stack.push( res );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::mod( std::vector<Lexer::tokens> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *first % *second;
		this->stack.push( res );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::print( std::vector<Lexer::tokens> const & ) {}
//void	Execution::exit( std::vector<Lexer::tokens>const & input );

void	Execution::handleExecution( Lexer const & lexer ) {
	std::vector<std::vector<Lexer::tokens > >
		tokens = lexer.getTokens();
	try {
		for (auto&& instruction : tokens) {
			try {
				if (instruction[0].first == EXIT)
					return ;
				(this->*fMap[instruction[0].first])(instruction);
			} catch ( ... ) {
				this->handleException( instruction[0].second );
				return ;
			}
		}
		throw MissingExitInstruction();
	} catch ( MissingExitInstruction &e ) {
		this->handleException( "exit" );
	}
}
