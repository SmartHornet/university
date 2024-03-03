#include "stdafx.h"
/*#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>*/
#include <math.h>
#include <fstream>

using namespace std;

struct listOfIDsItem//������� ������ ���������������
{
	int Id;
	struct listOfIDsItem *NextItem;
};


class listOfIDs //������ ���������������
{
public:
	struct listOfIDsItem *_FirstItem;

	listOfIDs()
	{
		_FirstItem = NULL;
	}

	~listOfIDs() //����������� ������
	{
		if(_FirstItem != NULL) DestroyList(_FirstItem);
		//DestroyList(_FirstItem);
	}

	void DestroyList(struct listOfIDsItem *FirstItem)
	{
		if(FirstItem -> NextItem != NULL) DestroyList(FirstItem -> NextItem);
		delete FirstItem;
	}

	/*void DestroyList(struct listOfIDsItem *FirstItem)
	{
		struct listOfIDsItem *CurrItem, *FirstItem1;
		FirstItem1 = FirstItem;
		CurrItem = FirstItem1;
		while(FirstItem1 != NULL)
		{
			FirstItem1 = CurrItem -> NextItem;
			delete CurrItem;
			CurrItem = FirstItem1;
		}
	}*/

	void Clear(void);

	/*void Clear(void)
	{
		if(_FirstItem != NULL) DestroyList(_FirstItem);
		//DestroyList(_FirstItem);
		_FirstItem = NULL;
	}*/

	int GetNewId(void)
	{
		struct listOfIDsItem *CurrItem, *PrevItem;
		bool found;
		int Id;

		CurrItem = _FirstItem; found = 0; Id = 0; PrevItem = NULL;
		while((CurrItem != NULL) && !found)
		{
			if(Id != CurrItem -> Id) {found = 1;}
				else {PrevItem = CurrItem;  Id++;}

			CurrItem = CurrItem -> NextItem;
		}

		CurrItem = new struct listOfIDsItem;
		CurrItem -> Id = Id;

		if(PrevItem == NULL) {CurrItem -> NextItem = _FirstItem; _FirstItem = CurrItem;}
			else{CurrItem -> NextItem = PrevItem -> NextItem; PrevItem -> NextItem = CurrItem;}

		return Id;
	}

	void AddId(int Id)//������� ��� �������� �� ���������������, ������������ ��� �������� �� �����
	{
		struct listOfIDsItem *CurrItem, *PtrItem;

		CurrItem = new struct listOfIDsItem;
		CurrItem -> Id = Id;
		CurrItem -> NextItem = NULL;

		if(_FirstItem == NULL){_FirstItem = CurrItem;} 
		else
		{
			PtrItem = _FirstItem;
			while(PtrItem -> NextItem != NULL){PtrItem = PtrItem -> NextItem;}
			PtrItem -> NextItem = CurrItem;
		}
	}

	void RemoveId(int Id)
	{
		struct listOfIDsItem *CurrItem, *PrevItem;
		bool Found;

		CurrItem = this -> _FirstItem; 
		PrevItem = NULL;
		Found = 0;

		while((CurrItem != NULL) && !Found)
		{
			if(CurrItem -> Id == Id){Found = 1;}
				else {PrevItem = CurrItem; CurrItem = CurrItem -> NextItem;}
		}
		
		if(Found)
		{
			if(PrevItem == NULL){this -> _FirstItem = CurrItem -> NextItem;} //���� ������� ������
				else{PrevItem -> NextItem = CurrItem -> NextItem;}

			delete CurrItem;
		}
	}
};

/*struct ItemDataVertex
{
	int Id,X,Y;
	//int Color;
	//int selection - ������ ���������
	//string Text;
	struct VertexData *NextItem;
};

struct ItemDataEdge
{
	int Weight,X1,Y1,X2,Y2,Id1,Id2;
	//int selection - ������ ���������
	//int Color; //����
	//string Text;
	struct ItemDataEdge *NextItem;
};*/

struct VertexData
{
	int Id,X,Y;
};

struct EdgeData
{
	int BeginVertexId, EndVertexId, Weight;
};

struct Vertex;
struct Edge
{
	struct Vertex *IncidentVertex;
	struct Edge *NextEdge;
	int Weight;
	char Label;//�����, 0 - ����� �� ��������
	//int selection - ������ ���������
	//int Color; //����
	//string Text;
};

