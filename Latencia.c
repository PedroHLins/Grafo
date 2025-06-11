#include <stdio.h>
#include <string.h>

#define MAX 8
#define INDEFINIDO 9999

typedef struct {
    char roteadores[MAX][20]; //Nome
    int latencia[MAX][MAX]; //Latencia dos roteadores x <-> y
    int num_roteadores; //Contador de roteadores
} Grafo;

int encontrar_indice(Grafo *g, char nome[]) { //Passando o nome para saber qual o indice desse roteador na matriz
    int i;
	for (i = 0; i < g->num_roteadores; i++) {
        if (strcmp(g->roteadores[i], nome) == 0) //Compara os nomes do roteadores
            return i;							 //Retorna o indice em que foi encontrado
    }
    return -1;
}

void adicionar_roteador(Grafo *g, char nome[]) {
    int i;												
	strcpy(g->roteadores[g->num_roteadores], nome); 	//Vai pegar o indice de um roteador disponivel e atribuir o nome
    for (i = 0; i <= g->num_roteadores; i++) {			//Vai deixar os roteadores ainda não conectados como 9999 de latencia
        g->latencia[g->num_roteadores][i] = INDEFINIDO;
        g->latencia[i][g->num_roteadores] = INDEFINIDO;
    }

    g->latencia[g->num_roteadores][g->num_roteadores] = 0; //Latencia do roteador até ele mesmo

    g->num_roteadores++;
}

void conectar(Grafo *g, char origem[], char destino[], int valor) {
    int i = encontrar_indice(g, origem);
    int j = encontrar_indice(g, destino);
    if (i == -1 || j == -1) {
        printf("Erro: roteador nao encontrado!\n");
        return;
    }
    
    g->latencia[i][j] = valor; //Definindo latencia bidirecional
    g->latencia[j][i] = valor;
}

void ping(Grafo *g, char origem[], char destino[]) {
	int i, j;
    int dist[MAX], visitado[MAX], anterior[MAX];
    int inicio = encontrar_indice(g, origem);
    int fim = encontrar_indice(g, destino);

    if (inicio == -1 || fim == -1) {
        printf("Erro: roteador nao encontrado!\n");
        return;
    }

    for (i = 0; i < g->num_roteadores; i++) { //Inicializa todos os valores como indefinidos e não visitados
        dist[i] = INDEFINIDO; // = a distância do roteador de origem até o roteador i
        visitado[i] = 0; //Boolean
        anterior[i] = -1;
    }

    dist[inicio] = 0; //Distância do início até ele mesmo é 0

    for (j = 0; j < g->num_roteadores - 1; j++) {
        int min = INDEFINIDO, menorDistancia = -1;
        for (i = 0; i < g->num_roteadores; i++) {
            if (!visitado[i] && dist[i] < min) {
                min = dist[i];
                menorDistancia = i;				//Indice do menor nó encontrado
            }
        }

        if (menorDistancia == -1) break;	   //Se não achar nenhum nó ele finaliza o programa
        

        visitado[menorDistancia] = 1;					  //Indica que o nó foi visitado

        for (i = 0; i < g->num_roteadores; i++) {		//Atualiza dist dos vizinhos de menorDistancia
            if (!visitado[i] && g->latencia[menorDistancia][i] != INDEFINIDO && dist[menorDistancia] + g->latencia[menorDistancia][i] < dist[i]) {
                dist[i] = dist[menorDistancia] + g->latencia[menorDistancia][i];
                anterior[i] = menorDistancia;
            }
        }
    }

    if (dist[fim] == INDEFINIDO) {
        printf("Não existe caminho entre %s e %s\n", origem, destino);
        return;
    }

    int caminho[MAX], tam = 0;
    for (i = fim; i != -1; i = anterior[i])
        caminho[tam++] = i; //Vai preenchendo o caminho ao contrário (do fim até o início) pegando os indices

    printf("Caminho com menor latência: ");
    for (i = tam - 1; i >= 0; i--)     //Imprime ao contrario
        printf("%s%s", g->roteadores[caminho[i]], (i > 0 ? " -> " : ""));
    printf("\nLatencia total: %d ms\n", dist[fim]); // imprime o valor do dist
}

void mostrarMatriz(Grafo *g){
    int i, j;

    printf("\nMatriz atual: \n");
    for(j=0; j<g->num_roteadores; j++){
        printf("\n");
        for(i=0; i<g->num_roteadores; i++){
            printf("%d ", g->latencia[j][i]);
        }
    }
    printf("\n");
}

int main() {
    Grafo g;
    g.num_roteadores = 0;

    int opcao;
    char nome1[20], nome2[20];
    int valor;

    while (1) {
        printf("\n1. Adicionar roteador\n2. Conectar roteadores\n3. Ping\n4. Mostrar matriz\n0. Sair\n\nOpcao:");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome do roteador: ");
                scanf("%s", nome1);
                adicionar_roteador(&g, nome1);
                break;
            case 2:
                printf("Nome do roteador de origem: ");
                scanf("%s", nome1);
                printf("Nome do roteador de destino: ");
                scanf("%s", nome2);
                printf("Latencia (ms): ");
                scanf("%d", &valor);
                conectar(&g, nome1, nome2, valor);
                break;
            case 3:
                printf("Ping de: ");
                scanf("%s", nome1);
                printf("Para: ");
                scanf("%s", nome2);
                ping(&g, nome1, nome2);
                break;
            case 4:
                mostrarMatriz(&g);
                break;
            case 0:
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }
}