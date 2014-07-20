#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <climits>
#include <cstdlib>

using namespace std;

// tamanhos

#define TAMANHO_LINHA 83
#define BASEDADOS_SIZE 4

//variaveis

#define PIB_ 0
#define INVESTIMENTO 1
#define CONSUMOPRIVADO 2
#define CONSUMOPUBLICO 3

typedef struct
{

    int ano;
    float PIB;
    float investimento;
    float consumoprivado;
    float consumopublico;

} linha;

typedef struct
{

    linha dados[150];
    int last;

} structDados;

typedef struct
{

    char variavel[50];
    float media;
    float minimo;
    float maximo;
    float mediana;
    float variancia;
    float taxamediacrescimento;

} structStats;

structDados baseDados;
structStats baseStats[BASEDADOS_SIZE];

void init();
void readFromFile();
void calcStats();
void writeToFile(char *filename);

float minimo(float *valores);
float maximo(float *valores);
float media(float *valores);
float mediaCrescimento(float *valores, int numValores);
float mediana(float *valores);
float variancia(float *valores, float media);
float taxamediacrescimento(float *valores);

float *getPIB();
float *getInvestimento();
float *getConsumoPublico();
float *getConsumoPrivado();

int main(int argc, char *argv[])
{
    char output[50];
    char opcao[2];
    strcpy(output, "stats.txt");

    void init();
    cout << "Variaveis internas inicializadas" << endl;
    cout << "Iniciada leitura do ficheiro" << endl;
    readFromFile();
    cout << "Finalizada leitura do ficheiro" << endl;
    cout << "Iniciados calculos estatisticos" << endl;
    calcStats();
    cout << "Finalizados calculos estatisticos" << endl;
    cout << "Iniciada escrita do ficheiro" << endl;
    writeToFile(output);
    cout << "Finalizada escrita do ficheiro" << endl;

    cout << endl << "Pretende abrir o ficheiro gerado? (S/N) ";
    cin >> opcao;
    cout << endl;

    if((!strcmp(opcao, "s")) || (!strcmp(opcao, "S")) || (!strcmp(opcao, "sim")) || (!strcmp(opcao, "SIM"))){
        cout << "A abrir ficheiro stats.txt" << endl;
        system("C:\\WINDOWS\\notepad.exe stats.txt");
    }

    else cout << "Os dados estao disponiveis no ficheiro stats.txt" << endl;
    return EXIT_SUCCESS;
}

void init()
{
    int i;

    for(i = 0; i < 150; i++)
    {
        baseDados.dados[i].ano = 0;
        baseDados.dados[i].PIB = 0.0;
        baseDados.dados[i].investimento = 0.0;
        baseDados.dados[i].consumoprivado = 0.0;
        baseDados.dados[i].consumopublico = 0.0;
    }
    baseDados.last = 0;
    i = 0;

    for(i = 0; i < 4; i++)
    {
        baseStats[i].media = 0.0;
        baseStats[i].minimo = 0.0;
        baseStats[i].maximo = 0.0;
        baseStats[i].mediana = 0.0;
        baseStats[i].variancia = 0.0;
        baseStats[i].taxamediacrescimento = 0.0;
    }

    strcpy(baseStats[PIB_].variavel, "PIB");
    strcpy(baseStats[INVESTIMENTO].variavel, "Investimento");
    strcpy(baseStats[CONSUMOPRIVADO].variavel, "Consumo Privado");
    strcpy(baseStats[CONSUMOPUBLICO].variavel, "Consumo Publico");
}

void readFromFile()
{
    char filename[100];
    strcpy(filename, "dados.txt");

    FILE *newFile = fopen(filename, "r");
    if (newFile == NULL) cout << "Erro na abertura do ficheiro." << endl;

    int dim_line = TAMANHO_LINHA;
    char line[dim_line];
    char ano[15];
    char PIB[15];
    char investimento[15];
    char consumoprivado[15];
    char consumopublico[15];

    strcpy(line, "");
    strcpy(ano, "");
    strcpy(PIB, "");
    strcpy(investimento, "");
    strcpy(consumoprivado, "");
    strcpy(consumopublico, "");

    fgets(line, sizeof(line), newFile);

    while(fgets(line, sizeof(line), newFile) != NULL)
    {

        sscanf(line, "%s %s %s %s %s\n", ano, PIB, investimento, consumoprivado, consumopublico);

        // converter strings para numeros

        int anoInt = atoi(ano);
        float PIBfloat = atof(PIB);
        float investimentofloat = atof(investimento);
        float consumoprivadofloat = atof(consumoprivado);
        float consumopublicofloat = atof(consumopublico);

        if((anoInt > 0) && (PIBfloat > 0) && (investimentofloat > 0) && (consumoprivadofloat > 0) && (consumopublicofloat > 0))
        {

            baseDados.dados[baseDados.last].ano = anoInt;
            baseDados.dados[baseDados.last].PIB = PIBfloat;
            baseDados.dados[baseDados.last].investimento = investimentofloat;
            baseDados.dados[baseDados.last].consumoprivado = consumoprivadofloat;
            baseDados.dados[baseDados.last].consumopublico = consumopublicofloat;

            baseDados.last ++;
        }

        strcpy(line, "");
        strcpy(ano, "");
        strcpy(PIB, "");
        strcpy(investimento, "");
        strcpy(consumoprivado, "");
        strcpy(consumopublico, "");
    }

    fclose(newFile);
}

