// A Bison parser, made by GNU Bison 3.3.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "parser.hh"


// Unqualified %code blocks.
#line 17 "parser.yy" // lalr1.cc:435

# include "driver.hh"

#line 49 "parser.cc" // lalr1.cc:435


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 144 "parser.cc" // lalr1.cc:510

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (driver& drv_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 58: // args
        value.YY_MOVE_OR_COPY< Args* > (YY_MOVE (that.value));
        break;

      case 55: // block
        value.YY_MOVE_OR_COPY< Block* > (YY_MOVE (that.value));
        break;

      case 47: // class_body
        value.YY_MOVE_OR_COPY< Body* > (YY_MOVE (that.value));
        break;

      case 61: // boolean-literal
        value.YY_MOVE_OR_COPY< BoolLit* > (YY_MOVE (that.value));
        break;

      case 46: // class
        value.YY_MOVE_OR_COPY< Classe* > (YY_MOVE (that.value));
        break;

      case 45: // classes
        value.YY_MOVE_OR_COPY< Classes* > (YY_MOVE (that.value));
        break;

      case 57: // expr
        value.YY_MOVE_OR_COPY< Expr* > (YY_MOVE (that.value));
        break;

      case 56: // exprx
        value.YY_MOVE_OR_COPY< Exprx* > (YY_MOVE (that.value));
        break;

      case 59: // exprxx
        value.YY_MOVE_OR_COPY< Exprxx* > (YY_MOVE (that.value));
        break;

      case 49: // field
        value.YY_MOVE_OR_COPY< Field* > (YY_MOVE (that.value));
        break;

      case 48: // field-method
        value.YY_MOVE_OR_COPY< FieldMethod* > (YY_MOVE (that.value));
        break;

      case 54: // formal
        value.YY_MOVE_OR_COPY< Formal* > (YY_MOVE (that.value));
        break;

      case 52: // formals
        value.YY_MOVE_OR_COPY< Formals* > (YY_MOVE (that.value));
        break;

      case 53: // formalx
        value.YY_MOVE_OR_COPY< Formalx* > (YY_MOVE (that.value));
        break;

      case 60: // literal
        value.YY_MOVE_OR_COPY< Literal* > (YY_MOVE (that.value));
        break;

      case 50: // method
        value.YY_MOVE_OR_COPY< Method* > (YY_MOVE (that.value));
        break;

      case 44: // program
        value.YY_MOVE_OR_COPY< Programm* > (YY_MOVE (that.value));
        break;

      case 51: // type
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case 26: // INTEGER_LITERAL
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 24: // TYPE_IDENTIFIER
      case 25: // OBJECT_IDENTIFIER
      case 27: // STRING_LITERAL
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 58: // args
        value.move< Args* > (YY_MOVE (that.value));
        break;

      case 55: // block
        value.move< Block* > (YY_MOVE (that.value));
        break;

      case 47: // class_body
        value.move< Body* > (YY_MOVE (that.value));
        break;

      case 61: // boolean-literal
        value.move< BoolLit* > (YY_MOVE (that.value));
        break;

      case 46: // class
        value.move< Classe* > (YY_MOVE (that.value));
        break;

      case 45: // classes
        value.move< Classes* > (YY_MOVE (that.value));
        break;

      case 57: // expr
        value.move< Expr* > (YY_MOVE (that.value));
        break;

      case 56: // exprx
        value.move< Exprx* > (YY_MOVE (that.value));
        break;

      case 59: // exprxx
        value.move< Exprxx* > (YY_MOVE (that.value));
        break;

      case 49: // field
        value.move< Field* > (YY_MOVE (that.value));
        break;

      case 48: // field-method
        value.move< FieldMethod* > (YY_MOVE (that.value));
        break;

      case 54: // formal
        value.move< Formal* > (YY_MOVE (that.value));
        break;

      case 52: // formals
        value.move< Formals* > (YY_MOVE (that.value));
        break;

      case 53: // formalx
        value.move< Formalx* > (YY_MOVE (that.value));
        break;

      case 60: // literal
        value.move< Literal* > (YY_MOVE (that.value));
        break;

      case 50: // method
        value.move< Method* > (YY_MOVE (that.value));
        break;

      case 44: // program
        value.move< Programm* > (YY_MOVE (that.value));
        break;

      case 51: // type
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case 26: // INTEGER_LITERAL
        value.move< int > (YY_MOVE (that.value));
        break;

      case 24: // TYPE_IDENTIFIER
      case 25: // OBJECT_IDENTIFIER
      case 27: // STRING_LITERAL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 58: // args
        value.move< Args* > (that.value);
        break;

      case 55: // block
        value.move< Block* > (that.value);
        break;

      case 47: // class_body
        value.move< Body* > (that.value);
        break;

      case 61: // boolean-literal
        value.move< BoolLit* > (that.value);
        break;

      case 46: // class
        value.move< Classe* > (that.value);
        break;

      case 45: // classes
        value.move< Classes* > (that.value);
        break;

      case 57: // expr
        value.move< Expr* > (that.value);
        break;

      case 56: // exprx
        value.move< Exprx* > (that.value);
        break;

      case 59: // exprxx
        value.move< Exprxx* > (that.value);
        break;

      case 49: // field
        value.move< Field* > (that.value);
        break;

      case 48: // field-method
        value.move< FieldMethod* > (that.value);
        break;

      case 54: // formal
        value.move< Formal* > (that.value);
        break;

      case 52: // formals
        value.move< Formals* > (that.value);
        break;

      case 53: // formalx
        value.move< Formalx* > (that.value);
        break;

      case 60: // literal
        value.move< Literal* > (that.value);
        break;

      case 50: // method
        value.move< Method* > (that.value);
        break;

      case 44: // program
        value.move< Programm* > (that.value);
        break;

      case 51: // type
        value.move< Type* > (that.value);
        break;

      case 26: // INTEGER_LITERAL
        value.move< int > (that.value);
        break;

      case 24: // TYPE_IDENTIFIER
      case 25: // OBJECT_IDENTIFIER
      case 27: // STRING_LITERAL
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 58: // args
        yylhs.value.emplace< Args* > ();
        break;

      case 55: // block
        yylhs.value.emplace< Block* > ();
        break;

      case 47: // class_body
        yylhs.value.emplace< Body* > ();
        break;

      case 61: // boolean-literal
        yylhs.value.emplace< BoolLit* > ();
        break;

      case 46: // class
        yylhs.value.emplace< Classe* > ();
        break;

      case 45: // classes
        yylhs.value.emplace< Classes* > ();
        break;

      case 57: // expr
        yylhs.value.emplace< Expr* > ();
        break;

      case 56: // exprx
        yylhs.value.emplace< Exprx* > ();
        break;

      case 59: // exprxx
        yylhs.value.emplace< Exprxx* > ();
        break;

      case 49: // field
        yylhs.value.emplace< Field* > ();
        break;

      case 48: // field-method
        yylhs.value.emplace< FieldMethod* > ();
        break;

      case 54: // formal
        yylhs.value.emplace< Formal* > ();
        break;

      case 52: // formals
        yylhs.value.emplace< Formals* > ();
        break;

      case 53: // formalx
        yylhs.value.emplace< Formalx* > ();
        break;

      case 60: // literal
        yylhs.value.emplace< Literal* > ();
        break;

      case 50: // method
        yylhs.value.emplace< Method* > ();
        break;

      case 44: // program
        yylhs.value.emplace< Programm* > ();
        break;

      case 51: // type
        yylhs.value.emplace< Type* > ();
        break;

      case 26: // INTEGER_LITERAL
        yylhs.value.emplace< int > ();
        break;

      case 24: // TYPE_IDENTIFIER
      case 25: // OBJECT_IDENTIFIER
      case 27: // STRING_LITERAL
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 90 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Programm* > () = new Programm(yystack_[0].value.as < Classes* > ());}
#line 882 "parser.cc" // lalr1.cc:919
    break;

  case 3:
