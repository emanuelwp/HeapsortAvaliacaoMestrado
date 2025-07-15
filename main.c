#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct Pacote
typedef struct {
    int identificador;
    int tiposervico;
    int tamanho;
    int prioridade;
} Pacote;

// Mapeamento de prioridade com base no ToS
int mapeamentoPrioridade[256];

// Função para obter prioridade com base no ToS
int obterprioridadedetos(int tiposervico) {
    if (tiposervico >= 0 && tiposervico <= 255) {
        return mapeamentoPrioridade[tiposervico];
    } else {
        return 4;
    }
}

// Função auxiliar para troca
void trocar(Pacote *a, Pacote *b) {
    Pacote temp = *a;
    *a = *b;
    *b = temp;
}

// Função heapify para HeapSort
void heapify(Pacote *vetor, int n, int i) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < n && vetor[esquerda].prioridade < vetor[menor].prioridade)
        menor = esquerda;

    if (direita < n && vetor[direita].prioridade < vetor[menor].prioridade)
        menor = direita;

    if (menor != i) {
        trocar(&vetor[i], &vetor[menor]);
        heapify(vetor, n, menor);
    }
}

// HeapSort
void heapsort(Pacote *vetor, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(vetor, n, i);

    for (int i = n - 1; i > 0; i--) {
        trocar(&vetor[0], &vetor[i]);
        heapify(vetor, i, 0);
    }

    for (int i = 0; i < n / 2; i++)
        trocar(&vetor[i], &vetor[n - 1 - i]);
}

// Funções de geração de tráfego
Pacote* gerartrafego(int total, int tamanhomedio, int variacao) {
    Pacote *pacotes = (Pacote*) malloc(sizeof(Pacote) * total);
    for (int i = 0; i < total; i++) {
        pacotes[i].identificador = i;
        pacotes[i].tiposervico = rand() % 256;
        pacotes[i].tamanho = tamanhomedio + (rand() % (2 * variacao + 1)) - variacao;
        pacotes[i].prioridade = obterprioridadedetos(pacotes[i].tiposervico);
    }
    return pacotes;
}

// Leitura do mapeamento do usuário
void lermapeamentodousuario() {
    int novoMapeamento[256];
    int inicio, fim, prioridade;
    char continuar = 's';

    for (int i = 0; i < 256; i++) novoMapeamento[i] = 4;

    printf("\n--- Definicao do Mapeamento de Prioridade ---\n");
    while (continuar == 's' || continuar == 'S') {
        printf("Digite inicio da faixa, fim da faixa, prioridade (0-4): ");
        if (scanf("%d %d %d", &inicio, &fim, &prioridade) != 3) {
            while (getchar() != '\n');
            continue;
        }

        if (inicio < 0 || fim > 255 || inicio > fim || prioridade < 0 || prioridade > 4) {
            printf("Valores fora da faixa. Tente novamente.\n");
            continue;
        }

        for (int i = inicio; i <= fim; i++)
            novoMapeamento[i] = prioridade;

        printf("Deseja adicionar outra faixa? (s/n): ");
        scanf(" %c", &continuar);
    }

    for (int i = 0; i < 256; i++)
        mapeamentoPrioridade[i] = novoMapeamento[i];
}

//Simulação de cenários
void simularcenario(const char* nomecenario, int quantidade, int tamanhomedio, int variacao) {
    printf("\n--- Simulando %s ---\n", nomecenario);

    Pacote *pacotes = gerartrafego(quantidade, tamanhomedio, variacao);

    clock_t inicio = clock();

    heapsort(pacotes, quantidade);

    double tempototalprocessamento = 0;
    double tempomin = 1e6, tempomax = 0;

    for (int i = 0; i < quantidade; i++) {
        clock_t ini_proc = clock();

        clock_t fim_proc = clock();
        double tempo = (double)(fim_proc - ini_proc) / CLOCKS_PER_SEC;
        tempototalprocessamento += tempo;

        if (tempo < tempomin) tempomin = tempo;
        if (tempo > tempomax) tempomax = tempo;
    }

    clock_t fim = clock();
    double total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Total de Pacotes: %d\n", quantidade);
    printf("Tempo Total: %f segundos\n", total);
    printf("Tempo Medio de Processamento: %e segundos\n", tempototalprocessamento / quantidade);
    printf("Tempo Minimo: %e segundos\n", tempomin);
    printf("Tempo Maximo: %e segundos\n", tempomax);

    free(pacotes);
}

int main() {
    srand(time(NULL));

    lermapeamentodousuario();

    //Diferentes cenários
    simularcenario("Trafego Constante", 100000, 1500, 100);
    simularcenario("Trafego em Rajada", 100000, 1500, 50);
    simularcenario("Trafego Variavel", 100000, 1500, 200);

    return 0;
}