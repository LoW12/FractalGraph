#include <iostream>

#include "CGraph.h"

int main()
{
    try
    {
       //The lower level (0)  nodes of the fractal graph on the left
	CGraph<int> * cSub1Sub1Sub1 = new CGraph<int>("A1");
	CGraph<int> * cSub1Sub1Sub2 = new CGraph<int>("A2");

	//The nodes of level (1) of the fractal graph on the left
	CGraph<int> * cSub1Sub1 = new CGraph<int>("A");
	cSub1Sub1->AddNode(cSub1Sub1Sub1);
	cSub1Sub1->AddNode(cSub1Sub1Sub2);
	CArc<int> * a1 = new CArc<int>("a1",cSub1Sub1Sub1,cSub1Sub1Sub2,8);
	cSub1Sub1->AddArc(a1);

	CGraph<int> * cSub1Sub2 = new CGraph<int>("B");

	//The left fractal graph of level (2)
	CGraph<int> * cSub1 = new CGraph<int>("Sub1");
	cSub1->AddNode(cSub1Sub1);
	cSub1->AddNode(cSub1Sub2);
	CArc<int> * a2 = new CArc<int>("a2",cSub1Sub1,cSub1Sub2,3);
	cSub1->AddArc(a2);


	//The lower level (0) nodes of the fractal graph on the right
	CGraph<int> * cSub2Sub1Sub1 = new CGraph<int>("C1");
	CGraph<int> * cSub2Sub1Sub2 = new CGraph<int>("C2");
	CGraph<int> * cSub2Sub1Sub3 = new CGraph<int>("C3");


	//The nodes of level (1) of the fractal graph on the right
	CGraph<int> * cSub2Sub1 = new CGraph<int>("C");
	cSub2Sub1->AddNode(cSub2Sub1Sub1);
	cSub2Sub1->AddNode(cSub2Sub1Sub2);
	cSub2Sub1->AddNode(cSub2Sub1Sub3);
	CArc<int> * a3 = new CArc<int>("a3",cSub2Sub1Sub1,cSub2Sub1Sub2,6);
	CArc<int> * a4 = new CArc<int>("a4",cSub2Sub1Sub2,cSub2Sub1Sub3,4);
	CArc<int> * a5 = new CArc<int>("a5",cSub2Sub1Sub3,cSub2Sub1Sub1,9);
	cSub2Sub1->AddArc(a3);
	cSub2Sub1->AddArc(a4);
	cSub2Sub1->AddArc(a5);

	//The left fractal graph of level (2)
	CGraph<int> * cSub2 = new CGraph<int>("Sub2");
	cSub2->AddNode(cSub2Sub1);

	//The main graph level (3)
	CGraph<int> * cGraph = new CGraph<int>("MainGraph");
	cGraph->AddNode(cSub1);
	cGraph->AddNode(cSub2);
	CArc<int> * a6 = new CArc<int>("a6",cSub1,cSub2,1);
	CArc<int> * a7 = new CArc<int>("a7",cSub2,cSub1,2);
	cGraph->AddArc(a6);
	cGraph->AddArc(a7);
	
	//Export as dot
	cGraph->SaveAsDot("graph.dot");
    }
    catch ( LDException * e )
    {
        std::cout << "Exception was caught: " << e->what() << std::endl;
    }
  return 0;
}