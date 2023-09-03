#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
struct bomba{
    float x,y,radius;
};

struct graph{
    int n;
    int **matrica_susednosti;
    struct bomba *bombe;
};
//zad1
int **create_graph(int n){
    int **matrica_susednosti=malloc(sizeof(int*)*n);
    for(int i=0;i<n;i++){
        matrica_susednosti[i]= calloc(n,sizeof(int));
    }
    return matrica_susednosti;
}

void print_graph(int n,int **matrica_susednosti){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",matrica_susednosti[i][j]);
        }
        putchar('\n');
    }
}

void add_node(int n,int ***matrica_susednosti_pointer){
    *matrica_susednosti_pointer= realloc(*matrica_susednosti_pointer,sizeof(int*)*(n+1));
    (*matrica_susednosti_pointer)[n]=calloc(n+1,sizeof(int));
    for(int i=0;i<n;i++){
        (*matrica_susednosti_pointer)[i]= realloc((*matrica_susednosti_pointer)[i],sizeof(int)*(n+1));
        (*matrica_susednosti_pointer)[i][n]=0;
    }
}

void delete_node(int index,int n, int ***matrica_susednosti_pointer){
    int j_1=0;
    for(int i=0;i<n;i++){
        if(i==index){
            free((*matrica_susednosti_pointer)[i]);
            continue;
        }
        (*matrica_susednosti_pointer)[j_1++]=(*matrica_susednosti_pointer)[i];
    }
    for(int i=0;i<n-1;i++){
        j_1=0;
        for(int j=0;j<n;j++){
            if(j==index){
                continue;
            }
            (*matrica_susednosti_pointer)[i][j_1++]=(*matrica_susednosti_pointer)[i][j];
        }
        (*matrica_susednosti_pointer)[i]= realloc((*matrica_susednosti_pointer)[i],sizeof(int)*(n-1));
    }
}

void add_link(int i,int j,int ***matrica_susednosti_pointer){
    (*matrica_susednosti_pointer)[i][j]=1;
}

void remove_link(int i,int j,int ***matrica_susednosti_pointer) {
    (*matrica_susednosti_pointer)[i][j] = 0;
}

void delete_graph(int ***matrica_susednosti_pointer,int n){
    for(int i=0;i<n;i++){
        free((*matrica_susednosti_pointer)[i]);
    }
    free((*matrica_susednosti_pointer));
    *matrica_susednosti_pointer=NULL;
}
//zad1end

int in_radius(float x1,float y1,float r1,float x,float y){
    float dist= sqrt(pow(x1-x,2)+pow(y1-y,2));
    if(dist<=r1){
        return 1;
    }
    return 0;

}

struct graph input_graph(FILE* input_file){
    struct graph graf;
    int n;
    int check=fscanf(input_file,"%d",&n);
    graf.n=n;
    if(!check){
        printf("GRESKA PRI UCITAVANJU");
        exit(0);
    }
    graf.matrica_susednosti= create_graph(n);
    graf.bombe=malloc(sizeof(struct bomba)*n);
    for(int i=0;i<n;i++){
        check=fscanf(input_file,"%f %f %f",&graf.bombe[i].x,&graf.bombe[i].y,&graf.bombe[i].radius);
        if(!check){
            printf("GRESKA PRI UCITAVANJU");
            exit(0);
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                continue;
            }
            if(in_radius(graf.bombe[i].x,graf.bombe[i].y,graf.bombe[i].radius,graf.bombe[j].x,graf.bombe[j].y)){
                add_link(i,j,&graf.matrica_susednosti);
            }
        }
    }
    return graf;
}

void push(int* stack,int *top,int x,int max_size){
    if((*top)==max_size-1){
        printf("STACK JE PUN");
    }
    stack[++(*top)]=x;
}

int pop(int *stack,int *top,int max_size){
    if((*top)==-1){
        printf("STACK JE PRAZAN");
    }
    return stack[(*top)--];
}

void efficency_of_mine_path(struct graph graf, int element){
    int* visited=calloc(graf.n,sizeof(int));
    int stack[graf.n],detonated[graf.n],top=-1;
    int br=1;
    push(stack,&top,element,graf.n);
    visited[element]=1;
    detonated[0]=element;
    int j=1;
    while(top!=-1){
        int next=pop(stack,&top,graf.n);
        for(int i=0;i<graf.n;i++){
            if(!visited[i] && graf.matrica_susednosti[next][i]){
                br++;
                push(stack,&top,i,graf.n);
                visited[i]=1;
                detonated[j++]=i;
            }
        }
    }
    printf("Efikasnost je:%d \ndetonirano je:",br);
    for(int i=0;i<j;i++){
        printf("%d ",detonated[i]);
    }
    putchar('\n');
    free(visited);
}

