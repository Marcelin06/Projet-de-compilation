
/* unary-and-binary tree structure */
struct _expr_tree {
  char rule;                    /* "name" of the rule/operation operation */
  double number;                /* double  for value */
  int is_it_calculable;          /*pour savoir si on peut l'expr, si ce n'est pas une variable*/
  int boolean;                  /* 1 for true 0 for false*/
  char *ident;                   /* terminal iddentifiant*/
  int taille;                   /* taille du noeud*/
  double value;                 /*value of the expression if it does not contain variables and if it is a number*/
  struct _expr_tree* left;      /* NULL if unary node or leaf*/
  struct _expr_tree* right;     /* used for unary node but NULL if leaf */
};

typedef struct _expr_tree* AST_expr;

struct _command_tree {
  /* "name" of the rule/operation operation 
  *'c' for command like : expression ;
  *'i' for command like : import IDENT;
  *'w' for command like : while exprssion command
  *'d' for command like : do dommand while expreesion
  *'f' for command like : if expression command else command
  */
  char rule;                    
  int taille;                    /* taille du noeud*/
  struct _expr_tree* expr1;      /* used for command with at least one sub-expression or for do_while and while command*/
  struct _expr_tree* expr_if;    /* used for the expression inside the ifthenelse*/
  struct _command_tree* com1;    /* used for the first command to evaluate inside the if_the_else command or for while and do_while command*/
  struct _command_tree* com2;    /* used for the 2nd command to evaluate inside the if_then_else command*/
};

typedef struct _command_tree* AST_comm;

struct _prog_tree {
  char rule;                    /* "name" of the rule/operation operation*/
  int taille;                   /* taille du noeud*/
  struct _command_tree* com1;                /* first command of the programm, NULL if any*/
  struct _prog_tree* next;      /* list of program after the first command, NULL if any */
};

typedef struct _prog_tree* AST_prog;

/* create an AST from a root value and two AST sons */
AST_expr new_binary_expr(char rule, AST_expr left, AST_expr right);

/* create an AST from a root value and one AST son */
AST_expr new_unary_expr(char rule, AST_expr son);

/* create an AST leaf from a value */
AST_expr new_number_expr(double number);

/* create an AST leaf from a boolean */
AST_expr new_boolean_expr(int boolean);

/* create an AST leaf from a value */
AST_expr new_ident_expr(char *ident);

/* create an AST leaf from a value */
AST_comm new_command(char rule, AST_expr expression);

/* create an AST leaf from a value */
AST_comm new_if_then_else_command(char rule, AST_expr expr_if, AST_comm com1, AST_comm com2);

/* create an AST leaf from a value */
AST_comm new_while_command(char rule, AST_expr expr1, AST_comm com1);

/* create an AST leaf from a value */
AST_comm new_do_while_command(char rule, AST_comm com1,AST_expr expr1);

/* create an AST leaf from a value */
AST_prog new_prog(AST_comm com1, AST_prog next);

/* delete an AST */
void free_expr(AST_expr t);
void free_comm(AST_comm t);
void free_prog(AST_prog t);

/* print an AST*/
void print_expr(AST_expr t);
void print_comm(AST_comm t);




void print_prog(AST_prog t);

