// курсовая228.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include <fstream>
#include "cmath"
#include "stdlib.h"
#include "time.h"
#include "string"
#include "conio.h"
#include  <Windows.h>
using namespace std;

enum STATUS { Skoloda, Sigrok, Skrupie };

string name1 = "BJ_rules_1.txt";
string name2 = "BJ_rules_2.txt";

struct card
{
	int name;   // Название карты
	char suit;  // Масть карты
	int value;  // Номенал карты
	int status; // Статус карты
};

struct Tigrok
{
	int cards[11]; // Карты игрока
	int kol;       // Кол-во карт игрока
	int kapital;   // Кол-во фишек
	int s_value;   // Сумма значений набранных карт
	int status;
	int stavka;
};

char mast[4] = { 3, 4, 5, 6 };

//Сортировка карт
void filling(card x[]);
//Раздача карт
void razdacha(card x[], Tigrok & igrok, int k);
//Подсчёт карт
void podschet(card x[], Tigrok&);
//ИИ крупье
void ii(card x[], Tigrok&);
//сравнение карт
int comparison(card x[], Tigrok&, Tigrok&);
//Вывод карт
void display(card x[], Tigrok& igr, Tigrok& krp);
//Задержка перед выдачей карт для крупье
int gamerchoise(card x[], Tigrok& igr, Tigrok& krp);
//Новая игра
int new_game();
//Присваивание картам статуса "В колоде"
void refilling_cards(card x[]);
//Графика
void graphik(card x[], Tigrok& igr);
//Оформление стартового меню
int start_menu();
//Система ставок
int igr_stavka(Tigrok& igr);
//всё таки правила
void read_txt(string& str);
//правила-правила
int output_rules(string& str1, string& str2);
//ищи-свичи
int rules1(string& name1);
int rules2(string& name2);
//ASCII-art
void ascii_1();
void ascii_2();
void ascii_3();
void ascii_4();

int _tmain(int argc, _TCHAR* argv[])
{
	system("color 37");
	setlocale(LC_ALL, "Russian");


	cout << "Allo, blyat";
	_getch();

	if (start_menu() == 0) return 0;

	card x[52];
	Tigrok igrok;
	Tigrok krupie;
	filling(x);

	igrok.kapital = 200;
	igrok.status = Sigrok;
	krupie.kapital = 0;
	krupie.status = Skrupie;

	bool b = 0;
	do
	{
		system("cls");

		refilling_cards(x);
		igrok.kol = 0;
		igrok.s_value = 0;
		krupie.kol = 0;
		krupie.s_value = 0;

		igr_stavka(igrok);

		// Раздача карт
		for (int i = 0; i < 2; i++)
		{
			razdacha(x, igrok, igrok.kol);
			razdacha(x, krupie, krupie.kol);
		}

		if (gamerchoise(x, igrok, krupie) == 0)
			exit(0);
		//return 0;

		cout << "Ход переходит крупье." << endl;
		Sleep(3000);

		if (igrok.s_value < 22)
		{
			podschet(x, krupie);
			while (/*(igrok.s_value >= krupie.s_value) && */(krupie.s_value < 17))
				ii(x, krupie);
		}

		display(x, igrok, krupie);

		//Результаты игры
		if (comparison(x, igrok, krupie) == 0)
		{
			cout << "Вce при своих" << endl;
			igrok.kapital += igrok.stavka;
		}
		else
			if (comparison(x, igrok, krupie) == 1)
			{
				if ((igrok.kol == 2) && (igrok.s_value == 21))
				{
					igrok.stavka *= 3;
					igrok.stavka /= 2;
				}
				else
					igrok.stavka *= 2;

				igrok.kapital += igrok.stavka;
				cout << "Вы выиграли, ваш выигрыш составил " << igrok.stavka << " фишек" << endl;
			}
			else
			{
				if (igrok.kapital == 0)
				{
					cout << "Вы исчерпали свой кредит, приходите в следующий раз" << endl;
					_getch();
					return 0;
				}
				else
					cout << "Вы проиграли" << endl;
			}
		if (new_game() == 0) exit(0);

	} while (b == 0);
}

void filling(card x[])
{
	int k = 0;

	for (int j = 0; j<4; j++)
	{
		for (int i = 2; i <= 14; i++, k++)
		{
			x[k].suit = mast[j];
			x[k].name = i;
			x[k].status = Skoloda;
			x[k].value = (i<11) ? x[k].name : ((i != 14) ? 10 : 11);
		}
	}
}

