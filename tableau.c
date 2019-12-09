#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50; /*maximum formula length*/
int inputs =10;/* number of formulas expected in input.txt*/
int ThSize=100;/* maximum size of set of formulas, if needed*/
int TabSize=500; /*maximum length of tableau queue, if needed*/

//Use binary tree to visualise a tableau
struct tableau {
    char *content;
    struct tableau *left;
    struct tableau *right;
    struct tableau *parent;
};

char prop[3] = {"pqr"};
char bc[3] = {"v>^"};

/*You need to change this next bit and include functions for parse, closed and complete.*/

//Check what is in front and back of a char to ensure it is within the definition
int front_back(char c, char before, char after){
    int i, k;
    for(i = 0; i < 3; i++){
        if(prop[i] == c){
            for(k = 0; k < 3; k++){
                if(prop[k] == before || prop[k] == after || '-' == after){return 0;}
            }
        }
        else if(bc[i] == c){
            for(k = 0; k < 3; k++){
                if(bc[k] == before || bc[k] == after || '-' == before){return 0;}
            }
        };
    }
    return 1;
}

//check if the char after is what we doesn't want
int check_after(char c,char after, char d, char *arr){
    int i;
    for(i = 0; i < 3; i++){
        if(arr[i] == c && d == after){return 0;}
    }
    return 1;
}

//return 1 if the char is a binary connective
int in_bc(char c){
    int i;
    for(i = 0; i < 3; i++){
        if(bc[i] == c){return 1;}
    }
    return 0;
}

//return 1 if the char is a proposition
int check_prop(char c){
    int i;
    for(i = 0; i < 3; i++){
        if(c == prop[i]){return 1;}
    }
    return 0;
}

//check whether a binary formula is within the definition
int check_fmla(char *c){
    if(c[0] != '(' || strlen(c) < 5){return 0;}
    int parenCount = 0;
    int openParenCount = 0;
    int bcCount = 0;
    int i;
    for(i = 0; i < strlen(c); i++){
        if(c[i] == '('){
            parenCount += 1;
            openParenCount += 1;
        }
        else if(c[i] == ')'){parenCount -= 1;}
        else if(in_bc(c[i])){bcCount += 1;}
        else if (i > 0 && i < (strlen(c) - 1)) {
            if(front_back(c[i], c[i-1], c[i+1]) == 0){return 0;}
            else if(check_after(c[i], c[i+1], '(', prop) == 0){return 0;}
            else if(check_after(c[i], c[i+1], ')', bc) == 0){return 0;}
        }
        else if(i < strlen(c) - 1){
            if(c[i] == '('){
                if(in_bc(c[i+1])){return 0;}
                else if(c[i+1] == ')'){return 0;}
            }
            else if(c[i] == ')'){
                if(check_prop(c[i+1])){return 0;}
                else if(c[i+1] == '('){return 0;}
                else if(c[i-1] == '-' || c[i+1] == '-'){return 0;}
            }
        }
    }
    if(openParenCount != bcCount){return 0;}
    else if(parenCount == 0){return 3;}
    return 0;
}

//parse the formula
int parse(char *g){
    int len = strlen(g);
    if(len == 1){
        return check_prop(g[0]);
    }
    if(len > 1){
        if(g[0] == '-'){
            int i;
            for(i = 1; i < strlen(g); i++) {
                if(g[i] != '-') {
                    if (check_prop(g[i]) == 1) { return 2; }
                    else {
                        char *h = (char*)malloc(strlen(g) * sizeof(char));
                        strncpy(h, g + i, len - i);
                        if (check_fmla(h) == 3) {return 2; };
                        return 0;
                    }
                }
            }
        }
        return check_fmla(g);
    }
    return 0;
}

//find the index of the binaray connective which serves at the split point
int check_bc(char *g){
    int parenCount = 0;
    int i;
    for(i = 0; i < strlen(g); i++){
        if(g[i] == '('){
            parenCount += 1;
        }
        else if(g[i] == ')'){parenCount -= 1;}
        else if(in_bc(g[i]) && parenCount == 1){
            return i;
        }
    }
    return 0;
}

//get the first part of a binary formula
char *partone(char *g){
    int index = check_bc(g);
    char *first = (char*)malloc(index * sizeof(char));
    int i;
    for(i = 0; i < (index - 1); i++){
        first[i] = g[i+1];
    }
    first[index - 1] = '\0';
    return first;
}

//get the second part of binary formula
char *parttwo(char *g){
    int index = check_bc(g);
    char *second = (char*)malloc((strlen(g) - index - 1) * sizeof(char));
    int i;
    for(i = 0; i < (strlen(g) - index - 2); i++){
        second[i] = g[index + 1 + i];
    }
    second[strlen(g) - index - 2] = '\0';
    return second;
}

//get the substring after the first char
char *tail(char *g){
    char *result = (char*) malloc(strlen(g) * sizeof(char));
    int i;
    for(i = 0; i < strlen(g) - 1; i++){
        result[i] = g[i+1];
    }
    result[strlen(g)-1] = '\0';
    return result;
}

//get the binary connective which serves as the splitting point
char get_bc(char *g){
    return g[check_bc(g)];
}

