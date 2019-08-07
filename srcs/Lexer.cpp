#include "Lexer.hpp"

/*
 * Canonical form
*/
Lexer::Lexer( void ) {}
Lexer::~Lexer( void ) {}
Lexer::Lexer( Lexer const & src ) {
	*this = src;
	return ;
}
Lexer	& Lexer::operator=( Lexer const & rhs ) {
	if ( this != &rhs ) {
		this->stream = rhs.stream;
		this->tokens = rhs.tokens;
		this->error = rhs.error;
	}
	return *this;
}

/*
 * Additionnal constructor
*/

Lexer::Lexer( std::string const & input, Options const & opts )
: stream( input ), error(0) {
	this->registerToken(PUSH, "push");
	this->registerToken(POP, "pop");
	this->registerToken(DUMP, "dump");
	this->registerToken(ASSERT, "assert");
	this->registerToken(ADD, "add");
	this->registerToken(SUB, "sub");
	this->registerToken(MUL, "mul");
	this->registerToken(DIV, "div");
	this->registerToken(MOD, "mod");
	this->registerToken(PRINT, "print");
	this->registerToken(EXIT, "exit");
	this->registerToken(O_BRACKET, "(");
	this->registerToken(C_BRACKET, ")");
	this->registerToken(INT8, "int8");
	this->registerToken(INT16, "int16");
	this->registerToken(INT32, "int32");
	this->registerToken(FLOAT, "float");
	this->registerToken(DOUBLE, "double");
	this->registerToken(N, "");
	this->registerToken(Z, "");
	this->registerToken(OTHER, "");
	this->lex( opts );
	return ;
}


/*
 * Exceptions class
*/
const char*	Lexer::UnknownInstructionException::what( void ) const throw() {
	return "Unknown instruction";
}

bool	Lexer::getError( void ) const {
	return this->error;
}
// End of Exceptions

void	Lexer::registerToken( tokenLabel label, std::string const & value ) {
	this->allTokens[value] = label;
}

std::vector<std::vector<Lexer::token> >	Lexer::getTokens( void ) const {
	return this->tokens;
}

std::vector<std::string>
Lexer::splitStr(std::string const &str, std::string const & spl) const {
	size_t	nl;
	size_t	pos = 0;
	std::string	substr;
	std::vector<std::string>	line;
	const size_t	len = str.size();

	while ( pos < len ) {
		nl = str.find(spl, pos);
		if ( nl == std::string::npos )
			line.push_back(str.substr(pos, len - pos));
		else if ((substr = str.substr(pos, nl - pos)) != "")
			line.push_back(substr);
		pos = nl + 1 == 0 ? len : nl + 1;
	}
	return ( line );
}

/*
 * First we need to split the stream into a vector of lines,
 * then we remove comments, and we only takes words
*/
std::vector<std::vector<std::string> >	Lexer::filterStream( void ) const {
	size_t	nl;
	std::regex	parenthese("([[:alnum:]]) *\\( *(.*) *\\)");
	std::vector<std::string>	lines;
	std::vector<std::string>	words;
	std::vector<std::vector <std::string> >	splitted;

	lines = splitStr( this->stream, "\n" );
	for ( auto&& l : lines ) {
		nl = l.find( ";", 0 );
		if ( nl != std::string::npos )
			l.erase( nl, l.size() - nl );
		l = std::regex_replace ( l, parenthese, "$1 ( $2 )" );
		std::fill( words.begin(), words.end(), 0 );
		words = splitStr( l, " " );
		if ( words.size() > 0 )
			splitted.push_back( words );
	}
	return ( splitted );
}

Lexer::token	Lexer::createSingleToken( std::string const & it ) const {
	std::regex isInt("-?[[:digit:]]+");
	std::regex isFloat("-?[[:digit:]]+.[[:digit:]]+");

	std::map<std::string, tokenLabel>::const_iterator	match = this->allTokens.find(it);
	if ( match != this->allTokens.end())
		return ( Lexer::token(match->second, it) );
	else if ( std::regex_match(it, isInt) )
		return ( Lexer::token(N, it) );
	else if ( std::regex_match(it, isFloat) )
		return ( Lexer::token(Z, it) );
	return ( Lexer::token(OTHER, it) );
}

void	Lexer::tokenize( std::vector<std::vector<std::string> > const & lines ) {
	Lexer::token	newToken;

	for( auto&& l : lines ) {
		std::vector<token>	tokenLine;
		for(auto&& words : l) {
			newToken = this->createSingleToken( words );
			tokenLine.push_back( newToken );
		}
		if ( tokenLine.size() )
			this->tokens.push_back(tokenLine);
	}
}

void	Lexer::displayErrors( void ) {
	for ( auto&& lines : this->tokens ) {
		for ( auto&& token : lines ) {
			try {
				if ( token.first == OTHER )
					throw UnknownInstructionException();
			} catch ( UnknownInstructionException & e ) {
				std::cout << "\033[1;31mError\033[0m: Line " << &lines-&this->tokens[0] + 1
					<< ": " << e.what() << ": " << token.second << '\n';
				this->error |= 1 << 0;
			}
		}
	}
}

//Analyze lines by lines
void	Lexer::lex( Options const & opts ) {
	std::vector<std::vector<std::string> >	lines = this->filterStream();

	this->tokenize( lines );
	if ( opts.getEffective() & OPT_VERBOSE )
		std::cout << *this;
	this->displayErrors();
	return ;
}

static std::string	findLabel( Lexer const & rhs, Lexer::token const & token ) {
	std::map<std::string, tokenLabel>::const_iterator m = rhs.allTokens.find(token.second);
	std::string	label;
	if (m == rhs.allTokens.end()) {
		switch ( token.first ) {
			case N:
				label = "N";
				break;
			case Z:
				label = "Z";
				break;
			default:
				label = "OTHER";
		}
	}
	else 
		label = m->first;
	std::transform(label.begin(), label.end(), label.begin(), ::toupper);
	return label;
}

std::ostream &	operator<<( std::ostream & o, Lexer const & rhs ) {
	std::vector< std::vector<Lexer::token > >	t_tokens = rhs.getTokens();
	o << "\033[1;32m---------------------------- Lexer ----------------------------" << '\n';
	for ( auto&& lines : t_tokens ) {
		o << "Line : " << &lines-&t_tokens[0] + 1 << std::endl;
		for ( auto&& token : lines ) {
			std::string label = findLabel( rhs, token );
			if (label == "OTHER")
				o << "\033[0m\033[1;31mToken :\t" << label << "\tValue:\t"
					<< token.second << "\033[0m\033[1;32m" << '\n';
			else
				o << "Token :\t" << label << "\tValue:\t" << token.second << std::endl;
		}
	}
	o << "------------------------- End of Lexer ------------------------\033[0m" << '\n';
	return (o);
}
