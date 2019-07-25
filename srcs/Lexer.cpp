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
	size_t	nl;
	size_t	pos = 0;
	std::string	substr;
	std::vector<std::string>	lines;

	while ( pos < this->_stream.size() ) {
		nl = this->_stream.find("\n", pos);
		std::cout << nl << std::endl;
		if ( nl == std::string::npos )
			lines.push_back(this->_stream.substr(pos, this->_stream.size() - pos));
		else if ((substr = this->_stream.substr(pos, nl - pos)) != "")
			lines.push_back(substr);
		pos = nl + 1;
	}
	return ( lines );
}

void	Lexer::tokenize( std::vector<std::string> const & lines ) {
	std::vector<tokens>	token_line;
	
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
		
	}

}

//Analyze lines by lines
void	Lexer::lex( void ) {
	std::vector<std::string>	lines = this->split();

	for(size_t i = 0; i < lines.size(); i++) {
		std::cout << lines[i] << std::endl;
	}

	return ;
}
