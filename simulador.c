#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>
#include <string.h>

/* Estruturas */




//Estrutura dos Eventos

typedef struct evento{
    char Nome[10];
    int time_delay;
    float prob;
    bool re;
    char newstate[30];
}Evento;



//Estrutura de Cada Vértice do Grafo

typedef struct pessoa{
    char state;
}Pessoa;



//Estrutura do Grafo

typedef struct TGrafo{
  Pessoa *Vertice;
  int qv;               //quantidade de vértices
  int **matrizadj; //matriz de adjacência alocada dinamicamente
}Grafo;



//Estrutura da Fila de Prioridades

typedef struct fp{
    Evento *X;
    int n, tam;
}Fila_Prioridades;






/* Fila De Prioridade - Funções*/




//Função que aloca um espaço na memória para criar uma fila de prioridades

Fila_Prioridades* Cria_Filap(int tam){
    Fila_Prioridades* fp = (Fila_Prioridades*)malloc(tam * sizeof(Evento));
    fp->X = (Evento*)malloc(tam * sizeof(Evento));
    fp->n = 0;
    fp->tam = tam;
    return fp;
}



//Função que ordena uma fila de prioridades pelo método de "inserction sort"

void inserction_sort(Fila_Prioridades *fp){
    for(int i=1;i<fp->n;i++){
        for(int j=i;j>=0;j--){
            if(fp->X[j-1].time_delay > fp->X[j].time_delay){
                Evento aux = fp->X[j-1];
                fp->X[j-1] = fp->X[j];
                fp->X[j] = aux;   
            }
        }
    }
}



//Função que insere um evento na fila de prioridades e em seguida a ordena

void insere_fp(Fila_Prioridades* fp, Evento X){
    fp->X[fp->n] = X;
    fp->n++;    
    inserction_sort(fp);
}



//Função que remove um evento na fila de prioridades

void remove_fp(Fila_Prioridades *fp){
    for(int i=0;i<fp->n-1;i++){
        fp->X[i] = fp->X[i+1];
    }
    fp->n--;
}



//Função que Insere novamente um evento caso ele se tenha que se repetir

void reinsere_fila(Fila_Prioridades *fp){
    Evento aux = fp->X[0];
    aux.time_delay += fp->X[0].time_delay;
    remove_fp(fp);
    insere_fp(fp,aux);

}



//Função que libera a memória alocada para a fila de prioridades

void liberafila(Fila_Prioridades *fp){
    free(fp->X);
    free(fp);
}





/*Grafo - Funções*/




//Função que aloca um espaço na memória para criar um grafo

Grafo* criaGrafo(int qv){
    Grafo* g = (Grafo*) malloc(sizeof(Grafo)); 
    g->qv = qv; 
    //alocando o vetor com as informacoes de cada vertice
    g->Vertice = (Pessoa*)malloc(qv*sizeof(Pessoa*));
    //Aloca a matriz |V|x|V| 
    g->matrizadj = (int**) malloc(qv*sizeof(int*));
    for (int i = 0; i < qv; i++)
        g->matrizadj[i] = (int*) malloc(qv*sizeof(int));
    return g; 
}



//Função que cria as ligações(arestas) do grafo

void criaAresta(Grafo* g, int u, int v){
    if (u < 0 || u >= g->qv) return;
    if (v < 0 || v >= g->qv) return;
    g->matrizadj[u][v] = 1; 
    g->matrizadj[v][u] = 1; 
}



//Função que cria um grafo do tipo grade

void create_grade_graph(Grafo* g){
    bool gradepar;
    int aux = g->qv;
    int aux2;
// como o trabalho pede o numero de vertices entre 15 e 30 , escolhi 5 como o melhor numero para teste de inicio para o numero de vertices para cada linha da grade    
    int i = 5;
// aqui é testado se a quantidade de vertices é impar ou par    
    if(aux%2==0){
        gradepar = true;
    }else{
        gradepar = false;
    }

//aqui é selecionado a melhor escolha de numero de vertices para a grade, escolhendo um divisor , comecando de 5
    
    while(true){
        // caso o qv(numero de vertices) seja impar, a grade sera da mesma maneira que seria uma grade (qv+1) porem com 1 unidade a menos
        if(gradepar == false){
            aux++;
        }


        if(aux%i == 0){
            aux2 = i;
            break;
        }
        i--;
    }

//temos um for para preencher as arestas
    for(int i=0;i<=g->qv;i++){
        if((i+1)%aux2!=0){
            criaAresta(g,i,i+1);
        }
        if((i+aux2)<=g->qv){
            criaAresta(g,i,i+aux2);
        }
    }
}



