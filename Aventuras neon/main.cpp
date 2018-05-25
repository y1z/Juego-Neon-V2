#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>
#include <vector>
#include <chrono>
#include "Jugador.h"
#include "Oponentes.h"
std::vector<std::vector<char>> MapaOriginal;

bool Juego_Activo = true;

int Salida_X;
int Salida_Y;

int max_x = 7;
int max_y = 8;

int Jefe_X;
int Jefe_Y;

std::vector<std::vector<bool> > visitados;// usar esto para revisar los cuartos que visite

std::vector<std::vector<bool> > Enemigos_ubicacion; // estos vectores sirven para saber si ya visite a un enemigo en sierta parte del mapa 

std::vector<std::vector<bool>> Tesoro;

std::vector<std::vector<char>> salida;

std::vector<std::vector<bool>> Puertas;

std::vector<std::vector<bool>> UbicacionJefe;


std::vector<std::vector<char>> Nivel_1 // uso estos vectores para dibujar los Nivels del juego este es el Primer piso 
{
	{ '*', '*', '*', '!', '*', 'K' },
	{ 'F', '#', ' ', 'b', ' ', ' ' },
	{ '*', '*', 'k', ' ', '*', '!' },
	{ 'k', ' ', ' ', ' ', ' ', '*' },
	{ 'E', ' ', ' ', ' ', '#', '!' },
	{ ' ', ' ', '#', ' ', ' ', '*' },
	{ ' ', '*', '!', '*', 'e', '*' },
	{ ' ', '*', '*', '*', 'e', ' ' },
	{ ' ', ' ', 'S', ' ', ' ', ' ' },
};


std::vector<std::vector<char>> Nivel_2// esto es el segundo nivel
{
	{ '*','E','*',' ',' ',' ',' ','*','*' },
	{ '*','#','*',' ',' ','*','!','E',' ' },
	{ 'S','k',' ',' ',' ','*','*',' ',' ' },
	{ '*',' ',' ','*','!',' ','*','k','*' },
	{ '!','#',' ','*','!',' ','!','*','*' },
	{ '!','e',' ',' ','*',' ',' ',' ','*' },
	{ '*',' ','#','!','#',' ',' ','!','*' },
	{ '!','E',' ','*','*','*','*','b','!' },
	{ '*',' ',' ','#',' ',' ','!','!','F' }
};

std::vector<std::vector<char>> Nivel_3// esto es el trecer nivel
{
	{ 'B',' ',' ',' ',' ',' ',' ','!' },
	{ ' ',' ',' ','*','!','e',' ',' ' },
	{ ' ',' ',' ','*','*','!','e',' ' },
	{ ' ',' ',' ',' ',' ','*',' ',' ' },
	{ '!','!','*',' ',' ',' ','*',' ' },
	{ '!','!','*',' ',' ',' ','S',' ' }
};

std::vector<std::vector<char>> Test// para probar el fog of war del juego( y el juego en general ) 
{
	{ '1','2','3','4','5' },
	{ '1','2','!','!','!' },
	{ '#','E',' ',' ','!' },
	{ '#','e',' ',' ','!' },
	{ '#','#','#','#','#' },
};

std::vector<std::vector<char>> Personaje_Jugador
{
	{ ' ',' ',' ',' ',' ',' ', '\t' },
	{ ' ',' ',' ','@',' ',' ', '\t' },
	{ ' ',' ','/','|','\\',' ', '\t' },
	{ ' ',' ',' ','|',' ',' ', '\t' },
	{ ' ',' ','/',' ','\\',' ', '\t' },
	{ ' ',' ','|',' ','|',' ', '\t' },
	{ ' ',' ',' ',' ',' ',' ', '\t' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\t' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\t' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\t' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\t' },
};

