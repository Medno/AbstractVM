#include "Execution.hpp"

Execution::Execution( void ) {}
Execution::~Execution( void ) {}
Execution::Execution( Execution const & src ) { *this = src; }
Execution &	Execution::operator=( Execution const & ) {
	return *this;
}
