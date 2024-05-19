#include <stdio.h>
#include <stdlib.h>
#include "AST.h" 
#include <string.h>
#include <math.h>

/* create an AST from a root value and two AST sons */
AST_expr new_binary_expr(char rule, AST_expr left, AST_expr right) {
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule=rule;
    t->left=left;
    t->right=right;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST from a root value and one AST son */
AST_expr new_unary_expr(char rule, AST_expr son)
{
  return new_binary_expr(rule, NULL,son);
}

/* create an AST leaf from a value */
AST_expr new_number_expr(double number){
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule='N';
    t->number=number;
    t->left=NULL;
    t->right=NULL;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST leaf from a boolean */
AST_expr new_boolean_expr(int boolean){
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule='B';
    t->boolean=boolean;
    t->left=NULL;
    t->right=NULL;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST leaf from a value */
AST_comm new_command(char rule, AST_expr expression){
  AST_comm t =  malloc(sizeof(struct _command_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule = rule;
    
    if(rule == 'c'){
      t->expr1 = expression;
    }
    if(rule == 'i'){
   
      t->expr1 = NULL;
    }
    
    
  } else printf("ERR : MALLOC ");
  return t;

}


/* create an AST leaf from a value */
AST_prog new_prog(AST_comm com1, AST_prog next){
  AST_prog t =  malloc(sizeof(struct _prog_tree));
  
  if (t!=NULL){	/* malloc ok */
    //t->rule = 'p';
  
    t->com1 = com1;
    t->next = next;
    
  } else printf("ERR : MALLOC function new prog");
  return t;

}

/* delete an AST */
void free_expr(AST_expr t)
{
  if (t!=NULL) {
    free_expr(t->left);
    free_expr(t->right);
    free(t);
  }
}
void free_comm(AST_comm t)
{
  if (t!=NULL) {
    free_expr(t->expr1);
    free(t);
  }
}

void free_prog(AST_prog t){
  if(NULL != t){
    free_comm(t->com1);
    free(t->next);
    free(t);
  }
}

/* infix print an AST*/
void print_expr(AST_expr t){
  if (t!=NULL) {
    printf("[ ");
    print_expr(t->left);

    if(t->rule=='N'){
      if(t->number == 0.0){
        printf(":%.3lf: ",t->number);
      }
      else if(t->number >= 1000.0 || t->number <= 0.001){
        printf(":%.3e: ",t->number);
      }
      else{
        printf(":%.3lf: ",t->number);
      }
      
    }
    else if(t->rule=='B'){
      if(t->boolean==1){
        printf(":True: ");
      }
      else{
        printf(":False: ");
      }
    }
    else{
      printf(":%c: ",t->rule);
    }

    print_expr(t->right);
    printf("] ");
  }
  else{
    
  }
}

void print_comm(AST_comm t){
  
  if (t!=NULL && t->rule != 'i') {
    printf("[ ");
    printf(":: ");
    print_expr(t->expr1);
    printf("] ");
  }

}

void print_prog(AST_prog t){
  if (t!=NULL) {
    
    printf("[ "); 
    printf(":%c: ", t->rule);
    print_comm(t->com1);
    print_prog(t->next);
    
     
    printf("] ");
  }
  
}