void calcStats()
{

    baseStats[PIB_].minimo = minimo(getPIB());
    baseStats[PIB_].maximo = maximo(getPIB());
    baseStats[PIB_].media = media(getPIB());
    baseStats[PIB_].mediana = mediana(getPIB());
    baseStats[PIB_].variancia = variancia(getPIB(), media(getPIB()));
    baseStats[PIB_].taxamediacrescimento = taxamediacrescimento(getPIB());

    cout << "A calcular dados PIB..." << endl;

    baseStats[INVESTIMENTO].minimo = minimo(getInvestimento());
    baseStats[INVESTIMENTO].maximo = maximo(getInvestimento());
    baseStats[INVESTIMENTO].media = media(getInvestimento());
    baseStats[INVESTIMENTO].mediana = mediana(getInvestimento());
    baseStats[INVESTIMENTO].variancia = variancia(getInvestimento(), media(getInvestimento()));
    baseStats[INVESTIMENTO].taxamediacrescimento = taxamediacrescimento(getInvestimento());

    cout << "A calcular dados Investimento..." << endl;

    baseStats[CONSUMOPRIVADO].minimo = minimo(getConsumoPrivado());
    baseStats[CONSUMOPRIVADO].maximo = maximo(getConsumoPrivado());
    baseStats[CONSUMOPRIVADO].media = media(getConsumoPrivado());
    baseStats[CONSUMOPRIVADO].mediana = mediana(getConsumoPrivado());
    baseStats[CONSUMOPRIVADO].variancia = variancia(getConsumoPrivado(), media(getConsumoPrivado()));
    baseStats[CONSUMOPRIVADO].taxamediacrescimento = taxamediacrescimento(getConsumoPrivado());

    cout << "A calcular dados Consumo Privado..." << endl;

    baseStats[CONSUMOPUBLICO].minimo = minimo(getConsumoPublico());
    baseStats[CONSUMOPUBLICO].maximo = maximo(getConsumoPublico());
    baseStats[CONSUMOPUBLICO].media = media(getConsumoPublico());
    baseStats[CONSUMOPUBLICO].mediana = mediana(getConsumoPublico());
    baseStats[CONSUMOPUBLICO].variancia = variancia(getConsumoPublico(), media(getConsumoPublico()));
    baseStats[CONSUMOPUBLICO].taxamediacrescimento = taxamediacrescimento(getConsumoPublico());

    cout << "A calcular dados Consumo Publico..." << endl;
}

void writeToFile(char *filename)
{
    FILE *file = fopen(filename, "w+");
    char linha[200];
    char cabecalho[200];
    char linhaVazia[200];
    char variavel[15];
    char minimo[15];
    char maximo[15];
    char media[15];
    char mediana[15];
    char variancia[15];
    char taxamediacrescimento[30];
    char nomesVariaveis[4][20];

    strcpy(nomesVariaveis[0], "PIB");
    strcpy(nomesVariaveis[1], "Investimento");
    strcpy(nomesVariaveis[2], "Consumo Privado");
    strcpy(nomesVariaveis[3], "Consumo Publico");

    strcpy(variavel, "Variavel");
    strcpy(minimo, "Minimo");
    strcpy(maximo, "Maximo");
    strcpy(media, "Media");
    strcpy(mediana, "Mediana");
    strcpy(variancia, "Variancia");
    strcpy(taxamediacrescimento, "Taxa Media Crescimento");

    sprintf(cabecalho,"%-25s %-15s %-15s %-15s %-15s %-15s %-28s\n", variavel, minimo, maximo, media, mediana, variancia, taxamediacrescimento);
    sprintf(linhaVazia, "%-100s\n", "");

    fprintf(file, cabecalho);
    fprintf(file, linhaVazia);

    int i;
    char percentagem[3];
    strcpy(percentagem, "%%");

    for(i = 0; i < 4; i++)
    {
        sprintf(linha,"%-25s %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %2.2f%s\n", nomesVariaveis[i], baseStats[i].minimo, baseStats[i].maximo, baseStats[i].media, baseStats[i].mediana, baseStats[i].variancia, baseStats[i].taxamediacrescimento, percentagem);
        fprintf(file, linha);
    }

    fclose(file);
}