int efikasnost_mine(struct graph graf,int element){
    int* visited=calloc(graf.n,sizeof(int));
    int stack[graf.n],top=-1;
    int br=1;
    push(stack,&top,element,graf.n);
    visited[element]=1;

    while(top!=-1){
        int next=pop(stack,&top,graf.n);
        for(int i=0;i<graf.n;i++){
            if(!visited[i] && graf.matrica_susednosti[next][i]){
                br++;
                push(stack,&top,i,graf.n);
                visited[i]=1;

            }
        }
    }

    free(visited);
    return br;
}

int mina_maximalna_efikasnost(struct graph graf){
    int max_ef=0;
    int max_mina=-1;
    for(int i=0;i<graf.n;i++){
        int ef= efikasnost_mine(graf,i);
        if(ef>max_ef){
            max_ef=ef;
            max_mina=i;
        }
    }
    return max_mina;
}

void explozija_rakete(struct graph *graf,float x,float y, float r){
    add_node(graf->n,&graf->matrica_susednosti);
    graf->n+=+1;
    int index=graf->n-1;
    for(int i=0;i<graf->n;i++){
        if(i==index){
            continue;
        }
        if(in_radius(x,y,r,graf->bombe[i].x,graf->bombe[i].y)){
            add_link(index,i,&graf->matrica_susednosti);
        }
    }
    efficency_of_mine_path(*graf, index);
    delete_node(graf->n,graf->n,&graf->matrica_susednosti);
    graf->n=graf->n-1;
}

int* najduzi_put(struct graph graf,int element){
    int* visited=calloc(graf.n,sizeof(int));
    int stack[graf.n],*detonated=malloc(sizeof(int)*graf.n),top=-1;
    push(stack,&top,element,graf.n);
    visited[element]=1;
    detonated[0]=element;
    int j=1;
    while(top!=-1){
        int next=pop(stack,&top,graf.n);
        for(int i=0;i<graf.n;i++){
            if(!visited[i] && graf.matrica_susednosti[next][i]){
                push(stack,&top,i,graf.n);
                visited[i]=1;
                detonated[j++]=i;
            }
        }
    }
    detonated[j]=-1;
    free(visited);
    return detonated;

}

