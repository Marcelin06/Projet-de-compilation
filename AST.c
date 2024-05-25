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
    t->is_it_calculable = 0;
    //pour les expressions : -exprression
    if(rule == 'M'){
      t->taille = 1 + t->right->taille;
      if(1 == t->right->is_it_calculable){
        t->value = t->right->value;
        t->is_it_calculable = 1;
      }
    }
    //pour les expressions : expression && expression
    if('A' == rule){
      t->taille = 3 + t->left->taille + t->right->taille;
    }
    if('=' == rule){
       
      t->taille = 1 + t->right->taille;
      
    }

    //pour les autres expressions binaires
    if('A' != rule && 'M' != rule && '=' != rule){
      t->taille = 1 + t->left->taille + t->right->taille;

    }

    if('+' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value + t->left->value;
        t->is_it_calculable = 1;
      }
    }

    if('-' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value - t->left->value;
        t->is_it_calculable = 1;
      }
    }

    if('*' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value * t->left->value;
        t->is_it_calculable = 1;
      }
    }
  
    if('/' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value / t->left->value;
        t->is_it_calculable = 1;
      }
    }

    

  } else {
    printf("ERR : MALLOC ");
  }

  
 
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
    t->value = number;
    t->is_it_calculable = 1;
    t->taille = 1;
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
    t->is_it_calculable = 0;
    t->taille = 1;
    t->left=NULL;
    t->right=NULL;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST leaf from a value */
AST_expr new_ident_expr(char *ident){
  
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule='v';
    t->ident=ident;
    t->is_it_calculable = 0;
    t->taille = 1;
    t->left=NULL;
    t->right=NULL;
  } else {
    printf("ERR : MALLOC ");
  }
  
  return t;
}

/* create an AST leaf from a value */
AST_comm new_command(char rule, AST_expr expression){
  AST_comm t =  malloc(sizeof(struct _command_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule = rule;
    
    if(rule == 'c'){
      t->expr1 = expression;
      t->taille = t->expr1->taille;
      
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
    
    if(NULL == com1 && NULL == next){
      t = NULL;
      
    }
    else if(NULL != com1 && NULL == next){
      t->taille = t->com1->taille;      
    }
    else if(NULL == com1 && NULL != next){
      t->taille = t->next->taille;
    }
    else if(NULL != com1 && NULL != next){
      t->taille = t->com1->taille + t->next->taille;    
    }
    else{}
    
    
  } else printf("ERR : MALLOC function new prog");
  return t;

}

/* delete an AST */
void free_expr(AST_expr t)
{
  if (t!=NULL) {
    free(t->ident);
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
    else if(t->rule=='v'){
      printf(":%s: ",t->ident);
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
    //printf(":%c: ", t->rule);
    print_expr(t->expr1);
    printf("] ");
  }

}


/* affichage code*/
void affichage_code(AST_expr t){
  
  /*if((NULL == t->left) && (NULL == t->right)){
    
    printf("\nCsteNB %lf", t->number);
  }*/

  
  

  if(t->rule == 'N'){
    if(t->number >= 1000.0 || t->number <= 0.001){
      printf("\nCsteNb %.3e",t->number);
    }
    else{
      printf("\nCsteNb %.3lf",t->number);
    }
  }

  if(t->rule == 'B'){
    if(t->boolean == 1){
      printf("\nCsteBo True ");
    }
    else{
      printf("\nCsteBo False ");
    }    
  }

  if(t->rule == '+'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nAddiNb");
  }

  if(t->rule == '*'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nMultNb");
  }

  if(t->rule == '-'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nSubiNb");
  }

  if(t->rule == '/'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nDiviNb");
  }

  if(t->rule == '%'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nModuNb");
  }

  if((t->rule == 'M')){
    affichage_code(t->right);
    printf("\nNegaNb");
  } 

  if(t->rule == 'E'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nEquals");
  }

  if(t->rule == 'G'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nGrEqNb");
  }

  if(t->rule == 'L'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nLoEqNb");
  }

  if(t->rule == 'D'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nNotEql");
  }

  if(t->rule == '!'){
    affichage_code(t->right);
    printf("\nNot");
  }

  if(t->rule == '<'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nLoStNb");
  }

  if(t->rule == '>'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nGrStNb");
  }

  if(t->rule == 'A'){ //si c'est un noeud &&
    affichage_code(t->left);
    printf("\nCondJmp %d", t->right->taille + 1);
    affichage_code(t->right);
    printf("\nJump 1");
    printf("\nCste False");
  }

  if(t->rule == '='){
    affichage_code(t->right);
    printf("\nSetVar %s", t->left->ident);
  }

  if(t->rule == 'v'){
    
    printf("\nGetVar %s", t->ident);
  }



  
}


void affichage_code_prog(AST_prog p){
  if(NULL == p || NULL == p->com1){
    return;
  }

  if(p->com1->rule != 'i'){
    affichage_code(p->com1->expr1);
  }
  
  affichage_code_prog(p->next);
}

void print_prog(AST_prog t){

  if (t!=NULL && t->com1 != NULL) {
  //printf(":%c: ", t->rule);
  print_comm(t->com1);
  printf("\n");
  print_prog(t->next);    
  }
  
}