void razdacha(card x[], Tigrok & igr, int k)
{
	srand(time(NULL));
	int i = k;

	do
		k = rand() % 52;//случайное число
	while (x[k].status != Skoloda);

	igr.cards[i] = k;
	igr.kol++;
	x[k].status = igr.status;

}

void podschet(card x[], Tigrok& igr)
{
	int k;
	int S = 0;

	for (int i = 0; i < igr.kol; i++)
	{
		k = igr.cards[i];
		if (x[k].status == igr.status)
			S += x[k].value;
	}

	igr.s_value = S;

	return;
}

void display(card x[], Tigrok& igr, Tigrok& krp)
{
	system("cls");

	cout << "Фишек у вас на руках: " << igr.kapital << "; Ваши карты:\n";

	graphik(x, igr);
	podschet(x, igr);

	cout << "Текущая ставка: " << igr.stavka << "; Карты крупье:\n";

	graphik(x, krp);
	podschet(x, krp);
}

void ii(card x[], Tigrok& igr)
{
	razdacha(x, igr, igr.kol);
	podschet(x, igr);
}

int comparison(card x[], Tigrok& igr, Tigrok& krp)
{
	podschet(x, igr);
	podschet(x, krp);

	if (igr.s_value > 21)
	{
		return 2;
	}
	else
		if (igr.s_value == 21)
			if (krp.s_value == 21) return 0;
			else return 1;
		else
			if (krp.s_value == 21) return 2;
			else
			{
				if (krp.s_value > 21) return 1;
				else
				{
					if (krp.s_value<igr.s_value) return 1;
					else
						if (krp.s_value>igr.s_value) return 2;
						else return 0;
				}
			}
}

int gamerchoise(card x[], Tigrok& igr, Tigrok& krp)
{
	int n = 0;
	display(x, igr, krp);

	if (igr.s_value < 20) {

		do
		{
			ascii_1();
			n = 0;
			n = _getch();

			switch (n)
			{
			case 49:
				razdacha(x, igr, igr.kol);
				break;
			case 50:

				if ((igr.stavka * 2) < igr.kapital)
				{
					igr.stavka *= 2;
					igr.kapital -= igr.stavka;

					razdacha(x, igr, igr.kol);
					display(x, igr, krp);

					return 1;
				}
				else
				{
					cout << "У вас недостаточно средств для совершения этого действия" << endl;
					Sleep(2000);
				}
				break;
			case 51:
				display(x, igr, krp);
				return 1;
			case 48:
				return 0;
				//default:
				//break;
			}

			display(x, igr, krp);

		} while (igr.s_value < 20);
	}
}

int new_game()
{
	char E;

	ascii_2();

	E = _getch();
	switch (E)
	{
	case 50:
		break;
	case 48:
		return 0;
		//default:
		//break;
	}
	return 1;
}

void refilling_cards(card x[])
{
	for (int i = 0; i < 53; i++)
		x[i].status = Skoloda;
}