struct Vertex
{
	int Id,X,Y;
	int Label;//�����, 0 - ������� �� ��������
	//int Color;
	struct Vertex *NextVertex;
	//int selection - ������ ���������
	struct Edge *FirstEdge; //������ ���������
	struct Edge *LastEdge;
	//struct Edge *CurrEdge;
	//string Text;
};


class Graph
{
    public:
	struct Vertex *_FirstVertex;
	struct Vertex *_LastVertex;

	//���������� ��� ���������/����������� ������ ��������� � �������������
	int **_incidence/*������� �������������*/,**_adjacency;/*������� ���������*/
	int _NumVertex;

	//struct Vertex *_CurrVertex;
	//string Text;

	//default ��������

	listOfIDs *_listOfIDs; //������ �������������� ���������������
	bool _Directed; //1-���� ���������������

	Graph(bool Directed)
	{
		_listOfIDs = new listOfIDs;
		_FirstVertex = NULL;
		_LastVertex = NULL;
		//Text.reserve(50);
		_Directed = Directed;
		_incidence = NULL;
		_adjacency = NULL;
	}

	~Graph()
	{
		delete _listOfIDs;
		//DestroyListVertex(_FirstVertex);
		if(_FirstVertex != NULL) DestroyListVertex(_FirstVertex);
		DestroyMatrix(_incidence, _NumVertex);
		DestroyMatrix(_adjacency, _NumVertex);
	}
    
    void DestroyListEdge(struct Edge *FirstEdge)
	{
		if(FirstEdge -> NextEdge != NULL) DestroyListEdge(FirstEdge -> NextEdge);
		delete FirstEdge;
	}
    
	void DestroyListVertex(struct Vertex *FirstVertex)
	{
		if(FirstVertex -> NextVertex != NULL) DestroyListVertex(FirstVertex -> NextVertex);
		if(FirstVertex -> FirstEdge != NULL) DestroyListEdge(FirstVertex -> FirstEdge);
		delete FirstVertex;
	}

	void DestroyMatrix(int **Matrix, int Rows)
	{
		int i;
		if(Matrix != NULL)
		{
			for(i = 0; i < Rows; i++)
			{
				delete[] Matrix[i];
			}
			delete[] Matrix;
		}
	}
	/*void DestroyListEdge(struct Edge *FirstEdge)
	{
		struct Edge *CurrEdge, *FirstEdge1;
		FirstEdge1 = FirstEdge;
		CurrEdge = FirstEdge1;
		while(FirstEdge1 != NULL)
		{
			FirstEdge1 = CurrEdge -> NextEdge;
			delete CurrEdge;
			CurrEdge = FirstEdge1;
		}
	}
	void DestroyListVertex(struct Vertex *FirstVertex)
	{
		struct Vertex *CurrVertex, *FirstVertex1;
		FirstVertex1 = FirstVertex;
		CurrVertex = FirstVertex1;
		while(FirstVertex1 != NULL)
		{
			FirstVertex1 = CurrVertex -> NextVertex;
			DestroyListEdge(CurrVertex -> FirstEdge);
			delete CurrVertex;
			CurrVertex = FirstVertex1;
		}
	}*/

	void Clear(void)
	{
		
		if(_FirstVertex != NULL) DestroyListVertex(_FirstVertex);
		_listOfIDs -> Clear();
		//DestroyListVertex(_FirstVertex);
		_FirstVertex = NULL;
		_LastVertex = NULL;
	}

	void _AddVertex(int Id, int X, int Y)
	{
		struct Vertex *CurrVertex;

		CurrVertex = new struct Vertex;
		CurrVertex -> Id = Id;
		CurrVertex -> FirstEdge = NULL;
		CurrVertex -> LastEdge = NULL;
		CurrVertex -> Label = 0;
		CurrVertex -> X = X;
		CurrVertex -> Y = Y;
		CurrVertex -> NextVertex = NULL;
		//CurrVertex -> Text.reserve(10);
		//*(CurrVertex -> Text) = Convert::ToString(CurrVertex -> Id);

		if(_FirstVertex == NULL) {_FirstVertex = CurrVertex;}
			else {_LastVertex -> NextVertex = CurrVertex;}

		_LastVertex = CurrVertex;
	}

