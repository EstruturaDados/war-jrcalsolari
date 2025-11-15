// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define TAM_NOME 50
#define TAM_COR 20

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa);
void exibirMissao(int missaoId);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* mapa, int atacante, int defensor);
int sortearMissao(void);
int verificarVitoria(const Territorio* mapa, int missaoId, const char* corJogador);

// Função utilitária:
void limparBufferEntrada(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }
    
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa);
    
    // - Define a cor do jogador e sorteia sua missão secreta.
    char corJogador[TAM_COR] = "Azul";
    int missaoId = sortearMissao();
    int vitoria = 0;
    int opcao;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    do {
        // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                  🗺️  MAPA DO MUNDO 🗺️                      ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        exibirMapa(mapa);
        
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                      📋 SUA MISSÃO 📋                      ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        exibirMissao(missaoId);
        
        exibirMenuPrincipal();
        
        // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            case 1:
                // - Opção 1: Inicia a fase de ataque.
                faseDeAtaque(mapa);
                break;
            case 2:
                // - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
                vitoria = verificarVitoria(mapa, missaoId, corJogador);
                if (vitoria) {
                    printf("\n╔════════════════════════════════════════════════════════════╗\n");
                    printf("║              🎉 PARABÉNS! MISSÃO CUMPRIDA! 🎉              ║\n");
                    printf("╚════════════════════════════════════════════════════════════╝\n");
                    break;
                } else {
                    printf("\n❌ Missão ainda não foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\n👋 Obrigado por jogar! Até logo!\n");
                break;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
                break;
        }
        
        // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        if (opcao != 0 && !vitoria) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
        
    } while (opcao != 0 && !vitoria);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(void) {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa) {
    strcpy(mapa[0].nome, "América do Norte");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 3;
    
    strcpy(mapa[1].nome, "Europa");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 2;
    
    strcpy(mapa[2].nome, "Ásia");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 4;
    
    strcpy(mapa[3].nome, "África");
    strcpy(mapa[3].cor, "Verde");
    mapa[3].tropas = 2;
    
    strcpy(mapa[4].nome, "Oceania");
    strcpy(mapa[4].cor, "Amarelo");
    mapa[4].tropas = 1;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal(void) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                      📜 MENU PRINCIPAL 📜                   ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  1 - ⚔️  Atacar                                            ║\n");
    printf("║  2 - ✅ Verificar Missão                                   ║\n");
    printf("║  0 - 🚪 Sair                                               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("\n┌─────────────────────────────┬──────────────────┬──────────┐\n");
    printf("│ Território                 │ Cor do Exército  │ Tropas   │\n");
    printf("├─────────────────────────────┼──────────────────┼──────────┤\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("│ %-27s │ %-16s │ %-8d │\n", 
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    
    printf("└─────────────────────────────┴──────────────────┴──────────┘\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int missaoId) {
    switch (missaoId) {
        case 0:
            printf("\n🎯 MISSÃO: Destruir completamente o exército Verde!\n");
            printf("   Você deve eliminar todas as tropas dos territórios\n");
            printf("   dominados pelo exército Verde.\n");
            break;
        case 1:
            printf("\n🎯 MISSÃO: Conquistar 3 territórios!\n");
            printf("   Você deve conquistar pelo menos 3 territórios\n");
            printf("   para o seu exército Azul.\n");
            break;
        default:
            printf("\n🎯 MISSÃO: Desconhecida\n");
            break;
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa) {
    int atacante, defensor;
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    ⚔️  FASE DE ATAQUE ⚔️                   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\nEscolha o território ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &atacante);
    limparBufferEntrada();
    
    printf("Escolha o território DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &defensor);
    limparBufferEntrada();
    
    if (atacante < 1 || atacante > NUM_TERRITORIOS || 
        defensor < 1 || defensor > NUM_TERRITORIOS) {
        printf("\n❌ Território inválido! Escolha um número entre 1 e %d.\n", NUM_TERRITORIOS);
        return;
    }
    
    if (atacante == defensor) {
        printf("\n❌ Um território não pode atacar a si mesmo!\n");
        return;
    }
    
    if (mapa[atacante - 1].tropas == 0) {
        printf("\n❌ O território atacante não possui tropas!\n");
        return;
    }
    
    if (mapa[defensor - 1].tropas == 0) {
        printf("\n❌ O território defensor não possui tropas!\n");
        return;
    }
    
    simularAtaque(mapa, atacante - 1, defensor - 1);
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* mapa, int atacante, int defensor) {
    int dadoAtacante = (rand() % 6) + 1;  // Dado de 1 a 6
    int dadoDefensor = (rand() % 6) + 1;  // Dado de 1 a 6
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    🎲 RESULTADO DA BATALHA 🎲              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n⚔️  %s (%s) ataca %s (%s)\n", 
           mapa[atacante].nome, mapa[atacante].cor,
           mapa[defensor].nome, mapa[defensor].cor);
    printf("🎲 Dado do Atacante: %d\n", dadoAtacante);
    printf("🛡️  Dado do Defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante >= dadoDefensor) {
        // Atacante vence ou empata (empate favorece o atacante)
        printf("\n✅ O atacante venceu!\n");
        mapa[defensor].tropas--;
        
        if (mapa[defensor].tropas == 0) {
            // Território conquistado
            printf("\n🏆 %s foi CONQUISTADO!\n", mapa[defensor].nome);
            strcpy(mapa[defensor].cor, mapa[atacante].cor);
            mapa[defensor].tropas = 1;  // Move uma tropa para o território conquistado
            mapa[atacante].tropas--;    // Remove uma tropa do atacante
            printf("   Uma tropa foi movida de %s para %s.\n", 
                   mapa[atacante].nome, mapa[defensor].nome);
        } else {
            printf("   O defensor perdeu 1 tropa. Restam %d tropas.\n", mapa[defensor].tropas);
        }
    } else {
        // Defensor vence
        printf("\n❌ O defensor venceu!\n");
        printf("   O atacante não causou danos.\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio* mapa, int missaoId, const char* corJogador) {
    switch (missaoId) {
        case 0: {
            // Missão: Destruir o exército Verde
            int tropasVerdes = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Verde") == 0) {
                    tropasVerdes += mapa[i].tropas;
                }
            }
            return (tropasVerdes == 0);
        }
        case 1: {
            // Missão: Conquistar 3 territórios
            int territoriosConquistados = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    territoriosConquistados++;
                }
            }
            return (territoriosConquistados >= 3);
        }
        default:
            return 0;
    }
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
