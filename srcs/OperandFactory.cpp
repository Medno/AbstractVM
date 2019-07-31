#include "OperandFactory.hpp"

/*
 * Canonical form
*/ 
OperandFactory::OperandFactory( void ) {
	this->registerOperand(Int8, &OperandFactory::createInt8);
	this->registerOperand(Int16, &OperandFactory::createInt16);
	this->registerOperand(Int32, &OperandFactory::createInt32);
	this->registerOperand(Float, &OperandFactory::createFloat);
	this->registerOperand(Double, &OperandFactory::createDouble);
}

OperandFactory::~OperandFactory( void ) {
	this->factory_map.clear();
}
OperandFactory::OperandFactory( OperandFactory const & ) {}
OperandFactory	& OperandFactory::operator=( OperandFactory const & rhs ) { return *this; }

void	OperandFactory::registerOperand( eOperandType type, OperandFactory::memberPtr ptr ) {
	this->fMap[type] = ptr;
}

IOperand const *
OperandFactory::createOperand(eOperandType type, std::string const & value ) const {
	OperandFactory::factoryMap::iterator	it = fMap.find(type);
	if ( it == fMap.end() )
		return NULL;
	return it->second;
}

OperandFactory *	OperandFactory::getOp( void ) {
	static OperandFactory	instance;
	return &instance;
}

IOperand const *
OperandFactory::createInt8( std::string const & value ) const {
	int8_t	converted;

	try {
		converted = boost::lexical_cast<int8_t>(value);
		return new Operand<int8_t>( converted, value );
	} catch ( boost::bad_lexical_cast const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt16( std::string const & value ) const {
	int16_t	converted;

	try {
		converted = boost::lexical_cast<int16_t>(value);
		return new Operand<int16_t>( converted, value );
	} catch ( boost::bad_lexical_cast const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt32( std::string const & value ) const {
	int32_t	converted;

	try {
		converted = boost::lexical_cast<int32_t>(value);
		return new Operand<int32_t>( converted, value );
	} catch ( boost::bad_lexical_cast const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createFloat( std::string const & value ) const {
	float	converted;

	try {
		converted = boost::lexical_cast<float>(value);
		return new Operand<float>( converted, value );
	} catch ( boost::bad_lexical_cast const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createDouble( std::string const & value ) const {
	double	converted;

	try {
		converted = boost::lexical_cast<double>(value);
		return new Operand<double>( converted, value );
	} catch ( boost::bad_lexical_cast const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}