float *getPIB()
{

    float *PIB = (float*) malloc(sizeof(float) * baseDados.last);
    int i;

    for(i = 0; i < baseDados.last; i++)
    {
        PIB[i] = 0.0;
    }

    for(i = 0; i < baseDados.last; i++)
    {
        if(baseDados.dados[i].PIB != 0)
        {
            PIB[i] = baseDados.dados[i].PIB;
        }
    }

    return PIB;
}

float *getInvestimento()
{

    float *Investimento = (float*) malloc(sizeof(float) * baseDados.last);
    int i;

    for(i = 0; i < baseDados.last; i++)
    {
        Investimento[i] = 0.0;
    }

    for(i = 0; i < baseDados.last; i++)
    {
        if(baseDados.dados[i].investimento != 0)
        {
            Investimento[i] = baseDados.dados[i].investimento;
        }
    }

    return Investimento;
}

float *getConsumoPublico()
{

    float *ConsumoPublico = (float*) malloc(sizeof(float) * baseDados.last);
    int i;

    for(i = 0; i < baseDados.last; i++)
    {
        ConsumoPublico[i] = 0.0;
    }

    for(i = 0; i < baseDados.last; i++)
    {
        if(baseDados.dados[i].consumopublico != 0)
        {
            ConsumoPublico[i] = baseDados.dados[i].consumopublico;
        }
    }

    return ConsumoPublico;
}

float *getConsumoPrivado()
{

    float *ConsumoPrivado = (float*) malloc(sizeof(float) * baseDados.last);
    int i;

    for(i = 0; i < baseDados.last; i++)
    {
        ConsumoPrivado[i] = 0.0;
    }

    for(i = 0; i < baseDados.last; i++)
    {
        if(baseDados.dados[i].consumoprivado != 0)
        {
            ConsumoPrivado[i] = baseDados.dados[i].consumoprivado;
        }
    }

    return ConsumoPrivado;
}

float minimo(float *valores)
{

    float min = 10000000;
    int numElementos = baseDados.last;

    int i;

    for(i = 0; i < numElementos; i++)
    {
        if(valores[i] < min)
        {
            min = valores[i];
        }
    }

    return min;
}

float maximo(float *valores)
{

    float max = 0.0;

    int numElementos = baseDados.last;

    int i;

    for(i = 0; i < numElementos; i++)
    {
        if(valores[i] > max)
        {
            max = valores[i];
        }
    }

    return max;
}

float media(float *valores)
{

    float med = 0.0;

    int numElementos = baseDados.last;

    int i;

    float soma = 0.0;

    for(i = 0; i < numElementos; i++)
    {
        soma += valores[i];
    }

    med = soma / numElementos;

    return med;
}

float mediaCrescimento(float *valores, int numValores)
{

    float med = 0.0;

    int i;

    float soma = 0.0;

    for(i = 0; i < numValores; i++)
    {
        soma += valores[i];
    }

    med = soma / numValores;

    return med;
}

float mediana(float *valores)
{

    float median = 0.0;

    //////////////////////algoritmo bubble sort///////////////////////////

    int i, j;
    int n = baseDados.last;
    float temp = 0.0;

    for(i=0 ; i<n ; i++)
    {
        for(j=0 ; j<n-i-1 ; j++)
        {
            if(valores[j]>valores[j+1])
            {
                temp=valores[j];
                valores[j]=valores[j+1];
                valores[j+1]=temp;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////

    if((n % 2) == 1)  // numero impar de elementos
    {
        median = valores[(int)(n / 2)]; // vector começa no 0
    }

    else if((n % 2) == 0)  // numero par de elementos
    {
        median = (valores[(int)((n / 2) - 1)] + valores[(int)(n / 2)]) / 2;
    }

    return median;
}

float variancia(float *valores, float media)
{

    float var = 0.0;
    int i;

    for(i = 0; i < baseDados.last; i ++)
    {
        var += (valores[i] - media) * (valores[i] - media);
    }

    var = var / (float)baseDados.last;

    return var;
}

float taxamediacrescimento(float *valores)
{

    float taxa = 0.0;
    float *crescimento = (float*) malloc(sizeof(float) * (baseDados.last - 1));
    int i;

    for(i = 1; i < baseDados.last; i ++)
    {
        crescimento[i-1] = ((valores[i] - valores[i-1]) / valores[i-1]) * 100;
    }

    taxa = mediaCrescimento(crescimento, (baseDados.last-1));

    return taxa;
}
