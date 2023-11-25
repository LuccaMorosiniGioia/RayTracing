#include "base.hpp"
#include "scenes.hpp"
#include "screen.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>

using namespace std;

pair<R3Vector, R3Vector>  pegar_inputs(R3Vector* C, R3Vector* M, R3Vector* UP,
    double* distancia, double* altura, double* largura){
        cout << "Coloque a origem da camera (X,Y,Z): ";
        cin >> C->x >> C->y >> C->z;
        cout << "Coloque o vetor onde a camera aponta (X,Y,Z): ";
        cin >> M->x >> M->y >> M->z; *M = normalize(*M);
        
        R3Vector V;
        while(1){ // Pegar o vetor UP e checar se é válido
            cout << "Coloque o vetor que aponta para cima (X,Y,Z): ";
            cin >> UP->x >> UP->y >> UP->z; *UP = normalize(*UP);
            V = crossProduct(*M, *UP);
            if (tamanho(V) == 0) {cout << "Valor sem componente ortogonal a câmera." << endl;}
            else { V = normalize(V); break;}
        }
        R3Vector U = normalize(crossProduct(V, *M)); 

        cout << "Coloque a distancia da camera: ";
        cin >> *distancia;
        cout << "Coloque agora a largura e altura, em ordem: ";
        cin >> *largura >> *altura;
    
    return make_pair(V, U);
}

pair<vector<Sphere>, vector<Plane>> pegar_objetos (){
    int quant;
    vector<Sphere> esferas; vector<Plane> planos;

    cout << "Coloque quantas esferas: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector centro, color;
        double raio;
        cout << i << "- Coloque o centro da esfera (X,Y,Z): ";
        cin >> centro.x >> centro.y >> centro.z;
        cout << i << "- Coloque Agora o raio: ";
        cin >> raio;
        cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        cin >> color.x >> color.y >> color.z;
        
        esferas.push_back(Sphere{centro,raio,color});
    }

    cout << "Coloque quantos planos: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector ponto, color, normal;
        cout << i << "- Agora um ponto do plano (X,Y,Z): ";
        cin >> ponto.x >> ponto.y >> ponto.z;
        cout << i << "- Coloque o vetor normal (X,Y,Z): ";
        cin >> normal.x >> normal.y >> normal.z;
        cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        cin >> color.x >> color.y >> color.z;

        planos.push_back(Plane{ponto,normal,color});
    }

    return make_pair(esferas, planos);
}

int main(){
    R3Vector C, M, UP;
    double distancia, altura, largura;
    R3Vector V, U;
    tie (V, U) = pegar_inputs(&C, &M, &UP, &distancia, &altura, &largura);
    R3Vector W = M; // Só para padronizar o nome

    R3Vector origem = addVector(C, scalarProduct(W, distancia)); // Pegar a origem da tela

    // Pegar agora os objetos das cenas
    vector<Sphere> esferas; vector<Plane> planos;
    tie (esferas, planos) = pegar_objetos();

    // Checar agora a interseção
    vector<vector<R3Vector>> colors = make_screen(U, V, origem, C, esferas, planos, altura, largura);
    print_ppm(colors, altura, largura);
}