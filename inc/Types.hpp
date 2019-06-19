#ifndef TYPES_HPP
# define TYPES_HPP

template<typename T>
class Types {
public:
	Types( void );
	~Types( void );
	Types( Types const & );
	Types	operator=( Types const & );

	int				getPrecision( void ) const;
	eOperandType	getType( void ) const;

private:
	T				_value;
	int				_precision;
	eOperandType	_type;
};

#endif
