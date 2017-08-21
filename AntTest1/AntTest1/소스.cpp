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

//사전 정의 필요 상수
const double P = 0.2; // 증발률
const double Q = 40;     //페로몬 양
const double ALPHA = 0.5;
const double BETA = 1;

int N, M;
int S, E;
long long int TIME = 0L;
double interval = 500;
pdd edges[333][333]; // first = 거리 , secode = 페로몬 양
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
	// N: 노드 개수, M 간선 개수
	scanf("%d %d", &N, &M);
	//간선 입력
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
	//출발 노드, 먹이 노드
	scanf("%d %d", &S, &E);
	printf("시작\n");
	ants.push_back(ant(ants.size(), S));
	TIME--;
	while (true){
		TIME++;
		if (TIME > 300) break;
		printf("현재 시간 %lld초\n", TIME);
		printf("개미수 : %d\n", ants.size());
		//Sleep(interval);
		memset(deltaPheromone, 0, sizeof(deltaPheromone));
		for (int i = 0; i < ants.size(); i++){
			ant &a = ants[i];
			int  x = min(a.prevN, a.curN), y = max(a.curN, a.prevN);
			if (a.reachTime == TIME){
				printf("개미 %d가 %d에서 %d로 도착\n", a.id, a.prevN, a.curN);
				//현재 간선 페로몬 추가
				if (a.foundFood){
					//deltaPheromone[a.prevN][a.curN] += Q / edges[a.prevN][a.curN].first;
					deltaPheromone[x][y] += Q / edges[x][y].first;
				}
				//음식을 찾았을때
				if (!a.foundFood && a.curN == E){
					printf("개미 %d가 음식을 찾음\n", a.id);
					a.foundFood = true;
					memset(visited[a.id], 0, sizeof(visited[a.id]));
					visited[a.id][E] = 1;
				}
				//음식을 찾고 집에 돌아왔을때
				if (a.foundFood && a.curN == S){
					//음식 찾음
					printf("--------------------------개미 %d가 음식을 찾고 집으로 돌아옴----------------------\n", a.id);
					int id = a.id;
					ants.erase(ants.begin() + i);
					ants.push_back(ant(id, S));
					if (ants.size() < 50){
						ants.push_back(ant(ants.size(), S));
					}
					i--;
					//TODO 음식 추가
					continue;
				}
				// TODO 간선 선택
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

				//TODO 더이상 갈 곳이 없을때
				if (probEdge.size() == 0){
					printf("-----------개미%d가 길을 잃음------------\n", a.id);
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
				//간선 선택 완료
				int nextEdge = getNextEdge(probEdge, probSum);
				a.trace.push_back(nextEdge);
				a.prevN = a.curN;
				a.curN = nextEdge;
				a.reachTime = a.reachTime + edges[a.prevN][a.curN].first;
				visited[a.id][a.curN] = 1;
			}
		}
		printf("페로몬 상황\n");
		//TODO 모든 간선 업데이트
		for (int i = 0; i < allEdges.size(); i++){
			int x = allEdges[i].first, y = allEdges[i].second;
			edges[x][y].second = max(1, (1 - P)* edges[x][y].second);
			edges[x][y].second += deltaPheromone[x][y];
			if (x <= y)
				printf("%d - %d 페로몬 양: %lf\n", x, y, edges[x][y].second);
		}
		printf("개미 상황\n");
		for (ant a : ants){
			printf("개미 %d: 0 ", a.id);
			for (int i : a.trace){
				printf("- %d ", i);
			}
			printf("\n");
		}
	}
	return 0;
}
/***
	원래 공식대로 사용할 경우 문제점:
	1.페로몬 양이 0 인 간선들은 전혀 선택을 하지 않은다.
	2. 페로몬 업데이트 간격이 초인지, 개미가 집으로 돌아갔을 경우인지 설명이 애매함.
	3. 개미가 더이상 갈 곳이 없을때, 어떻게 할 것인지.
	개선 방안:
	1.페로몬 양이 0부터 시작하지 않고 1부터 시작,
	2.업데이트 간격은 초, 가능한 모든 공식은 원래 공식과 비슷하게 설정
	3. 방문 배열을 먹이를 찾았으면 끝점 제외, 못찾았으면 시작점 제외로 초기화
	***/