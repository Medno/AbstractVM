#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "IOperand.hpp"
#include <cmath>

template<typename T>
class Operand : public IOperand {
public:
	Operand( T value, std::string str ) : _value(value), _str(str) {}
	~Operand( void ) {}
	Operand( Operand const & src ) { *this = src; }
	Operand	& operator=( Operand const & ) { return *this; }

	int				getPrecision( void ) const { return ( this->_precision ); }
	eOperandType	getType( void ) const { return ( this->_type ); }

	virtual IOperand const * operator+( IOperand const & rhs ) const {
		std::cout << this->_value << std::endl;
		std::cout << static_cast<T>(stod(rhs.toString())) << std::endl;
		T	value = this->_value + static_cast<T>(stod(rhs.toString()));
		const std::string	toStr = std::to_string(value);
		std::cout << toStr << std::endl;
		return new Operand<T>( value, toStr );
	}
	virtual IOperand const * operator-( IOperand const & rhs ) const {
		T	value = this->_value - static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		return new Operand( value, toStr );
	}
	virtual IOperand const * operator*( IOperand const & rhs ) const {
		T	value = this->_value * static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		return new Operand( value, toStr );
	}
	virtual IOperand const * operator/( IOperand const & rhs ) const {
		T	value = this->_value / static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		return new Operand( value, toStr );
	}
	virtual IOperand const * operator%( IOperand const & rhs ) const {
		T	value = std::fmod(this->_value, static_cast<T>(stod(rhs.toString())));
		std::string	toStr = std::to_string(value);
		return new Operand( value, toStr );
	}

	std::string const & toString( void ) const { return ( this->_str ); }
	IOperand const * createOperand( eOperandType type, std::string const & value ) const;
	T	getValue( void ) const { return (this->_value); }

private:
	T				_value;
	int				_precision;
	eOperandType	_type;
	std::string const &	_str;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;
};

#endif