	void AddVertex(void)
	{
		int Id;
		Id = _listOfIDs -> GetNewId();
		_AddVertex(Id,0,0);
	}

	bool SearchVertex(struct Vertex **GetCurrVertex, struct Vertex **GetPrevVertex, int Id)
	{
		struct Vertex *CurrVertex, *PrevVertex;
		bool Found;

		CurrVertex = _FirstVertex;
		PrevVertex = NULL;
		Found = 0;
		while((CurrVertex != NULL) && !Found)
		{
			if(CurrVertex -> Id == Id){Found = 1;}
				else {PrevVertex = CurrVertex; CurrVertex = CurrVertex -> NextVertex;}
		}

		if(GetPrevVertex != NULL) *GetPrevVertex = PrevVertex;
		if(GetCurrVertex != NULL) *GetCurrVertex = CurrVertex;

		return Found;
	}

	void DelVertex(int Id)//���� ������� �� ������� � ���������� 1, ����� ������ �� ������ � ���� 0
	{
		struct Vertex *CurrVertex, *PrevVertex;
		bool Found;

		Found = SearchVertex(&CurrVertex, &PrevVertex, Id);
        
		if(Found){DelVertexPtr(PrevVertex, CurrVertex);}
	}

	void DelVertexPtr(struct Vertex *PrevVertex, struct Vertex *CurrVertex)
	{
		struct Vertex *PtrVertex, *CurrVertex1;

		if(CurrVertex == _FirstVertex){_FirstVertex = CurrVertex -> NextVertex;}
			else {PrevVertex -> NextVertex = CurrVertex -> NextVertex;}
		if(CurrVertex == _LastVertex) _LastVertex = PrevVertex;

		PtrVertex = CurrVertex;

		_listOfIDs -> RemoveId(CurrVertex -> Id);
		if(CurrVertex -> FirstEdge != NULL) DestroyListEdge(CurrVertex -> FirstEdge);
		delete CurrVertex;

		CurrVertex1 = _FirstVertex;
		while(CurrVertex1 != NULL)
		{
			DelEdgeInVertex(CurrVertex1, PtrVertex); 
			CurrVertex1 = CurrVertex1 -> NextVertex;
		}
	}

	struct Vertex *GetPtrVertex(int Id)//���� ��������� �����a���� null
	{
		struct Vertex *CurrVertex;
		bool found = 0;

		CurrVertex = _FirstVertex;
		while((CurrVertex != NULL) && !found)
		{
			if(Id == CurrVertex -> Id){found = 1;} 
				else {CurrVertex = CurrVertex -> NextVertex;}
		}

		return CurrVertex;
	}

	bool AddEdge(int BeginVertexId, int EndVertexId, int Weight)//1-������� 0-�� �������
	{
		struct Vertex *BeginVertex, *EndVertex;
		bool Found;

		BeginVertex = GetPtrVertex(BeginVertexId);
		EndVertex = GetPtrVertex(EndVertexId);

		if(BeginVertex == NULL || EndVertex == NULL) Found = 0;
			else
			{
				Found = AddEdgeInVertex(BeginVertex, EndVertex, Weight);
				if(_Directed == 0) AddEdgeInVertex(EndVertex, BeginVertex, Weight);
			}
		return Found;
	}
    
    bool SearchEdge(struct Vertex *BeginVertex, struct Vertex *EndVertex, struct Edge **GetCurrEdge, struct Edge **GetPrevEdge)
	{
		struct Edge *CurrEdge, *PrevEdge;
		bool Found;

		CurrEdge = BeginVertex -> FirstEdge;
		PrevEdge = NULL;
		Found = 0;
		while((CurrEdge != NULL) && !Found)
		{
			if(CurrEdge -> IncidentVertex == EndVertex){Found = 1;}
				else {PrevEdge = CurrEdge; CurrEdge = CurrEdge -> NextEdge;}
		}

		if(GetPrevEdge != NULL) *GetPrevEdge = PrevEdge;
		if(GetCurrEdge != NULL) *GetCurrEdge = CurrEdge;

		return Found;
	}

