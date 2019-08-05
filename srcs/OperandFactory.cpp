#include "OperandFactory.hpp"
#include "Operand.hpp"

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
	this->fMap.clear();
}
OperandFactory::OperandFactory( OperandFactory const & ) {}
OperandFactory	& OperandFactory::operator=( OperandFactory const & ) { return *this; }

/*
 * Exception classes
*/
OperandFactory::UnderflowException::UnderflowException( const char * str ) : underflow_error(str) {}
OperandFactory::OverflowException::OverflowException( const char * str ) : overflow_error(str) {}

void	OperandFactory::handleException( std::string const & value ) const {
	std::ostringstream	output;
	output << "Runtime error: Operand creation: ";
	try {
		throw;
	} catch ( OverflowException const &e ) {
		output << e.what();
	} catch ( UnderflowException const &e ) {
		output << e.what();
	} catch ( std::out_of_range const &e ) {
		output << e.what();
	} catch ( std::exception & e ) {
		output << e.what();
	}
	output << " on value : " << value << '\n';
	std::cout << output.str();
}

void	OperandFactory::registerOperand( eOperandType type, OperandFactory::memberPtr ptr ) {
	this->fMap[type] = ptr;
}

IOperand const *
OperandFactory::createOperand(eOperandType type, std::string const & value ) const {
	std::cout << "DEBUG : " << value << '\n';
	OperandFactory::factoryMap::const_iterator	it = this->fMap.find(type);
	if ( it == this->fMap.end() )
		return NULL;
	try {
		IOperand const *	newOperand = (*this.*it->second)(value);
		return newOperand;
	} catch ( ... ) {
		this->handleException( value );
	}
	return NULL;
}

OperandFactory *	OperandFactory::getOp( void ) {
	static OperandFactory	instance;
	return &instance;
}

IOperand const *
OperandFactory::createInt8( std::string const & value ) const {
	int		toCast;
	std::ostringstream	filtered;

	try {
		std::cout << "DEBUG : " << value << '\n';
		toCast = std::stoi(value);
		if ( toCast > INT8_MAX) 
			throw OverflowException("Overflow on int8 type");
		else if ( toCast < INT8_MIN )
			throw UnderflowException("Underflow on int8 type");
		filtered << toCast;
		return new Operand<int8_t>( static_cast<int8_t>(toCast), filtered.str(), Int8 );
	} catch ( ... ) {
		throw;
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt16( std::string const & value ) const {
	int		toCast;
	std::ostringstream	filtered;

	try {
		toCast = std::stoi(value);
		if ( toCast > INT16_MAX) 
			throw OverflowException("Overflow on int16 type");
		else if ( toCast < INT16_MIN )
			throw UnderflowException("Underflow on int16 type");
		filtered << toCast;
		return new Operand<int16_t>( static_cast<int16_t>(toCast), filtered.str(), Int16 );
	} catch ( ... ) {
		throw;
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt32( std::string const & value ) const {
	int		toCast;
	std::ostringstream	filtered;

	try {
		toCast = std::stoi(value);
		if ( toCast > INT32_MAX) 
			throw OverflowException("Overflow on int32 type");
		else if ( toCast < INT32_MIN )
			throw UnderflowException("Underflow on int32 type");
		filtered << toCast;
		return new Operand<int32_t>( static_cast<int32_t>(toCast), filtered.str(), Int32 );
	} catch ( ... ) {
		throw;
	}
	return NULL;
}

IOperand const *
OperandFactory::createFloat( std::string const & value ) const {
	float	converted;
	std::ostringstream	filtered;

	try {
		converted = std::stof(value);
		if ( converted > std::numeric_limits<float>::max())
			throw OverflowException("Overflow on float type");
		else if ( converted < std::numeric_limits<float>::min() )
			throw UnderflowException("Underflow on float type");
		filtered << converted;
		return new Operand<float>( converted, filtered.str(), Float );
	} catch ( ... ) {
		throw;
	}
	return NULL;
}

IOperand const *
OperandFactory::createDouble( std::string const & value ) const {
	double	converted;
	std::ostringstream	filtered;

	try {
		converted = converted = std::stod(value);
		filtered << converted;
		return new Operand<double>( converted, filtered.str(), Double );
	} catch ( ... ) {
		throw;
	}
	return NULL;
}
