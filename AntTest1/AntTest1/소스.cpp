#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <memory.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
using namespace std;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;

//���� ���� �ʿ� ���
const double P = 0.2; // ���߷�
const double Q = 40;     //��θ� ��
const double ALPHA = 0.5;
const double BETA = 1;

int N, M;
int S, E;
long long int TIME = 0L;
double interval = 500;
pdd edges[333][333]; // first = �Ÿ� , secode = ��θ� ��
vector<pii> allEdges;
vector<int> adj[333];
int visited[333][333] = { 0, };

struct ant
{
	int id;
	int prevN;
	int curN;
	int reachTime;
	bool foundFood;
	vector<int> trace;
	ant(){
		foundFood = false;
		reachTime = TIME;
		prevN = curN = S;
	}
	ant(int id, int curN) :id(id), curN(curN){
		memset(visited[id], 0, sizeof(visited[id]));
		foundFood = false;
		visited[id][curN] = 1;
		reachTime = TIME;
		prevN = S;
	}
	ant(int curN) :curN(curN){
		foundFood = false;
		visited[id][curN] = 1;
		reachTime = TIME;
		prevN = S;
	}
};
vector<ant> ants;
double deltaPheromone[333][333] = { 0, };

random_device r;
default_random_engine generator{ r() };
uniform_real_distribution<double> distribution(0.0, 1.0);
int getNextEdge(vector<pair<int, double> > probEdge, double probSum){
	if (probEdge.size() == 1)
		return probEdge[0].first;
	double ran = distribution(generator);
	double tmp = 0;
	for (int i = 0; i < probEdge.size(); i++)
	{
		if (tmp <= ran && ran < tmp + probEdge[i].second / probSum)
			return probEdge[i].first;
		tmp += probEdge[i].second / probSum;
	}
	return probEdge[probEdge.size() - 1].first;
}
int main(){
	// N: ��� ����, M ���� ����
	scanf("%d %d", &N, &M);
	//���� �Է�
	for (int i = 0; i < M; i++)
	{
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);
		edges[a][b] = make_pair(c, 1);
		adj[a].push_back(b);
		allEdges.push_back({ a, b });
		edges[b][a] = make_pair(c, 1);
		adj[b].push_back(a);
		allEdges.push_back({ b, a });
	}
	//��� ���, ���� ���
	scanf("%d %d", &S, &E);
	printf("����\n");
	ants.push_back(ant(ants.size(), S));
	TIME--;
	while (true){
		TIME++;
		if (TIME > 300) break;
		printf("���� �ð� %lld��\n", TIME);
		printf("���̼� : %d\n", ants.size());
		//Sleep(interval);
		memset(deltaPheromone, 0, sizeof(deltaPheromone));
		for (int i = 0; i < ants.size(); i++){
			ant &a = ants[i];
			int  x = min(a.prevN, a.curN), y = max(a.curN, a.prevN);
			if (a.reachTime == TIME){
				printf("���� %d�� %d���� %d�� ����\n", a.id, a.prevN, a.curN);
				//���� ���� ��θ� �߰�
				if (a.foundFood){
					//deltaPheromone[a.prevN][a.curN] += Q / edges[a.prevN][a.curN].first;
					deltaPheromone[x][y] += Q / edges[x][y].first;
				}
				//������ ã������
				if (!a.foundFood && a.curN == E){
					printf("���� %d�� ������ ã��\n", a.id);
					a.foundFood = true;
					memset(visited[a.id], 0, sizeof(visited[a.id]));
					visited[a.id][E] = 1;
				}
				//������ ã�� ���� ���ƿ�����
				if (a.foundFood && a.curN == S){
					//���� ã��
					printf("--------------------------���� %d�� ������ ã�� ������ ���ƿ�----------------------\n", a.id);
					int id = a.id;
					ants.erase(ants.begin() + i);
					ants.push_back(ant(id, S));
					if (ants.size() < 50){
						ants.push_back(ant(ants.size(), S));
					}
					i--;
					//TODO ���� �߰�
					continue;
				}
				// TODO ���� ����
				int probEdgeCnt = 0;
				double probSum = 0;
				vector<pair<int, double> > probEdge;
				{
					for (int i = 0; i < adj[a.curN].size(); i++){
						int nN = adj[a.curN][i];
						int nX = min(a.curN, nN), nY = max(a.curN, nN);
						if (a.curN == nN || visited[a.id][nN] == 1){
							continue;
						}
						probEdgeCnt++;
						double curProb = pow(edges[nX][nY].second, ALPHA) * pow(((double)1) / edges[nX][nY].first, BETA);
						probSum += curProb;
						probEdge.push_back({ nN, curProb });
					}
				}

				//TODO ���̻� �� ���� ������
				if (probEdge.size() == 0){
					printf("-----------����%d�� ���� ����------------\n", a.id);
					memset(visited[a.id], 0, sizeof(visited[a.id]));
					visited[a.id][a.foundFood ? E : S] = 1;
					{
						probSum = 0;
						probEdge.clear();
						for (int i = 0; i < adj[a.curN].size(); i++){
							int nN = adj[a.curN][i];
							int nX = min(a.curN, nN), nY = max(a.curN, nN);
							if (a.curN == nN || visited[a.id][nN] == 1){
								continue;
							}
							probEdgeCnt++;
							double curProb = pow(edges[nX][nY].second, ALPHA) * pow(((double)1) / edges[nX][nY].first, BETA);
							probSum += curProb;
							probEdge.push_back({ nN, curProb });
						}
					}
					if (probEdge.size() == 0){
						//error
					}
				}
				//���� ���� �Ϸ�
				int nextEdge = getNextEdge(probEdge, probSum);
				a.trace.push_back(nextEdge);
				a.prevN = a.curN;
				a.curN = nextEdge;
				a.reachTime = a.reachTime + edges[a.prevN][a.curN].first;
				visited[a.id][a.curN] = 1;
			}
		}
		printf("��θ� ��Ȳ\n");
		//TODO ��� ���� ������Ʈ
		for (int i = 0; i < allEdges.size(); i++){
			int x = allEdges[i].first, y = allEdges[i].second;
			edges[x][y].second = max(1, (1 - P)* edges[x][y].second);
			edges[x][y].second += deltaPheromone[x][y];
			if (x <= y)
				printf("%d - %d ��θ� ��: %lf\n", x, y, edges[x][y].second);
		}
		printf("���� ��Ȳ\n");
		for (ant a : ants){
			printf("���� %d: 0 ", a.id);
			for (int i : a.trace){
				printf("- %d ", i);
			}
			printf("\n");
		}
	}
	return 0;
}
/***
	���� ���Ĵ�� ����� ��� ������:
	1.��θ� ���� 0 �� �������� ���� ������ ���� ������.
	2. ��θ� ������Ʈ ������ ������, ���̰� ������ ���ư��� ������� ������ �ָ���.
	3. ���̰� ���̻� �� ���� ������, ��� �� ������.
	���� ���:
	1.��θ� ���� 0���� �������� �ʰ� 1���� ����,
	2.������Ʈ ������ ��, ������ ��� ������ ���� ���İ� ����ϰ� ����
	3. �湮 �迭�� ���̸� ã������ ���� ����, ��ã������ ������ ���ܷ� �ʱ�ȭ
	***/