	struct Edge *GetPtrEdge(struct Vertex *BeginVertex, struct Vertex *EndVertex)//���� ��������� �����a���� null
	{
		struct Edge *CurrEdge;
		bool Found;

		Found = 0;
		if(BeginVertex == NULL || EndVertex == NULL){CurrEdge = NULL;} else {CurrEdge = BeginVertex -> FirstEdge;}
		while((CurrEdge != NULL) && !Found)
		{
			if(CurrEdge -> IncidentVertex == EndVertex){Found = 1;}
			else {CurrEdge = CurrEdge -> NextEdge;}
		}
		return CurrEdge;
	}
    
	bool AddEdgeInVertex(struct Vertex *BeginVertex, struct Vertex *EndVertex, int Weight)//1-������� 0-���
	{
		bool Found;
		//Found = SearchEdge(BeginVertex, EndVertex, NULL, NULL);
		struct Edge *PtrEdge;
		PtrEdge = GetPtrEdge(BeginVertex,EndVertex);

		if(PtrEdge == NULL)
		//if(Found == 0)
		{
			struct Edge *CurrEdge;

			CurrEdge = new struct Edge;
			CurrEdge -> IncidentVertex = EndVertex;
			CurrEdge -> Weight = Weight;
			CurrEdge -> NextEdge = NULL;
			CurrEdge -> Label = 0;

			if(BeginVertex -> FirstEdge == NULL) BeginVertex -> FirstEdge = CurrEdge; 
				else BeginVertex -> LastEdge -> NextEdge = CurrEdge;

			BeginVertex -> LastEdge = CurrEdge;

			Found = 1;
		}
		else {Found = 0;}

		return Found;
	}

	bool DelEdge(int BeginVertexId, int EndVertexId)
	{
		struct Vertex *BeginVertex, *EndVertex;
		bool Found;

		BeginVertex = GetPtrVertex(BeginVertexId);
		EndVertex = GetPtrVertex(EndVertexId);

		if(BeginVertex == NULL || EndVertex == NULL) Found = 0;
			else
			{
				Found = DelEdgePtr(BeginVertex, EndVertex);
			}
		return Found;
	}

	bool DelEdgePtr(struct Vertex *BeginVertex, struct Vertex *EndVertex)
	{
		bool Found;
		Found = DelEdgeInVertex(BeginVertex, EndVertex);
		if(_Directed == 0) DelEdgeInVertex(EndVertex, BeginVertex);
		return Found;
	}

	bool DelEdgeInVertex(struct Vertex *BeginVertex, struct Vertex *EndVertex)//���� ������� �� ������� � ���������� 1, ����� ������ �� ������ � ���� 0
	{
		struct Edge *CurrEdge, *PrevEdge;
		bool Found;

		Found = SearchEdge(BeginVertex, EndVertex, &CurrEdge, &PrevEdge);
		
		if(Found)
		{
			if(CurrEdge == BeginVertex -> FirstEdge) BeginVertex -> FirstEdge = CurrEdge -> NextEdge;
				else PrevEdge -> NextEdge = CurrEdge -> NextEdge;
			if(CurrEdge == BeginVertex -> LastEdge) BeginVertex -> LastEdge = PrevEdge;

			delete CurrEdge;
		}
		return Found;
	}

