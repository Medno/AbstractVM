#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "IOperand.hpp"
#include <iostream>
#include <cmath>
#include "OperandFactory.hpp"

template<typename T>
class Operand : public IOperand {
public:
	~Operand( void ) {}
	Operand( T v, std::string const & s, eOperandType t ) : value(v), str(s), type(t) {
		this->precision = static_cast<int>(t);
	}

	std::string const & toString( void ) const { return ( this->str ); }
	int				getPrecision( void ) const { return ( this->precision ); }
	eOperandType	getType( void ) const { return ( this->type ); }

	static IOperand const *castOperator( eOperandType type, double res ) {
		if (type == 0 && res <= INT8_MAX && res >= INT8_MIN)
			return OperandFactory::getOp()->createOperand( type,
					std::to_string(static_cast<int8_t>(res)));
		else if (type == 1 && res <= INT16_MAX && res >= INT16_MIN)
			return OperandFactory::getOp()->createOperand( type,
					std::to_string(static_cast<int16_t>(res)));
		else if (type == 2 && res <= INT32_MAX && res >= INT32_MIN)
			return OperandFactory::getOp()->createOperand( type,
					std::to_string(static_cast<int32_t>(res)));
		else if (type == 3 && res <= std::numeric_limits<float>::max()
				&& res >= std::numeric_limits<float>::lowest() )
			return OperandFactory::getOp()->createOperand( type,
					std::to_string(static_cast<float>(res)));
		else if (type == 4)
			return OperandFactory::getOp()->createOperand( type,
					std::to_string(res));
		throw std::out_of_range("Out of range");
	}
	class	ModuloByZeroException: public std::exception {
		public:
			virtual const char*	what( void ) const throw() {
				return "Modulo by 0";
			}
	};
	class	DivisionByZeroException: public std::exception {
		public:
			virtual const char*	what( void ) const throw() {
				return "Division by 0";
			}
	};

private:
	Operand( void ) {}
	Operand( Operand const & src ) { *this = src; }
	Operand	& operator=( Operand const & rhs ) {
		if ( this != &rhs ) {
			this->value = rhs.value;
			this->precision = rhs.precision;
			this->str = rhs.str;
			this->type = rhs.type;
		}
		return *this;
	}

	virtual IOperand const * operator+( IOperand const & rhs ) const {
		double res = stod(this->toString()) + stod(rhs.toString());
		eOperandType	newType = this->precision > rhs.getPrecision()
			? this->type
			: rhs.getType();
		return Operand::castOperator(newType, res);
	}
	virtual IOperand const * operator-( IOperand const & rhs ) const {
		double res = stod(this->toString()) - stod(rhs.toString());
		eOperandType	newType = this->precision > rhs.getPrecision()
			? this->type
			: rhs.getType();
		return Operand::castOperator(newType, res);
	}
	virtual IOperand const * operator*( IOperand const & rhs ) const {
		double res = stod(this->toString()) * stod(rhs.toString());
		eOperandType	newType = this->precision > rhs.getPrecision()
			? this->type
			: rhs.getType();
		return Operand::castOperator(newType, res);
	}
	virtual IOperand const * operator/( IOperand const & rhs ) const {
		if (rhs.toString() == "0")
			throw DivisionByZeroException();
		double res = stod(this->toString()) / stod(rhs.toString());
		eOperandType	newType = this->precision > rhs.getPrecision()
			? this->type
			: rhs.getType();
		return Operand::castOperator(newType, res);
	}
	virtual IOperand const * operator%( IOperand const & rhs ) const {
		if (rhs.toString() == "0")
			throw ModuloByZeroException();
		double res = std::fmod(stod(this->toString()), stod(rhs.toString()));
		eOperandType	newType = this->precision > rhs.getPrecision()
		? this->type
		: rhs.getType();
		return Operand::castOperator(newType, res);
	}

	T				value;
	int				precision;
	std::string const	str;
	eOperandType	type;
};

#endif
