/***********************************************************************
 * Module:
 *    Week 08, Huffman
 *    Brother Kirby, CS 235
 * Author:
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 * Summary:
 *    This program will implement the huffman() function
 ************************************************************************/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include "bnode.h"
#include "pair.h"


void huffman(const std::string & fileName);

namespace custom
{
   /************************************************
    * HUFFMAN
    * Container class element that contains info
    * on previous and next elements in a list
    ***********************************************/
   class Huffman
   {
      public:
      Huffman() : pTree{nullptr} {}
      Huffman(pair<float, std::string> dataPair);

      Huffman & add(Huffman & nodeLeft, Huffman & nodeRight); 
      
      custom::BNode<custom::pair<float, std::string>> * pTree;

   };
}



#endif // HUFFMAN_h
