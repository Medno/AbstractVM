#ifndef OPERANDFACTORY_HPP
#define OPERANDFACTORY_HPP

#include "IOperand.hpp"
#include <map>

class OperandFactory {
private:
	OperandFactory( void );
	OperandFactory( OperandFactory const & );
	OperandFactory & operator=( OperandFactory const & );

	typedef IOperand const * ( OperandFactory::*memberPtr )( std::string const & ) const;
	typedef std::map<eOperandType, memberPtr>	factoryMap;
	factoryMap	fMap;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;
public:
	~OperandFactory( void );
	void	registerOperand( eOperandType type, memberPtr ptr );
	IOperand const * createOperand( eOperandType type, std::string const & value ) const;
	static OperandFactory *	getOp( void );

};

#endif
