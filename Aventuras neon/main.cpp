#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>


std::vector<std::vector<char>> Nivel_1 // uso estos vectores para dibujar los Nivels del juego este es el Primer piso 
{

	{ '*', '*', '*', '!', '*', 'K' },
{ 'F', '#', ' ', 'b', ' ', ' ' },
{ '*', '*', 'k', ' ', '*', '!' },
{ 'k', ' ', ' ', ' ', ' ', ' ' },
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
{ '*',' ','#','!','>',' ',' ','!','*' },
{ '!','E',' ','*','*','*','*','b','!' },
{ '*',' ',' ','#',' ',' ','!','!','f' }

};

std::vector<std::vector<char>> Nivel_3// esto es el trecer nivel
{

	{ 'B',' ',' ',' ',' ',' ',' ','!' },
{ ' ',' ',' ','*','!','e',' ',' ' },
{ ' ',' ',' ','*','*','!','e',' ' },
{ ' ',' ',' ',' ',' ','*',' ',' ' },
{ '!','!','*',' ',' ',' ','*',' ' },
{ '!','!','*',' ',' ',' ',' ','S' }

};

std::vector<std::vector<char>> Test// para probar el fog of war del juego( y el juego en general ) 
{
	{ '1','2','3','4','5' },
{ '1','2','#','k','K' },
{ '#','E',' ',' ','!' },
{ '#','e',' ',' ','#' },
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

bool Juego_Activo = true;


int max_x = 7;
int max_y = 8;

std::vector<std::vector<bool> > visitados;// usar esto para revisar los cuartos que visite

std::vector<std::vector<bool> > Enemigos_ubicacion; // estos vectores sirven para saber si ya visite a un enemigo en sierta parte del mapa 

std::vector<std::vector<bool>> Tesoro;
void extra_line(int x)
{
	for (int i = 0; i < x; ++i)
	{
		std::cout << std::endl;
	}
}

void stop()
{
	std::cin.ignore();
	std::cin.get();
}

class Jugador // esta classe tambien se usa para los enemigos o boses del juego 
{
private:
	enum Hechisos// posible que no los use ( o le cambio el nombre y use un calendario chino para determinar que Hechisos puedas usar )
	{
		Fire = 2,
		Firaga = 4
	};

public:

	struct Experiensa
	{
		int enemigos_derotados;
		int nivle = 1;

	};

	Experiensa ex;

	std::string M_Nombre;// lo ingresa el jugador

	double M_Vida = 10;
	double M_VidaMaxima = 10; // puede cambiar
	double M_Ataque = 2;	// puede cambiar 

	std::vector<std::string> inventario;

	int M_X;	// horizontales
	int M_Y;	// verticales

	Jugador();
	~Jugador();

};

Jugador::Jugador()
{
}

Jugador::~Jugador()
{
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

void Combate(std::vector<std::vector<char>> Figura_1, std::vector<std::vector<char>> Figura_2, Jugador &jugador, char tipo_enemigo) // esta funcion se encargara de dibujar los personajes que van estar en combate( possiblemente tambien los ataques ) 
{
	Jugador Enemigo_Debil;// usare la clase del jugador para crear los enemigos del mismo jugador 

	Enemigo_Debil.M_Ataque = 1;
	Enemigo_Debil.M_Vida = 3;

	Jugador Enemigo_fuerte;

	Enemigo_fuerte.M_Ataque = 2;
	Enemigo_fuerte.M_Vida = 5;

	std::string Acciones_Batalla;

	bool enemigo_vivo = true;

	// las dos figuras deberia ser del mismo tamaño

	DibujarCombate(Figura_1, Figura_2);
	

	switch (tipo_enemigo)
	{

	case('e'):
		while (enemigo_vivo == true)
		{
			std::cout << " ?que vas hacer ?" << std::endl;
			std::cout << " -------- -----  " << std::endl;
			std::cout << "| ataque | heal |" << std::endl;
			std::cout << " -------- -----  ";

			extra_line(3);

			std::cin >> Acciones_Batalla;

			if (Acciones_Batalla == "ataque" || Acciones_Batalla == "Ataque" || Acciones_Batalla == "ATAQUE" || Acciones_Batalla == "a" || Acciones_Batalla == "A")
			{
				Enemigo_Debil.M_Vida -= jugador.M_Ataque;
				if (Enemigo_Debil.M_Vida < 1) 
				{
					Enemigo_Debil.M_Vida = 0;
				}

				std::cout << " Vida Oponente : " << Enemigo_Debil.M_Vida<< std::endl;
			}
			else if (Acciones_Batalla == "Cure" || Acciones_Batalla == "heal" || Acciones_Batalla =="h" || Acciones_Batalla=="H")
			{
				if (jugador.M_Vida >= jugador.M_VidaMaxima)
				{
					std::cout << " no puedes sanar algo que no esta herido ";
				}
				else
				{
					jugador.M_Vida += 3;
				}

			}

			if (Enemigo_Debil.M_Vida < 1)
			{
				enemigo_vivo = false;
				std::cout << "lo matastes men "<<std::endl;
			}
			else
			{
				jugador.M_Vida -= Enemigo_Debil.M_Ataque;
				std::cout << " te golpean por : " << Enemigo_Debil.M_Ataque << " puntos de vida " << std::endl;
				std::cout << "tu vida es : " << jugador.M_Vida << std::endl;
			}

			DibujarCombate(Figura_1, Figura_2);
		}
		break;

	case('E'):

		while (enemigo_vivo == true)
		{
			std::cout << " ?que vas hacer ?" << std::endl;
			std::cout << " -------- -----  " << std::endl;
			std::cout << "| ataque | heal |" << std::endl;
			std::cout << " -------- -----  ";

			extra_line(3);

			std::cin >> Acciones_Batalla;

			if (Acciones_Batalla == "ataque" || Acciones_Batalla == "Ataque" || Acciones_Batalla == "ATAQUE" || Acciones_Batalla == "a" || Acciones_Batalla == "A")
			{
				Enemigo_fuerte.M_Vida -= jugador.M_Ataque;
				if (Enemigo_fuerte.M_Vida < 1) 
				{
					Enemigo_fuerte.M_Vida = 0;
				}
				std::cout << " Vida Oponente : " << Enemigo_fuerte.M_Vida;
				std::cout << std::endl;
			}
			else if (Acciones_Batalla == "Cure" || Acciones_Batalla == "heal" || Acciones_Batalla =="h" || Acciones_Batalla=="H")
			{
				if (jugador.M_Vida >= jugador.M_VidaMaxima)
				{
					std::cout << " no puedes sanar algo que no esta herido ";
				}
				else
				{
					jugador.M_Vida += 3;
				}

			}

			if (Enemigo_fuerte.M_Vida < 1)
			{
				enemigo_vivo = false;
				std::cout << "lo matastes men " << std::endl;
			}
			else
			{
				jugador.M_Vida -= Enemigo_fuerte.M_Ataque;
				std::cout << " te golpean por " << Enemigo_fuerte.M_Ataque << " puntos de vida " << std::endl;
				std::cout << "vida actual : " << jugador.M_Vida << std::endl;
			}
			DibujarCombate(Figura_1, Figura_2);
		}

	default:
		break;
	}

}


void Fortuna( Jugador &jugador,char X) 
{
	int reconpensa = 0;
	srand(time(0));

	extra_line(20);
	reconpensa = rand();

	switch (X) 
	{
	case '!':
			if (reconpensa % 3 == 0) 
			{
				jugador.inventario.push_back("pocion");
			}
			else if (reconpensa % 3 == 1) 
			{
				jugador.inventario.push_back("contenedor_de_corazon");
			
			}
			else if (reconpensa == 2) 
			{
				jugador.inventario.push_back("anime");
			
			}
			std::cout << "tu has cosiguido tesoro "<<std::endl;
			break;
	
	default:
		break;
	case'k':
		jugador.inventario.push_back("llave");
		std::cout<< "resibeste una llave ";
		extra_line(2);
		break;
	case 'K':
		jugador.inventario.push_back("llave");
		std::cout << "resibeste una llave ";
		extra_line(2);
		break;


	}
		
}

void dibujar_mapa(int x, int y, std::vector<std::vector<char>> &mapa, Jugador &jugador)// los parametros de esta funcion son 2 int , vector de vectors de charcateres , y la class jugador 
{
	int eje_x = x;
	int eje_y = y;

	char simbolo_mapa;

	for (int j = 0; j < mapa.size(); j++)
	{
		std::cout << '\n';
		for (int i = 0; i < mapa[j].size(); i++)
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
				else
				{
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
	std::cout << " 'ayuda' para ver un consejo que talvez te ayude , tambien esta el comando 'guardar', 'save'( hacen los mismo )para " << std::endl << "guardar la partida del juego." << std::endl << " aqui esta la lista de que simbolo que se usan para representar visualmente todo del juego " << std::endl;
	std::cout << " @ = tu personaje " << std::endl
		<< " * = Una pared (no puedes atravesarlas )" << std::endl
		<< " e = enemigo debil " << std::endl
		<< " E = enemigo fuerte" << std::endl
		<< " k = llave " << std::endl
		<< " # = puertas " << std::endl;
	std::cout << "pos data DEJA DE VALER VERGA gracias ;)" << std::endl;
}


//  Funcion de guardar la partida 
void Guardar_Partida(Jugador &jugador, std::vector<std::vector<char>> mapa, std::vector<std::vector<bool> > visibilidad_mapa)
{
	std::ofstream file("partida.txt");

	file << jugador.M_Nombre << ' ' << jugador.M_Ataque << ' ' << jugador.M_Vida << ' ' << jugador.M_X << ' ' << jugador.M_Y << ' ' << mapa.size() << ' ' << mapa[0].size() <<' '<< max_x <<' '<< max_y << std::endl;
	
	
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
	file << '~';
	file.close();

}

// funcion main  
int main()
{

	//stop();
	std::vector<std::vector<char>> Copia_nivel;

	Jugador Mi_Jugador;

	std::string Nombre_jugador;
	std::string comando;
	std::string Nombre_Partida;

	int nuevo_max_x = 0;// para determinar el borde del eje x en el juego
	int nuevo_max_y = 0;// para determinar el borde del eje y en el juego

	std::cout << " ingresa tu nombre  " << std::endl;

	std::cin >> Nombre_jugador;

	std::cout << " dime que piso quieres tienes estas opciones ([{ 1,2,3}]) "; // esta parte del codigo es SOLO para probar el juego y tambien el std::cin>>comando que esta debajo

	std::cin >> comando;

	if (comando == "1")
	{
		Copia_nivel = Nivel_1;
	}
	else if (comando == "2")
	{
		Copia_nivel = Nivel_2;
	}
	else if (comando == "3")
	{
		Copia_nivel = Nivel_3;
	}
	else if (comando == "p")
	{
		Copia_nivel = Test;
	}
	else
	{
		Copia_nivel = Nivel_1;
	}
	for (int i = 0; i < Copia_nivel.size(); ++i) // este for es para crear los bordes de los mapas para no sailrse de ellos 
	{

		if (nuevo_max_x < Copia_nivel[i].size())
		{
			nuevo_max_x = Copia_nivel[i].size();
		}
		++nuevo_max_y;

	}
	max_x = nuevo_max_x;
	max_y = nuevo_max_y;

	Instrucciones();

	Mi_Jugador.M_Nombre = Nombre_jugador;

	Mi_Jugador.M_X = 2;
	Mi_Jugador.M_Y = 2;

	visitados.reserve(6);

	for (int j = 0; j < Copia_nivel.size(); ++j)// crear un vector vacio 
	{
		visitados.push_back({});

		for (int i = 0; i < max_x; ++i)// Inicializar las posiciones como no vististadas o false 
		{
			visitados[j].push_back(false);
		}
	}
	for (int j = 0; j < Copia_nivel.size(); ++j)
	{
		Enemigos_ubicacion.push_back({});

		for (int i = 0; i < max_x; ++i)
		{
			if (Copia_nivel[j][i] == 'e')
			{
				Enemigos_ubicacion[j].push_back(true);
			}
			else if (Copia_nivel[j][i] == 'E')
			{
				Enemigos_ubicacion[j].push_back(true);
			}
			else
			{
				Enemigos_ubicacion[j].push_back(false);
			}
		}
	}
	for (int j = 0; j < Copia_nivel.size(); ++j)
	{
		for (int i = 0; i < Copia_nivel[j].size(); ++i)
		{
			if (Copia_nivel[j][i] == 'S')
			{
				Mi_Jugador.M_Y = j;
				Mi_Jugador.M_X = i;
				break;
			}

		}
	}

	for (int j = 0; j < Copia_nivel.size(); ++j) 
	{
		Tesoro.push_back({});
	
		for (int i = 0; i < Copia_nivel[j].size(); ++i) 
		{
		
			if (Copia_nivel[j][i] == '!' || Copia_nivel[j][i]=='K' || Copia_nivel[j][i]=='k') 
			{
				Tesoro[j].push_back(true);
			}
			else
			{
				Tesoro[j].push_back(false);
			}
		}
	
	}

	dibujar_mapa(Mi_Jugador.M_X, Mi_Jugador.M_Y, Copia_nivel, Mi_Jugador);

	while (Juego_Activo == true)
	{
		std::cout <<std::endl<<" dime lo que quieres hacer \n";
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
		else if (comando == "guardar" || comando == "Guardar" || comando == "GUARDAR" || comando == "gardar" || comando == "Gardar" || comando == "save" || comando == "SAVE" || comando == "Save")
		{
			Guardar_Partida(Mi_Jugador, Copia_nivel, visitados);

		}
		else if (comando == "cargar" || comando == "CARGAR" || comando == "Cargar")
		{

			/*
			faltan mas cosas que cargar en la partida 

			 los limites de los niveles los enemigos mierda del inventario 
			*/

			std::ifstream partida("partida.txt");

			if (partida.is_open())// crea una funion que aga lo mismo 
			{

				char simbolos = ' ';

				int renglones = 0;

				int columnas = 0;

				bool valor_binario = false;

				std::vector<std::vector<char>> Mapa_De_Partida;

				std::vector<std::vector<bool>> map_valores_binario;


				partida >> Mi_Jugador.M_Nombre >> Mi_Jugador.M_Ataque >> Mi_Jugador.M_Vida >> Mi_Jugador.M_X >> Mi_Jugador.M_Y>>columnas>>renglones>>max_x>>max_y;

				for (int i = 0; i < columnas; ++i)
				{
					Mapa_De_Partida.push_back({});

				}

				for (int i = 0; i < columnas; ++i) 
				{
					map_valores_binario.push_back({});
				}
				
				while (partida>>simbolos && simbolos!='~')
				{

					Mapa_De_Partida[0].push_back(simbolos);// el while agara el primer simbolo del mapa que hace  que todo este fuera de orden con la forma anterior de como estabam los 'for' del 'while'.
					
					for (int j = 1; j < renglones; ++j) 
					{
						partida >> simbolos;
						if (simbolos == '.') 
						{
							Mapa_De_Partida[0].push_back(' ');
						}
						else
						{
							Mapa_De_Partida[0].push_back(simbolos);
						}
						
					}
						

					for (int i = 1; i < columnas; ++i) 
					{
						
						for (int j = 0; j < renglones; ++j) 
						{
							partida >> simbolos;
							if (simbolos == '.')
							{
								Mapa_De_Partida[i].push_back(' ');
							}
							else
							{
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
					for (int i = 0; i < columnas; ++i) 
					{
						Enemigos_ubicacion.push_back({});

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
							Enemigos_ubicacion[i].push_back(valor_binario);

						}

					}


				}// aqui termina el while 


				visitados = map_valores_binario;

				Copia_nivel = Mapa_De_Partida;

				std::cout << "se logro cargar la partida con exito " << std::endl;

				partida.close();
			}
			else
			{
				std::cout << "algo sailo mal y o no guardaste la partida ( el juego lo hacer automaticamente cuando ingrecas el commando " << std::endl << "---> 'fin' ";
			}

		}
		else if (comando == "ayuda" || comando == "Ayuda" || comando == "AYUDA") // original mente esto era un troll face pero  cuando insertaba el troll face solo saila '?' en vez de los caracteres que conformaban la cara del troll
		{
			extra_line(1);
			Instrucciones();
			//std::cout << "° <|°_°|> °" << std::endl;
			//std::cout << "_°_/ | \_°_" << std::endl;
			//std::cout << "  _°0|0°_  " << std::endl;
			//std::cout << " _ 0 ° 0 _ " << std::endl;
			//std::cout << "_   °0°  _ " << std::endl;
			//std::cout << " |°|_0_|°| " << std::endl;
			//std::cout << "°°°°° te estoy viendo °°°°°°°°°°°°°°";
			extra_line(2);
		}
		else if (comando == "int" || comando=="Int" || comando=="inventario" || comando=="stuff" ||comando=="cosas") 
		{
			std::cout << "tienes :";
			for (int i = 0; i < Mi_Jugador.inventario.size(); ++i) 
			{
			 std::cout<< Mi_Jugador.inventario[i] << ',';
		

			}
		
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

1) guardar la partida utilizando archivos { progresando }

2) multiples pisos( niveles )planiados 3  [X]

3) objetos para el personaje

4) inventario

5)  formas de enseñar las estadisticas del jugador [X]

6) sistema de combate basado en tomar turnos

7) que el juego pueda trasferir el jugador a un mapa a otro #valiendo brega
*/