void graphik(card x[], Tigrok& igr)
{
	//char SUIT[] = { 'J', 'Q', 'K', 'A' };

	//Верхушка
	for (int i = 0; i < igr.kol; i++)
		cout << " _ _ _ _ _ \t";

	cout << endl;

	//Второй ряд

	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name > 1) && (x[j].name < 11))
				cout << "|" << x[j].name << "\t  |\t";
			else
				if (x[j].name == 11)
					cout << "|" << x[j].suit << "     ___|\t";
				else
					if (x[j].name == 12)
						cout << "|" << x[j].suit << " _____  |\t";
					else
						if (x[j].name == 13)
							cout << "|" << x[j].suit << "__  __  |\t";
						else
							cout << "|A\t  |\t";
		}
	}

	cout << endl;

	//Третий ряд
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name > 4) && (x[j].name < 11))
				cout << "|  " << x[j].suit << "   " << x[j].suit << "  |\t";
			else
				if ((x[j].name > 1) && (x[j].name < 5))
					cout << "|    " << x[j].suit << "    |\t";
				else
					if (x[j].name == 11)
						cout << "|       | |\t";
					else
						if (x[j].name == 12)
							cout << "| /     \\ |\t";
						else
							if (x[j].name == 13)
								cout << "|  |  /   |\t";
							else
								cout << "|" << x[j].suit << "\t  |\t";
		}
	}

	cout << endl;

	//Четвёртый ряд
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name > 1) && (x[j].name < 8))
				cout << "|         |\t";
			else
				if (x[j].name == 8)
					cout << "|    " << x[j].suit << "    |\t";
				else
					if (x[j].name == 9)
						cout << "| " << x[j].suit << "     " << x[j].suit << " |\t";
					else
						if (x[j].name == 10)
							cout << "| " << x[j].suit << "     " << x[j].suit << " |\t";
						else
							if (x[j].name == 11)
								cout << "|       | |\t";
							else
								if (x[j].name == 12)
									cout << "|/       \\|\t";
								else
									if (x[j].name == 13)
										cout << "|  | /    |\t";
									else
										cout << "|\t  |\t";
		}
	}

	cout << endl;

	//Середина
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name == 3) || (x[j].name == 5) || (x[j].name == 9) || (x[j].name == 14))
				cout << "|    " << x[j].suit << "    |\t";
			else
				if ((x[j].name == 4) || (x[j].name == 6) || (x[j].name == 8) || (x[j].name == 10))
					cout << "|  " << x[j].suit << "   " << x[j].suit << "  |\t";
				else
					if (x[j].name == 7)
						cout << "| " << x[j].suit << "  " << x[j].suit << "  " << x[j].suit << " |\t";
					else
						if (x[j].name == 2)
							cout << "|\t  |\t";
						else
							if (x[j].name == 11)
								cout << "|       | |\t";
							else
								if (x[j].name == 12)
									cout << "||       ||\t";
								else
									if (x[j].name == 13)
										cout << "|  |/\\    |\t";
		}
	}

	cout << endl;

	//Шестой ряд
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name > 1) && (x[j].name < 8))
				cout << "|         |\t";
			else
				if (x[j].name == 8)
					cout << "|    " << x[j].suit << "    |\t";
				else
					if (x[j].name == 9)
						cout << "| " << x[j].suit << "     " << x[j].suit << " |\t";
					else
						if (x[j].name == 10)
							cout << "| " << x[j].suit << "     " << x[j].suit << " |\t";
						else
							if (x[j].name == 11)
								cout << "| __    | |\t";
							else
								if (x[j].name == 12)
									cout << "|\\       /|\t";
								else
									if (x[j].name == 13)
										cout << "|  |  \\   |\t";
									else
										cout << "|\t  |\t";
		}
	}

	cout << endl;

	//Седьмой ряд
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if ((x[j].name > 4) && (x[j].name < 11))
				cout << "|  " << x[j].suit << "   " << x[j].suit << "  |\t";
			else
				if ((x[j].name > 1) && (x[j].name < 5))
					cout << "|    " << x[j].suit << "    |\t";
				else
					if (x[j].name == 11)
						cout << "|  \\____/ |\t";
					else
						if (x[j].name == 12)
							cout << "| \\____\\_ |\t";
						else
							if (x[j].name == 13)
								cout << "| _|   \\_ |\t";
							else
								cout << "|        " << x[j].suit << "|\t";
		}
	}

	cout << endl;

	// Последний ряд
	for (int j = 0; j < 53; j++)
	{
		if (x[j].status == igr.status)
		{
			if (x[j].name < 10)
				cout << "|_ _ _ _ " << x[j].name << "|\t";
			else
				if (x[j].name == 10)
					cout << "|_ _ _ _" << x[j].name << "|\t";
				else
					if ((x[j].name > 10) && (x[j].name < 14))
						cout << "|_ _ _ _ " << x[j].suit << "|\t";
					else
						cout << "|_ _ _ _ A|\t";
		}
	}



	cout << endl;
}

int start_menu()
{
	bool f = 1;
	char E;

	for (;;)
	{
		system("cls");

		ascii_3();

		E = _getch();
		switch (E)
		{
		case 50:
			output_rules(name1, name2);
			break;
		case 53:
			system("cls");
			return 1;
			break;
		case 48:
			return 0;
		}
	}
}

int igr_stavka(Tigrok& igr)
{
	igr.stavka = 10;
	int temp;
	int max;

	for (;;) {
		system("cls");
		cout << "Ваш баланс: " << igr.kapital << " фишек; Ваша ставка: " << igr.stavka << " фишек;\n";

		ascii_4();

		char E1;

		E1 = _getch();
		switch (E1)
		{
		case 52:
			if (igr.stavka == 10);
			else
				if (igr.stavka < 100)
					igr.stavka -= 10;
				else
					igr.stavka -= 50;
			break;
		case 54:
			if (igr.stavka == 200);
			else
				if ((igr.stavka < 100) && ((igr.stavka + 10) <= igr.kapital))
					igr.stavka += 10;
				else
					if ((igr.stavka + 50) <= igr.kapital)
						igr.stavka += 50;
			break;
		case 48:
			igr.kapital -= igr.stavka;
			return 0;
			//default:
			//break;
		}
	}
}

