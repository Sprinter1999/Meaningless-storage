/***************************************************************************
 *   Copyright (C) 2005 by Robert Kovacevic                                *
 *   robert.kovacevic@etfos.hr                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "KarnaughMap.h"
#include <math.h>
#include <iostream>
using namespace std;

KarnaughMap::KarnaughMap(int n)
{
	// Set number of variables
	numberOfVariables=n;
	numberOfDCares=0;
	// Determine width and height from n. of vars
	width=(unsigned int)pow(2, ceil((double)n/2));
	height=(unsigned int)pow(2, floor((double)n/2));
	
	// Fill map with 0s and clear the list of KarnaughNodes
	Reset();
	
	// Fill map kmapValues with values that each cell in the map
	// has. Look here for rules:
	// http://www.allaboutcircuits.com/vol_4/chpt_8/3.html
	if(numberOfVariables>2)
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				// Set every 4x4 block's first 4 vars to gray code
				if(width>=4)
					kmapValues[make_pair(j, i)]=GrayEncode(j%4+((i%4)*4));
				else
					kmapValues[make_pair(j, i)]=GrayEncode(j%4+((i%4)*width));
				// Combine 4x4 blocks into map (5th and higher variables)
				kmapValues[make_pair(j, i)]+=16*(j/4+((i/4)*(width/4)));
			}
		}
	}
	else
	{
		if(numberOfVariables==2)
		{
			kmapValues[make_pair(0, 0)]=0;
			kmapValues[make_pair(1, 0)]=1;
			kmapValues[make_pair(0, 1)]=2;
			kmapValues[make_pair(1, 1)]=3;
		}
		if(numberOfVariables==1)
		{
			kmapValues[make_pair(0, 0)]=0;
			kmapValues[make_pair(1, 0)]=1;
		}
	}
}

void KarnaughMap::Reset()
{
	/* Fills map with zeros and deletes all nodes from the solution list */
	
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			Set(j, i, 0);
	blocks.clear();
}

void KarnaughMap::Solve()
{
	/* Iterates through all possible ways that 'Don't cares' can be
	arranged, and finds one with fewest number of nodes in the solution
	(bestc). If there are more ways that give the same number of nodes
	in the solution choose the one with biggest nodes (bestsc) */
	
	list <KarnaughNode> best;
	int bestc=-1;
	unsigned int bestsc=0;
	for(unsigned int i=0; i<pow(2, numberOfDCares); i++)
	{
		vector<bool> b;
		unsigned int j=i;
		while(j>0)
		{
			b.insert(b.begin(), (bool)(j%2));
			j=j/2;
		}
		for(j=b.size(); j<numberOfDCares; j++)
			b.insert(b.begin(), 0);

		blocks.clear();
		
		unsigned int c=0;
		for(int k=0; k<height; k++)
		{
			for(int l=0; l<width; l++)
			{
				if(kmapDCare[make_pair(l, k)]==1)
				{
					if(b[c]==1) kmap[make_pair(l, k)]=1;
						else kmap[make_pair(l, k)]=0;
					c++;
				}
			}
		}
		
		Solve2();
		
		if( (bestc==-1) || (blocks.size()<=bestc) )
		{
			unsigned int sc=0;
			list <KarnaughNode>::const_iterator iter;
			for (iter=blocks.begin(); iter != blocks.end(); iter++)
			{
				for(int i=0; i<iter->values.size(); i++)
					if(iter->values[i]==2) sc++;
			}
			
			if( (bestc==-1) || (blocks.size()<bestc) )
			{
				best=blocks;
				bestc=best.size();
				bestsc=sc;
			}
			else
			{
				if( sc>bestsc )
				{
					best=blocks;
					bestc=best.size();
					bestsc=sc;
				}
			}
		}
	}
	blocks=best;
}

