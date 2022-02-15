#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;
#include <List>


FILE* soJournal;// для сбора статистики о времени ожидания товара 
FILE* que; // для сбора стат. о длине очереди: пополнение 1->7 дней

long int entered = 0L; // счетчик общего числа заявок на товар
long int rejected = 0L; // счет. чрисла заявок вышедних из система
long int satisfied = 0; // счет. числа заявок немедленно обработаных
int numOrders = 0; // счет. числа сделанных заказов
float sojAve = 0; // для подсчета среднего ожидания
float queAve = 0; // для подсчет средней длинны очереди
long int total; // такты модельного времени (человеко-часов)


class Client 
{
public:
	friend class SuperMarket;
	Client() 
	{
		ID * entered - rejected - satisfied + 1; // вычисле. заявки по счету, какая сейчас в очереди: от момента модели. и ставлю в очередь
		this->hours = 0;
	}
	void Print() const noexcept; // для вывода 
	inline long int getID();
	inline int getTime();
private:
	long int ID; // ID клиента
	int hours; // время клиента в сети проведенное
};

void Client::Print() const noexcept
{
	// общее количество часов в неделю 168(24*7)
	std::cout << "ID " << ID << " в ожидании заказа " << hours / 168 << " недель " << std::endl;
}

// получение данных о проведенном времени в системе
inline int Client::getTime() {return(hours);}

// ID заявки
inline long int Client::getID() {return(ID);}

class SuperMarket 
{
	


private:
	int timeToArrival; // до прибытия следующей заявки
	int timeToOrder; // до исполнения заказа
	int order; // объем ожидаемого заказа
	int timeToCheck; // время до следу. проверки
	int queueLength; // длина очереди
	int existProduct; // текущее колич. товара
	list<Client>* queueOrder;
	// перечень константных полей данных
	static constexpr float mu = 0.03f; // заявок в час
	static constexpr int checking = 28; // периодичность проверки запасов 
	static constexpr int ordering = 21; // время выполнение заказа
	static constexpr int lowLevel = 18; // нижняя граница запаса товара
	static constexpr int HighLvl = 72; // верхняя граница товаров 
	static constexpr int percentAge = 80; // % заявок которые не смогли быть обработанными в виду отсутсвия товаров

public:
	SuperMarket(int i);
	void run();
	void arrival();
	void complete();
	void check();
	void Print() const noexcept;
	inline int getLength();
	int queueExtraLength; // для начал. длины очереди
};

// параметр исходное количество товара
SuperMarket::SuperMarket(int index) 
{
	this->queueLength = 0;
	this->queueExtraLength = 0;
	que = nullptr;
	this->timeToArrival = (int)(exp(mu)); // захардил random() величину в 1: (1-exp(0.03*0.5) = 0.014)
	if (timeToArrival == 0)
	{
		timeToArrival = 1;
		timeToOrder = -1;
		order = 0;
		existProduct = index;

		timeToCheck = (rand() % checking + 1) * 24;
	}
}
inline int SuperMarket::getLength() { return(queueLength); }

void SuperMarket::Print() const
{
	std::cout << "Следующая заявка поступит через " << timeToArrival << " часов "<<std::endl;
	if (timeToOrder > 0) 
	{
		std::cout << "Заказ был принят через " << 
			timeToOrder / 24 << " дней, он включает " << 
			order << " единиц товаров " << std::endl;
	}
	else 
	{
		std::cout << "Заказов нет" << std::endl;
	}
	std::cout << "Следующая проверка запасов будет через " << timeToCheck / 24 << " дней " << std::endl;
	std::cout << "Всего оиждание клиентов " << queueLength << " своих заказов " << std::endl;
	std::cout << "Имеется " << existProduct << " единиц товаров "<< std::endl;
}

void SuperMarket::arrival() 
{
	int i;
	Client* ptrOnClient = nullptr;
	// моделирую новый интервал между прыбитиями
	timeToArrival = (int)(exp(mu));
	if (timeToArrival == 0) 
	{
		timeToArrival = 1;
		entered++; // +1 в общему счетчику запросов
		if (existProduct > 0) 
		{
			existProduct--; // товар есть - значить -1 от количества
			satisfied++; // плюс один клиент который получил
		}
		else 
		{
			if (rand() % 100 < percentAge) // клиент слился
			{
				rejected++; // +1 один потерянный клиент
				return; // простой возврат из функции
			}
		}
		ptrOnClient = new Client(); // делаю нового клиента
		list<Client> ptrOnListClient = list<Client>(ptrOnClient, NULL);
		if (queueLength == 0) que = nullptr;
		else
			ptrOnListClient.push_front(ptrOnClient);
		queueLength++;			
	}
	return;
}


// Прибытие заказа
void SuperMarket::complete() 
{
	int mi, i, b, c;
	timeToOrder = -1;
	existProduct += order;
	order = 0;
	if (queueLength == 0)
		return;
	if (existProduct < queueLength) mi = existProduct; else mi = queueLength;
	for (i = 0; i < mi; i++) 
	{
	
		// выдача товара клиента
		b = que->_Placeholder()->getTime();
		std::cout << soJournal << " " << ((float)b) / 24;
		c = entered - rejected - satisfied - queueLength + queueExtraLength + 1;
		sojAve = sojAve * (1 - 1.0 / c) + (float)b / c; // пересчет среднего временр ожидания
		// удаление елемента из головы
		std::list<Client>* ptr = que;
		que = que->Next();
		delete ptr;
		queueLength--;
		existProduct--;
	}
}

void SuperMarket::check() 
{
	int a;
	timeToCheck = checking * 24;
	// состояние запасов
	a = existProduct + order - queueLength;
	if (a >= lowLevel)
		return;
	// заказ делать нужно
	timeToOrder = ordering * 24;
	// вычисление обьема 
	order = HighLvl - a;
	numOrders++;
}

void SuperMarket::run() 
{
	int i;
	float a;
	std::list<Client>* ptr;
	timeToArrival--;
	if (timeToArrival == 0) arrival();
	if (timeToOrder > 0) timeToOrder--;
	if (timeToOrder == 0) complete();
	timeToCheck--;
	if (timeToCheck == 0) check();
	// инкримент времени прибывания 
	if (que != NULL)
	{
		ptr = que;
		while (ptr != NULL) 
		{
			((ptr->Data())->hours)++;
			ptr = ptr->Next();
		}
	}
	// еженедельны отчет
	if (total % 7 == 0)
		std::cout << que << " " << queueLength << std::endl;
	queAve = queAve * (1 - 1.0 / (total + 1)) + ((float)queueLength) / (total + 1);
	return;
}