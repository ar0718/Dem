#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <array>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

int main () {
	int num_literals,num_clauses;
	cin>>num_literals>>num_clauses;
	vector<int>state(num_literals+1,-1);
	vector<vector<int>> clauses; 
	vector<vector<int>> clauses1;
	for(int i=0;i<num_clauses;i++){
		int t;
		cin>>t;
		clauses.push_back(vector<int>());
		while(t){
			clauses[i].push_back(t);
			cin >> t;
		}
	}
	clauses1=clauses;
	function<int(vector<vector<int>>&,vector<int>&)> unit_propagation=[&](vector<vector<int>>& clauses,vector<int>&state){
		for(int i=0;i<clauses.size();i++){
			for(int j=0;j<clauses[i].size();j++){
				if(clauses[i][j]>0&&state[clauses[i][j]]==1){
					clauses[i].clear();
					break;
				}
				else if(clauses[i][j]<0&&state[abs(clauses[i][j])]==0){
					clauses[i].clear();
					break;
				}
				else if(state[abs(clauses[i][j])]>=0){
					clauses[i].erase(clauses[i].begin()+j);
					if(clauses[i].size()==0){
						return 0;
					}
				}
			}
		}
		for(int i=0;i<clauses.size();i++){
			if(clauses[i].size()==1){
				if(state[abs(clauses[i][0])]>=0){
					if(clauses[i][0]<0&&state[abs(clauses[i][0])]==1){
						return 0;
					}
					else if(clauses[i][0]>0&&state[abs(clauses[i][0])]==0){
						return 0;
					}
				}
				if(clauses[i][0]>0){
					state[clauses[i][0]]=1;
				}
				else{
					state[abs(clauses[i][0])]=0;
				}
				clauses[i].erase(clauses[i].begin());
			}
		}
		for(int i=0;i<clauses.size();i++){
			for(int j=0;j<clauses[i].size();j++){
				if(clauses[i][j]>0&&state[clauses[i][j]]==1){
					clauses[i].clear();
					break;
				}
				else if(clauses[i][j]<0&&state[abs(clauses[i][j])]==0){
					clauses[i].clear();
					break;
				}
				else if(state[abs(clauses[i][j])]>=0){
					clauses[i].erase(clauses[i].begin()+j);
					if(clauses[i].size()==0){
						return 0;
					}
				}
			}
		}
		return 1;
	};
	function<int(vector<vector<int>>&,vector<int>&)> pure_literal1=[&](vector<vector<int>>& clauses,vector<int>&state){
		vector<int> check(state.size(),0);
		for(int i=0;i<clauses.size();i++){
			for(int j=0;j<clauses[i].size();j++){
				if(check[abs(clauses[i][j])]==0){
					if(clauses[i][j]>0){
						check[clauses[i][j]]=1;
					}
					else{
						check[abs(clauses[i][j])]=-1;
					}
				}
				else{
					if(clauses[i][j]>0&&check[clauses[i][j]]==-1){
						check[clauses[i][j]]=-2;
					}
					else if(clauses[i][j]<0&&check[abs(clauses[i][j])]==1){
						check[abs(clauses[i][j])]=-2;
					}
				}
			}
		}
		for(int i=1;i<state.size();i++){
			if(check[i]==1&&state[i]==0){
				check[i]=-2;
			}
			else if(check[i]==-1&&state[i]==1){
				check[i]=-2;
			}
			else{
				if(check[i]==1){
					state[i]=1;
				}
				else if(check[i]==-1){
					state[i]=0;
				}
			}
		}
		for(int i=0;i<clauses.size();i++){
			for(int j=0;j<clauses[i].size();j++){
				if(check[abs(clauses[i][j])]==-1||check[abs(clauses[i][j])]==1){
					clauses[i].clear();
					break;
				}

			}
		}
		return 1;
	};
	auto pure_literal=[&](vector<vector<int>>&clauses,vector<int>&state){
		return 1;
	};
	function<int(vector<vector<int>>&,vector<int>&,int)> dpll=[&] (vector<vector<int>>&clauses,vector<int>&state,int i){
		if(i==state.size()) return 1;
		vector<vector<int>> clauses1;
		vector<int> state1;
		clauses1=clauses;
		state1=state;
		if(state[i]==-1){
			state[i]=1;
			if(unit_propagation(clauses,state)&&pure_literal1(clauses,state)&&dpll(clauses,state,i+1)){
				return 1;
			}
			state=state1;
			state[i]=0;
			clauses=clauses1;
			if(unit_propagation(clauses,state)&&pure_literal1(clauses,state)&&dpll(clauses,state,i+1)) return 1;
		}
		else{
			if(unit_propagation(clauses,state)&&pure_literal1(clauses,state)&&dpll(clauses,state,i+1)){
				return 1;
			}
		}
		
		return 0;
	};
	
	
	if(dpll(clauses,state,1)){
		for(int i=1;i<state.size();i++){
			cout<<state[i]<<" ";
		}
		cout<<"\n";
		for(int i=0;i<clauses1.size();i++){
			int check=0;
			for(int j=0;j<clauses1[i].size();j++){
				if(clauses1[i][j]>0&&state[clauses1[i][j]]==1){
					check=1;
				}
				else if(clauses1[i][j]<0&&state[abs(clauses1[i][j])]==0){
					check=1;
				}
			}
			if(check==0){
				cout<<"!!!!!!!!!!!!!!!!  WRONG ANSWER OVER FOR YOU !!!!!!!!!!!!!!!!\n";
				return 0;
			}
		}
		return 0;
	}
		cout<<"NOT POSSIBLE\n";
		for(int i=1;i<state.size();i++){
			cout<<state[i]<<" ";
		}
		cout<<"\n";
	return 0;
}
