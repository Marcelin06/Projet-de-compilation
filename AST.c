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
        t->value = -(t->right->value);
        t->is_it_calculable = 1;
        t->taille--;
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
        t->taille -=2;
      }
    }

    if('-' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value - t->left->value;
        t->is_it_calculable = 1;
        t->taille -=2;
      }
    }

    if('*' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->right->value * t->left->value;
        t->is_it_calculable = 1;
         t->taille -=2;
      }
    }
  
    if('/' == rule){
      if((1 == t->right->is_it_calculable) && (1 == t->left->is_it_calculable)){
        t->value = t->left->value / t->right->value;
        t->is_it_calculable = 1;
         t->taille -=2;
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
AST_comm new_if_then_else_command(char rule, AST_expr expr_if, AST_comm com1, AST_comm com2){
  AST_comm t =  malloc(sizeof(struct _command_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule = rule;
    t->expr_if = expr_if;
    t->com1 = com1;
    t->com2 = com2;
    t->taille = 2 + expr_if->taille + com1->taille + com2->taille;    
    
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
    free_prog(t->next);
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
  
  if (t!=NULL) {
    if('c' == t->rule){
      printf("[ ");
      //printf(":%c: ", t->rule);
      print_expr(t->expr1);
      printf("] \n");
    }

    if('f' == t->rule){
      printf("[ if( ");

      print_expr(t->expr_if);
      printf(")\n\t");
      print_comm(t->com1);
      printf("Else");
      printf("\n\t");
      print_comm(t->com2);
      printf("] \n");

    }
  }

}


/* affichage post-fix du code assembleur d'une expression*/
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
    if(1 == t->is_it_calculable){
      if(t->value >= 1000.0 || t->value <= 0.001){
        printf("\nCsteNb %.3e",t->value);
      }
      else{
       printf("\nCsteNb %.3lf",t->value);
      }
    }
    else{
      affichage_code(t->left);
      affichage_code(t->right);
      printf("\nAddiNb");
    }
    
  }

  if(t->rule == '*'){
    if(1 == t->is_it_calculable){
      if(t->value >= 1000.0 || t->value <= 0.001){
        printf("\nCsteNb %.3e",t->value);
      }
      else{
       printf("\nCsteNb %.3lf",t->value);
      }
    }
    else{
      affichage_code(t->left);
      affichage_code(t->right);
      printf("\nMultNb");
    }
  }

  if(t->rule == '-'){
    if(1 == t->is_it_calculable){
      if(t->value >= 1000.0 || t->value <= 0.001){
        printf("\nCsteNb %.3e",t->value);
      }
      else{
       printf("\nCsteNb %.3lf",t->value);
      }
    }
    else{
      affichage_code(t->left);
      affichage_code(t->right);
      printf("\nSubiNb");
    }
  }

  if(t->rule == '/'){
    if(1 == t->is_it_calculable){
      if(t->value >= 1000.0 || t->value <= 0.001){
        printf("\nCsteNb %.3e",t->value);
      }
      else{
       printf("\nCsteNb %.3lf",t->value);
      }
    }
    else{
      affichage_code(t->left);
      affichage_code(t->right);
      printf("\nDiviNb");
    }
  }

  if(t->rule == '%'){
    affichage_code(t->left);
    affichage_code(t->right);
    printf("\nModuNb");
  }

  if((t->rule == 'M')){
    if(1 == t->is_it_calculable){
      if(t->value >= 1000.0 || t->value <= 0.001){
        printf("\nCsteNb %.3e",t->value);
      }
      else{
        printf("\nCsteNb %.3lf",t->value);
      }
    }
    else{
      affichage_code(t->right);
      printf("\nNegaNb");
    }
    
    
    
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


/* affichage post-fix du code assembleur d'une commande*/
void affichage_code_com(AST_comm c){
  if(NULL == c){
    return;
  }

  if('c' == c->rule){
    affichage_code(c->expr1);
  }

  if('f' == c->rule){
    affichage_code(c->expr_if);
    printf("\nCondJmp %d", c->com1->taille + 1);
    affichage_code_com(c->com1);
    printf("\nJump %d", c->com2->taille);
    affichage_code_com(c->com2);
  }
}

/* affichage post-fix du code assembleur d'un programme*/
void affichage_code_prog(AST_prog p){
  if(NULL == p || NULL == p->com1){
    return;
  }

  affichage_code_com(p->com1);
  
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

