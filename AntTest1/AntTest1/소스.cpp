#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <memory.h>
#include <algorithm>
#include <iostream>
using namespace std;
typedef pair<int, int> pii;
int N, M;
int S, E;
long long int time;
double interval = 500;
struct edge
{
	double phermone = 0;
	int cost;
	int s;
	int e;
	edge(){}
	edge(int s, int e, int cost) :s(s), e(e), cost(cost){
	}
};
struct ant
{
	int id;
	int curN;
	int reachTime;
	bool foundFood;
	int visited[333] = { 0, };
	ant(){ foundFood = false; }
	ant(int id, int curN) :id(id), curN(curN){
		foundFood = false;
	}
	ant(int curN) :curN(curN){
		foundFood = false;
	}
};
vector<edge> edges[333];
vector<ant> ants;
int main(){
	// N: ��� ����, M ���� ����
	scanf("%d %d", &N, &M);
	//���� �Է�
	for (int i = 0; i < M; i++)
	{
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		edges[a].push_back(edge(a, b, c));
		edges[b].push_back(edge(b, a, c));
	}
	//��� ���, ���� ���
	scanf("%d %d", &S, &E);
	cout << "����" << endl;
	ants.push_back(ant());
	while (true){
		time++;
		//Sleep(interval);
		for (ant a : ants){
			if (a.reachTime == time){
				//TODO ���� ���� üũ
				// TODO ���� ����
			}
		}
		//TODO ��� ���� ������Ʈ
	}

	return 0;
}