	void DelLabelEdge(int Label)
	{
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge, *PrevEdge, *PtrEdge;

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrEdge = CurrVertex -> FirstEdge;
			PrevEdge = NULL;
			while(CurrEdge != NULL)
			{
				if(CurrEdge -> Label == Label)
				{
					if(CurrEdge == CurrVertex -> FirstEdge) CurrVertex -> FirstEdge = CurrEdge -> NextEdge;
					else PrevEdge -> NextEdge = CurrEdge -> NextEdge;
					if(CurrEdge == CurrVertex -> LastEdge) CurrVertex -> LastEdge = PrevEdge;

					PtrEdge = CurrEdge;
					CurrEdge = CurrEdge -> NextEdge;
					delete PtrEdge;
				}
				else {PrevEdge = CurrEdge; CurrEdge = CurrEdge -> NextEdge;}
			}
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	int _GetNumVertex(struct Vertex *FirstVertex)
	{
		if(FirstVertex == NULL) return 0; else return 1 + _GetNumVertex(FirstVertex -> NextVertex);
	}

	int GetNumVertex(void){return _GetNumVertex(_FirstVertex);}

	int GetNumEdgeInVertex(struct Edge *FirstEdge)
	{
		if(FirstEdge == NULL) return 0; else return 1 + GetNumEdgeInVertex(FirstEdge -> NextEdge);
	}

	int _GetNumEdge(struct Vertex *FirstVertex)
	{
		if(FirstVertex == NULL) return 0; else return GetNumEdgeInVertex(FirstVertex -> FirstEdge) + _GetNumEdge(FirstVertex -> NextVertex);
	}

	int GetNumEdge(void)
	{
		int NumEdge;
		NumEdge = _GetNumEdge(_FirstVertex);
		if(_Directed == 0) NumEdge = (int)NumEdge/2;
		return NumEdge;
	}

	int _GetWeightGraph(struct Vertex *FirstVertex)
	{
		if(FirstVertex == NULL) return 0; else return GetWeightInVertex(FirstVertex -> FirstEdge) + _GetWeightGraph(FirstVertex -> NextVertex);
	}

	int GetWeightInVertex(struct Edge *FirstEdge)
	{
		if(FirstEdge == NULL) return 0; else return FirstEdge -> Weight + GetWeightInVertex(FirstEdge -> NextEdge);
	}

	int GetWeightGraph(void)
	{
		int Weight;
		Weight = _GetWeightGraph(_FirstVertex);
		if(_Directed == 0) Weight = (int)Weight/2;
		return Weight;
	}

	void ToLabelAll(int Label)
	{
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrVertex -> Label = Label;
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
				CurrEdge -> Label = Label;
				CurrEdge = CurrEdge -> NextEdge;
			}
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	void ToLabelAllVertex(int Label)
	{
		struct Vertex *CurrVertex;
		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrVertex -> Label = Label;
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	void ChangeLabelVertex(int OldLabel, int NewLabel)
	{
		struct Vertex *CurrVertex;

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			if(CurrVertex -> Label == OldLabel) CurrVertex -> Label = NewLabel;
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	void ToLabelAllEdge(int Label)
	{
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
				CurrEdge -> Label = Label;
				CurrEdge = CurrEdge -> NextEdge;
			}
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	void ToLabelEdge(int Label, struct Vertex *BeginVertex, struct Vertex *EndVertex)
	{
		struct Edge *CurrEdge;
		//SearchEdge(BeginVertex, EndVertex, &CurrEdge, NULL);
		CurrEdge = GetPtrEdge(BeginVertex, EndVertex);
		CurrEdge -> Label = Label;
		if(_Directed == 0)
		{
			//SearchEdge(EndVertex, BeginVertex, &CurrEdge, NULL);
			CurrEdge = GetPtrEdge(EndVertex, BeginVertex);
			CurrEdge -> Label = Label;
		}
	}

	/*//begin = 1 �������� ��������� � ������ ������, 0 - �������� ��������� �� ������ ���������
	struct Vertex *GetPNextVertex(bool begin)
	{
		if(begin) {_CurrVertex = _FirstVertex;}
			else if(_CurrVertex != NULL){_CurrVertex = _CurrVertex -> NextVertex;}
	  
		return _CurrVertex;
	}*/


	//��������� �����:
	//bool _Directed; 1-���� ���������������
	//int NumVertex; ���-�� ������
	//NumVertex �������� struct VertexData
	//int NumEdge ���-�� �����
	//NumEdge �������� struct EdgeData

	//������ 1-��� ������ 2-������ �������� �����
	/*const char *path*/
	//int SaveGraph(const char *Path)
	int SaveGraph(void)
	{
		bool Directed;
		struct VertexData *VDTmp;
		struct EdgeData *EDTmp;
		int i,num;
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;

		//ofstream ofs(Path);
		ofstream ofs("tmp.vg");
		if(!ofs){return 2;}

		Directed = _Directed;
		ofs.write((char*)&Directed,sizeof(bool));

		num = GetNumVertex();
		ofs.write((char*)&num,sizeof(int));

		VDTmp = new struct VertexData[num];
		CurrVertex = _FirstVertex;
		for(i = 0; i < num; i++)
		{
			VDTmp[i].Id = CurrVertex -> Id;
			VDTmp[i].X = CurrVertex -> X;
			VDTmp[i].Y = CurrVertex -> Y;
			CurrVertex = CurrVertex -> NextVertex;
		}
		ofs.write((char*)VDTmp,num*sizeof(struct VertexData));
		
		num = GetNumEdge();
		ofs.write((char*)&num,sizeof(int));

		EDTmp = new struct EdgeData[num];
		CurrVertex = _FirstVertex;
		i = 0;
		while(CurrVertex != NULL)
		{
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
				if(CurrEdge -> Label == 0)
				{
					EDTmp[i].BeginVertexId = CurrVertex -> Id;
					EDTmp[i].EndVertexId = CurrEdge -> IncidentVertex -> Id;
					EDTmp[i].Weight = CurrEdge -> Weight;
					i = i + 1;
					ToLabelEdge(1,CurrVertex,CurrEdge -> IncidentVertex);
				}
				CurrEdge = CurrEdge -> NextEdge;
			}
			CurrVertex = CurrVertex -> NextVertex;
		}
		ofs.write((char*)EDTmp,num*sizeof(struct EdgeData));
		ToLabelAllEdge(0);

		delete[] VDTmp;
		delete[] EDTmp;
		ofs.close();
		return 1;
	}

	//������ 1-��� ������ 2-������ �������� �����
	/*const char *path "tmp.vg"*/
	//int LoadGraph(char *Path)
	int LoadGraph(void)
	{
		bool Directed;
		struct VertexData *VDTmp;
		struct EdgeData *EDTmp;
		int i,num;

		Clear();

		//ifstream ifs(Path);
		ifstream ifs("tmp.vg");
		if(!ifs){return 2;}

		ifs.read((char*)&Directed,sizeof(bool));
		_Directed = Directed;

		ifs.read((char*)&num,sizeof(int));

		VDTmp = new struct VertexData[num];
		ifs.read((char*)VDTmp,num*sizeof(struct VertexData));
		for(i = 0; i < num; i++)
		{
			_AddVertex(VDTmp[i].Id, VDTmp[i].X, VDTmp[i].Y);
			_listOfIDs -> AddId(VDTmp[i].Id);
		}
		
		ifs.read((char*)&num,sizeof(int));

		EDTmp = new struct EdgeData[num];
		ifs.read((char*)EDTmp,num*sizeof(struct EdgeData));
		for(i = 0; i < num; i++)
		{
			AddEdge(EDTmp[i].BeginVertexId, EDTmp[i].EndVertexId, EDTmp[i].Weight);
		}
		
		delete[] VDTmp;
		delete[] EDTmp;
		ifs.close();
		return 1;
	}

	int **GetIncidence(int *getn, int *getm)
	{
		int NumVertex, NumEdge, i, j, Mltplr;
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;

		DestroyMatrix(_incidence, _NumVertex);
		
		if(_Directed){Mltplr = -1;} else {Mltplr = 1;}

		NumVertex = GetNumVertex();
		NumEdge = GetNumEdge();

		_incidence = new int* [NumVertex];
		for(i = 0; i < NumVertex; i++)
		{
			_incidence[i] = new int [NumEdge];
		}
		_NumVertex = NumVertex;

		for(i = 0; i < NumVertex; i++)
		{
			for(j = 0; j < NumEdge; j++)
			{
				_incidence[i][j] = 0;
			}
		}

		CurrVertex = _FirstVertex;
		for(i = 0; i < NumVertex; i++)
		{
			CurrVertex -> Label = i;
			CurrVertex = CurrVertex -> NextVertex;
		}

		CurrVertex = _FirstVertex;
		i = 0;
		while(CurrVertex != NULL)
		{
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
				if(CurrEdge -> Label == 0)
				{
					_incidence[CurrVertex -> Label][i] = CurrEdge -> Weight;
					_incidence[CurrEdge -> IncidentVertex -> Label][i] = CurrEdge -> Weight * Mltplr;
					i = i + 1;
					ToLabelEdge(1,CurrVertex,CurrEdge -> IncidentVertex);
				}
				CurrEdge = CurrEdge -> NextEdge;
			}
			CurrVertex = CurrVertex -> NextVertex;
		}

		ToLabelAll(0);
		
		if(getn != NULL) *getn = NumVertex;
		if(getm != NULL) *getm = NumEdge;

		return _incidence;
	}

	int **GetAdjacency(int *getn)
	{
		int NumVertex, i, j;
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;

		DestroyMatrix(_adjacency, _NumVertex);

		NumVertex = GetNumVertex();

		_adjacency = new int* [NumVertex];
		for(i = 0; i < NumVertex; i++)
		{
			_adjacency[i] = new int [NumVertex];
		}
		_NumVertex = NumVertex;

		for(i = 0; i < NumVertex; i++)
		{
			for(j = 0; j < NumVertex; j++)
			{
				_adjacency[i][j] = 0;
			}
		}

		CurrVertex = _FirstVertex;
		for(i = 0; i < NumVertex; i++)
		{
			CurrVertex -> Label = i;
			CurrVertex = CurrVertex -> NextVertex;
		}

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
				_adjacency[CurrVertex -> Label][CurrEdge -> IncidentVertex -> Label] = CurrEdge -> Weight;
				CurrEdge = CurrEdge -> NextEdge;
			}
			CurrVertex = CurrVertex -> NextVertex;
		}

