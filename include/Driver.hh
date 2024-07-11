#ifndef __Driver_hh__
#define __Driver_hh__

#include <FlexLexer.h>
#include <parser.hh>

namespace yy {

struct Driver {
private:
  FlexLexer *plex_;

public:
  Driver(FlexLexer *plex);

  parser::token_type yylex(parser::semantic_type *yylval); 
  bool parse();
};

} // namespace yy

#endif
