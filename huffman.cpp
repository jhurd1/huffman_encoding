/***********************************************************************
 * Module:
 *    Week 08, Huffman
 *    Brother Kirby, CS 235
 * Author:
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 * Summary:
 *    This program will implement the huffman() function
 ************************************************************************/

#include <iostream>        // for CIN and COUT
#include <fstream>         // for IFSTREAM
#include <cassert>         // for ASSERT
#include <string>          // for STRING: binary representation of codes
#include "bnode.h"         // for BINARY_NODE class definition
#include "list.h"          // for list container
#include "pair.h"          // for PAIR container
#include "huffman.h"       // for HUFFMAN() prototype

using std::cout;
using std::cin;
using std::ifstream;
using std::endl;
using std::string;
using std::bad_alloc;
using namespace custom;

void readFile(custom::list<Huffman> & huffmanList,const string & fileName);
void getCodes(BNode< pair<float, string> > * pTree, custom::list<pair<string, string>> & codeList , std::string codeNum);
void display(custom::list<pair<string, string>> & codeList);

/*******************************************
 * HUFFMAN
 * Driver program to exercise the huffman generation code
 *******************************************/
void huffman(const string & fileName)
{
   // read values into data structure
   custom::list<Huffman> huffmanList;

   // read file into a list of Huffman type
   readFile(huffmanList, fileName);

   //Huffman nodule that contains the result of two other nodes
   Huffman addedTree;

   //Huffman compression/sorting
   //  While there is more than one item in the list
   //  as they are added, the combine up to when they become only one
   while(huffmanList.size() > 1)
   {
      //iterators to traverse huffmanList
      custom::list<Huffman>::iterator it = huffmanList.begin();
      custom::list<Huffman>::iterator itEnd = huffmanList.end();


      //iterators to handle smallest and second smallest
      custom::list<Huffman>::iterator itSmallest = it;
      custom::list<Huffman>::iterator itSecondSmallest;

      //Traverse the list to find the smallest item
      while(it != itEnd)
      {
         //(*it).pTree->data is the pair that holds the frequencies
         //   we are comparing with the "<" from the pair class
         if ((*it).pTree->data < (*itSmallest).pTree->data )
         {
            itSmallest = it++;
         }
         else
         {
            it++;
         }
      }

      //Save the smallest pair in a separate variable so that it
      //   won't be lost when the node is deleted.
      Huffman nodeLeft;
      nodeLeft.pTree = copyBTree((*itSmallest).pTree);
      huffmanList.erase(itSmallest);
      

      //reset the iterators
      it = huffmanList.begin();
      itSecondSmallest = it;
      itEnd = huffmanList.end();
      
      //This whole section is repeated, it could have been a function but
      //   we wanted ran out of time.
      //Traverse the list to find the smallest item
      while(it != itEnd)
      {
         //(*it).pTree->data is the pair that holds the frequencies
         //   we are comparing with the "<" from the pair class
         if ((*it).pTree->data < (*itSecondSmallest).pTree->data )
         {
            itSecondSmallest = it++;
         }
         else
         {
            it++;
         }
      }

      //Save the smallest pair in a separate variable so that it
      //   won't be lost when the node is deleted.
      Huffman nodeRight;
      nodeRight.pTree = copyBTree((*itSecondSmallest).pTree);
      huffmanList.erase(itSecondSmallest);

      //This piece of code makes the node with the bigger tree size be saved on the left
      //   whenever there are two nodes that have the same value

      // if either left or right node is a tree with multiple levels AND
      //   the frequencies of said nodes have the same value
      if ( (sizeBTree(nodeLeft.pTree) > 1 || sizeBTree(nodeRight.pTree) > 1) &&
         (nodeLeft.pTree->data.getFirst() == nodeRight.pTree->data.getFirst()))
      {
         // if the left node has the bigger tree...
         if ((sizeBTree(nodeLeft.pTree) > sizeBTree(nodeRight.pTree)))
         {
            //...add it on the left
            addedTree.add(nodeLeft, nodeRight);
         }
         else
         {
            //...add it on the right
            addedTree.add(nodeRight, nodeLeft);
         }
      }
      else
      {
         //if they don't have the same value, just add the lesser one on
         //   the left.
         addedTree.add(nodeLeft, nodeRight);
      }      

      //re-insert the new combined node back into the list for
      //   further comparison.
      huffmanList.push_back(addedTree);

   }
   //instance a list object, codeList
   custom::list<pair<string, string>> codeList;
   string codeNum;
   //pass the list object, codeList, into function: getCodes()
   getCodes(addedTree.pTree, codeList, codeNum);
   //display results
   display (codeList);
   
   return;
}

/********************************************
 * FUNCTION:  READFILE
 * RETURNS:   N/A
 * PARAMETER: takes a list and string filename
 ********************************************/
