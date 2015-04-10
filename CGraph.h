/**
 * FractalGraph
 * By : Bride Hadrien (hadrien.bride@femto-st.fr)
 * 	and Rousset Alban (alban.rousset@femto-st.fr)
 */



#ifndef CGRAPH_H_
#define CGRAPH_H_

#include "LDException.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

template<typename T>
class CGraph;

template<typename T>
class CGraphElemnt
{
public:
    CGraphElemnt(std::string Label = "DefaultLabel")
    {
        this->SetLabel(Label);
        this->m_Parent = NULL;
    }

    std::string GetLabel()
    {
        return this->m_Label;
    }

    void SetLabel(std::string Label)
    {
        this->m_Label = Label;
    }

    CGraph<T> *GetParent()
    {
        return this->m_Parent;
    }

    void SetParent(CGraph<T> *Node)
    {
        this->m_Parent = Node;
    }

    void UnSetParent()
    {
        this->m_Parent = NULL;
    }

private:
    std::string m_Label;
    CGraph<T> *m_Parent;

};

template<typename T>
class CArc : public CGraphElemnt<T>
{
public:
    CArc(std::string Label, CGraph<T> *Source, CGraph<T> *Target, T Value) : CGraphElemnt<T>(Label)
    {
        this->SetValue(Value);
        this->SetSource(Source);
        this->SetTarget(Target);
    }

    void SetParent(CGraph<T> *Node)
    {
        CGraph<T> *Parent = this->GetParent();
        if(Parent == NULL)
        {
            CGraphElemnt<T>::SetParent(Node);
        }
        else
        {
            Parent->DeleteArc(this);
            Node->AddArc(this);
            CGraphElemnt<T>::SetParent(Node);
        }
    }

    T GetValue()
    {
        return this->m_Value;
    }

    void SetValue(T Value)
    {
        this->m_Value = Value;
    }

    std::string GetValueAsString()
    {
        std::ostringstream oss;
        oss << this->m_Value;
        return oss.str();
    }

    CGraph<T> *GetSource()
    {
        return this->m_Source;
    }

    void SetSource(CGraph<T> *Source)
    {
        this->m_Source = Source;
    }

    CGraph<T> *GetTarget()
    {
        return this->m_Target;
    }

    void SetTarget(CGraph<T> *Target)
    {
        this->m_Target = Target;
    }

private:
    T m_Value;
    CGraph<T> *m_Source;
    CGraph<T> *m_Target;
};


template<typename T>
class CGraph : public CGraphElemnt<T>
{
public:
    CGraph(std::string Label) : CGraphElemnt<T>(Label)
    {
        this->m_vNodes = new std::vector<CGraph *>();
        this->m_vArcs = new std::vector<CArc<T> *>();

        this->m_InNeighbors = new std::vector<CGraph *>();
        this->m_OutNeighbors = new std::vector<CGraph *>();
    }

    void SetParent(CGraph<T> *Node)
    {
        CGraph<T> *Parent = this->GetParent();
        if(Parent == NULL)
        {
            CGraphElemnt<T>::SetParent(Node);
        }
        else
        {
            Parent->DeleteNode(this);
            Node->AddNode(this);
        }
    }

    CGraph<T> *GetNodeByLabel(std::string Label)
    {
        for (int i = 0; i < this->m_vNodes->size(); i++)
        {
            if (this->m_vNodes->at(i)->GetLabel() == Label)
            {
                return this->m_vNodes->at(i);
            }
        }
        return NULL;
    }

    void AddNode(CGraph<T> *Node)
    {
        if (this->GetNodeByLabel(Node->GetLabel()) == NULL)
        {
            Node->SetParent(this);
            this->m_vNodes->push_back(Node);
        }
        else
        {
            throw new LDException("CGraph : AddNode, node already in the graph(" + Node->GetLabel() + ")");
        }
    }