void KarnaughMap::Solve2()
{
	/* Check for special case that all cells in the map are the same */
	bool a=1;
	for(int i=0; i<height; i++)
	{
		if(a==0) break;
		for(int j=0; j<width; j++)
			if( kmap[make_pair(j, i)]!=kmap[make_pair(0, 0)] )
			{
				a=0;
				break;
			}
	}

	if(a==1)
	{
		/* Clear the list so that all those nodes with one item are deleted */
		blocks.clear();
		
		// If there are only zeros in the map there's nothing to solve
		if (kmap[make_pair(0, 0)]==0) return;
		else
		{
			// If there are only ones, solution is one element as big as the map
			KarnaughNode n;
			n.numberOfItems = width*height;
			for(int j=0; j<numberOfVariables; j++)
				n.values.insert(n.values.begin(), 2);
			blocks.push_back(n);
			return;
		}
	}
	
	/* Put all blocks with 1 element in list */
	for(unsigned int i=0; i<height; i++)
	{
		for(unsigned int j=0; j<width; j++)
		{
			if(kmap[make_pair(j, i)]==1)
			{
				KarnaughNode n;
				n.numberOfItems=1;
				n.flag=0;
				n.values=GetMapBoolValue(j, i);
				blocks.push_back(n);
			}
		}
	}
	
	// Joining blocks into blocks with 2^i elements
	for(int i = 1; i <= (int)log2(width*height); i++)
	{
		/* Check every block with every other block and see if they can be joined
		into a bigger block */
		
		for(list <KarnaughNode>::iterator a=blocks.begin(); a!=blocks.end(); a++)
		{
			for(list <KarnaughNode>::iterator b=blocks.begin(); b!=blocks.end(); b++)
			{
				if( 	(a->numberOfItems == (int)pow(2, i-1)) && 
					(b->numberOfItems == (int)pow(2, i-1)) )
				{
					unsigned int x=IsJoinable(a->values, b->values);
					if(x>0)
					{
						/* If they can be joined make a new block with 2 in the place
						of the one bit where they a and b are different */
						KarnaughNode n;
						n.numberOfItems=a->numberOfItems*2;
						n.flag=0;
						for(int i=0; i<a->values.size(); i++)
						{
							if(i!=(x-1))
								n.values.insert(n.values.end(), a->values[i]);
							else
								n.values.insert(n.values.end(), 2);
						}
						
						/* Mark that a node is part of a larger node */
						a->flag=1;
						b->flag=1;
						
						/* Check if that block already exists in the list */
						bool exist=0;
						for(list <KarnaughNode>::const_iterator c=blocks.begin(); c!=blocks.end(); c++)
							if(n.values==c->values) exist=1;
							
						if(!exist) blocks.push_back(n);
					}
				}
			}
		}
	}
	
	/* Deletes nodes that are cointained in larger nodes */
	for(list <KarnaughNode>::iterator a=blocks.begin(); a!=blocks.end(); a++)
	{
		if(a->flag==1)
		{
			blocks.erase(a);
			a--;
		}
	}
	
	/* Deletes unneeded nodes. Draws a temp map with all nodes but one
	and if that map is same as the main map, node that wasn't drawn can be deleted */
	
	map <pair<unsigned int, unsigned int>, unsigned short int> temp;
	
	for(list <KarnaughNode>::iterator a=blocks.begin(); a!=blocks.end(); a++)
	{	
		for(int i=0; i<height; i++)
			for(int j=0; j<width; j++)
				temp[make_pair(j, i)]=0;
		
		for(list <KarnaughNode>::iterator b=blocks.begin(); b!=blocks.end(); b++)
		{
			if(a!=b)
			{
				for(int i=0; i<height; i++)
				{
					for(int j=0; j<width; j++)
					{
						if(IsAtCell(j, i, b->values))
							temp[make_pair(j, i)]=1;
					}
				}
			}
		}
		
		bool del=1;
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(temp[make_pair(j, i)]!=kmap[make_pair(j, i)]) 
				{
					del=0;
					break;
				}
			}
			if(!del) break;
		}
				
				
		if(del)
		{
			a=blocks.erase(a);
			a--;
		}
	}

}

bool KarnaughMap::IsAtCell(int x, int y, vector<unsigned short int> a)
{
	vector<unsigned short int> b=GetMapBoolValue(x, y);
	for(int i=0; i<a.size(); i++)
		if( (a[i]!=b[i]) && (a[i]!=2) ) return 0;
	return 1;
}

vector<unsigned short int> KarnaughMap::GetMapBoolValue(unsigned int x, unsigned int y)
{
	vector<unsigned short int> b;
	unsigned int i=GetMapValue(x, y);
	while(i>0)
	{
		b.insert(b.begin(), i%2);
		i=i/2;
	}
	for(int j=b.size(); j<numberOfVariables; j++)
		b.insert(b.begin(), 0);
	return b;
}

/* Checks if 2 karnaugh nodes with values a and b are joinable (only differ in one bit),
and if they are returns (place where they differ + 1), otherwise returns 0 */
unsigned int KarnaughMap::IsJoinable(vector<unsigned short int> a, vector<unsigned short int> b)
{
	int c=0;
	int x;
	for(int i=0; i<a.size(); i++)
	{
		if(a[i]!=b[i])
		{
			c++;
			x=i;
		}
	}
	if(c==1) return x+1;
		else return 0;
}

unsigned int KarnaughMap::GrayEncode(unsigned int g)
{
	switch(g)
	{
		case 0: return 0;
		case 1: return 1;
		case 2: return 3;
		case 3: return 2;
		
		case 4: return 4;
		case 5: return 5;
		case 6: return 7;
		case 7: return 6;
		
		case 8: return 12;
		case 9: return 13;
		case 10: return 15;
		case 11: return 14;
		
		case 12: return 8;
		case 13: return 9;
		case 14: return 11;
		case 15: return 10;
	}
}

void KarnaughMap::Set(unsigned int x, unsigned int y, unsigned short int value)
{
	kmap[make_pair(x, y)]=value;
	if(value==2) 
	{
		kmapDCare[make_pair(x, y)]=1;
		numberOfDCares++;
	}
	else kmapDCare[make_pair(x, y)]=0;
}

unsigned short int KarnaughMap::Get(unsigned int x, unsigned int y)
{
	if(!kmapDCare[make_pair(x,y)]) return kmap[make_pair(x,y)];
		else return 2;
}

unsigned int KarnaughMap::GetMapValue(unsigned int x, unsigned int y)
{
	return kmapValues[make_pair(x,y)];
}

unsigned int const KarnaughMap::GetWidth()
{
	return width;
}

unsigned int const KarnaughMap::GetHeight()
{
	return height;
}

list <KarnaughNode> KarnaughMap::GetSolutions()
{
	return blocks;
}

unsigned int const KarnaughMap::GetNumberOfVars()
{
	return numberOfVariables;
}
