// *********************************************************************
//
//  Cálculo de PI con hebras POSIX
//
//  Created by RAFAEL NOGALES VAQUERO on 17/03/15.
//  Copyright (c) 2015 RAFAEL NOGALES VAQUERO. All rights reserved.
//
// *********************************************************************


#include <iostream>  //Para entrada/salida
#include <pthread.h> //Para hebras POSIX
#include <chrono>    //Para cronometrar


//COMPILAR CON: g++ -o pi pi.cpp  -lpthread  -std=gnu++0x
//el switch -lpthreads sirve para incluir las funciones de hebras POSIX
//el switch -std=gnu++0x sirve para incluir las funciones chronos

//Testado en Linux Ubuntu 14.04 satisfactoriamente.
//Testado en Mac OSX 10.9 satisfactoriamente.


using namespace std;

//Variables Globales
const unsigned long m = 3*10^6 ;  // número de muestras (tres millones)
const unsigned n      = 4 ;                      // número de hebras
double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)


//Funcion a la que se le calcula la integral (en este caso 4/(1+x^2) )
double f(double x){
    return 4.0/(1+x*x) ;
}

/* Utiliza el metodo de Rieman (el de los rectangulitos) y evalua la funcion en el punto medio del rectangulo) */
double calcular_integral_secuencial(){
    double suma = 0.0 ;                      // inicializar suma
    for( unsigned long i = 0 ; i < m ; i++) // para cada $i$ entre $0$ y $m-1$
        suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
    return suma/m ;                          // devolver valor promedio de $f$
}


/*
 Utilizo el metodo de Rieman pero evaluando en el inicio del rectangulito (porque el limite es el mismo y asi corre mas)
 Divido en n subintervalos iguales y en cada uno de ellos actua una hebra.
 Cada hebra podría coger aquellos indices tales que sean congruentes en modulo n con el id_hebra.
 */
void * funcion_hebra( void * id_hebra_param ){ 					// función que ejecuta cada hebra
    unsigned long id_hebra = (unsigned long) id_hebra_param;     // número o índice de esta hebra
    double sumap = 0.0;
    // calcular suma parcial en "sumap"
    unsigned long inicio = m/n*id_hebra;
    unsigned long fin = (m/n*id_hebra)+ m/n;
    for( unsigned long i = inicio; i < fin ; i++)    // para cada $i$ entre inicio y fin
        sumap += f( (i+0.5)/m );               	     // $~~~~~$ añadir $f(x_i)$ a la sumap actual
    resultado_parcial[id_hebra] = sumap/m; 			 // guardar sumap parcial en vector.
    return NULL ;
}

double calcular_integral_concurrente(){
    pthread_t id_hebras[n]; //declaramos un vector de hebras
    double resultado=0;
    for(long int i=0; i<n; i++){    //NOTA: Uso long int para que sizeof(long int) = sizeof(void*)
        pthread_create(&(id_hebras[i]),NULL,funcion_hebra,(void*) i);
    }
    // esperar (join) a que termine cada hebra, sumar su resultado
    for(unsigned i=0; i<n; i++){
        pthread_join(id_hebras[i],NULL);
    }
    // devolver resultado completo
    for(int i=0; i<n; i++){
        resultado+=resultado_parcial[i];
    }
    return resultado;
}

int main() {
    
    //variables para resultados
    double resultado_pi_secuencial;
    double resultado_pi_concurrente;
    
    //variables para el tiempo
    std::chrono::high_resolution_clock::time_point tantes, tdespues;
    std::chrono::duration<double> transcurrido;
    cout.precision(20); //precision del reloj
    
    //Test Secuencial
    tantes = std::chrono::high_resolution_clock::now(); //coger tiempo antes de la operación
    
    resultado_pi_secuencial = calcular_integral_secuencial(); //Caluclo de PI
    
    tdespues = std::chrono::high_resolution_clock::now();//coger tiempo después de la operación
    cout<<"Calculo secuencial:"<<resultado_pi_secuencial<<endl;
    transcurrido = std::chrono::duration_cast<std::chrono::duration<double> >(tdespues - tantes);
    cout <<  "Tiempo secuencial: "<<transcurrido.count() << endl;
    
    //Test Concurrente
    tantes = std::chrono::high_resolution_clock::now(); //coger tiempo antes de la operación
    
    resultado_pi_concurrente = calcular_integral_concurrente(); //Caluclo de PI
    
    tdespues = std::chrono::high_resolution_clock::now();//coger tiempo después de la operación
    cout<<"Calculo concurrente:"<< resultado_pi_concurrente <<endl;
    transcurrido = std::chrono::duration_cast<std::chrono::duration<double> >(tdespues - tantes);
    cout << "Tiempo concurrente: "<<transcurrido.count() << endl;
    return 0;
}

