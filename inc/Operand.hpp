#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "IOperand.hpp"
#include <iostream>
#include <cmath>
#include "OperandFactory.hpp"

template<typename T>
class Operand : public IOperand {
public:
	Operand( T v, std::string const & s, eOperandType t ) : value(v), str(s), type(t) {
		std::cout << "LAST : " << str << '\n';
		this->precision = static_cast<int>(t);
	}
	~Operand( void ) {}
	Operand( Operand const & src ) { *this = src; }
	Operand	& operator=( Operand const & ) { return *this; }

	int				getPrecision( void ) const { return ( this->precision ); }
	eOperandType	getType( void ) const { return ( this->type ); }

	virtual IOperand const * operator+( IOperand const & rhs ) const {
		T	value = this->value + static_cast<T>(stod(rhs.toString()));
		const std::string	toStr = std::to_string(value);
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		std::cout << toStr << '\n';
		return OperandFactory::getOp()->createOperand( newType, toStr );
	}
	virtual IOperand const * operator-( IOperand const & rhs ) const {
		T	value = this->value - static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		return OperandFactory::getOp()->createOperand( newType, toStr );
	}
	virtual IOperand const * operator*( IOperand const & rhs ) const {
		T	value = this->value * static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		return OperandFactory::getOp()->createOperand( newType, toStr );
	}
	virtual IOperand const * operator/( IOperand const & rhs ) const {
		T	value = this->value / static_cast<T>(stod(rhs.toString()));
		std::string	toStr = std::to_string(value);
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		return OperandFactory::getOp()->createOperand( newType, toStr );
	}
	virtual IOperand const * operator%( IOperand const & rhs ) const {
		T	value = std::fmod(this->value, static_cast<T>(stod(rhs.toString())));
		std::string	toStr = std::to_string(value);
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		return OperandFactory::getOp()->createOperand( newType, toStr );
	}

	std::string const & toString( void ) const { std::cout << "Str : " << this->str << '\n' << "Val: " << this->value << "\nend\n"; return ( this->str ); }
	T	getValue( void ) const { return (this->value); }

private:
	T				value;
	int				precision;
	std::string const &	str;
	eOperandType	type;
};

#endif