		ToLabelAllVertex(0);
		
		if(getn != NULL) *getn = NumVertex;

		return _adjacency;
	}
	
	void TraversalV(struct Vertex *FirstVertex)
	{
		if(FirstVertex != NULL) if(FirstVertex -> Label == 0){FirstVertex -> Label = 1; TraversalE(FirstVertex -> FirstEdge);}
	}

	void TraversalE(struct Edge *FirstEdge)
	{
		if(FirstEdge != NULL){TraversalV(FirstEdge -> IncidentVertex); TraversalE(FirstEdge -> NextEdge);}
	}
	
	bool Connected(void)
	{
		struct Vertex *CurrVertex;
		bool flag;

		TraversalV(_FirstVertex);

		flag = 1;
		CurrVertex = _FirstVertex;
		while((CurrVertex != NULL) && flag)
		{
			if(CurrVertex -> Label != 1) flag = 0;
			CurrVertex = CurrVertex -> NextVertex;
		}

		ToLabelAllVertex(0);
		return flag;
	}

	//���������� ������ �������� � ������� ������� ���� ���� � ������� ����� ����������� ����
	//����� ������� ����� ����� ��������� �� ������� ����
	void ToFlat(int X1, int Y1, int X2, int Y2)
	{
		int n,i;
		double seg,a,b;
		struct Vertex *CurrVertex;

		n = GetNumVertex();
		seg = 6.28/n;
		a = (X2 - X1)/2;
		b = (Y2 - Y1)/2;

		CurrVertex = _FirstVertex;
		for(i = 0; i < n; i++)
		{
			CurrVertex -> X = X1 + (int)(a + a*cos(i*seg));
			CurrVertex -> Y = Y1 + (int)(b + b*sin(i*seg));
			CurrVertex = CurrVertex -> NextVertex;
		}
	}

	//����� ����� ������������ ���� ����� ����������(VertexLabel) ������ � ����������(EdgeLabel) �����
	//���� VertexLabel = -1, �� ����� ����� ���� ������
	void FindMinLabelEdge(int VertexLabel, int EdgeLabel, struct Vertex **GetBeginVertex, struct Vertex **GetEndVertex)
	{
		struct Vertex *BeginVertex, *EndVertex, *CurrVertex;
		struct Edge *CurrEdge;
		int MinWeight;
		bool flag,NegVertexLabel;

		if(VertexLabel >= 0) NegVertexLabel = 0; else NegVertexLabel = 1;

		BeginVertex = NULL;
		EndVertex = NULL;
		CurrVertex = _FirstVertex;
		flag = 1;
		MinWeight = 0;
		while(CurrVertex != NULL)
		{
			if((CurrVertex -> Label == VertexLabel) || NegVertexLabel)
			{
				CurrEdge = CurrVertex -> FirstEdge;
				while(CurrEdge != NULL)
				{
					if(CurrEdge -> Label == EdgeLabel)
					{
						if(flag)
						{
							MinWeight = CurrEdge -> Weight; 
							BeginVertex = CurrVertex; 
							EndVertex = CurrEdge -> IncidentVertex; 
							flag = 0;
						}

						if(CurrEdge -> Weight < MinWeight)
						{
							MinWeight = CurrEdge -> Weight; 
							BeginVertex = CurrVertex; 
							EndVertex = CurrEdge -> IncidentVertex;
						}
					}
					CurrEdge = CurrEdge -> NextEdge;
				}
			}
			CurrVertex = CurrVertex -> NextVertex;
		}

		if(GetBeginVertex != NULL) *GetBeginVertex = BeginVertex;
		if(GetEndVertex != NULL) *GetEndVertex = EndVertex;
	}


