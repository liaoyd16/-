/*
 备注：
 0、本程序用于列写一个命题公式的真值表
 1、命题公式输入规范（合式公式）：
    1.1、命题联结词对照：
        1、and: &
        2、or: v
        3、not: ~
        4、imply: >
    1.2、形成规则（设A是任意合式公式）
        1、单个字母的原子命题(如：'p')是合式公式
        2、(~A)是合式公式
        3、(AvA')是合式公式
        4、(A&A')是合式公式
        5、(A>A')是合式公式
 2、程序允许量级：
    2.1、语句长度：100
    2.2、命题变项个数：10
 */

#include <iostream>
using namespace std;

const int MAXN = 100;

/*tree*/
int pos_of_conj(char sent[MAXN],int l,int r);//if no conj exists(l==r): return proposition/find upmost conj
struct node{
    char cont;
    node* son[2];
};
node* tree(char sent[MAXN],int l,int r,node* dest);
bool is_conj(char c);//is conjunction or not
int arity(char c);//arity

/*variable_counts*/
struct v_t{
    char v;
    bool v_truth;
};
void subt_var(char sent[MAXN],v_t*var_truth);//从语句中提取命题变项

/*iterate*/
void iter(node*root,v_t*var_truth,int k);

    /*get truth from given assignments*/
bool get_truth(node*root,v_t*var_truth);

    /*output*/
void print_table(v_t*var_truth,bool value);

int main(){
    char sent[MAXN];cin >> sent;
    int l = strlen(sent);
    
    //structurize sentence to a tree
    node nodes[MAXN];
    node*root = tree(sent, 0, l-1, nodes);
    
    //subtract variables
    v_t var_truth[10];
    subt_var(sent,var_truth);
    
    //get truth
    iter(root,var_truth,0);
    
    return 0;
}

bool is_conj(char c){
    if(c=='v'||c=='&'||c=='~'||c=='>'){return 1;}
    return 0;
}//c
int arity(char c){
    if(c=='~'){return 1;}
    if(c=='v'||c=='&'||c=='>'){return 2;}
    return -1;
}//c
int pos_of_conj(char sent[MAXN],int l,int r){
    if(l==r){return l;}
    int k = 0;
    for(int i = l;i<=r;i++){
        if(sent[i]=='(') k++;
        if(sent[i]==')') k--;
        if(k==1&&is_conj(sent[i])){
            return i;
        }
    }
    cout << "error: conj/prop not returned" << endl;
    return -1;
}
node* tree(char sent[MAXN],int l,int r,node* dest){
    int post = pos_of_conj(sent, l, r);
    int seek = 0;
    for(;seek<=MAXN;seek++){
        if(dest[seek].cont==0){break;}
    }
    dest[seek].cont = sent[post];
    
    if( is_conj(sent[post]) ){
        if(arity(sent[post])==2){
            dest[seek].cont = sent[post];
            dest[seek].son[0] = tree(sent, l+1, post-1, dest);
            dest[seek].son[1] = tree(sent, post+1, r-1, dest);
        }
        if(arity(sent[post])==1){
            dest[seek].cont = sent[post];
            dest[seek].son[0] = tree(sent, l+2, r-1, dest);
        }
    }else{
        dest[seek].cont = sent[post];
        dest[seek].son[0] = NULL,dest[seek].son[1] = NULL;
    }
    
    return dest+seek;
}

void subt_var(char sent[MAXN],v_t*var_truth){
    int len = strlen(sent);
    int g = 0;
    for(int seek = 0;seek<len;seek++){
        if(is_conj(sent[seek])||sent[seek]=='('||sent[seek]==')'){continue;}
        
        bool added = 0;
        for(int r = 0;r<g;r++){
            if(var_truth[r].v==sent[seek]){
                added = 1;break;
            }
        }
        if(added){continue;}
        
        var_truth[g].v = sent[seek];g++;
    }
}//c

void iter(node*root,v_t*var_truth,int k){
    if(var_truth[k].v==0){
        print_table(var_truth, get_truth(root, var_truth));
        return;
    }
    var_truth[k].v_truth = 0;
    iter(root,var_truth, k+1);
    var_truth[k].v_truth = 1;
    iter(root,var_truth, k+1);
    var_truth[k].v_truth = 0;
    return;
}

bool get_truth(node*root,v_t*var_truth){
    //root node is a conj
    if(is_conj(root->cont)){
        if(arity(root->cont)==1){//'~'
            return !(get_truth(root->son[0],var_truth));
        }else{//'&' 'v' '>'
            if(root->cont=='&'){
                return get_truth(root->son[0],var_truth)&&get_truth(root->son[1],var_truth);
            }
            if(root->cont=='v'){
                return get_truth(root->son[0],var_truth)||get_truth(root->son[1],var_truth);
            }
            if(root->cont=='>'){
                return !( get_truth(root->son[0],var_truth) )||get_truth(root->son[1],var_truth);
            }
        }
    }
    //root node is a variable
    for(int seek = 0;seek<10;seek++){
        if(var_truth[seek].v==root->cont){
            return var_truth[seek].v_truth;
        }
    }
    cout << "error: proposition not found" << endl;
    return 0;
}

void print_table(v_t*var_truth,bool value){
    for(int i = 0;i<10;i++){
        if(var_truth[i].v==0){break;}
        cout << var_truth[i].v_truth << "\t";
    }
    cout << value << endl;
}