#line 93 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Classes* > () = new Classes(yystack_[1].value.as < Classes* > (),yystack_[0].value.as < Classe* > ());}
#line 888 "parser.cc" // lalr1.cc:919
    break;

  case 4:
#line 94 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Classes* > () = new Classes(yystack_[0].value.as < Classe* > ());}
#line 894 "parser.cc" // lalr1.cc:919
    break;

  case 5:
#line 97 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Classe* > () = new Classe(yystack_[1].value.as < std::string > (),yystack_[0].value.as < Body* > ());}
#line 900 "parser.cc" // lalr1.cc:919
    break;

  case 6:
#line 98 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Classe* > () = new Classe(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::string > (),yystack_[0].value.as < Body* > ());}
#line 906 "parser.cc" // lalr1.cc:919
    break;

  case 7:
#line 101 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Body* > () = new Body(yystack_[1].value.as < FieldMethod* > ());}
#line 912 "parser.cc" // lalr1.cc:919
    break;

  case 8:
#line 104 "parser.yy" // lalr1.cc:919
    { yylhs.value.as < FieldMethod* > () = nullptr;}
#line 918 "parser.cc" // lalr1.cc:919
    break;

  case 9:
#line 105 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < FieldMethod* > () = new FieldMethod(yystack_[1].value.as < FieldMethod* > (),yystack_[0].value.as < Field* > ());}
#line 924 "parser.cc" // lalr1.cc:919
    break;

  case 10:
#line 106 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < FieldMethod* > () = new FieldMethod(yystack_[1].value.as < FieldMethod* > (),yystack_[0].value.as < Method* > ());}
#line 930 "parser.cc" // lalr1.cc:919
    break;

  case 11:
#line 109 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Field* > () = new Field(yystack_[3].value.as < std::string > (),yystack_[1].value.as < Type* > ());}
#line 936 "parser.cc" // lalr1.cc:919
    break;

  case 12:
#line 110 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Field* > () = new Field(yystack_[5].value.as < std::string > (),yystack_[3].value.as < Type* > (),yystack_[1].value.as < Expr* > ());}
#line 942 "parser.cc" // lalr1.cc:919
    break;

  case 13:
#line 113 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Method* > () = new Method(yystack_[6].value.as < std::string > (),yystack_[4].value.as < Formals* > (),yystack_[1].value.as < Type* > (),yystack_[0].value.as < Block* > ());}
#line 948 "parser.cc" // lalr1.cc:919
    break;

  case 14:
#line 116 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Type* > () = new Type(yystack_[0].value.as < std::string > ());}
#line 954 "parser.cc" // lalr1.cc:919
    break;

  case 15:
#line 117 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Type* > () = new Type(std::string("INT32"));}
#line 960 "parser.cc" // lalr1.cc:919
    break;

  case 16:
#line 118 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Type* > () = new Type(std::string("BOOL"));}
#line 966 "parser.cc" // lalr1.cc:919
    break;

  case 17:
#line 119 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Type* > () = new Type(std::string("STRING"));}
#line 972 "parser.cc" // lalr1.cc:919
    break;

  case 18:
#line 120 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Type* > () = new Type(std::string("UNIT"));}
#line 978 "parser.cc" // lalr1.cc:919
    break;

  case 19:
#line 123 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Formals* > () = nullptr;}
#line 984 "parser.cc" // lalr1.cc:919
    break;

  case 20:
#line 124 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Formals* > () = new Formals(yystack_[1].value.as < Formal* > (),yystack_[0].value.as < Formalx* > ());}
#line 990 "parser.cc" // lalr1.cc:919
    break;

  case 21:
#line 127 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Formalx* > () = nullptr;}
#line 996 "parser.cc" // lalr1.cc:919
    break;

  case 22:
#line 128 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Formalx* > () = new Formalx(yystack_[1].value.as < Formal* > (),yystack_[0].value.as < Formalx* > ());}
#line 1002 "parser.cc" // lalr1.cc:919
    break;

  case 23:
#line 131 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Formal* > () = new Formal(yystack_[2].value.as < std::string > (),yystack_[0].value.as < Type* > ());}
#line 1008 "parser.cc" // lalr1.cc:919
    break;

  case 24:
#line 134 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Block* > () = new Block(yystack_[2].value.as < Expr* > (),yystack_[1].value.as < Exprx* > ());}
#line 1014 "parser.cc" // lalr1.cc:919
    break;

  case 25:
#line 137 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Exprx* > () = nullptr;}
#line 1020 "parser.cc" // lalr1.cc:919
    break;

  case 26:
#line 138 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Exprx* > () = new Exprx(yystack_[1].value.as < Expr* > (),yystack_[0].value.as < Exprx* > ());}
#line 1026 "parser.cc" // lalr1.cc:919
    break;

  case 27:
#line 141 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new If(yystack_[4].value.as < Expr* > (),yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1032 "parser.cc" // lalr1.cc:919
    break;

  case 28:
#line 142 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new If(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1038 "parser.cc" // lalr1.cc:919
    break;

  case 29:
#line 143 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new While(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1044 "parser.cc" // lalr1.cc:919
    break;

  case 30:
#line 144 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Let(yystack_[4].value.as < std::string > (),yystack_[2].value.as < Type* > (),yystack_[0].value.as < Expr* > ());}
#line 1050 "parser.cc" // lalr1.cc:919
    break;

  case 31:
#line 145 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Let(yystack_[6].value.as < std::string > (),yystack_[4].value.as < Type* > (),yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1056 "parser.cc" // lalr1.cc:919
    break;

  case 32:
#line 146 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Assign(yystack_[2].value.as < std::string > (),yystack_[0].value.as < Expr* > ());}
#line 1062 "parser.cc" // lalr1.cc:919
    break;

  case 33:
#line 147 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Not(yystack_[0].value.as < Expr* > ());}
#line 1068 "parser.cc" // lalr1.cc:919
    break;

  case 34:
#line 148 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new And(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1074 "parser.cc" // lalr1.cc:919
    break;

  case 35:
#line 149 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Equal(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1080 "parser.cc" // lalr1.cc:919
    break;

  case 36:
#line 150 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Lower(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1086 "parser.cc" // lalr1.cc:919
    break;

  case 37:
#line 151 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new LowerEqual(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1092 "parser.cc" // lalr1.cc:919
    break;

  case 38:
#line 152 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Plus(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1098 "parser.cc" // lalr1.cc:919
    break;

  case 39:
#line 153 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Minus(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1104 "parser.cc" // lalr1.cc:919
    break;

  case 40:
#line 154 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Times(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1110 "parser.cc" // lalr1.cc:919
    break;

  case 41:
#line 155 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Div(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1116 "parser.cc" // lalr1.cc:919
    break;

  case 42:
#line 156 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Pow(yystack_[2].value.as < Expr* > (),yystack_[0].value.as < Expr* > ());}
#line 1122 "parser.cc" // lalr1.cc:919
    break;

  case 43:
#line 157 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Minus1(yystack_[0].value.as < Expr* > ());}
#line 1128 "parser.cc" // lalr1.cc:919
    break;

  case 44:
#line 158 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new IsNull(yystack_[0].value.as < Expr* > ());}
#line 1134 "parser.cc" // lalr1.cc:919
    break;

  case 45:
#line 159 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Function(yystack_[3].value.as < std::string > (),yystack_[1].value.as < Args* > ());}
#line 1140 "parser.cc" // lalr1.cc:919
    break;

  case 46:
#line 160 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Dot(yystack_[5].value.as < Expr* > (),yystack_[3].value.as < std::string > (),yystack_[1].value.as < Args* > ());}
#line 1146 "parser.cc" // lalr1.cc:919
    break;

  case 47:
#line 161 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new New(yystack_[0].value.as < std::string > ());}
#line 1152 "parser.cc" // lalr1.cc:919
    break;

  case 48:
#line 162 "parser.yy" // lalr1.cc:919
    {ObjID tmp = ObjID(yystack_[0].value.as < std::string > ());yylhs.value.as < Expr* > () = &tmp;}
#line 1158 "parser.cc" // lalr1.cc:919
    break;

  case 49:
#line 163 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = yystack_[0].value.as < Literal* > ();}
#line 1164 "parser.cc" // lalr1.cc:919
    break;

  case 50:
#line 164 "parser.yy" // lalr1.cc:919
    {ObjID tmp = ObjID(std::string("()")); yylhs.value.as < Expr* > () = &tmp;}
#line 1170 "parser.cc" // lalr1.cc:919
    break;

  case 51:
#line 165 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = new Parenthese(yystack_[1].value.as < Expr* > ());}
#line 1176 "parser.cc" // lalr1.cc:919
    break;

  case 52:
#line 166 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Expr* > () = yystack_[0].value.as < Block* > ();}
#line 1182 "parser.cc" // lalr1.cc:919
    break;

  case 53:
#line 169 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Args* > () = new Args();}
#line 1188 "parser.cc" // lalr1.cc:919
    break;

  case 54:
#line 170 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Args* > () = new Args(yystack_[1].value.as < Expr* > (),yystack_[0].value.as < Exprxx* > ());}
#line 1194 "parser.cc" // lalr1.cc:919
    break;

  case 55:
#line 173 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Exprxx* > () = nullptr;}
#line 1200 "parser.cc" // lalr1.cc:919
    break;

  case 56:
#line 174 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Exprxx* > () = new Exprxx(yystack_[1].value.as < Expr* > (),yystack_[0].value.as < Exprxx* > ());}
#line 1206 "parser.cc" // lalr1.cc:919
    break;

  case 57:
#line 177 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Literal* > () = new IntLit(yystack_[0].value.as < int > ());}
#line 1212 "parser.cc" // lalr1.cc:919
    break;

  case 58:
#line 178 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Literal* > () = new StrLit(yystack_[0].value.as < std::string > ());}
#line 1218 "parser.cc" // lalr1.cc:919
    break;

  case 59:
#line 179 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < Literal* > () = yystack_[0].value.as < BoolLit* > ();}
#line 1224 "parser.cc" // lalr1.cc:919
    break;

  case 60:
#line 182 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < BoolLit* > () = new BoolLit(true);}
#line 1230 "parser.cc" // lalr1.cc:919
    break;

  case 61:
#line 183 "parser.yy" // lalr1.cc:919
    {yylhs.value.as < BoolLit* > () = new BoolLit(false);}
#line 1236 "parser.cc" // lalr1.cc:919
    break;


#line 1240 "parser.cc" // lalr1.cc:919
            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -35;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
       6,    10,    46,     6,   -35,     7,   -35,   -35,   -35,    23,
     -35,    -3,    47,   -35,     8,   -35,   -35,   -35,    24,     3,
      45,    48,    60,   -35,   -35,   -35,   -35,   -35,    16,     3,
      49,    24,   -35,   -35,   102,   -35,     3,    60,   102,    77,
     -35,   102,   102,    30,    33,   -35,   102,     1,   -35,   -35,
     102,   102,   -35,   118,   -35,   -35,    67,   -35,   129,   -35,
      26,   158,    -6,    64,   -35,   171,   102,   102,   203,   -11,
     -35,   102,   102,   102,   102,   102,   102,   102,   102,   102,
      50,   -35,   102,    69,   -35,   102,     3,   102,   107,    68,
     194,   203,   212,   212,   212,   -11,   -11,    -6,    -6,    -6,
      71,   129,   -35,   183,    -7,   194,   102,   -35,   -35,   102,
     -35,   102,   102,   102,   107,    75,   194,   144,   194,   -35,
     -35,   102,   194
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,     4,     0,     1,     3,     8,     0,
       5,     0,     0,     7,     0,     9,    10,     6,    19,     0,
       0,     0,    21,    16,    15,    17,    18,    14,     0,     0,
       0,     0,    20,    11,     0,    23,     0,    21,     0,     0,
      61,     0,     0,     0,     0,    60,     0,    48,    57,    58,
       0,     0,    52,     0,    49,    59,     0,    22,    25,    50,
       0,     0,    44,     0,    47,     0,    53,     0,    33,    43,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,     0,     0,    51,     0,     0,     0,    55,     0,
      32,    34,    35,    37,    36,    38,    39,    40,    41,    42,
       0,    25,    24,    28,     0,    29,     0,    54,    45,    53,
      26,     0,     0,     0,    55,     0,    27,     0,    30,    56,
      46,     0,    31
  };

  const signed char
  parser::yypgoto_[] =
  {
     -35,   -35,   -35,    81,    73,   -35,   -35,   -35,   -27,   -35,
      51,    55,    34,   -12,   -34,   -16,   -17,   -35,   -35
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     2,     3,     4,    10,    11,    15,    16,    28,    21,
      32,    22,    52,    83,    88,    89,   107,    54,    55
  };

  const signed char
  parser::yytable_[] =
  {
      53,    13,    35,   112,    58,    60,    66,    61,    62,    56,
       8,    67,    65,    18,    23,    19,    68,    69,     1,    24,
       9,   113,    14,    25,    33,    26,    34,    27,    77,    78,
      79,    80,    84,    90,     5,    79,    80,    91,    92,    93,
      94,    95,    96,    97,    98,    99,     6,    12,   101,    20,
       8,   103,    29,   105,    30,    63,    36,    64,    71,   104,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    31,
      38,    86,   114,   102,   108,   100,   109,   116,   117,   118,
      38,   120,    39,    59,     7,    17,    37,   122,    57,   110,
      81,    40,    41,   115,    42,    43,    44,   119,    45,     0,
      46,     0,    47,    48,    49,    38,     0,    39,     0,     0,
      50,     0,     0,     0,   106,    51,    40,    41,     0,    42,
      43,    44,     0,    45,     0,    46,    70,    47,    48,    49,
       0,     0,     0,     0,     0,    50,     0,    82,     0,    71,
      51,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,    80,   121,     0,     0,     0,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    85,     0,     0,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    87,     0,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,    80,   111,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    -1,    -1,    75,
      76,    77,    78,    79,    80
  };

  const signed char
  parser::yycheck_[] =
  {
      34,     4,    29,    10,    38,    39,     5,    41,    42,    36,
       3,    10,    46,     5,    11,     7,    50,    51,    12,    16,
      13,    28,    25,    20,     8,    22,    10,    24,    39,    40,
      41,    42,     6,    67,    24,    41,    42,    71,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    24,    82,    25,
       3,    85,     7,    87,     6,    25,     7,    24,    32,    86,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     9,
       3,     7,   106,     4,     6,    25,     5,   111,   112,   113,
       3,     6,     5,     6,     3,    12,    31,   121,    37,   101,
      56,    14,    15,   109,    17,    18,    19,   114,    21,    -1,
      23,    -1,    25,    26,    27,     3,    -1,     5,    -1,    -1,
      33,    -1,    -1,    -1,     7,    38,    14,    15,    -1,    17,
      18,    19,    -1,    21,    -1,    23,     8,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,     8,    -1,    32,
      38,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    28,    -1,    -1,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    29,    -1,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    34,    35,    36,    37,
      38,    39,    40,    41,    42
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    12,    44,    45,    46,    24,     0,    46,     3,    13,
      47,    48,    24,     4,    25,    49,    50,    47,     5,     7,
      25,    52,    54,    11,    16,    20,    22,    24,    51,     7,
       6,     9,    53,     8,    10,    51,     7,    54,     3,     5,
      14,    15,    17,    18,    19,    21,    23,    25,    26,    27,
      33,    38,    55,    57,    60,    61,    51,    53,    57,     6,
      57,    57,    57,    25,    24,    57,     5,    10,    57,    57,
       8,    32,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    55,     8,    56,     6,    29,     7,    30,    57,    58,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      25,    57,     4,    57,    51,    57,     7,    59,     6,     5,
      56,    31,    10,    28,    57,    58,    57,    57,    57,    59,
       6,    28,    57
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    43,    44,    45,    45,    46,    46,    47,    48,    48,
      48,    49,    49,    50,    51,    51,    51,    51,    51,    52,
      52,    53,    53,    54,    55,    56,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    59,    60,    60,    60,
      61,    61
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     3,     5,     3,     0,     2,
       2,     4,     6,     7,     1,     1,     1,     1,     1,     0,
       2,     0,     3,     3,     4,     0,     3,     6,     4,     4,
       6,     8,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     4,     6,     2,     1,     1,
       2,     3,     1,     0,     2,     0,     3,     1,     1,     1,
       1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "LBRACE", "RBRACE", "LPAR",
  "RPAR", "COLON", "SEMICOLON", "COMMA", "ASSIGN", "BOOL", "CLASS",
  "EXTENDS", "FALSE", "IF", "INT32", "ISNULL", "LET", "NEW", "STRING",
  "TRUE", "UNIT", "WHILE", "TYPE_IDENTIFIER", "OBJECT_IDENTIFIER",
  "INTEGER_LITERAL", "STRING_LITERAL", "IN", "THEN", "DO", "ELSE", "AND",
  "NOT", "EQUAL", "LOWER_EQUAL", "LOWER", "PLUS", "MINUS", "TIMES", "DIV",
  "POW", "DOT", "$accept", "program", "classes", "class", "class_body",
  "field-method", "field", "method", "type", "formals", "formalx",
  "formal", "block", "exprx", "expr", "args", "exprxx", "literal",
  "boolean-literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    90,    90,    93,    94,    97,    98,   101,   104,   105,
     106,   109,   110,   113,   116,   117,   118,   119,   120,   123,
     124,   127,   128,   131,   134,   137,   138,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   169,   170,   173,   174,   177,   178,   179,
     182,   183
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1732 "parser.cc" // lalr1.cc:1242
#line 185 "parser.yy" // lalr1.cc:1243


void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}