void read_txt(string& str)
{
	ifstream file;
	string name, text;
	name = str;
	file.open(name);
	if (!file.is_open()) cout << "error";
	while (!file.eof()) {
		getline(file, text);
		cout << text << endl;
	}
}

int output_rules(string& str1, string& str2)
{
	bool f = 0;

	do {
		if (rules1(str1) == 0) return 0;
		else
			if (rules2(str2) == 0) f = 1;
	} while (f == 0);

	return 0;
}

int rules1(string& name1)
{
	char R1;

	system("cls");
	read_txt(name1);
	cout << "\n\n\t\t\t\t\t\t\t<-- Меню : Стр.2 -->";

	R1 = _getch();

	switch (R1)
	{
	case 52:
		return 0;
	case 54:
		return 1;
	}
}

int rules2(string& name2)
{
	char R2;

	system("cls");
	read_txt(name2);
	cout << "\n\n\n\n\t\t\t\t\t\t\t<-- Стр.1 : Меню-->";

	R2 = _getch();

	switch (R2)
	{
	case 52:
		return 1;
	case 54:
		return 0;
	}
}

void ascii_1()
{
	cout << " ____		        ____             ____                _________ " << endl
		<< "||1 ||		       ||2 ||           ||3 ||	            ||0      ||" << endl
		<< "||__||	- Выдать карту ||__|| - Удвоить ||__|| - Достаточно ||_______|| - Выход" << endl
		<< "|/__\\|		       |/__\\|           |/__\\|              |/_______\\|" << endl;
}

void ascii_2()
{
	cout << " ____		     _________ " << endl
		<< "||2 ||		    ||0      ||" << endl
		<< "||__|| - Новая игра ||_______|| - Выход" << endl
		<< "|/__\\|		    |/_______\\|" << endl;
}

void ascii_3()
{
	cout << "__________.__                __                  _.              __    " << endl
		<< "\\  ___   \\  | _____    ____ |  | __             | |____    ____ |  | __" << endl
		<< " |   |  _/  | \\__  \\ _/ ___\\|  |/ /  ______     | \\__  \\ _/ ___\\|  |/ /" << endl
		<< " |   |   \\  |__/ __ \\\\  \\___|    <  /_____/ /\\__| |/ __ \\\\  \\___|    < " << endl
		<< " |_____  /____(____  /\\___  >__|_ \\         \\_____(____  /\\___  >__|_ \\ " << endl
		<< "       \\/          \\/     \\/     \\/                    \\/     \\/     \\/" << endl;
	cout << "\t\t\t\t\t\tby Anatoliy Brazhko\n" << endl << endl;

	cout << " ____		      ____	       _________ " << endl
		<< "||5 ||		     ||2 ||	      ||0      ||" << endl
		<< "||__||	- Новая игра ||__|| - Правила ||_______|| - Выход" << endl
		<< "|/__\\|		     |/__\\|           |/_______\\|" << endl;
}

void ascii_4()
{
	cout << " ____		        ____	               _________ " << endl
		<< "||4 ||		       ||6 ||	              ||0      ||" << endl
		<< "||__||-Понизить ставку ||__||-Повысить ставку ||_______||-Подтвердить ставку" << endl
		<< "|/__\\|		       |/__\\|                 |/_______\\|" << endl;
}

/*

_ _ _ _ _ _
|0      ___ |
|	     |  |
|        |  |
|        |  |
| __     |  |
|  \____/   |
|_ _ _ _ _ 0|

_ _ _ _ _ _
|0 __   __  |
|   |  /    |
|   | /     |
|   |/\     |
|   |  \    |
|  _|   \_  |
|_ _ _ _ _ 0|

_ _ _ _ _ _
|0 _______  |
| /       \ |
| |       | |
| |       | |
| |       | |
|  \_____\_ |
|_ _ _ _ _ 0|

_ _ _ _ _ _
|			|
|			|
|			|
|			|
|			|
|			|
|_ _ _ _ _ _|

_ _ _ _ _ _
|*\*\*\*\*\*|
|*\*\*\*\*\*|
|*\*\*\*\*\*|
|*\*\*\*\*\*|
|*\*\*\*\*\*|
|*\*\*\*\*\*|
|_*_*_*_*_*_|
*/