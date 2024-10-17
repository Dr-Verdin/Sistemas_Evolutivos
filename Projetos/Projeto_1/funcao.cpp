#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <algorithm>  // Para usar std::max e std::min

const int POP_SIZE = 10; // tamanho da população
const double MIN = 0.0; // minimo valor do dominio (x) da função
const double MAX = 30.0; // maximo valor do dominio (x) da função
const float TAXA_MUT = 0.1; // taxa de mutação
const int NUM_GEN = 10; // numero de gerações

// Struct para o Individuo
struct INDIVIDUAL {
    double x; // genoma
    double fitness;

    INDIVIDUAL(double value) : x(value) {
        fitness = evaluateFitness();
    }

    double evaluateFitness() {
        return -((x * x)/10) + 3 * x;
    }
};

int gen = 1; // geração atual; contador
INDIVIDUAL individuo[POP_SIZE];

// Função para desenhar o gráfico
void display() { // OK
    glClear(GL_COLOR_BUFFER_BIT);  // Limpa a tela, preparando-a para o novo desenho

    // Desenha a curva da função de aptidão.
    glColor3f(0.0, 0.0, 0.0); // Define a cor da linha como branca
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
void initGL() { // OK
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Define a cor de fundo da tela como preto
    glMatrixMode(GL_PROJECTION);  // Seleciona a matriz de projeção
    glLoadIdentity();  // Carrega a matriz identidade
    gluOrtho2D(MIN, MAX, -10, 30); // Define a projeção 2D com o domínio da função
}

// FUnção para iniciar a população
void initPop(int tamPop, INDIVIDUAL individuo[]){ // OK
    srand(time(NULL)); // Gerador de numeros aleatorios

    // Inicializa a populção
    for(int i=0; i <= tamPop; i++){
        individuo[i] = INDIVIDUAL(MIN + (MAX - MIN) * rand() / (RAND_MAX + 1.0));
    }
}

// Função para avaliar a aptidão do individuo
void evaluePop(){ // OK
    printf("Geracao %d\n", gen);
    for(int i = 0; i < POP_SIZE; i++){
        individuo[i].fitness = individuo[i].evaluateFitness();
        printf("Individuo %d: x = %.2f, fitness = %.2f\n", i, individuo[i].x, individuo[i].fitness);
    }
}

// Função para encontrar o melhor individuo
INDIVIDUAL theBest(){ // OK
    INDIVIDUAL best = individuo[0];
    for(int i = 1; i < POP_SIZE; i++){
        if(individuo[i].fitness > best.fitness){
            best = individuo[i];
        }
    }
    return best;
}

// FUnção para aplicar mutação
void mutate(INDIVIDUAL &ind){ // OK (tem que comentar, o que é o RAND_MAX)
    if((rand() / (RAND_MAX + 1.0)) < TAXA_MUT){
        ind.x = std::clamp(MIN + (MAX - MIN) * rand() / (RAND_MAX + 1.0), MIN, MAX); // Novo valor aleatório
    }
}

void nextGen(){
    INDIVIDUAL newPop[POP_SIZE];

    newPop[0] = theBest();

    // Criação de uma nova população por eletismo; Crossover: o melhor individuo transfere parte dos seus genes para os demais
    for (int i = 1; i < POP_SIZE; i++) {
        double parentX = MIN + (MAX - MIN) * rand() / (RAND_MAX + 1.0);
        parentX = (parentX + newPop[0].x) / 2;
        newPop[i] = INDIVIDUAL(parentX);
        mutate(newPop[i]); // Aplica mutação ao novo indivíduo
    }

    // Atualiza a população
    for (int i = 0; i < POP_SIZE; i++) {
        individuo[i] = newPop[i];
    }
}

// Função para executar o algoritmo genetico e atualizar o grafico
void runAlgorithm() { // OK
    evaluePop();  // Avalia a população atual
    display();       // Atualiza o gráfico
    glutPostRedisplay();  // Solicita uma atualização da tela
    nextGen();  // Gera nova população
    gen++;
}

// Função chamada ao pressionar uma tecla
void keyboard(unsigned char key, int x, int y) { // OK
    if (key == 13) {  // Tecla Enter (ASCII 13)
        if (gen <= NUM_GEN) {
            runAlgorithm();  // Executa o algoritmo para a próxima geração
        } else {
            printf("Numero maximo de gerações atingido.\n");
        }
    }
}

// Função principal
int main(int argc, char **argv) { // OK
    initPop(POP_SIZE, individuo); // inicia a população

    // Inicia o OpenGL e o GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Algoritmo Genético");

    initGL();  // Configura o OpenGL

    // Registra as funções de callback
    glutDisplayFunc(display); // Função de exibição
    glutKeyboardFunc(keyboard); // Função para lidar com o teclado

    // Inicia o loop de eventos do GLUT
    glutMainLoop();

    return 0;  // Retorna 0 ao terminar o programa
}