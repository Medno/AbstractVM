#include "Execution.hpp"

Execution::Execution( void ) {}
Execution::Execution( Lexer const & lexer, Options const & option )
: opt(option.getEffective()){
	this->registerHandler(PUSH, &Execution::push);
	this->registerHandler(POP, &Execution::pop);
	this->registerHandler(DUMP, &Execution::dump);
	this->registerHandler(ASSERT, &Execution::m_assert);
	this->registerHandler(ADD, &Execution::add);
	this->registerHandler(SUB, &Execution::sub);
	this->registerHandler(MUL, &Execution::mul);
	this->registerHandler(DIV, &Execution::div);
	this->registerHandler(PRINT, &Execution::print);

	this->registerHandler(AND, &Execution::m_and);
	this->registerHandler(OR, &Execution::m_or);
	this->registerHandler(XOR, &Execution::m_xor);
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
	return "Mismatch between operands... abort execution";
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

void	Execution::push( std::vector<Lexer::token> const & instr ) {
	try {
		IOperand const * newOperand =
		OperandFactory::getOp()->createOperand(this->typeMap[instr[1].first], instr[3].second);
		this->stack.push( newOperand );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::pop( std::vector<Lexer::token> const & ) {
	if ( this->stack.size() < 1 )
		throw StackLessThanOneException();
	this->stack.pop();
}

void	Execution::dump( std::vector<Lexer::token> const & ) {
	std::stack<IOperand const *>	copy(this->stack);
	std::ostringstream out;
	while ( !copy.empty() ) {
		IOperand const *	popped = copy.top();
		out << popped->toString() << '\n';
		copy.pop();
	}
	std::cout << out.str();
}

void	Execution::m_assert( std::vector<Lexer::token> const & instr ) {
	if ( this->stack.size() < 1 )
		throw StackLessThanOneException();
	IOperand const *	popped = this->stack.top();
	if ( popped->toString() != instr[3].second )
		throw InvalidAssertException();
}

void	Execution::add( std::vector<Lexer::token> const & ) {
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

void	Execution::sub( std::vector<Lexer::token> const & ) {
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

void	Execution::mul( std::vector<Lexer::token> const & ) {
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

void	Execution::div( std::vector<Lexer::token> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *second / *first;
		this->stack.push( res );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::mod( std::vector<Lexer::token> const & ) {
	if ( this->stack.size() < 2 )
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	try {
		IOperand const *	res = *second % *first;
		this->stack.push( res );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::print( std::vector<Lexer::token> const & ) {
	if ( this->stack.size() < 1 )
		throw StackLessThanOneException();
	IOperand const *	popped = this->stack.top();
	if ( popped->getType() != Int8 )
		throw InvalidAssertException();
	std::cout << static_cast<int8_t>(std::stoi(popped->toString(), nullptr));
}

void	Execution::printStack( void ) const {
	std::stack<IOperand const *>	copy(this->stack);
	std::ostringstream out;

	out << "\033[1;36mStack: < ";
	while ( !copy.empty() ) {
		IOperand const *	popped = copy.top();
		out << popped->toString();
		copy.pop();
		if ( !copy.empty() )
			out << ", ";
	}
	out << " >\033[0m\n";
	std::cout << out.str();
}

void	Execution::printInstruction( std::string const & instr ) const {
	std::cout << "\033[1m" << instr << " instruction...\033[0m\n";
}

void	Execution::m_and( std::vector<Lexer::token> const & ) {
	//CHECK IF BOTH VALUES ARE INT
	if (this->stack.size() < 2)
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	int	res = (stoi(second->toString()) & stoi(first->toString()));
	eOperandType	newType = first->getType() > second->getType() ?
		first->getType() : second->getType();
	try {
		IOperand const * newOperand = Operand<int>::castOperator(newType, res);
		this->stack.push( newOperand );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::m_or( std::vector<Lexer::token> const & ) {
	if (this->stack.size() < 2)
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	int	res = (stoi(second->toString()) | stoi(first->toString()));
	eOperandType	newType = first->getType() > second->getType() ?
		first->getType() : second->getType();
	try {
		IOperand const * newOperand = Operand<int>::castOperator(newType, res);
		this->stack.push( newOperand );
	} catch ( ... ) {
		throw;
	}
}

void	Execution::m_xor( std::vector<Lexer::token> const & ) {
	if (this->stack.size() < 2)
		throw StackLessThanTwoException();
	IOperand const *	first = this->stack.top();
	this->stack.pop();
	IOperand const *	second = this->stack.top();
	this->stack.pop();
	int	res = (stoi(second->toString()) ^ stoi(first->toString()));
	eOperandType	newType = first->getType() > second->getType() ?
		first->getType() : second->getType();
	try {
		IOperand const * newOperand = Operand<int>::castOperator(newType, res);
		this->stack.push( newOperand );
	} catch ( ... ) {
		throw;
	}
}


void	Execution::handleExecution( Lexer const & lexer ) {
	std::vector<std::vector<Lexer::token > >
		tokens = lexer.getTokens();
	try {
		for (auto&& instruction : tokens) {
			try {
				if (instruction[0].first == EXIT)
					// Catch instruction after exit ? 
					return ;
				if (this->opt & OPT_VERBOSE)
					this->printInstruction(instruction[0].second);
				(this->*fMap[instruction[0].first])(instruction);
			} catch ( ... ) {
				this->handleException( instruction[0].second );
				return ;
			}
			if ( (this->opt & OPT_VERBOSE) )
				this->printStack();
		}
		throw MissingExitInstruction();
	} catch ( MissingExitInstruction &e ) {
		this->handleException( "exit" );
	}
}