void readFile(custom::list<Huffman> & huffmanList,const string & fileName)
{
   //open file
   ifstream file;
   file.open(fileName.c_str());
   
   //file.open("/mnt/c/huffman1.txt"); // for local testing

   if ( file.fail() )
   {
      cout << "Unable to read file\n";
      return;
   }

   //values to create pair
   string second;
   float first{0.0};
   pair<float, string> dataPair;

   //While it can read from file AND is not the end of the file
   while (file >> second >> first && !file.eof() )
   {
      //create a Huffman tree with only one node
      Huffman singleTree(pair<float, string>(first, second));

      //insert Huffman tree into list of Huffmans
      huffmanList.push_back(singleTree);

   }

   //always remember to close the file
   file.close();

}


/********************************************
 * FUNCTION:  ADD
 * RETURNS:   HUFFMAN
 * PARAMETER: takes two nodes
 ********************************************/
Huffman & Huffman::add(Huffman & nodeLeft, Huffman & nodeRight)
{
   //Pairs to generate resultingPair: this is so that the combined
   //   node has a combined value, but not a "name"
   pair<float, string> pairLeft(nodeLeft.pTree->data);
   pair<float, string> pairRight(nodeRight.pTree->data);
   pair<float, string> resultingPair(pairLeft.getFirst() + pairRight.getFirst(),"");

   try
   {
      //create a new head of the tree with the resultingPair
      pTree = new BNode< pair<float, string> >(resultingPair);

      //add the children nodes
      addLeft(this->pTree, nodeLeft.pTree);
      addRight(this->pTree, nodeRight.pTree);
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }


   return *this;
}

/********************************************
 * FUNCTION:  GETCODES
 * RETURNS:   N/A
 * PARAMETER: bnode, list, and string
 ********************************************/
void getCodes(BNode< pair<float, string> > * pTree,
              custom::list<pair<string, string>> & codeList,
              std::string codeNum)
{ 
   string codeNumInside;
	// Assign 0 to left edge and recur 
	if (pTree->pLeft) { 
		getCodes(pTree->pLeft, codeList, codeNum + "0"); 
	} 

	// Assign 1 to right edge and recur 
	if (pTree->pRight) { 
		getCodes(pTree->pRight, codeList, codeNum + "1"); 
	} 

   //create a pair with the number code (codeNum)
   pair<string, string> code( (*pTree).data.getSecond(), codeNum );
   
   //and push the code onto the back of the list collection
   codeList.push_back(code);
}

/********************************************
 * FUNCTION:  DISPLAY
 * RETURNS:   N/A
 * PARAMETER: TAKES A LIST
 ********************************************/
void display(custom::list<pair<string, string>> & codeList)
{
   //iterators to traverse and compare codeList and save it into codeListOrganized
   typename list <pair<string, string>> :: iterator it = codeList.begin();
   typename list <pair<string, string>> :: iterator itEnd = codeList.end();
   typename list <pair<string, string>> :: iterator itSmallest = it;
   custom::list<pair<string, string>> codeListOrganized;

   //While the unorganized list still has elements in it
   while( codeList.size() ) // organize the huffman code list for output
   {
      //iterate through the list to find the smallest leaf
      while(it != itEnd)
      {
         //if it doesn't have a "name", it is not a leaf
         if ( (*it).getFirst() != "")
         {
            //(*it).getFirst() is the first element of the pair or "name"
            //   string.compare() returns < 0 when the string being compared is
            //   "smaller", meaning it comes first in the ASCII table
            if(   ((*it).getFirst().compare( (*itSmallest).getFirst() ) < 0 ) )
            {
               itSmallest = it;
            }
         }
         else
         {
            //if it doesn't have a name, delete it, as it is not a list
            it = codeList.erase(it);
         }
         
         it++;
      }
      
      //push the smallest element 
      codeListOrganized.push_back(*itSmallest);
      
      //erase it after added to the organized class
      codeList.erase(itSmallest);
      
      //reset pointers
      it = codeList.begin();
      itSmallest = it;
   }

   //output the organized huffman code list
   typename list <pair<string, string>> :: iterator disit = codeListOrganized.begin();

   for (disit = codeListOrganized.begin(); disit != codeListOrganized.end(); ++disit)
   {
      std::cout << (*disit).getFirst() << " = " << (*disit).getSecond() << std::endl;
   }
      
}

/**********************************************************
 * HUFFMAN non-default constructor
 *********************************************************/
Huffman::Huffman(pair<float, string> dataPair)
{
   try
   {
      //assign to pTree a new Bnode containing a pair object, 
      //   which contains two values--float and string
      //   and passes in var dataPair
      pTree = new BNode< pair<float, string> >(dataPair);
   }
   //in the event of an error
   //     output what() and carriage return
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
      
}