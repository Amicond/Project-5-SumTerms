// SumNumTerms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;
vector<int> nodes_of_cur_route;

const string delim="\\";
const string input_dir="input_data"+delim;
const string output_dir="results"+delim;
const string type_str[3]={"l","nl","ns"};
string root_path;

struct Soperator
{
	int n;
	char name;
	bool operator<(Soperator right)
	{
		if(right.n>n)
		{
			return true;
		}
		else 
			if(right.n<n)
			{
				return false;
			}
			else
			{
				if(right.name>name)
					return true;
				else
					return false;
			}
	}
	bool operator==(Soperator right)
	{
		if((n==right.n)&&(name==right.name))
		{
			return true;
		}
		else
			return false;
	}

};

struct term
{
	char name;
	vector<Soperator> Sop;
	double value;
	bool operator ==(term right)
	{
		if(name!=right.name)
			return false;
		if(Sop.size()!=right.Sop.size())
			return false;
		for(int i=0;i<Sop.size();i++)
			if(!(Sop[i]==right.Sop[i]))
				return false;
		return true;
	}
	term operator+(term right)
	{
		value+=right.value;
		return *this;
	}
};

vector<term> results; //полные результаты и только 1ый и 2ой порядок
vector<int> good_nl[5];//


void sum_terms(int type,int order,int order_nums[][3],int point_num, vector<string> &points,int s_length,int a_operators)
{
	ostringstream str;
	int cur_num;
	ifstream terms_and_nodes;
	ifstream res_file;
	int kkk1;
	int subOrder;
	if(type==0)
		subOrder=2;
	if(type==1)
		subOrder=5;
	if(type==2)
		subOrder=6;
	char temp;
	term cur_term;

	bool stopflag=true;
	cur_num=0;
	string s,s2;
	bool check;
	while(subOrder<=order)
	{
		ostringstream str1;
		str1<<input_dir<<subOrder<<"_uniq_routes_"<<type_str[type]<<".txt";
		terms_and_nodes.open(str1.str(),ios::in);
		kkk1=0;
		//////////////////////////////////////////////////////////////////////////
		bool skip_flag;//для nl
		//////////////////////////////////////////////////////////////////////////

		while(!terms_and_nodes.eof()&&stopflag)
		{

			getline(terms_and_nodes,s);
			
			if(s.length()>0)
			{
				if(s[0]=='n')//нашли новый маршрут
				{
					cout<<points[point_num]<<" "<<order<<"."<<subOrder<<"."<<++kkk1<<"\n";
					cur_num++;
					check=(order==subOrder)&&(type==1)&&(find(good_nl[order-5].begin(),good_nl[order-5].end(),cur_num)==good_nl[order-5].end());
					if(cur_num>order_nums[subOrder][type])
					{
						stopflag=false;
						break;
					}
					str.str("");
					str<<root_path<<points[point_num]<<"\\"<<"res"<<type_str[type]<<"$"<<order<<"$"<<subOrder<<"$"<<cur_num<<"_"<<points[point_num]<<".txt";
					if(res_file)
					{
						res_file.close();
					} 
				}
				else if((s[0]=='i')&&(!check))
				{
					istringstream line;
					line.str(s);
					line>>temp;
					int tint;
					bool flag;
					nodes_of_cur_route.clear();
					for(int j=0;j<subOrder;j++)
					{
						line>>tint;
						flag=true;
						for(int k=0;k<nodes_of_cur_route.size();k++)
							if(nodes_of_cur_route[k]==tint)
								flag=false;
						if(flag)
							nodes_of_cur_route.push_back(tint);
					}
					res_file.open(str.str(),ios::in);					
					while(!res_file.eof())
					{
						bool if_cur_term=true;
						getline(res_file,s);
						if(s.length()>0)
						{
							int sAmount;
							if(s[0]=='C')
							{
								s=s.substr(5);
								if(s[0]==' ') 
									s=s.substr(1);
								line.str(s);
								cur_term.name='C';
								//	line>>cur_term.value;
								cur_term.value=atof(s.c_str())/nodes_of_cur_route.size();
								cur_term.Sop.clear();
								sAmount=0;
							}
							else
							{
								cur_term.name=s[0];
								cur_term.value=0;
								cur_term.Sop.clear();
								sAmount=(s.find_first_of('$',0)-1);
								if (sAmount<=s_length)
								{
									Soperator Sop_cur;
									for(int k=0;k<sAmount;k++)
									{
										Sop_cur.name=s[k+1];
										cur_term.Sop.push_back(Sop_cur);
									}
									s=s.substr(sAmount+2);
									s2=s[0];
									//int gg;
									for(int k=0;k<sAmount;k++)
									{
										cur_term.Sop[k].n=atoi(s2.c_str());
										s=s.substr(2);
										s2=s[0];
									}
									s=s.substr(3);
									cur_term.value=atof(s.c_str());
									if (abs(cur_term.value)<0.0000001)
										if_cur_term = false;
								}
								else
									if_cur_term=false;
							}
							if(if_cur_term)
							{
								for(int kk=0;kk<sAmount;kk++)
								{
									cur_term.Sop[kk].n=nodes_of_cur_route[cur_term.Sop[kk].n];
								}
								sort(cur_term.Sop.begin(),cur_term.Sop.end());

								bool condition=false;
								//bool condition = (cur_term.Sop.size() == 0) || (cur_term.Sop.size() == 1 && cur_term.Sop[0].n == 0) || (cur_term.Sop.size() == 2 && cur_term.Sop[0].n == 0 && cur_term.Sop[1].n < 9);
								//	if(s_length==4)
								//	condition = condition || (cur_term.Sop.size() == 3 && cur_term.Sop[0].n == 0 && (cur_term.Sop[1].n < 9 && (cur_term.Sop[2].n < 9 || cur_term.Sop[2].name == 'z')));
								
								if (cur_term.Sop.size()==s_length)
								{
									int za = 0;
									int na = 0;
									for (int jj = 1; jj < cur_term.Sop.size(); jj++)
									{
										if (cur_term.Sop[jj].n>8&&cur_term.Sop[jj].name != 'z')
										{ 
											za = 1;
											break;
										}
										if (cur_term.Sop[jj].name != 'z'&&cur_term.Sop[jj].n <= 8)
											na++;

									}
									if (za == 1)
										condition == false;
									else
										condition = (condition || (cur_term.Sop[0].n == 0))&&(na<2);
								}

								
								//для преобразования в Sz
								if (cur_term.Sop.size() == s_length)
								{
									int flag = (cur_term.Sop[0].n == 0);
									for (int jj = 1; jj < cur_term.Sop.size(); jj++)
									{
										if ( cur_term.Sop[jj].name != 'z')
										{
											flag = false;
											break;
										}
									}
									if (flag)
										condition = true;
								}

								if (s_length == 2&&cur_term.Sop.size()==2)
								{
									if (cur_term.Sop[0].n == 0 && cur_term.Sop[1].n > 8)
									{
										if (cur_term.Sop[0].name == 'p'&&cur_term.Sop[1].name == 'p')
											condition = true;
										if (cur_term.Sop[0].name == 'p'&&cur_term.Sop[1].name == 'm')
											condition = true;
										if (cur_term.Sop[0].name == 'm'&&cur_term.Sop[1].name == 'p')
											condition = true;
										if (cur_term.Sop[0].name == 'm'&&cur_term.Sop[1].name == 'm')
											condition = true;

									}
								}

								//only current terms
								condition = false;

								//////////////////////////////////////////////////
								//for pp
								///////////////////////////////////////////////////
								char var[4][2] = { { 'p', 'p' }, { 'p', 'm' }, { 'm', 'p' }, { 'm', 'm' } };

								if (s_length >= 2 && cur_term.Sop.size() == s_length)
								{

									for (int ii = 0; ii < 4; ii++)
									{
										bool flag = true;
										int  pos = -1;
										if (cur_term.Sop[0].name == var[ii][0])
										{
											for (int jj = 1; jj < s_length; jj++)
											{
												if (var[ii][1] == cur_term.Sop[jj].name)
												{
													if (pos == -1)
													{
														pos = jj;
													}
													else
													{
														flag = false;
														break;
													}
												}
												else if (cur_term.Sop[jj].name != 'z')
												{
													flag = false;
													break;
												}
											}
											if (flag && pos != -1)
											{
												condition = true;
												break;
											}
										}

									}
								}



								//triple TERMS Zp/m p/mZ
								if (a_operators>2)
									condition = false;
								if (s_length >= 2 && cur_term.Sop.size() == s_length&&cur_term.Sop[0].n==0) 
								{
									int pm_amount=0;
									for (int ii = 0; ii < s_length; ii++)
									{
										if (cur_term.Sop[ii].name != 'z')
											pm_amount++;
									}
									if (a_operators == 3 && (pm_amount == 1 || (pm_amount == 3 && cur_term.Sop[0].name != 'z')))
										condition = true;
									if (a_operators == 4 && (pm_amount == 0 || (pm_amount == 2) || (pm_amount == 4 && cur_term.Sop[0].name != 'z')))
										condition = true;
								}





								if(condition)//добавляем только члены содержащие 0 в инд
								{
									bool flag=true;
									for(int ii=0;ii<results.size();ii++)
									{
										if(cur_term==results[ii])
										{
											results[ii].value+=cur_term.value;
											flag=false;
										}
									}
									if(flag)
									{
										results.push_back(cur_term);
									}
								}

							}
						}
					}
					res_file.close();
				}
			}
		}
		terms_and_nodes.close();
		subOrder++;
		cur_num=0;
	}

}