    void AddNode(std::vector<CGraph<T> *> *Nodes)
    {
        for (int i = 0; i < Nodes->size(); i++)
        {
            this->AddNode(Nodes->at(i));
        }
    }

    void DeleteNode(CGraph<T> *Node)
    {
        int i = this->GetIndexOfNode(Node);
        if (i != -1)
        {
            Node->UnSetParent();
            this->RemoveRelatedArcs(Node);
            this->m_vNodes->erase(m_vNodes->begin() + i);
        }
        else
        {
            throw new LDException("CGraph : DeleteNode, node not in the graph(" + Node->GetLabel() + ")");
        }
    }

    void DeleteNode(std::vector<CGraph<T> *> *Nodes)
    {
        for (int i = 0; i < Nodes->size(); i++)
        {
            this->DeleteNode(Nodes->at(i));
        }
    }

    CArc<T> *GetArcByLabel(std::string Label)
    {
        for (int i = 0; i < this->m_vArcs->size(); i++)
        {
            if (this->m_vArcs->at(i)->GetLabel() == Label)
            {
                return this->m_vArcs->at(i);
            }
        }
        return NULL;
    }

    void AddArc(CArc<T> *Arc)
    {
        if (this->GetArcByLabel(Arc->GetLabel()) == NULL)
        {
            if(this->GetNodeByLabel(Arc->GetSource()->GetLabel()) == NULL)
            {
                throw new LDException("CGraph : AddArc, Source not in the graph(" + Arc->GetSource()->GetLabel() + ")");
            }
            if(this->GetNodeByLabel(Arc->GetTarget()->GetLabel()) == NULL)
            {
                throw new LDException("CGraph : AddArc, Target not in the graph(" + Arc->GetTarget()->GetLabel() + ")");
            }
            Arc->SetParent(this);
            Arc->GetSource()->AddOutNeighbor(Arc->GetTarget());
            Arc->GetTarget()->AddInNeighbor(Arc->GetSource());
            this->m_vArcs->push_back(Arc);
        }
        else
        {
            throw new LDException("CGraph : AddArc, arc already in the graph(" + Arc->GetLabel() + ")");
        }
    }

    void AddArc(std::vector<CArc<T> *> *Arcs)
    {
        for (int i = 0; i < Arcs->size(); i++)
        {
            this->AddArc(Arcs->at(i));
        }
    }

    void DeleteArc(CArc<T> *Arc)
    {
        int i = this->GetIndexOfArc(Arc);
        if (i != -1)
        {
            Arc->UnSetParent();
            Arc->GetSource()->DeleteOutNeighbor(Arc->GetTarget());
            Arc->GetTarget()->DeleteInNeighbor(Arc->GetSource());
            this->m_vArcs->erase(m_vArcs->begin() + i);
        }
        else
        {
            throw new LDException("CGraph : DeleteArc, arc not in the graph(" + Arc->GetLabel() + ")");
        }
    }

    void DeleteArc(std::vector<CArc<T> *> *Arcs)
    {
        for (int i = 0; i < Arcs->size(); i++)
        {
            this->DeleteArc(Arcs->at(i));
        }
    }

    CGraph *GetInNeighborByLabel(std::string Label)
    {
        for (int i = 0; i < this->m_InNeighbors->size(); i++)
        {
            if (this->m_InNeighbors->at(i)->GetLabel() == Label)
            {
                return this->m_InNeighbors->at(i);
            }
        }
        return NULL;
    }

    void AddInNeighbor(CGraph<T> *Node)
    {
        if (this->GetInNeighborByLabel(Node->GetLabel()) == NULL)
        {
            this->m_InNeighbors->push_back(Node);
        }
        else
        {
            throw new LDException("CGraph : AddInNeighbor, node already in InNeighbors(" + Node->GetLabel() + ")");
        }
    }

