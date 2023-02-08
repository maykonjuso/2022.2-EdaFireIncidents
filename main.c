#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20000

//nó da árvore binária
typedef struct NoDeArvore
{

    float chave;
    int linha;
    struct NoDeArvore *esquerda;
    struct NoDeArvore *direita;

} Arvore;

//função para desalocar o nó
void removerNos(Arvore *no)
{

    if (no != NULL)
    {
        removerNos(no->esquerda);

        if (no->direita != NULL)
        {
            removerNos(no->direita);
        }

        free(no);
    }
}

void imprimirLinha(long int numBytes, FILE *arquivo)
{

    char linhaFile[850];

    fseek(arquivo, numBytes, SEEK_SET);

    printf("%s", fgets(linhaFile, sizeof(linhaFile), arquivo));
    printf("\n");
}

void imprimirLinhas_OrdemCrescente(Arvore *no, long int vetorBytes[], FILE *arquivo)
{

    int caractere;
    char linhaFile[850];

    if (no != NULL)
    {
        imprimirLinhas_OrdemCrescente(no->esquerda, vetorBytes, arquivo);

        // fseek(arquivo, vetorBytes[no->linha], SEEK_SET);

        // printf ("%s", fgets(linhaFile, sizeof(linhaFile), arquivo));
        // while ((caractere = fgetc(arquivo)) != '\n'){
        //     printf ("%c", caractere);
        // }
        // printf("\n");
        imprimirLinha(vetorBytes[no->linha], arquivo);

        if (no->direita != NULL)
        {
            imprimirLinhas_OrdemCrescente(no->direita, vetorBytes, arquivo);
        }
    }
}

//contar quantidade de linhas do arquivo
int quantidadeDeLinhas(FILE *ptr_para_file)
{
    int numLinhas = 0, letra;

    while ((letra = fgetc(ptr_para_file)) != EOF)
    {
        if (letra == '\n')
        {
            numLinhas++;
        }
    }

    return numLinhas;
}

//criando árvore binária
Arvore *registrarABP(Arvore *noPai, long int linhaDeLeitura, float SomaLongitude_Latitude)
{
    //se o nó atual estiver vazio
    if (noPai == NULL)
    {
        noPai = (Arvore *)malloc(sizeof(Arvore));
        noPai->chave = SomaLongitude_Latitude;
        noPai->linha = linhaDeLeitura;
        noPai->esquerda = NULL;
        noPai->direita = NULL;
    }
    //se a chave do nó inserido for menor do que a do atual
    else if (SomaLongitude_Latitude <= noPai->chave)
    {
        noPai->esquerda = registrarABP(noPai->esquerda, linhaDeLeitura, SomaLongitude_Latitude);
    }
    //se a chave for maior
    else
    {
        noPai->direita = registrarABP(noPai->direita, linhaDeLeitura, SomaLongitude_Latitude);
    }
    return noPai;
}

int main(void)
{

    FILE *csvInfo;

    Arvore *noPai = NULL;
    char nomeArquivo[200];
    int numLinhas;
    int nBytes = 0;
    long int posLinhaBytes[MAX];
    posLinhaBytes[0] = 0;

    char linhaDoArquivo[850];

    long int localLinha = 1;
    float somaColunas = 0;

    char *linhaSeparada;

    int escolha = 0;
    do
    {
        printf("Qual opcao deseja realizar?\n\t1 = Carregar arquivo\n\t2 = Imprimir relatorio\n\t3 = Sair do programa\n\n");
        scanf("%d", &escolha);

        //menu de escolha do usuário
        switch (escolha)
        {
        case 1:
            //carregar arquivo na ABP
            printf("\nQual arquivo a ser carregado?\n\n");

            scanf("%s", nomeArquivo);

            csvInfo = fopen(nomeArquivo, "r");
            if (csvInfo == NULL)
            {
                printf("Erro ao abrir arquivo");
                exit(1);
            }

            numLinhas = quantidadeDeLinhas(csvInfo);

            fseek(csvInfo, 0, SEEK_SET);

            while (fgets(linhaDoArquivo, sizeof(linhaDoArquivo) + 1, csvInfo))
            {
                
                nBytes = nBytes + strlen(linhaDoArquivo);
                posLinhaBytes[(localLinha)] = nBytes;

                if (localLinha > 1)
                {

                    somaColunas = 0;

                    //armazena todas as colunas até o campo "latitude"
                    linhaSeparada = strtok(linhaDoArquivo, ";");
                    for (int i = 0; i < 16; i++)
                    {
                        linhaSeparada = strtok(NULL, ";");
                    }
                    
                    

                    linhaSeparada[2] = '.';
                    somaColunas = strtof(linhaSeparada, NULL);

                    linhaSeparada = strtok(NULL, ";");

                    linhaSeparada[2] = '.';
                    somaColunas += strtof(linhaSeparada, NULL);

                  
                    noPai = registrarABP(noPai, localLinha, somaColunas);
                    localLinha++;
                }

                else
                {
                    localLinha++;
                }
            }

            printf("\nArquivo '%s'carregado com sucesso!\n\n", nomeArquivo);
          
            break;


        case 2:
            //imprime ABP carregado pelo case 1 em ordem crescente
            printf("\n");
            imprimirLinhas_OrdemCrescente(noPai, posLinhaBytes, csvInfo);
            break;

        case 3:
            //desaloca memória
            removerNos(noPai);
            break;

        default:
            printf("\nOpção Inválida! Tente novamente.\n\n");
            escolha = 0;
            break;
        }

    } while (escolha == 0 || escolha == 1 || escolha == 2);

    return 0;
}