//Função que cria um grafo do tipo Small World(Grupos)

void create_smallworld_graph(Grafo *g, int gps){
    int qtd_de_gp = gps; //aqui é escolhido a quantidade de grupos que se deseja ter no grafo
    int aux = g->qv;
    int max_p_gp = aux/qtd_de_gp; //esta variavel indica qual é o maximo de vertices que se pode ter por grupo
    int i=0;
    int j;
    int seed;
    for(int i=0;i<aux;i++){
        if(i%max_p_gp == 0){ //aqui é feito as ligacoes dos vertices que ligam aos outros grupos
            seed = i;
            for(int k=i+max_p_gp;k<aux;k+=max_p_gp){
                criaAresta(g,i,k);
            }
        }
        for(int j=i+1;j<seed+max_p_gp;j++){ // aqui é feito as ligacoes dos vertices dentro de cada grupo
            criaAresta(g,i,j);
        }
    }
}



//Função que inicializa os valores dos vértices do grafo

void inicializaMatrizAdj(Grafo* g){
    for(int i = 0; i < g->qv; i++){
    //inicializa todas as pessoas de modo que fiquem com estado suscetivel
        g->Vertice[i].state = 'S'; 
        for (int j = 0; j < g->qv; j++){
            g->matrizadj[i][j] = 0;
        }
    }
}



//Função que libera a Memória Alocada do Grafo

void liberaGrafo(Grafo *g){
    for (int i = 0; i < g->qv; i++) 
        free(g->matrizadj[i]);
    free(g->matrizadj); 
    free(g); 
}



//Função que imprime a matriz de adjacência do grafo ***Função para teste apenas, já que não é necessária para o trabalho***

void imprimeMatrizAdj(Grafo* g){	
    printf("Matriz de adjacência:\n"); 
    for (int i = 0; i < g->qv; i++) {
        for (int j = 0; j < g->qv; j++)
            printf(" %d ", g->matrizadj[i][j]);
        printf("\n");
    }
}



//Função que imprime as ligações de cada vértice do grafo ***Função para teste apenas, já que não é necessária para o trabalho***

void imprimeligacoes(Grafo *g){
    for(int i=0;i< g->qv;i++){
        for(int j=0;j<g->qv;j++){
            if(g->matrizadj[i][j] == 1){
                printf("(%d) tem ligação com %d\n",i,j);
            }
        }printf("\n");
    }
}





/*Funções dos Eventos*/




//Função que cria e preenche as informações de um evento

Evento cria_evento(char *n,int td,float pb,bool repeat){
    Evento x;
    strcpy(x.Nome,n);
    x.time_delay = td;
    x.prob = pb;
    x.re = repeat;
    return x;
}



//Função que infecta um vértice pela sua posição

void infectar(Grafo* g,int id){
    g->Vertice[id].state = 'I';
}



//Função que coloca como recuperado 50% dos vertices

void recupera50prct(Grafo* g){
	int ctt=0;
	int ramdom_v;
	int i=0;
	while(i<(g->qv)/2){
		ramdom_v = rand() % g->qv;
		if(g->Vertice[ramdom_v].state == 'S' && ramdom_v != ((g->qv)/2)+1 ){
			g->Vertice[ramdom_v].state = 'R';
			i++;
		}
	}

}



//Função que retorna se o evento irá acontecer de acordo com sua probabilidade | TRUE == ACONTECERÁ / FALSE == NÃO ACONTECERÁ | Obs: não funcionará para probabilidade com duas casas decimais, ex: 55%(0.55), 88%(0.88)

bool Event_ProbT(float prob){
    int num_test1 = prob*100;
    int num_ramd = (rand() % 100) + 1;
    if(num_ramd <= num_test1){
        return true;
    }else{
        return false;	
    }
}



//Função que percorrerá o grafo inteiro para executar os eventos