    void DeleteInNeighbor(CGraph<T> *Node)
    {
        int i = this->GetIndexOfInNeighbor(Node);
        if (i != -1)
        {
            this->m_InNeighbors->erase(m_InNeighbors->begin() + i);
        }
        else
        {
            throw new LDException("CGraph : DeleteInNeighbor, node not in the InNeighbors(" + Node->GetLabel() + ")");
        }
    }

    CGraph *GetOutNeighborByLabel(std::string Label)
    {
        for (int i = 0; i < this->m_OutNeighbors->size(); i++)
        {
            if (this->m_OutNeighbors->at(i)->GetLabel() == Label)
            {
                return this->m_OutNeighbors->at(i);
            }
        }
        return NULL;
    }

    void AddOutNeighbor(CGraph<T> *Node)
    {
        if (this->GetOutNeighborByLabel(Node->GetLabel()) == NULL)
        {
            this->m_OutNeighbors->push_back(Node);
        }
        else
        {
            throw new LDException("CGraph : AddOutNeighbor, node already in OutNeighbors(" + Node->GetLabel() + ")");
        }
    }

    void DeleteOutNeighbor(CGraph<T> *Node)
    {
        int i = this->GetIndexOfOutNeighbor(Node);
        if (i != -1)
        {
            this->m_OutNeighbors->erase(m_OutNeighbors->begin() + i);
        }
        else
        {
            throw new LDException("CGraph : DeleteOutNeighbor, node not in the OutNeighbors(" + Node->GetLabel() + ")");
        }
    }

    std::vector<CGraph *> *GetNodes()
    {
        return this->m_vNodes;
    }

    std::vector<CArc<T> *> *GetArcs()
    {
        return this->m_vArcs;
    }

    std::vector<CGraph *> *GetInNeighbors(int n=1)
    {
        if(n == 1)
        {
            return this->m_InNeighbors;
        }
        else
        {
            std::vector<CGraph *> * InNeighbors = new std::vector<CGraph *>();
            InNeighbors->insert(InNeighbors->end(), this->m_InNeighbors->begin(), this->m_InNeighbors->end() );
            for (int i = 0; i < this->m_InNeighbors->size(); i++)
            {
                std::vector<CGraph *> * sub_InNeighbors = this->m_InNeighbors->at(i)->GetInNeighbors(n-1);
                for (int j = 0; j < sub_InNeighbors->size(); j++)
                {
                    bool isPresent = false;
                    for (int k = 0; k < InNeighbors->size(); k++)
                    {
                        if(InNeighbors->at(k)->GetLabel() == sub_InNeighbors->at(j)->GetLabel())
                        {
                            isPresent = true;
                            break;
                        }
                    }
                    if(!isPresent)
                    {
                        InNeighbors->push_back(sub_InNeighbors->at(j));
                    }
                }
            }
            return InNeighbors;
        }
    }

    std::vector<CGraph *> *GetOutNeighbors(int n=1)
    {
        if(n == 1)
        {
            return this->m_OutNeighbors;
        }
        else
        {
            std::vector<CGraph *> * OutNeighbors = new std::vector<CGraph *>();
            OutNeighbors->insert(OutNeighbors->end(), this->m_OutNeighbors->begin(), this->m_OutNeighbors->end() );
            for (int i = 0; i < this->m_OutNeighbors->size(); i++)
            {
                std::vector<CGraph *> * sub_OutNeighbors = this->m_OutNeighbors->at(i)->GetOutNeighbors(n-1);
                for (int j = 0; j < sub_OutNeighbors->size(); j++)
                {
                    bool isPresent = false;
                    for (int k = 0; k < OutNeighbors->size(); k++)
                    {
                        if(OutNeighbors->at(k)->GetLabel() == sub_OutNeighbors->at(j)->GetLabel())
                        {
                            isPresent = true;
                            break;
                        }
                    }
                    if(!isPresent)
                    {
                        OutNeighbors->push_back(sub_OutNeighbors->at(j));
                    }
                }
            }
            return OutNeighbors;
        }
    }

