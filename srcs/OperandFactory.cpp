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

void	OperandFactory::registerOperand( eOperandType type, OperandFactory::memberPtr ptr ) {
	this->fMap[type] = ptr;
}

IOperand const *
OperandFactory::createOperand(eOperandType type, std::string const & value ) const {
	std::cout << "DEBUG : " << value << '\n';
	OperandFactory::factoryMap::const_iterator	it = this->fMap.find(type);
	if ( it == this->fMap.end() )
		return NULL;
	return (*this.*it->second)(value);
}

OperandFactory *	OperandFactory::getOp( void ) {
	static OperandFactory	instance;
	return &instance;
}

IOperand const *
OperandFactory::createInt8( std::string const & value ) const {
	int		toCast;

	try {
		std::cout << "DEBUG : " << value << '\n';
		if ( (toCast = std::stoi(value)) > INT8_MAX || toCast < INT8_MIN )
			throw std::out_of_range("Out of range");
		return new Operand<int8_t>( static_cast<int8_t>(toCast), value, Int8 );
	} catch ( std::out_of_range const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt16( std::string const & value ) const {
	int		toCast;

	try {
		if ( (toCast = std::stoi(value)) > INT16_MAX || toCast < INT16_MIN )
			throw std::out_of_range("Out of range");
		return new Operand<int16_t>( static_cast<int16_t>(toCast), value, Int16 );
	} catch ( std::out_of_range const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createInt32( std::string const & value ) const {
	int		toCast;

	try {
		if ( (toCast = std::stoi(value)) > INT32_MAX || toCast < INT32_MIN )
			throw std::out_of_range("Out of range");
		return new Operand<int32_t>( static_cast<int32_t>(toCast), value, Int32 );
	} catch ( std::out_of_range const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createFloat( std::string const & value ) const {
	float	converted;

	try {
		if ( (converted = std::stof(value)) > std::numeric_limits<float>::max()
			|| converted < std::numeric_limits<float>::min() )
			throw std::out_of_range("Out of range");
		return new Operand<float>( converted, value, Float );
	} catch ( std::out_of_range const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}

IOperand const *
OperandFactory::createDouble( std::string const & value ) const {
	double	converted;

	try {
		converted = converted = std::stod(value);
		return new Operand<double>( converted, value, Double );
	} catch ( std::out_of_range const &e ) {
		std::cout << e.what() << '\n';
	}
	return NULL;
}