//***************�������� ��������� ������������ ��������� ������ ������� ������,������� �����,������� ��������********************
	//������ 2 - ��� ������ ��� �����. 3 - ���� ���������. 1 - ��� ������
	//Label = 0 - ������ ��������� � �������� �����, Label = 1 - ������ ��������� � �������������� �����
	int AlgYarnik(void)
	{
		if(!Connected()) return 3; //���� ���� ���������

		struct Vertex *BeginVertex, *EndVertex;
		int n, ReadyVertex;

		ReadyVertex = 1;
		n = GetNumVertex();
		FindMinLabelEdge(0,0,&BeginVertex,&EndVertex);
		if(BeginVertex == NULL) return 2;//���� � ����� ��� ������ ��� �����
		
		do
		{
			BeginVertex -> Label = 1;
			EndVertex -> Label = 1;
			ToLabelEdge(1, BeginVertex, EndVertex);
			ReadyVertex++;

			FindMinLabelEdge(1,0,&BeginVertex,&EndVertex);
		}
		while(ReadyVertex < n);

		DelLabelEdge(0); //������� ������������(�����������������) �����
		ToLabelAll(0); //������� ������� �� ���� ������ � �����
		return 1;
	}

//***************�������� ��������� ������������ ��������� ������ ������� ��������***************************
	//������ 2 - ��� ������ ��� �����. 3 - ���� ���������. 1 - ��� ������
	//� ������: Label = 0 - ����� ��������� � �������� �����, Label = 1 - ����� ��������� � �������������� �����
	//Label = 2 - ����� ������ �� ���������������
	int AlgKraskal(void)
	{
		if(!Connected()) return 3; //���� ���� ���������

		struct Vertex *CurrVertex, *BeginVertex, *EndVertex;

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			CurrVertex -> Label = CurrVertex -> Id;
			CurrVertex = CurrVertex -> NextVertex;
		}

		FindMinLabelEdge(-1,0,&BeginVertex,&EndVertex);
		while(BeginVertex != NULL)
		{
			if(BeginVertex -> Label != EndVertex -> Label)
			{
				ChangeLabelVertex(EndVertex -> Label, BeginVertex -> Label);
				ToLabelEdge(1, BeginVertex, EndVertex);
			}
			else {ToLabelEdge(2, BeginVertex, EndVertex);}

			FindMinLabelEdge(-1,0,&BeginVertex,&EndVertex);
		}

		DelLabelEdge(2);
		ToLabelAll(0); //������� ������� �� ���� ������ � �����
		return 1;
	}

	/*void ListVertex()
	{
		struct Vertex *CurrVertex;
		struct Edge *CurrEdge;
		

		CurrVertex = _FirstVertex;
		while(CurrVertex != NULL)
		{
			printf("\nId: %d\n",CurrVertex -> Id);
			printf("PV: %d\n",CurrVertex);
			printf("NextV: %d\n",CurrVertex -> NextVertex);
			printf("label: %d\n",CurrVertex -> Label);
			
			printf("*�����:\n");
			CurrEdge = CurrVertex -> FirstEdge;
			while(CurrEdge != NULL)
			{
                printf("*PE: %d\n",CurrEdge);
                printf("*IncV: %d\n",CurrEdge -> IncidentVertex);
                printf("*NextE: %d\n",CurrEdge -> NextEdge);
                printf("*Weight: %d\n",CurrEdge -> Weight);
                printf("*label: %d\n",CurrEdge -> Label);
                CurrEdge = CurrEdge -> NextEdge;
			}
			
			CurrVertex = CurrVertex -> NextVertex;
		}
	
	}*/
};