void percorretest(Grafo *g, Fila_Prioridades *fp){
    //ESSE LAÇO É UTILIZADO PARA ATUALIZAR O NEWSTATE DA FILA COM O STATE ATUAL DOS VERTICES
    for(int k=0;k<g->qv;k++){
        fp->X[0].newstate[k] = g->Vertice[k].state;
    }
    //AQUI É O CASO DE O EVENTO A SER EXECUTADO SER O DE INFECCAO
    if(fp->X[0].Nome[0] == 'I'){ // STRNCMP(fp->X[0].Nome,"INFECCAO",8) NÃO FUNCIONOU, NAO SEI POR QUE, MAS ELE PEGAVA O SEGUNDO EVENTO DA FILA EM VEZ DO PRIMEIRO
        for(int i=0;i<g->qv;i++){
            if(g->Vertice[i].state == 'I'){
                for(int j=0;j<g->qv;j++){
                    if(g->matrizadj[i][j] == 1 && g->Vertice[j].state == 'S'){
                        bool Event_Success  = Event_ProbT(fp->X[0].prob);
                        if(Event_Success == true){
                            //printf("\n************INFECTOU VERTICE(%d)*********\n",j); ***Um Printf útil caso queira saber se a pessoa foi infectada***
                            fp->X[0].newstate[j] = 'I';
                        }       
                    }
                }
            }
        }
    }
    //AQUI É O CASO DE O EVENTO A SER EXECUTADO SER O DE RECUPERACAO
    else if(fp->X[0].Nome[0] == 'R'){
        for(int i=0;i<g->qv;i++){
            if(g->Vertice[i].state == 'I'){
                bool Event_Success = Event_ProbT(fp->X[0].prob);
                if(Event_Success == true){
                    //printf("\n************RECUPEROU VERTICE(%d)*********\n",i); ***Um Printf útil caso queira saber se a pessoa se recuperou***
                    fp->X[0].newstate[i] = 'R';
                }
            }
        }
    }else{
        printf("Evento Invalido");
    }
    //AQUI É ATUALIZADO OS VERTICES COM OS ESTADOS NOVOS
    for(int k=0;k<g->qv;k++){
        g->Vertice[k].state = fp->X[0].newstate[k];
    }
}



//Função que calcula e printa as % de Infectados , Recuperados e Suscetíveis, no final dos testes

void calc_porcIR(Grafo *g){
    int cttI = 0;
    int cttR = 0;
    int cttS = 0;
    for(int i=0;i<g->qv;i++){
        if(g->Vertice[i].state == 'I'){
            cttI++;
        }else if(g->Vertice[i].state == 'R'){
            cttR++;
        }else if(g->Vertice[i].state == 'S'){
            cttS++;
        }
    }
    printf("Porcentagem de infectados = %d%%\n",(cttI*100)/g->qv);
    printf("Porcentagem de recuperados = %d%%\n",(cttR*100)/g->qv);
    printf("Porcentagem de Suscetivos = %d%%",(cttS*100)/g->qv);
    printf("\n");
}



//Função que printa os estados de cada vértice

void printa_resultadosS(Grafo *g){
    for(int i=0;i<g->qv;i++){
        printf("Estado (%d) = %c\n",i,g->Vertice[i].state);
    }
}
	


//Função principal que executará todas as funções para gerar os eventos, etc...

void run(){
    int tfinal = 50;
    int t=0;
    //CRIE AQUI OS EVENTOS COM ESSE FORMATO: Evento "NOME DA VARIVAEL" = cria_evento("Nome do evento","time delay","probabilidade em float","true se ele ira se repetir; false se nao")
    Evento A = cria_evento("Infeccao",9,0.99,true);
    Evento B = cria_evento("Recuperacao",9,0.01,true);

    Fila_Prioridades* fp = Cria_Filap(2); //QUANTIDADE DE EVENTOS
    insere_fp(fp,B);
    insere_fp(fp,A);
    int qv;
    printf("Qual a quantidade de vertices do grafo:\n"); 
    scanf("%d", &qv);
    if (qv <= 0){
        printf("Digite um valor válido para a quantidade de vertices\n");
        exit(1); 
    }
    Grafo* grafo = criaGrafo(qv); 
    inicializaMatrizAdj(grafo);

    //selecionar qual tipo de grafo ira utilizar:   
    //create_grade_graph(grafo);
    create_smallworld_graph(grafo,4);// escolher quantos grupos tera o grafo


    //escolher as funcoes de recuperar50% e infectar um grafo:
    //infectar(grafo,2);
    //infectar(grafo,20);
    //infectar(grafo,15);
    //recupera50prct(grafo);


    //laço principal dos eventos
    while(t<=tfinal){
        printf("t = %d\n\n",t);
        if(fp->X[0].time_delay == t){
            //printf("Ocorre evento %s\n",fp->X[0].Nome);   ***Um Printf útil caso queira saber qual evento está sendo executado no momento***    
            percorretest(grafo,fp);
            if(fp->X[0].re == true){
                reinsere_fila(fp);
            }else{
                remove_fp(fp);
            }
        }

        printa_resultadosS(grafo);
        printf("\n\n");
        t++;
    }
    calc_porcIR(grafo);
    liberaGrafo(grafo);
}





/*Função Main*/




int main(void){
    srand(time(NULL));
    run();

    return 0; 
}	