void povrsina_explozijom_mine(struct graph graf,int mina){
    int *put= najduzi_put(graf,mina);
    srand(time(NULL));
    float x_min=graf.bombe[0].x-graf.bombe[0].radius,x_max=graf.bombe[0].x+graf.bombe[0].radius,y_min=graf.bombe[0].y-graf.bombe[0].radius,y_max=graf.bombe[0].y+graf.bombe[0].radius;
    for(int *tek=put;*tek!=-1;tek++){
        if(graf.bombe[*tek].x+graf.bombe[*tek].radius>x_max){
            x_max=graf.bombe[*tek].x+graf.bombe[*tek].radius;
        }
        if(graf.bombe[*tek].x-graf.bombe[*tek].radius<x_min){
            x_min=graf.bombe[*tek].x-graf.bombe[*tek].radius;
        }
        if(graf.bombe[*tek].y+graf.bombe[*tek].radius>y_max){
            y_max=graf.bombe[*tek].y+graf.bombe[*tek].radius;
        }
        if(graf.bombe[*tek].y-graf.bombe[*tek].radius<y_min){
            y_min=graf.bombe[*tek].y-graf.bombe[*tek].radius;
        }
    }
    int monte_num=10000;
    int num_in_circles=0;
    for(int i=0;i<monte_num;i++){
        float x=((float)rand()/RAND_MAX)*(x_max-x_min)+x_min;
        float y=((float)rand()/RAND_MAX)*(y_max-y_min)+y_min;
        //printf("(%f %f)\n",x,y);
        for(int *tek=put;*tek!=-1;tek++){
            if(in_radius(graf.bombe[*tek].x,graf.bombe[*tek].y,graf.bombe[*tek].radius,x,y)){
                num_in_circles++;
                break;
            }
        }
    }
    float povrsina_pravougaonika=(fabsf(x_max)+ fabsf(x_min))*((fabsf(y_max)+ fabsf(y_min)));
    float povrsina_krugova=povrsina_pravougaonika*((float)num_in_circles/(float)monte_num);
    printf("Povrsina zahvacena explozijom je:%f\n",povrsina_krugova);
}
void menuzad1(){
    printf("0 - Kreiraj novi graf\n");
    printf("1 - Dodaj cvor u graf\n");
    printf("2 - Oduzmi cvor iz graaf\n");
    printf("3 - Dodaj granu u graf\n");
    printf("4 - Oduzmi granu iz grafa\n");
    printf("5 - Ispisi graf\n");
    printf("6 - Obrisi graf\n");
    printf("7 - Terminacija\n");
}
void menuzad2(){
    printf("0 - Unesi podatke iz fajla\n");
    printf("1 - Efikasnost odredjene mine + detonirane mine\n");
    printf("2 - Pronalazenje mine max efikasnosti\n");
    printf("3 - Explozija rakete\n");
    printf("4 - Ukupna povrsina pokrivena explozijom mine max efikasnosti\n");
    printf("5 - Terminacija\n");

}
int main(){
    int t=0;
    printf("Unesi broj zadatka(1,2):");
    while(t!=1 && t!=2){
        scanf("%d",&t);
        if(t!=1 && t!=2){
            printf("Nepravilan unos unesite opet\n");
        }
    }
    if(t==1){
        int **matrica_susednosti=NULL,n;
        int t1=0;
        while(1){
            menuzad1();
            int test;
            scanf("%d",&t1);
            switch(t1) {
                case 0:
                    printf("Unesite broj cvorova:\n");
                    test=scanf("%d", &n);
                    if(test!=1){
                        break;
                    }
                    matrica_susednosti = create_graph(n);
                    break;
                case 1:
                    if(!matrica_susednosti){
                        printf("Prvo inicijalizuj graf\n");
                        break;
                    }
                    add_node(n++,&matrica_susednosti);
                    break;
                case 2:
                    if(!matrica_susednosti){
                        printf("Prvo inicijalizuj graf\n");
                        break;
                    }
                    printf("Unesi index cvora koji zelis da obrise:\n");
                    int i;
                    test=scanf("%d",&i);
                    if(i>n || test!=1){
                        printf("Ne postoji taj cvor\n");
                        break;
                    }
                    delete_node(i,n--,&matrica_susednosti);
                    break;
                case 3:
                    if(!matrica_susednosti){
                        printf("Prvo inicijalizuj graf\n");
                        break;
                    }
                    printf("Unesite indexe cvorova koju granu zelite da stvorite(i,j):\n");
                    int j;
                    test=scanf("%d,%d",&i,&j);
                    if(i>n || j>n || test!=2){
                        printf("Ne postoji jedan od cvorova\n");
                        break;
                    }
                    add_link(i,j,&matrica_susednosti);
                    break;
                case 4:
                    if(!matrica_susednosti){
                        printf("Prvo inicijalizuj graf\n");
                        break;
                    }
                    printf("Unesite indexe cvorova za granu koju zelite da obrisete(i,j):\n");
                    test=scanf("%d,%d",&i,&j);
                    if(i>n || j>n || test!=2){
                        printf("Ne postoji jedan od cvorova\n");
                        break;
                    }
                    remove_link(i,j,&matrica_susednosti);
                    break;
                case 5:
                    if(!matrica_susednosti){
                        printf("Prvo inicijalizuj graf\n");
                        break;
                    }
                    print_graph(n,matrica_susednosti);
                    break;
                case 6:
                    delete_graph(&matrica_susednosti,n);
                    printf("Graf obrisan\n");
                    break;
                case 7:
                    return 0;
                default:
                    printf("Unesi validnu opciju iz meni-a\n");
                    break;
            }
        }
    }
    else if(t==2){
        int t2=0;
        struct graph graf;
        graf.matrica_susednosti=NULL;
        while(1) {
            menuzad2();
            scanf("%d",&t2);
            switch (t2) {
                case 0:
                    printf("Unesite ime datoteke iz koje se ucitavaju pocetni podaci:\n");
                    char ime[20];
                    scanf("%s", ime);
                    FILE *input = fopen(ime, "r");
                    if (!input) {
                        printf("greska");
                        return 0;
                    }
                    graf = input_graph(input);
                    fclose(input);
                    break;
                case 1:
                    if(!graf.matrica_susednosti){
                        printf("Prvo unesi graf!\n");
                        break;
                    }
                    printf("Unesi index mine za koju zelite da odredite efikasnost:\n");
                    int i;
                    scanf("%d",&i);
                    efficency_of_mine_path(graf,i);
                    break;
                case 2:
                    if(!graf.matrica_susednosti){
                        printf("Prvo unesi graf!\n");
                        break;
                    }
                    printf("Mina sa max efikasnoscu je:%d", mina_maximalna_efikasnost(graf));
                    break;
                case 3:
                    if(!graf.matrica_susednosti){
                        printf("Prvo unesi graf!\n");
                        break;
                    }
                    printf("Unesi kordinate rakete i radijus(x,y,r):\n");
                    float x,y,radius;
                    scanf("%f,%f,%f",&x,&y,&radius);
                    explozija_rakete(&graf,x,y,radius);
                    break;
                case 4:
                    if(!graf.matrica_susednosti){
                        printf("Prvo unesi graf!\n");
                        break;
                    }
                    povrsina_explozijom_mine(graf,mina_maximalna_efikasnost(graf));
                    break;
                case 5:
                    return 0;
                default:
                    printf("Unesi validnu opciju iz meni-a\n");
                    break;
            }
        }
    }
}