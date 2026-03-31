%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node
{
    char val[10];
    struct node *left;
    struct node *right;
}node;

node* newnode(char *v,node *l,node *r)
{
    node *t=(node*)malloc(sizeof(node));
    strcpy(t->val,v);
    t->left=l;
    t->right=r;
    return t;
}

void preorder(node *t)
{
    if(t!=NULL)
    {
        printf("%s ",t->val);
        preorder(t->left);
        preorder(t->right);
    }
}

node *root;

void yyerror(char *s);
int yylex();
%}

%union{
    struct node *node;
}

%token ID
%type <node> expr term factor

%%

expr : expr '+' term { $$ = newnode("+",$1,$3); root=$$; }
     | expr '-' term { $$ = newnode("-",$1,$3); root=$$; }
     | term { $$ = $1; root=$$; }
     ;

term : term '*' factor { $$ = newnode("*",$1,$3); }
     | term '/' factor { $$ = newnode("/",$1,$3); }
     | factor { $$ = $1; }
     ;

factor : '(' expr ')' { $$ = $2; }
       | ID { $$ = newnode("id",NULL,NULL); }
       ;

%%

void yyerror(char *s)
{
    printf("Invalid Expression\n");
}

int main()
{
    printf("Enter Expression:\n");
    yyparse();

    printf("\nAbstract Syntax Tree (Prefix):\n");
    preorder(root);
    printf("\n");

    return 0;
}