void get_nums(string s, int nums[],int &amount)
{
	istringstream is;
	int pos = s.find_first_not_of("CSJmzp");
	s = s.substr(pos);
	is.str(s);
	int i = 0;
	char c;
	while (!is.eof())
	{
		is >> nums[i];
		is >> c;
		i++;
	}
	amount = i;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int points_amount,order,temp_num;
	int order_nums[10][3];
	vector<string> points;
	ifstream config("config.txt",ios::in);
	ifstream terms_and_nodes;
	ifstream res_file;
	string temp_str;
	int qudratic_type; 
	int s_length,a_operators;
	int nums[5];//с запасом))

	int cur_t=0;//текущий тип маршрутов
	getline(config,temp_str);//заголовок
	getline(config,root_path);
	getline(config,temp_str);//заголовок
	config>>points_amount;
	getline(config,temp_str);//заголовок
	getline(config,temp_str);//заголовок
	config>>order;
	getline(config,temp_str);//заголовок
	getline(config,temp_str);//заголовок
	config >> s_length;
	getline(config, temp_str);//заголовок
	getline(config, temp_str);//заголовок
	config >> a_operators;
	getline(config, temp_str);//заголовок
	getline(config, temp_str);//заголовок
	for(int i=2;i<=order;i++)
	{
		config>>temp_num>>order_nums[i][0]>>order_nums[i][1]>>order_nums[i][2];
	}
	config.close();

	ifstream points_file("points.txt",ios::in);
	for(int i=0;i<points_amount;i++)
	{
		points_file>>temp_str;
		points.push_back(temp_str);
	}

	for(int i=5;i<=order;i++)
	{
		ostringstream fname;
		int num;
		fname<<input_dir<<i<<"_good_nl.txt";
		ifstream inp(fname.str(),ios::in);
		while(!inp.eof())
		{
			inp>>num;
			good_nl[i-5].push_back(num);
		}
	}

	
	
	//ostringstream str;
	//char temp;
	//term cur_term;
	//string s,s2;
	//int subOrder=2; //текущая настоящая длина маршрутов	
	
	//int cur_num;
	//int kkk1;
	//bool stopflag=true;
	//cur_num=0;

	int point_num=0;
	
	for(point_num=0;point_num<points_amount;point_num++)
	{
		results.clear();

		sum_terms(0, order, order_nums, point_num, points, s_length, a_operators);
		sum_terms(1, order, order_nums, point_num, points, s_length, a_operators);
		sum_terms(2, order, order_nums, point_num, points, s_length, a_operators);


		ostringstream ostr;
		ostr << output_dir << points[point_num] << delim << order << "_results_" << points[point_num] << "_" << s_length << ".txt";
		ofstream outres(ostr.str(),ios::out);
		outres.setf(ios::fixed);
		outres<<setprecision(10);
		for(int i=0;i<results.size();i++)
		{
			if ((results[i].Sop.size()==(s_length-1))||abs(results[i].value) > 0.000001)
			{
				outres << results[i].name;
				for (int j = 0; j < results[i].Sop.size(); j++)
					outres << results[i].Sop[j].name;
				for (int j = 0; j < (int)results[i].Sop.size() - 1; j++)
					outres << results[i].Sop[j].n << "_";
				if (results[i].Sop.size()>0)
					outres << results[i].Sop[results[i].Sop.size() - 1].n;
				outres << " " << results[i].value;
				outres << "\n";
			}
		}
		outres.close();
		ostr.str("");
		ostr<<output_dir<<points[point_num]<<delim<<order<<"_results_2_"<<points[point_num]<<".txt";
		outres.open(ostr.str(),ios::out);
		for(int i=0;i<results.size();i++)
		{
			if (results[i].Sop.size()<3 || abs(results[i].value) > 0.000001)
			{
				outres<<results[i].name;
				for(int j=0;j<results[i].Sop.size();j++)
					outres<<results[i].Sop[j].name;
				//cout << (int)results[i].Sop.size() - 1;
				for(int j=0;j<(int)results[i].Sop.size()-1;j++)
					outres<<results[i].Sop[j].n<<"_";
				if (results[i].Sop.size()>0)
					outres << results[i].Sop[results[i].Sop.size() - 1].n;
				if(results[i].value!=0)
					outres<<" "<<results[i].value;
				else
					outres<<" 0";
				outres<<"\n";
			}
		}
		outres.close();
	}
	return 0;
}

