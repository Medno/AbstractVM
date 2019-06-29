#ifndef TYPES_HPP
# define TYPES_HPP

#include "IOperand.hpp"

template<typename T>
class Types {
public:
	Types( void ) {}
	~Types( void ) {}
	Types( Types const & src ) { *this = src; }
	Types	& operator=( Types const & rhs) { return *this; }

	int				getPrecision( void ) const { return ( this->_precision ); }
	eOperandType	getType( void ) const { return ( this->_type ); }

	 IOperand const * operator+( IOperand const & rhs ) const { return ( this->value + rhs.value ); }
	 IOperand const * operator-( IOperand const & rhs ) const { return ( this->value - rhs.value ); }
	 IOperand const * operator*( IOperand const & rhs ) const { return ( this->value * rhs.value ); }
	 IOperand const * operator/( IOperand const & rhs ) const { return ( this->value / rhs.value ); }
	 IOperand const * operator%( IOperand const & rhs ) const { return ( this->value % rhs.value ); }

	 std::string const & toString( void ) const { return ( to_string( this->_value ) ); }

private:
	T				_value;
	int				_precision;
	eOperandType	_type;
};

#endif
