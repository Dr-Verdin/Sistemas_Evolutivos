#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <algorithm>  // Para usar std::max e std::min

const int POP_SIZE = 15; // Tamanho da população
const double MIN = 0.0; // Mínimo do domínio
const double MAX = 30.0; // Máximo do domínio
const float TAXA_MUT = 0.1; // Taxa de mutação
const int NUM_GEN = 25; // Número de gerações

// Individuo
struct INDIVIDUAL {
    double x; // Genoma do indíviduo
    double fitness; // Aptidão do indivíduo

    INDIVIDUAL() : x(0), fitness(0) {} // Construtor padrão

    INDIVIDUAL(double value) : x(value) {
        fitness = evaluateFitness(); // Avalia a aptidão no momento da criação
    }

    double evaluateFitness() {
        return -((x * x)/10) + 3 * x; // Função de aptidão
    }
};

int gen = 1; // Contador da geração atual
INDIVIDUAL individuo[POP_SIZE]; // Array que armazena os indíviduos

// Função de desenho
void display() { 
    glClear(GL_COLOR_BUFFER_BIT);  // Limpa a tela

    // Desenha a curva da função de aptidão
    glColor3f(0.0, 0.0, 0.0); // Define a cor da linha 
    glBegin(GL_LINE_STRIP); // Inicia a definição de uma linha contínua
    for (float x = MIN; x <= MAX; x += 0.1) {
        // Calcula o valor de y com base na função de aptidão
        float y = -((x * x)/10) + 3 * x; 
        glVertex2f(x, y); // Adiciona o ponto (x, y) à linha
    }
    glEnd(); // Finaliza a definição da linha

    // Plota os pontos da população atual sobre a curva
    glPointSize(5.0); // Define o tamanho dos pontos
    glColor3f(0.5, 0.0, 0.5); // Define a cor dos pontos como rosa
    glBegin(GL_POINTS); // Inicia a definição de pontos individuais
    for (int i = 0; i <= POP_SIZE; i++) {
        // Calcula y para garantir que o ponto esteja na curva
        float y = -((individuo[i].x * individuo[i].x) / 10) + 3 * individuo[i].x;
        glVertex2f(individuo[i].x, y); // Adiciona o ponto (individuo[i].x, y) à lista de pontos
    }
    glEnd(); // Finaliza a definição dos pontos

    glFlush();  // Atualiza o buffer para exibir o desenho na tela
}

// Função para iniciar o OpenGL
void initGL() { 
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Define a cor de fundo da tela como preto
    glMatrixMode(GL_PROJECTION);  // Seleciona a matriz de projeção
    glLoadIdentity();  // Reseta a matriz
    gluOrtho2D(MIN, MAX, 0, 30); // Define a projeção 2D com o domínio da função
}

// FUnção para iniciar a população
void initPop(int tamPop, INDIVIDUAL individuo[]){ 
    srand(time(NULL)); // Semente para geração de numeros aleatorios

    // Inicializa a populção
    for(int i=0; i <= tamPop; i++){
        individuo[i] = INDIVIDUAL(MIN + (MAX - MIN) * rand() / (double)(RAND_MAX)); // Gera um valor aleatório
    }
}

// Função para avaliar a população
void evaluePop(){ 
    printf("Geracao %d\n", gen); // Imprime a geração atual
    for(int i = 0; i < POP_SIZE; i++){
        individuo[i].fitness = individuo[i].evaluateFitness(); // Avalia a aptidão
        printf("Individuo %d: x = %.2f, fitness = %.2f\n", i, individuo[i].x, individuo[i].fitness);
    }
}

// Função para encontrar o melhor individuo
INDIVIDUAL theBest(){ 
    INDIVIDUAL best = individuo[0];
    for(int i = 1; i < POP_SIZE; i++){
        if(individuo[i].fitness > best.fitness){
            best = individuo[i]; // Atualiza se um melhor for encontrado
        }
    }
    return best;
}

// FUnção para aplicar mutação
void mutate(INDIVIDUAL &ind){ 
    if((rand() / (double)RAND_MAX) < TAXA_MUT){
        ind.x = std::max(MIN, std::min(MIN + (MAX - MIN) * rand() / (double)(RAND_MAX), MAX)); // Novo valor aleatório
    }
}

// Função para gerar a próxima geração
void nextGen(){
    INDIVIDUAL newPop[POP_SIZE];

    newPop[0] = theBest(); // Mantém o melhor indivíduo da geração anterior

    // Criação de uma nova população por eletismo; Crossover: o melhor individuo transfere parte dos seus genes para os demais
    for (int i = 1; i < POP_SIZE; i++) {
        double parentX = MIN + (MAX - MIN) * rand() / (double)(RAND_MAX);
        parentX = (parentX + newPop[0].x) / 2; // Crossover simples
        newPop[i] = INDIVIDUAL(parentX);
        mutate(newPop[i]); // Aplica mutação 
    }

    // Atualiza a população
    for (int i = 0; i < POP_SIZE; i++) {
        individuo[i] = newPop[i];
    }
}

// Função para executar o algoritmo
void runAlgorithm() { // OK
    evaluePop();  // Avalia a população atual
    display();       // Atualiza o gráfico
    glutPostRedisplay();  // Solicita uma atualização da tela
    nextGen();  // Gera nova população
    gen++; // Atualiza contador
}

// Função chamada ao pressionar uma tecla
void keyboard(unsigned char key, int x, int y) { 
    if (key == 13) {  // Tecla Enter (ASCII 13)
        if (gen <= NUM_GEN) {
            runAlgorithm();  // Executa o algoritmo 
        } else {
            printf("Numero maximo de gerações atingido.\n");
        }
    }
}

// Função principal
int main(int argc, char **argv) { 
    initPop(POP_SIZE, individuo); // inicia a população

    // Inicia o OpenGL e o GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Projeto 1");

    initGL();  // Configura o OpenGL

    // Registra as funções de callback
    glutDisplayFunc(display); // Função de exibição
    glutKeyboardFunc(keyboard); // Função para lidar com o teclado

    // Inicia o loop de eventos do GLUT
    glutMainLoop();

    return 0;  // Retorna 0 ao terminar o programa
}