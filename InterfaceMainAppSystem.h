#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;
#include <List>


FILE* soJournal;// ��� ����� ���������� � ������� �������� ������ 
FILE* que; // ��� ����� ����. � ����� �������: ���������� 1->7 ����

long int entered = 0L; // ������� ������ ����� ������ �� �����
long int rejected = 0L; // ����. ������ ������ �������� �� �������
long int satisfied = 0; // ����. ����� ������ ���������� �����������
int numOrders = 0; // ����. ����� ��������� �������
float sojAve = 0; // ��� �������� �������� ��������
float queAve = 0; // ��� ������� ������� ������ �������
long int total; // ����� ���������� ������� (��������-�����)


class Client 
{
public:
	friend class SuperMarket;
	Client() 
	{
		ID * entered - rejected - satisfied + 1; // �������. ������ �� �����, ����� ������ � �������: �� ������� ������. � ������ � �������
		this->hours = 0;
	}
	void Print() const noexcept; // ��� ������ 
	inline long int getID();
	inline int getTime();
private:
	long int ID; // ID �������
	int hours; // ����� ������� � ���� �����������
};

void Client::Print() const noexcept
{
	// ����� ���������� ����� � ������ 168(24*7)
	std::cout << "ID " << ID << " � �������� ������ " << hours / 168 << " ������ " << std::endl;
}

// ��������� ������ � ����������� ������� � �������
inline int Client::getTime() {return(hours);}

// ID ������
inline long int Client::getID() {return(ID);}

class SuperMarket 
{
	


private:
	int timeToArrival; // �� �������� ��������� ������
	int timeToOrder; // �� ���������� ������
	int order; // ����� ���������� ������
	int timeToCheck; // ����� �� �����. ��������
	int queueLength; // ����� �������
	int existProduct; // ������� �����. ������
	list<Client>* queueOrder;
	// �������� ����������� ����� ������
	static constexpr float mu = 0.03f; // ������ � ���
	static constexpr int checking = 28; // ������������� �������� ������� 
	static constexpr int ordering = 21; // ����� ���������� ������
	static constexpr int lowLevel = 18; // ������ ������� ������ ������
	static constexpr int HighLvl = 72; // ������� ������� ������� 
	static constexpr int percentAge = 80; // % ������ ������� �� ������ ���� ������������� � ���� ��������� �������

public:
	SuperMarket(int i);
	void run();
	void arrival();
	void complete();
	void check();
	void Print() const noexcept;
	inline int getLength();
	int queueExtraLength; // ��� �����. ����� �������
};

// �������� �������� ���������� ������
SuperMarket::SuperMarket(int index) 
{
	this->queueLength = 0;
	this->queueExtraLength = 0;
	que = nullptr;
	this->timeToArrival = (int)(exp(mu)); // �������� random() �������� � 1: (1-exp(0.03*0.5) = 0.014)
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
	std::cout << "��������� ������ �������� ����� " << timeToArrival << " ����� "<<std::endl;
	if (timeToOrder > 0) 
	{
		std::cout << "����� ��� ������ ����� " << 
			timeToOrder / 24 << " ����, �� �������� " << 
			order << " ������ ������� " << std::endl;
	}
	else 
	{
		std::cout << "������� ���" << std::endl;
	}
	std::cout << "��������� �������� ������� ����� ����� " << timeToCheck / 24 << " ���� " << std::endl;
	std::cout << "����� �������� �������� " << queueLength << " ����� ������� " << std::endl;
	std::cout << "������� " << existProduct << " ������ ������� "<< std::endl;
}

void SuperMarket::arrival() 
{
	int i;
	Client* ptrOnClient = nullptr;
	// ��������� ����� �������� ����� ����������
	timeToArrival = (int)(exp(mu));
	if (timeToArrival == 0) 
	{
		timeToArrival = 1;
		entered++; // +1 � ������ �������� ��������
		if (existProduct > 0) 
		{
			existProduct--; // ����� ���� - ������� -1 �� ����������
			satisfied++; // ���� ���� ������ ������� �������
		}
		else 
		{
			if (rand() % 100 < percentAge) // ������ ������
			{
				rejected++; // +1 ���� ���������� ������
				return; // ������� ������� �� �������
			}
		}
		ptrOnClient = new Client(); // ����� ������ �������
		list<Client> ptrOnListClient = list<Client>(ptrOnClient, NULL);
		if (queueLength == 0) que = nullptr;
		else
			ptrOnListClient.push_front(ptrOnClient);
		queueLength++;			
	}
	return;
}


// �������� ������
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
	
		// ������ ������ �������
		b = que->_Placeholder()->getTime();
		std::cout << soJournal << " " << ((float)b) / 24;
		c = entered - rejected - satisfied - queueLength + queueExtraLength + 1;
		sojAve = sojAve * (1 - 1.0 / c) + (float)b / c; // �������� �������� ������� ��������
		// �������� �������� �� ������
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
	// ��������� �������
	a = existProduct + order - queueLength;
	if (a >= lowLevel)
		return;
	// ����� ������ �����
	timeToOrder = ordering * 24;
	// ���������� ������ 
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
	// ��������� ������� ���������� 
	if (que != NULL)
	{
		ptr = que;
		while (ptr != NULL) 
		{
			((ptr->Data())->hours)++;
			ptr = ptr->Next();
		}
	}
	// ����������� �����
	if (total % 7 == 0)
		std::cout << que << " " << queueLength << std::endl;
	queAve = queAve * (1 - 1.0 / (total + 1)) + ((float)queueLength) / (total + 1);
	return;
}