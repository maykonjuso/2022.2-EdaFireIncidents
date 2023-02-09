#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Criando estrutura do nó
typedef struct no
{
    int linha;
    float chave;
    struct no *esquerda;
    struct no *direita;
} Arvore;

// Preenche a ABP em ordem crescente
Arvore *preencherABP(Arvore *noAtual, long int numLinha, float sumChave)
{
    if (noAtual == NULL)
    {
        noAtual = (Arvore *)malloc(sizeof(Arvore));
        noAtual->chave = sumChave;
        noAtual->linha = numLinha;
        noAtual->esquerda = NULL;
        noAtual->direita = NULL;
    }
    else if (sumChave <= noAtual->chave)
    {
        noAtual->esquerda = preencherABP(noAtual->esquerda, numLinha, sumChave);
    }
    else
    {
        noAtual->direita = preencherABP(noAtual->direita, numLinha, sumChave);
    }
    return noAtual;
}

// desaloca todos os nos da arvore
void desalocarNo(Arvore *no)
{

    if (no != NULL)
    {
        desalocarNo(no->esquerda);

        if (no->direita != NULL)
        {
            desalocarNo(no->direita);
        }

        free(no);
    }
}

// Ordena as chaves em ordem crescente
void ordernarChave(char *nomeArquivo, Arvore *no)
{
    if (no != NULL)
    {
        ordernarChave(nomeArquivo, no->esquerda);
        printf("chave: %f, linha: %d\n", no->chave, no->linha);
        imprimir(nomeArquivo, no->linha);

        ordernarChave(nomeArquivo, no->direita);
    }
}

// Imprime a linha do arquivo
void imprimir(char *nomeArquivo, int numLinha)
{
    FILE *fp;
    char linha[1024];
    int linhaAtual = 1;

    fp = fopen(nomeArquivo, "r");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    while (fgets(linha, 1024, fp) != NULL)
    {
        if (linhaAtual == numLinha)
        {
            printf("%s\n", linha);
            break;
        }
        linhaAtual++;
    }

    fclose(fp);
}

int main(void)
{
    // Declarando variáveis
    FILE *csvFile;
    Arvore *noAtual = NULL;
    float chave;
    char *token;
    int flag, opcao = 0;
    char nomeArquivo[100], linhaDoArquivo[1000];
    long int linhaChave = 1;

    do
    {
        printf("Escolha uma das opcoes abaixo:\n\n\t1 => Carregar arquivo\n\t2 => Imprimir relatorio\n\t3 => Sair do programa\n\n> ");

        // Lendo opção desejada
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:

            printf("\nInsira o nome do arquivo.\n\n> ");

            scanf("%s", nomeArquivo);

            csvFile = fopen(nomeArquivo, "r");
            if (csvFile == NULL)
            {
                printf("error 500 - Erro ao abrir arquivo!");
                exit(1);
            }

            // seta flag como "arquivo aberto"
            flag = 1;

            // fseek aponta para o inicio do arquivo
            fseek(csvFile, 0, SEEK_SET);

            while (fgets(linhaDoArquivo, sizeof(linhaDoArquivo) + 1, csvFile))
            {
                if (linhaChave > 1)
                {
                    chave = 0;
                    token = strtok(linhaDoArquivo, ";");

                    // Avança para as colunas chaves (latitude, longitude)
                    for (int i = 0; i < 15; i++)
                    {
                        token = strtok(NULL, ";");
                    }

                    // Troca virgula por ponto
                    if (token[0] == '-')
                        token[3] = '.';
                    else
                        token[2] = '.';

                    chave = strtof(token, NULL);
                    token = strtok(NULL, ";");

                    if (token[0] == '-')
                        token[3] = '.';
                    else
                        token[2] = '.';

                    chave += strtof(token, NULL);

                    // Preenche ABP
                    noAtual = preencherABP(noAtual, linhaChave, chave);
                    linhaChave++;
                }
                else
                {
                    linhaChave++;
                }
            }

            printf("\nO Arquivo '%s' foi carregado!\n\n", nomeArquivo);
            break;

        case 2:
            if (flag == 0)
            {
                printf("\nerror 404 - Arquivo Vazio!!!\n\n> ");
                break;
            }
            printf("\n");
            ordernarChave(nomeArquivo, noAtual);
            break;

        case 3:
            // Desaloca memória alocada
            desalocarNo(noAtual);
            break;

        default:
            printf("\nerror 500 - Opcao invalida!\n\n> ");
            opcao = 0;
            break;
        }
    } while (opcao == 0 || opcao == 1 || opcao == 2);

    return 0;
}