//0 for not a formula, 1 for literal, 2 for alpha, 3 for double negation, 4 for beta
int get_type(char *g){
    if(parse(g) == 1 || (parse(g) == 2 && parse(tail(g)) == 1)) return 1;
    else if(parse(g) == 2 && parse(tail(g)) == 2) return 3;
    else if(parse(g) == 2 && parse(tail(g)) == 3){
        if(get_bc(g) == 'v') return 2;
        else if(get_bc(g) == '^') return 4;
        else if(get_bc(g) == '>') return 2;
    }
    else if(parse(g) == 3){
        if(get_bc(g) == 'v') return 4;
        else if(get_bc(g) == '^') return 2;
        else if(get_bc(g) == '>') return 4;
    }
    return 0;
}

//initiate a new left node
void new_left(struct tableau *tab, char *g){
    struct tableau *result = malloc(sizeof(struct tableau));
    result -> content = g;
    result -> parent = tab;
    result -> left = NULL;
    result -> right = NULL;
    tab -> left = result;
}

//initiate a new right node
void new_right(struct tableau *tab, char *g){
    struct tableau *result = malloc(sizeof(struct tableau));
    result -> content = g;
    result -> parent = tab;
    result -> left = NULL;
    result -> right = NULL;
    tab -> right = result;
}

//by default expand to the left. Helper function for alpha
void add_left(struct tableau *tab, char *g){
    if(tab -> left == NULL) new_left(tab, g);
    else add_left(tab -> left, g);
    if(tab->right != NULL) add_left(tab -> right, g);
}

//negate a formula
char *negate(char *g){
    char *result = (char*) malloc((strlen(g) + 2) * sizeof(char));
    result[0] = '-';
    int i;
    for(i = 1; i < strlen(g) + 1; i++){
        result[i] = g[i-1];
    }
    result[strlen(g) + 1] = '\0';
    return result;
}

//alpha expansion
void alpha(struct tableau *tab, char *g){
    if(get_type(g) == 2 && get_bc(g) == '^'){
        add_left(tab, partone(g));
        add_left(tab, parttwo(g));
    }
    else if(get_type(g) == 2 && get_bc(g) == 'v'){
        add_left(tab, negate(partone(tail(g))));
        add_left(tab, negate(parttwo(tail(g))));
    }
    else if(get_type(g) == 2 && get_bc(g) == '>'){
        add_left(tab, partone(tail(g)));
        add_left(tab, negate(parttwo(tail(g))));
    }
    else if(get_type(g) == 3){
        add_left(tab, tail(tail(g)));
    }
}

//beta expansion
void beta(struct tableau *tab, char *g){
    if(tab -> left == NULL){
        if(get_bc(g) == 'v'){
            new_left(tab, partone(g));
            new_right(tab, parttwo(g));
        }
        else if(get_bc(g) == '^'){
            new_left(tab, negate(partone(tail(g))));
            new_right(tab, negate(parttwo(tail(g))));
        }
        else if(get_bc(g) == '>'){
            new_left(tab, negate(partone(g)));
            new_right(tab, parttwo(g));
        }
    }
    else{
        beta(tab -> left, g);
        if(tab -> right != NULL) beta(tab -> right, g);
    }
}

//expand
void expand_tab(struct tableau *tab){
    char *g = tab -> content;
    if(get_type(g) == 2 || get_type(g) == 3) alpha(tab, g);
    else if(get_type(g) == 4) beta(tab, g);
}

//complete
void complete(struct tableau *tab){
    if(tab != NULL){
        expand_tab(tab);
        complete(tab -> left);
        complete(tab -> right);
    }
}

//helper fucntion for closed to check if there exist prop and -prop in the same branch
int check_child(struct tableau *tab, int *lits, int *neglits){
    int newLits[3], newNeglits[3];
    memcpy(newLits, lits, sizeof(int) * 3);
    memcpy(newNeglits, neglits, sizeof(int) * 3);
    char *content = tab -> content;
    int i;
    if(parse(content) == 1){
        for(i = 0; i < 3; i++){
            if(content[0] == prop[i]){
                newLits[i] = 1;
            }
        }
    }
    else if(parse(content) == 2 && parse(tail(content)) == 1){
        for(i = 0; i < 3; i++){
            if(content[1] == prop[i]){
                newNeglits[i] = 1;
            }
        }
    }
    for(i = 0; i < 3; i++){
        if(newLits[i] == 1 && newNeglits[i] == 1) return 1;
    }

    if(tab -> left == NULL && tab -> right == NULL) return 0;
    else if(tab -> left != NULL && tab -> right == NULL) return check_child(tab -> left, newLits, newNeglits);
    else if(tab -> left != NULL && tab -> right != NULL) return check_child(tab -> left, newLits, newNeglits) && check_child(tab -> right, newLits, newNeglits);
    return 0;
}

//return 1 if it is closed, 0 otherwise
int closed(struct tableau *tab) {
    //p,q,r (literals)
    int lits[3] = {0,0,0};
    //-p,-q,-q (negated literals)
    int neglits[3] = {0,0,0};
    check_child(tab, lits, neglits);
}

int main(){

    char *name = malloc(Fsize);
/*You should not need to alter the input and output parts of the program below.*/


 /*input 10 strings from "input.txt" */
    FILE *fp, *fpout;

  /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

    int j;

    for(j=0;j<inputs;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        switch (parse(name))
        {
            case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
            case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
            case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
            default:fprintf(fpout, "What the f***!  ");
        }



        if (parse(name)!=0)
        {

            struct tableau t;
            t.content = name;
            t.parent = NULL;
            t.left = NULL;
            t.right = NULL;

            complete(&t);

            if (closed(&t))  fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }


    fclose(fp);
    fclose(fpout);
    free(name);

  return(0);
}





