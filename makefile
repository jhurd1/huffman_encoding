###############################################################
# Program:
#     Assignment 08, Binary Tree
#     Brother Kirby, CS235
# Author:
#     Daniel Perez, Jamie Hurd, Benjamin Dyas
# Summary:
#     This program contains the implementation of binary nodes,
#     a list of pairs, and a huffman code generator.
# Time:
#     Pair programming: 07 hours
#     Daniel:           06 hours
#     Benjamin:         06 hours
#		Jamie:            06 hours
# The most difficult part:
#     Daniel:   The most difficult part was figuring out the 
#               Huffman sorting logic. Finding the smallest
#               leaf, figuring out the add function, and
#               extracting the code where very time consuming.
#	   Benjamin: The most difficult part was shown with the
#               sorted list of the huffman codes. This showed
#               an issue with the larger huffman tree. In the
#               comparison if the two nodes being compared to
#               be added onto the tree had the same value we
#               needed to sort them into the correct path based
#               on the leaf values of the nodes.
#	   Jamie:    The most difficult part for me comprised 
#	             learning the new environment for this team,
#               which will continue to prove a challenge to me
#               as I have been using xcode and git for the last
#               1.5 years of my experience at BYUI.
###############################################################

##############################################################
# The main rule
##############################################################
a.out: assignment08.o huffman.o 
	g++ -o a.out assignment08.o huffman.o -g
	tar -cf assignment08.tar *.h *.cpp makefile

##############################################################
# The individual components
#      assignment08.o : the driver program
#      huffman.o      : the logic for the huffman code program
##############################################################
assignment08.o: bnode.h huffman.h assignment08.cpp
	g++ -c assignment08.cpp -g

huffman.o: bnode.h list.h huffman.h huffman.cpp
	g++ -c huffman.cpp -g