    std::vector<CGraph *> *GetAllNeighbors(int n=1)
    {
        std::vector<CGraph *> * Neighbors = new std::vector<CGraph *>();
        std::vector<CGraph *> * OutNeighbors =  this->GetOutNeighbors(n);
        std::vector<CGraph *> * InNeighbors =  this->GetInNeighbors(n);

        Neighbors->insert(Neighbors->end(), OutNeighbors->begin(), OutNeighbors->end() );

        for (int j = 0; j < InNeighbors->size(); j++)
        {
            bool isPresent = false;
            for (int k = 0; k < Neighbors->size(); k++)
            {
                if(Neighbors->at(k)->GetLabel() == InNeighbors->at(j)->GetLabel())
                {
                    isPresent = true;
                    break;
                }
            }
            if(!isPresent)
            {
                Neighbors->push_back(InNeighbors->at(j));
            }
        }

        return Neighbors;
    }

    std::string toDot()
    {
        std::string out;
        for (int i = 0; i < this->m_vNodes->size(); i++)
        {
            out += "subgraph cluster_"+this->m_vNodes->at(i)->GetLabel()+"{\n";
            out += "style=rounded\n";
            out += "label="+ this->m_vNodes->at(i)->GetLabel() + "\n";
            out += this->m_vNodes->at(i)->toDot();
            out += "}\n";
        }
        for (int i = 0; i < this->m_vArcs->size(); i++)
        {
            out += "cluster_"+this->m_vArcs->at(i)->GetSource()->GetLabel() + "->" + "cluster_"+this->m_vArcs->at(i)->GetTarget()->GetLabel();
            out += "[label=\"" + this->m_vArcs->at(i)->GetLabel() + ":" + this->m_vArcs->at(i)->GetValueAsString() + "\"]\n";
        }
        return out;
    }

    void SaveAsDot(std::string sFile)
    {
        std::ofstream outputFile;
        outputFile.open(sFile.c_str());
        outputFile << "digraph G {\n";
        outputFile << this->toDot();
        outputFile << "}";
        outputFile.close();
    }

private:
    std::vector<CGraph<T> *> *m_vNodes;
    std::vector<CArc<T> *> *m_vArcs;

    std::vector<CGraph<T> *> *m_InNeighbors;
    std::vector<CGraph<T> *> *m_OutNeighbors;


    size_t GetIndexOfNode(CGraph<T> *Node)
    {
        for (int i = 0; i < this->m_vNodes->size(); i++)
        {
            if (this->m_vNodes->at(i)->GetLabel() == Node->GetLabel())
            {
                return i;
            }
        }
        return -1;
    }

    size_t GetIndexOfArc(CArc<T> *Arc)
    {
        for (int i = 0; i < this->m_vArcs->size(); i++)
        {
            if (this->m_vArcs->at(i)->GetLabel() == Arc->GetLabel())
            {
                return i;
            }
        }
        return -1;
    }

    size_t GetIndexOfOutNeighbor(CGraph<T> *Node)
    {
        for (int i = 0; i < this->m_OutNeighbors->size(); i++)
        {
            if (this->m_OutNeighbors->at(i)->GetLabel() == Node->GetLabel())
            {
                return i;
            }
        }
        return -1;
    }

    size_t GetIndexOfInNeighbor(CGraph<T> *Node)
    {
        for (int i = 0; i < this->m_InNeighbors->size(); i++)
        {
            if (this->m_InNeighbors->at(i)->GetLabel() == Node->GetLabel())
            {
                return i;
            }
        }
        return -1;
    }

    void RemoveRelatedArcs(CGraph<T> *Node)
    {
        typename std::vector<CArc<T> *>::iterator iter = this->m_vArcs->begin();
        while (iter != this->m_vArcs->end())
        {
            if ((*iter)->GetSource()->GetLabel() == Node->GetLabel()
                    || (*iter)->GetTarget()->GetLabel() == Node->GetLabel())
            {
                iter = this->m_vArcs->erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
};

#endif
