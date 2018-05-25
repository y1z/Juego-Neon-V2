#pragma once
#include<string>
#include<vector>

class Jugador // esta classe tambien se usa para los enemigos o boses del juego 
{
private:
	enum Hechisos// posible que no los use ( o le cambio el nombre y use un calendario chino para determinar que Hechisos puedas usar )
	{
		Fire = 2,
		Firaga = 4
	};

public:

	std::string M_Nombre;// lo ingresa el jugador

	double M_Vida = 10;
	double M_VidaMaxima = 10; // puede cambiar
	double M_Ataque = 2;	// puede cambiar 

	std::vector<std::string> inventario;

	int M_X;	// horizontales
	int M_Y;	// verticales

	struct Experiensa
	{
		int enemigos_derotados;
		int nivle = 1;
	};

	Experiensa ex;

public:
	Jugador();
	virtual~Jugador();
};
