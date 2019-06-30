#include "Lexer.hpp"

Lexer::Lexer( std::string const & input ) : _stream( input ) {
	this->lex();
	return ;
}

Lexer::~Lexer( void ) {}

Lexer::Lexer( Lexer const & src ) {
	*this = src;
	return ;
}

Lexer	& Lexer::operator=( Lexer const & rhs ) {
	if ( this != &rhs )
		this->_stream = rhs._stream;
	return *this;
}

std::vector<std::string>	Lexer::split( void ) {
	size_t	pos = 0;
	size_t	nl;
	std::vector<std::string>	lines;

	while ( pos < this->_stream.size() ) {
		nl = this->_stream.find("\n", pos);
		if ( nl == std::string::npos )
			lines.push_back(this->_stream.substr(pos, this->_stream.size() - pos));
		else
			lines.push_back(this->_stream.substr(pos, nl - pos));
		pos = nl + 1;
	}
	return ( lines );
}

//Analyze lines by lines
void	Lexer::lex( void ) {
	std::vector<std::string>	lines = this->split();

	for(size_t i = 0; i < lines.size(); i++) {
		std::cout << lines[i] << std::endl;
	}

	return ;
}