std::vector<std::vector<char>> personaje_2
{
	{ '\t',' ','?','?','?',' ', ' ' },
	{ '\t',' ','?','°','?',' ', ' ' },
	{ '\t',' ',' ','|',' ',' ', ' ' },
	{ '\t',' ','/','|','\\',' ', ' ' },
	{ '\t',' ','\\','0','/',' ', ' ' },
	{ '\t',' ',' ','-',' ',' ', ' ' },
	{ '\t',' ',' ',' ',' ',' ', ' ' },
	{ '\t','a','l','g','u','n',' ','W','A','Y',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
};

std::vector<std::vector<char>> Boss
{
	{ '\t',' ',' ',' ',' ',' ', ' ' },
	{ '\t',' ',' ','#','#',' ', ' ' },
	{ '\t',' ','^','|','|','^', ' ' },
	{ '\t',' ',' ','|','|',' ', ' ' },
	{ '\t',' ','\\',' ',' ','/', ' ' },
	{ '\t',' ',' ','-','-',' ', ' ' },
	{ '\t',' ',' ',' ',' ',' ', ' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
};

std::vector<std::vector<char>> Boss_Final
{
	{ '\t',' ',' ',' ',' ',' ', ' ' },
	{ '\t','0','|','^','|','0', ' ' },
	{ '\t',' ','^','|','^','^', ' ' },
	{ '\t','~',' ','|',' ','~', ' ' },
	{ '\t',' ','#',' ','#','*', ' '},
	{ '\t',' ',' ','-','*',' ', '*' },
	{ '\t',' ','°','°','°','*', ' ' },
	{ '\t',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ '\t','°','°','°','°','°','?','?','?','°',' ' },
	{ '\t','*',' ',' ','/','0','\\',' ',' ','*',' ' },
	{ '\t',' ','*',' ','\\',' ','/',' ','*',' ',' ' },
	{ '\t',' ',' ','*','*','*','*','*',' ',' ',' ' },
};// |°|_0_|°|


void extra_line(int x){
	for (int i = 0; i < x; ++i)
	{
		std::cout << std::endl;
	}
}

void stop(){
	std::cin.ignore();
	std::cin.get();
}

void DibujarCombate(std::vector<std::vector<char>> Figura_1, std::vector<std::vector<char>> Figura_2)
{
	extra_line(2);
	for (int j = 0; j < Figura_1.size(); j++)
	{
		std::cout << std::endl;
		for (int i = 0; i < Figura_1[j].size(); i++)
		{
			std::cout << Figura_1[j][i];
		}
		for (int l = 0; l < Figura_2[j].size(); l++)
		{
			std::cout << Figura_2[j][l];
		}
	}
	extra_line(2);
}

void MensajeBatalla() 
{
std::cout << " ?que vas hacer ?" << std::endl
		  << " -------- -----  " << std::endl
          << "| (a)taque | (h)eal |" << std::endl
	<< " -------- -----  "; extra_line(2);
}

Jugador EleccionOponente(char TipoEnemigo) 
{
	Jugador Enemigo_Debil;// usare la clase del jugador para crear los enemigos del mismo jugador 
	Enemigo_Debil.M_Ataque = 1;
	Enemigo_Debil.M_Vida = 3;

	Jugador Enemigo_fuerte;
	Enemigo_fuerte.M_Ataque = 2;
	Enemigo_fuerte.M_Vida = 5;

	Jugador Jefe;
	Jefe.M_Vida = 12;
	Jefe.M_Ataque = 2;

	Jugador El_mamietor;// jefe final 
	El_mamietor.M_Vida = 18;
	El_mamietor.M_Ataque = 4;
	switch (TipoEnemigo)
	{
	case('e'):
		return Enemigo_Debil;
		break;
	case('E'):
		return Enemigo_fuerte;
		break;
	case('b'):
		return Jefe;
		break;
	case('B'):
		return El_mamietor;
		break;
	default:
		break;
	}

}

void Combate(std::vector<std::vector<char>> Figura_1, std::vector<std::vector<char>> Figura_2, Jugador &jugador, char tipo_enemigo) // esta funcion se encargara de dibujar los personajes que van estar en combate( possiblemente tambien los ataques ) 
{
	Jugador Oponente;

	std::string Acciones_Batalla;

	bool enemigo_vivo = true;

	// las dos figuras deberia ser del mismo tamaño

	DibujarCombate(Figura_1, Figura_2);
	
	Oponente = EleccionOponente(tipo_enemigo);

	while (enemigo_vivo == true )
	{
		MensajeBatalla();
		if (jugador.M_Vida < 1) {
			std::cout << "Game Over try again";
			enemigo_vivo = false;    Juego_Activo = false; 
		}// termina el juego 

		std::cin >> Acciones_Batalla;
		if (Acciones_Batalla == "ataque" || Acciones_Batalla == "Ataque" || Acciones_Batalla == "ATAQUE" || Acciones_Batalla == "a" || Acciones_Batalla == "A") {
			Oponente.M_Vida -= jugador.M_Ataque;
			std::cout << "vida Oponente : " << Oponente.M_Vida<<std::endl;
			std::cout << "Vida " << jugador.M_Nombre << ": " << jugador.M_Vida<<std::endl;
		}
		else if (Acciones_Batalla == "Cure" || Acciones_Batalla == "heal" || Acciones_Batalla == "h" || Acciones_Batalla == "H") {
			if (jugador.M_Vida >= jugador.M_VidaMaxima){
				std::cout << " no puedes sanar algo que no esta herido ";
			}
			else{
				jugador.M_Vida += 3;
			}
		}
		if (Oponente.M_Vida < 1){
			enemigo_vivo = false;
			std::cout << "lo matastes men " << std::endl;
		}
		else {
				jugador.M_Vida -= Oponente.M_Ataque;
				std::cout << " Te atacan por : " << Oponente.M_Ataque << " puntos de vida "; extra_line(2);//para dar mas espacio
				std::cout << "tu vida es : " << jugador.M_Vida << std::endl;
		}
		DibujarCombate(Figura_1, Figura_2);
	}
	if (tipo_enemigo == 'B' && Oponente.M_Vida < 1) {
		std::cout << "Gracias por Jugar ya se va a terminar el juego solo preciona enter "; extra_line(3);
		stop();
		Juego_Activo = false;
	}
}

void Fortuna( Jugador &jugador,char X) {
long int reconpensa = 0;
	srand(time(0));

	extra_line(20);
	reconpensa = rand();

	switch (X) 
	{
	case '!':
			if (reconpensa % 3 == 0) {
				jugador.inventario.push_back("pocion");
			}
			else if (reconpensa % 3 == 1) {
				jugador.inventario.push_back("contenedor_de_corazon");
			}
			else if (reconpensa % 3 == 2) 	{
				jugador.inventario.push_back("anime");
			}
			std::cout << "tu has cosiguido tesoro ";
			extra_line(3); break;
	case'k':
			jugador.inventario.push_back("llave");
			std::cout<< "resibeste una llave ";
			extra_line(2); break;
	case 'K':
			jugador.inventario.push_back("llave");
			std::cout << "resibeste una llave ";
			extra_line(2); break;
	default:
		break;
	}
}
//-----------------------------------------------------------------//
void UsarInventario(Jugador &jugador) //fucion para el inventario 
{
	std::string Elejir;
	std::string ObjetoDesiado = "";
	int cantidad = 0;
	bool verificador = false;

	for (int i = 0; i < jugador.inventario.size(); ++i)
	{
		if (jugador.inventario[i] == "anime")
		{
			++cantidad;
		}
	}
	std::cout << "Tienes : " << cantidad << " anime, ";
	cantidad = 0;

	for (int i = 0; i < jugador.inventario.size(); ++i)
	{
		if (jugador.inventario[i] == "contenedor_de_corazon")
		{
			++cantidad;
		}
	}
	std::cout << cantidad << " contenedor de corazones, ";
	cantidad = 0;

	for (int i = 0; i < jugador.inventario.size(); ++i)
	{
		if (jugador.inventario[i] == "pocion")
		{
			++cantidad;
		}
	}
	std::cout << cantidad << " pocion, ";
	cantidad = 0;
	std::cout << " para usar preciona las numeros debajo del objeto que quieras usar " << std::endl << "	[1]		[2]		      [3]" << std::endl;
	std::cin >> Elejir;
	if (Elejir == "1")
	{
		ObjetoDesiado = "anime";
	}
	else if (Elejir == "2")
	{
		ObjetoDesiado = "contenedor_de_corazon";
	}
	else if (Elejir == "3"){
		ObjetoDesiado = "pocion";
	}
	else{
		std::cout << "no se que chigados es lo que quieres men intentalo de nuevo ";
	}

	if (Elejir == "1" || Elejir == "2" || Elejir == "3"){

		for (int i = 0; i < jugador.inventario.size(); ++i)
		{
			if (jugador.inventario[i] == ObjetoDesiado)
			{
				jugador.inventario.erase(jugador.inventario.begin() + i);
				verificador = true;
				break;
			}
		}
	}

	if (ObjetoDesiado == "anime"&& verificador == true){
		jugador.M_Ataque += 1;
		std::cout << "El anime te inspira para dar putazos mas fuertes PLUS ULTRA !!!!!!!!!!";
	}
	else if (ObjetoDesiado == "contenedor_de_corazon"&& verificador == true){
		jugador.M_VidaMaxima += 1;
		jugador.M_Vida = jugador.M_VidaMaxima;
		std::cout << "tu vida maxima ahora es " << jugador.M_VidaMaxima;
	}
	else if (ObjetoDesiado == "pocion"&& verificador == true){
		if (jugador.M_Vida >= jugador.M_VidaMaxima)
		{
			std::cout << "men que haces no estas herido casi gastas una pocion ";
			jugador.inventario.push_back("pocion");
		}
		else {
			jugador.M_Vida += jugador.M_VidaMaxima / 2;
			if (jugador.M_Vida > jugador.M_VidaMaxima) {
				jugador.M_Vida = jugador.M_VidaMaxima;
			}
			std::cout << "te sansate men ";
		}
	}
	else{
		std::cout << std::endl << "no tienes ese Objetos que esta pidiendo O pusiste un comando incorrecto ";
	}
}


void CrearNivle(std::vector<std::vector<char>> &MapaReferencia, Jugador &jugador, std::vector<std::vector<char>> &MapaAfectado)
{
	int nuevo_max_x = 0;
	int nuevo_max_y = 0;

		if (MapaReferencia == Nivel_1) 	{
			MapaReferencia = Nivel_2;
			MapaAfectado = Nivel_2;
		}
		else if (MapaReferencia == Nivel_2){
			MapaReferencia = Nivel_3;
			MapaAfectado = Nivel_3;
		}
		else{
			MapaReferencia = Nivel_1;
			MapaAfectado = Nivel_1;
		}

		for (int i = 0; i < MapaReferencia.size(); ++i) // este for es para crear los bordes de los mapas para no sailrse de ellos 
		{
			if (nuevo_max_x < MapaAfectado[i].size()){
				nuevo_max_x = MapaAfectado[i].size();
			}
			++nuevo_max_y;
		}
		max_x = nuevo_max_x;
		max_y = nuevo_max_y;

		visitados.clear();

		for (int j = 0; j < MapaReferencia.size(); ++j)// crear un vector vacio 
		{
			visitados.push_back({});
			for (int i = 0; i < max_x; ++i)// Inicializar las posiciones como no vististadas o false 
			{
				visitados[j].push_back(false);
			}
		}
		Enemigos_ubicacion.clear();

		for (int j = 0; j < MapaReferencia.size(); ++j)
		{
			Enemigos_ubicacion.push_back({});

			for (int i = 0; i < max_x; ++i){
				if (MapaReferencia[j][i] == 'e'|| MapaReferencia[j][i] == 'E'){
					Enemigos_ubicacion[j].push_back(true);
				}
				else{
					Enemigos_ubicacion[j].push_back(false);
				}
			}
		}
		for (int j = 0; j < MapaReferencia.size(); ++j)
		{
			for (int i = 0; i < MapaReferencia[j].size(); ++i)
			{
				if (MapaReferencia[j][i] == 'S')
				{
					jugador.M_Y = j;
					jugador.M_X = i;
					break;
				}
			}
		}

		for (int j = 0; j < MapaReferencia.size(); ++j) {
			for (int i = 0; i < MapaReferencia[j].size(); ++i) 
			{
				if (MapaReferencia[j][i] == 'F') {
					Salida_Y = j;
					Salida_X = i;
					break;
				}
			}
		}

		Tesoro.clear();
		for (int j = 0; j < MapaReferencia.size(); ++j){
			Tesoro.push_back({});
			for (int i = 0; i < MapaReferencia[j].size(); ++i)
			{
				if (MapaReferencia[j][i] == '!' || MapaReferencia[j][i] == 'K' || MapaReferencia[j][i] == 'k'){
					Tesoro[j].push_back(true);
				}
				else{
					Tesoro[j].push_back(false);
				}
			}
		}
		UbicacionJefe.clear();
		for (int j = 0; j < MapaReferencia.size(); ++j)
		{
			UbicacionJefe.push_back({});
			for (int i = 0; i < MapaReferencia[j].size(); ++i)
			{
				if (MapaReferencia[j][i] == 'b' || MapaReferencia[j][i] == 'B')
				{
					UbicacionJefe[j].push_back(true);
				}
				else{
					UbicacionJefe[j].push_back(false);
				}
			}
		}
		Puertas.clear();
		for (int j = 0; j < MapaReferencia.size(); ++j) 
		{
			Puertas.push_back({});
			for (int i = 0; i < MapaReferencia[j].size(); ++i) {
				if (MapaReferencia[j][i] == '#')
					Puertas[j].push_back(true);
				else {
					Puertas[j].push_back(false);
				}
			}
		}
}

void dibujar_mapa(int x, int y, std::vector<std::vector<char>> &mapa, Jugador &jugador)// los parametros de esta funcion son 2 int , vector de vectors de charcateres , y la class jugador 
{
	int eje_x = x;
	int eje_y = y;
	char simbolo_mapa;

	for (int j = 0; j < mapa.size(); ++j)
	{
		std::cout << '\n';
		for (int i = 0; i < mapa[j].size(); ++i)
		{
			//;// se convierte en el character que esta en la posicion del mapa actual 
			if (eje_y == j && eje_x == i)// imprime la ubicacion del personajes 
			{
				std::cout << '@';
				visitados[j][i] = true;
			}
			else if (mapa[eje_y][eje_x] == 'e' && Enemigos_ubicacion[eje_y][eje_x] == true)// me mete en un combate con un enemigo debil 
			{
				simbolo_mapa = mapa[eje_y][eje_x];
				Enemigos_ubicacion[eje_y][eje_x] = false;

				Combate(Personaje_Jugador, personaje_2, jugador, simbolo_mapa);
			}
			else if (mapa[eje_y][eje_x] == 'E' && Enemigos_ubicacion[eje_y][eje_x] == true) //  meterme en un combate con un enemigo fuerte 
			{
				simbolo_mapa = mapa[eje_y][eje_x];
				Enemigos_ubicacion[eje_y][eje_x] = false;

				Combate(Personaje_Jugador, personaje_2, jugador, simbolo_mapa);
			}
			else if (mapa[eje_y][eje_x] == 'b'&& UbicacionJefe[eje_y][eje_x] == true || mapa[eje_y][eje_x] == 'B'&& UbicacionJefe[eje_y][eje_x] == true)
			{
				simbolo_mapa = mapa[eje_y][eje_x];
				UbicacionJefe[eje_y][eje_x] = false;

				Combate(Personaje_Jugador, Boss, jugador, simbolo_mapa);

			}
			else if (mapa[eje_y][eje_x] == 'b'&& UbicacionJefe[eje_y][eje_x] == false)
			{
				mapa[eje_y][eje_x] = 'x';
			}
			else if (mapa[eje_y][eje_x] == 'B'&& UbicacionJefe[eje_y][eje_x] == false)
			{
				mapa[eje_y][eje_x] = 'X';
			}
			else if (mapa[eje_y][eje_x] == 'e' && Enemigos_ubicacion[eje_y][eje_x] == false)// para cuando un enemigo sea derotado intercambie su simbolo con una 'x' para indicar que esta muerto 
			{
				mapa[eje_y][eje_x] = 'x';
			}
			else if (mapa[eje_y][eje_x] == 'E' && Enemigos_ubicacion[eje_y][eje_x] == false)// para cuando un enemigo sea derotado intercambie su simbolo con una 'X' para indicar que esta muerto 
			{
				mapa[eje_y][eje_x] = 'X';

			}
			else if (mapa[eje_y][eje_x] == '!' && Tesoro[eje_y][eje_x] == true) 
			{
				Tesoro[eje_y][eje_x] = false;
				simbolo_mapa = mapa[eje_y][eje_x];
				Fortuna(jugador,simbolo_mapa);
			}
			else if (mapa[eje_y][eje_x] == 'k' && Tesoro[eje_y][eje_x] == true)
			{
				Tesoro[eje_y][eje_x] = false;
				simbolo_mapa = mapa[eje_y][eje_x];
				Fortuna(jugador, simbolo_mapa);
			}
			else if (mapa[eje_y][eje_x] == 'K' && Tesoro[eje_y][eje_x] == true)
			{
				Tesoro[eje_y][eje_x] = false;
				simbolo_mapa = mapa[eje_y][eje_x];
				Fortuna(jugador, simbolo_mapa);
			}
			else if (mapa[eje_y][eje_x] == 'K' && Tesoro[eje_y][eje_x] == false) // para indicar que la llave no esta ahi ( el jugador ahora la tiene )
			{
				mapa[eje_y][eje_x] = ' ';
			}
			else if (mapa[eje_y][eje_x] == 'k' && Tesoro[eje_y][eje_x] == false)// para indicar que la llave no esta ahi ( el jugador ahora la tiene )
			{
				mapa[eje_y][eje_x] = ' ';
			}
			else if (mapa[eje_y][eje_x] == '!' &&Tesoro[eje_y][eje_x] == false) //para cuando consigas algun tesoro 
			{
				mapa[eje_y][eje_x] = ' ';
			}
			else
			{
				if (visitados[j][i] == true)
				{
					std::cout << mapa[j][i];
				}
				else if (j == eje_y && i == eje_x + 1 && visitados[j][i] == false) // este 
				{
					std::cout << mapa[j][i];

					if (eje_x <= max_x) // checa si estamos dentro de los bordes del mapa y para no sailrnos del rango de los vectores 
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y && i == eje_x - 1 && visitados[j][i] == false) // oeste
				{
					std::cout << mapa[j][i];
					if (eje_x >= 1)
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y + 1 && i == eje_x && visitados[j][i] == false) // sur
				{
					std::cout << mapa[j][i];
					if (eje_y <= mapa.size())
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y - 1 && i == eje_x && visitados[j][i] == false) // norte
				{
					std::cout << mapa[j][i];
					if (eje_y > 0)
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y + 1 && i == eje_x + 1 && visitados[j][i] == false) // sur este
				{
					std::cout << mapa[j][i];
					if (eje_y <= mapa.size() && eje_x <= max_x)
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y + 1 && i == eje_x - 1 && visitados[j][i] == false) // sur oeste
				{
					std::cout << mapa[j][i];
					if (eje_y <= mapa.size() && eje_x >= 1)
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y - 1 && i == eje_x + 1 && visitados[j][i] == false)  //nor este
				{
					std::cout << mapa[j][i];
					if (eje_y > 0 && eje_x <= max_x)
					{
						visitados[j][i] = true;
					}
				}
				else if (j == eje_y - 1 && i == eje_x - 1 && visitados[j][i] == false) // nor oeste
				{
					std::cout << mapa[j][i];
					if (eje_y > 0 && eje_x >= 1)
					{
						visitados[j][i] = true;
					}
				}
				else	{
					std::cout << '.';
				}
			}
		}
	}
}
	// aqui abajo estan los Instrucciones del juego 
void Instrucciones()
{
	std::cout << std::endl << "te puedes mover con las teclar 'n' , 's', 'e','o' y 'w' , las teclas 'w' y 'o' hacen lo mismo " << std::endl;
	std::cout << "otras comandos que puedes hacer tambien son 'nav', para ver tus estadisticas [nombre] [ataque] [vida] en es orden , 'fin' para sailr del juego " << std::endl;
	std::cout << " 'ayuda' para ver estas intrucciones de nuevo , tambien esta el comando 'guardar', 'save'( hacen los mismo )para " << std::endl << "guardar la partida del juego." <<
		std::endl<<"usa la 'i' para ver tu inventario y el comando 'usar' para usar lo que esta en tu inventario "<< std::endl << " aqui esta la lista de que simbolo que se usan para representar visualmente todo del juego " << std::endl;
	std::cout << " @ = tu personaje " << '\t'
		<< " * = Una pared (no puedes atravesarlas )" << std::endl
		<< " e = enemigo debil " << '\t'
		<< " E = enemigo fuerte" << std::endl
		<< " k = llave " << '\t'
		<< " # = puertas " << std::endl;
	stop();
}


//  Funcion de guardar la partida 
void Guardar_Partida(Jugador &jugador, std::vector<std::vector<char>> mapa, std::vector<std::vector<bool> > visibilidad_mapa)
{
	std::ofstream file("partida.txt");

	file << jugador.M_Nombre << ' ' << jugador.M_Ataque << ' ' << jugador.M_Vida << ' ' << jugador.M_X << ' ' << jugador.M_Y << ' ' << mapa.size() << ' ' << mapa[0].size() <<' '<< max_x <<' '<< max_y <<' '<<Salida_X<<' '<<Salida_Y<< std::endl;
	
	for (int i = 0; i < mapa.size(); ++i)
	{
		for (int j = 0; j < mapa[i].size(); ++j)
		{
			if (mapa[i][j] == ' ') 
			{
				file << '.';
			}
			else 
			{
				file << mapa[i][j];
			}
		}
		file << std::endl;
	}
	for (int i = 0; i < visibilidad_mapa.size(); ++i)
	{
		for (int j = 0; j < visibilidad_mapa[i].size(); ++j)
		{
			if (visibilidad_mapa[i][j] == true)
			{
				file << 't';
			}
			else
			{
				file << 'f';
			}

		}
		file << std::endl;
	}

	for (int i = 0; i < Enemigos_ubicacion.size(); ++i) 
	{
		for (int j = 0; j < Enemigos_ubicacion[i].size(); ++j) 
		{
			if (Enemigos_ubicacion[i][j] == true)
			{
				file << 't';
			}
			else
			{
				file << 'f';
			}
		}
		file << std::endl;
	}
	for (int j = 0; j < Tesoro.size(); ++j) 
	{
		for (int i = 0; i < Tesoro[j].size(); ++i) 
		{
			if (Tesoro[j][i] == true) 
			{
				file << 't';
			}
			else
			{
				file << 'f';
			}
		}
		file << std::endl;
	}
	for (int j = 0; j < Puertas.size(); ++j) 
	{
	
		for(int i =0; i < Puertas[j].size(); ++i)
		{
			if (Puertas[j][i] == true) 
			{
				file << 't';
			}
			else
			{
				file << 'f';
			}
		}
		file << std::endl;
	}
	for (int j = 0; j < UbicacionJefe.size(); ++j)
	{
		for (int i = 0; i < UbicacionJefe[j].size(); ++i)
		{
			if (UbicacionJefe[j][i] == true)
			{
				file << 't';
			}
			else
			{
				file << 'f';
			}
		}
		file << std::endl;
	}
	for (int j = 0; j < jugador.inventario.size(); ++j) 
	{
		file << jugador.inventario[j] << ' ';
	}

	file << '~';
	file.close();
}

void VerificarObsatculos(std::vector<std::vector<char>> Mapa ,Jugador &jugador ,std::string direccion)
{
	char dir;
	bool TieneLlave = false;
	dir = direccion[0];
	switch (dir)
	{
	case('N'):
		dir = 'n'; break;
	case('S'):
		dir = 's'; break;
	case('E'):
		dir = 'e'; break;
	case('O'):
		dir = 'o'; break;
	case('W'):
		dir = 'w'; break;
	default:
		break;
	}
	for (int i = 0; i < jugador.inventario.size(); ++i)	{
		if (jugador.inventario[i] == "llave"){
			TieneLlave = true;
			break;
		}
	}

	if (TieneLlave == true)
	{
		Puertas[jugador.M_Y][jugador.M_X] = false;
		for (int i = 0; i < jugador.inventario.size(); ++i){
			if (jugador.inventario[i] == "llave"){
				jugador.inventario.erase(jugador.inventario.begin() + i);
				break;
			}
		}
		std::cout << "Usaste un llave men " << std::endl;
	}
	else{
		if (dir == 'n') { jugador.M_Y += 1; }
		else if (dir == 's') { jugador.M_Y -= 1; }
		else if (dir == 'e') { jugador.M_X -= 1; }
		else if (dir == 'o' || dir == 'w') { jugador.M_X += 1; }
	}

}

// funcion main  
int main()
{

	std::vector<std::vector<char>> Copia_nivel;

	Jugador Mi_Jugador;

	std::string Nombre_jugador;
	std::string comando;
	std::string Nombre_Partida;

	int nuevo_max_x = 0;// para determinar el borde del eje x en el juego
	int nuevo_max_y = 0;// para determinar el borde del eje y en el juego

	std::cout << " ingresa tu nombre  " << std::endl;

	std::cin >> Nombre_jugador;

	Mi_Jugador.M_Nombre = Nombre_jugador;

		Copia_nivel = Nivel_1;

		CrearNivle(MapaOriginal, Mi_Jugador,Copia_nivel);
	
		Instrucciones();

	dibujar_mapa(Mi_Jugador.M_X, Mi_Jugador.M_Y, Copia_nivel, Mi_Jugador);

	while (Juego_Activo == true)
	{
		std::cout <<std::endl<<" dime lo que quieres hacer \n";

		if (Mi_Jugador.M_X == Salida_X && Mi_Jugador.M_Y == Salida_Y){
			CrearNivle(MapaOriginal, Mi_Jugador, Copia_nivel);
			dibujar_mapa(Mi_Jugador.M_X, Mi_Jugador.M_Y, Copia_nivel, Mi_Jugador);
		}

		std::cin >> comando;

		if (comando == "N" || comando == "n" || comando == "norte" || comando == "Norte")// encargado del movimiento y que el jugador no se salga de las bareras del juego 
		{
			Mi_Jugador.M_Y -= 1;

			if (Mi_Jugador.M_Y < 0)
			{
				Mi_Jugador.M_Y += 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '*')// esto es para que el juguador no pueda atravesar paredes
			{
				Mi_Jugador.M_Y += 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '#' && Puertas[Mi_Jugador.M_Y][Mi_Jugador.M_X] == true)
			{
				VerificarObsatculos(Copia_nivel, Mi_Jugador, comando);
			}
		}
		else if (comando == "S" || comando == "s")// encargado del movimiento y que el jugador no se salga de las bareras del juego  
		{
			Mi_Jugador.M_Y += 1;

			if (Mi_Jugador.M_Y == max_y)
			{
				Mi_Jugador.M_Y -= 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '*')// esto es para que el juguador no pueda atravesar paredes
			{
				Mi_Jugador.M_Y -= 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '#' && Puertas[Mi_Jugador.M_Y][Mi_Jugador.M_X] == true)
			{
				VerificarObsatculos(Copia_nivel, Mi_Jugador, comando);
			}
		}
		else if (comando == "E" || comando == "e")// encargado del movimiento y que el jugador no se salga de las bareras del juego  
		{
			Mi_Jugador.M_X += 1;

			if (Mi_Jugador.M_X == max_x)
			{
				Mi_Jugador.M_X -= 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '*')// esto es para que el juguador no pueda atravesar paredes
			{
				Mi_Jugador.M_X -= 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '#' && Puertas[Mi_Jugador.M_Y][Mi_Jugador.M_X] == true)
			{
				VerificarObsatculos(Copia_nivel, Mi_Jugador, comando);
			}
		}
		else if (comando == "O" || comando == "o" || comando == "olaf" || comando == "w" || comando == "W" || comando == "wambo")// encargado del movimiento y que el jugador no se salga de las bareras del juego 
		{
			Mi_Jugador.M_X -= 1;

			if (Mi_Jugador.M_X < 0)
			{
				Mi_Jugador.M_X += 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '*')// esto es para que el juguador no pueda atravesar paredes
			{
				Mi_Jugador.M_X += 1;
			}
			else if (Copia_nivel[Mi_Jugador.M_Y][Mi_Jugador.M_X] == '#' && Puertas[Mi_Jugador.M_Y][Mi_Jugador.M_X] == true)
			{
				VerificarObsatculos(Copia_nivel, Mi_Jugador, comando);
			}
		}
		else if(comando=="jojo"||comando=="Jojo"||comando=="JOJO"){
			extra_line(2);
			std::cout << "ahora eres un dios "<<std::endl;
			Mi_Jugador.M_Vida += 20;
			Mi_Jugador.M_Ataque += 8;
			Mi_Jugador.M_VidaMaxima += 20;
			std::cout << "tu Vida es : " << Mi_Jugador.M_Vida << std::endl << "Tu ataque es : " << Mi_Jugador.M_Ataque << std::endl;
		}
		else if (comando == "Nombre" || comando == "nombre" || comando == "estadisticas" || comando == "todo" || comando == "nav" || comando == "NAV")
		{
			extra_line(2);
			std::cout << "Nombre : " << Mi_Jugador.M_Nombre;
			extra_line(2);
			std::cout << "Ataque : " << Mi_Jugador.M_Ataque;
			extra_line(2);
			std::cout << "Vida : " << Mi_Jugador.M_Vida;
			extra_line(2);
		}
		else if (comando == "N3P3" || comando == "n3p3" || comando == "Nani" || comando == "fin" || comando == "FIN")// comandos para sailr del juego ( posilblemente para guardar la partida tambien )
		{
			Juego_Activo = false;
			Guardar_Partida(Mi_Jugador, Copia_nivel, visitados);
		}
		else if (comando == "guardar" || comando == "Guardar" || comando == "GUARDAR" || comando == "gardar" || comando == "Gardar" || comando == "save" || comando == "SAVE" || comando == "Save"){
			Guardar_Partida(Mi_Jugador, Copia_nivel, visitados);
		}
		else if (comando == "z")
		{
			CrearNivle(MapaOriginal , Mi_Jugador, Copia_nivel);
		}
		else if (comando == "usar" || comando == "use" || comando == "Usar" || comando == "Use" || comando == "USE" || comando == "USAR"){
			UsarInventario(Mi_Jugador);
		}
		else if (comando == "cargar" || comando == "CARGAR" || comando == "Cargar")// crear un funcion que hace lo mismo 
		{
			std::ifstream partida("partida.txt");

			if (partida.is_open())// crea una funion que aga lo mismo 
			{

				char simbolos = ' ';

				int renglones = 0;
				int columnas = 0;

				std::string cosas = "";

				bool valor_binario = false;

				std::vector<std::vector<char>> Mapa_De_Partida;
				std::vector<std::vector<bool>> map_valores_binario;

				partida >> Mi_Jugador.M_Nombre >> Mi_Jugador.M_Ataque >> Mi_Jugador.M_Vida >> Mi_Jugador.M_X >> Mi_Jugador.M_Y >> columnas >> renglones >> max_x >> max_y >> Salida_X >> Salida_Y;

				for (int i = 0; i < columnas; ++i){
					Mapa_De_Partida.push_back({});

				}

				for (int i = 0; i < columnas; ++i) {
					map_valores_binario.push_back({});
				}
				
				while (partida>>simbolos && simbolos!='~')
				{
					Mapa_De_Partida[0].push_back(simbolos);// el while agara el primer simbolo del mapa que hace  que todo este fuera de orden con la forma anterior de como estabam los 'for' del 'while'.
					
					for (int j = 1; j < renglones; ++j) 
					{
						partida >> simbolos;
						if (simbolos == '.') {
							Mapa_De_Partida[0].push_back(' ');
						}
						else{
							Mapa_De_Partida[0].push_back(simbolos);
						}
						
					}
						
					for (int i = 1; i < columnas; ++i) 
					{
						for (int j = 0; j < renglones; ++j)
						{
							partida >> simbolos;
							if (simbolos == '.'){
								Mapa_De_Partida[i].push_back(' ');
							}
							else{
								Mapa_De_Partida[i].push_back(simbolos);
							}
						}
					}
	
					for (int i = 0; i < columnas; ++i)
					{
						for (int j = 0; j < renglones; ++j)
						{
							partida >> simbolos;

							if (simbolos == 'f')
							{
								valor_binario = false;
							}
							else
							{
								valor_binario = true;
							}
							map_valores_binario[i].push_back(valor_binario);
						}
					}

					Enemigos_ubicacion.clear();// esto esta aqui por si el caso que el jugador desa cargar una partida cuando esta jugado otra 
					for (int i = 0; i < columnas; ++i) {
						Enemigos_ubicacion.push_back({});
					}

					for (int i = 0; i < columnas; ++i)
					{

						for (int j = 0; j < renglones; ++j)
						{
							partida >> simbolos;

							if (simbolos == 'f'){
								valor_binario = false;
							}
							else{
								valor_binario = true;
							}
							Enemigos_ubicacion[i].push_back(valor_binario);
						}
					}
					Tesoro.clear();

					for (int i = 0; i < columnas; ++i){
						Tesoro.push_back({});
					}

					for (int j = 0; j < columnas; ++j) 
					{
					
						for (int i = 0; i < renglones; ++i) {

							partida >> simbolos;

							if (simbolos == 'f') 
							{
								valor_binario = false;
							}
							else
							{
								valor_binario = true;
							}
							Tesoro[j].push_back(valor_binario);
						}
					}
					Puertas.clear();
					for(int j=0; j < columnas;++j)
					{
						Puertas.push_back({});
						for (int i = 0; i < renglones; ++i) 
						{
							partida >> simbolos;

							if (simbolos == 't') 
							{
								valor_binario = true;
							}
							else
							{
								valor_binario = false;
							}
							Puertas[j].push_back(valor_binario);
						}
					
					}
					UbicacionJefe.clear();
					for (int j = 0; j < columnas; ++j)
					{
						UbicacionJefe.push_back({});
						for (int i = 0; i < renglones; ++i)
						{
							partida >> simbolos;
							if (simbolos == 't')
							{
								valor_binario = true;
							}
							else
							{
								valor_binario = false;
							}
							UbicacionJefe[j].push_back(valor_binario);
						}
					}
					break;
				}// aqui termina el while 
				while(partida>>cosas)
				{
					Mi_Jugador.inventario.push_back(cosas);

					partida >> cosas;
				}
				visitados = map_valores_binario;

				Copia_nivel = Mapa_De_Partida;

				std::cout << "se logro cargar la partida con exito " << std::endl;

				partida.close();
			}
			else{
				std::cout << "algo sailo mal y o no guardaste la partida ( el juego lo hacer automaticamente cuando ingrecas el commando " << std::endl << "---> 'fin' ";
			}
		}
		else if (comando == "ayuda" || comando == "Ayuda" || comando == "AYUDA") // original mente esto era un troll face pero  cuando insertaba el troll face solo saila '?' en vez de los caracteres que conformaban la cara del troll
		{
			extra_line(1);
			Instrucciones();
			extra_line(2);
		}
		else if (comando == "sana" || comando == "cura" || comando == "heal")
		{
			Mi_Jugador.M_Vida += 4;
			if (Mi_Jugador.M_Vida > Mi_Jugador.M_VidaMaxima)
			{
				Mi_Jugador.M_Vida = Mi_Jugador.M_VidaMaxima;
			}
		}
		else// esto deberia indicar que ingresaste un comando que no reconose el juego 
		{
			extra_line(2);
			printf("-------------------------------------------------------------------");
			extra_line(1);
			printf(" --> No entiendo lo que me estas diciendo ingresa otro comando <-- ");
			extra_line(1);
			printf("-------------------------------------------------------------------");
			extra_line(2);
		}
		dibujar_mapa(Mi_Jugador.M_X, Mi_Jugador.M_Y, Copia_nivel, Mi_Jugador);
		std::cout << std::endl;
		std::cout << "posicion en [x] " << Mi_Jugador.M_X << " posicion en [y] " << Mi_Jugador.M_Y;
	}
	return 0;
}

/* Este juego deberia tener los siguentes componentes

1) reducir lineas 
2) crear un funcio para cargar el juego 

*/