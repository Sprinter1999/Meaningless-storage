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

#ifndef KARNAUGH_MAP_H
#define KARNAUGH_MAP_H

#include <vector>
#include <list>
#include <map>
#include <utility>

using namespace std;

struct KarnaughNode
{
		vector<unsigned short int> values;
		unsigned int numberOfItems;
		bool flag;
};

class KarnaughMap
{
	public:
		KarnaughMap(int n);
		void Reset();
		void Set(unsigned int x, unsigned int y, unsigned short int value);
		unsigned short int Get(unsigned int x, unsigned int y);
		unsigned int GetMapValue(unsigned int x, unsigned int y);
		vector<unsigned short int> GetMapBoolValue(unsigned int x, unsigned int y);
		bool IsAtCell(int x, int y, vector<unsigned short int> a);
		unsigned int const GetWidth();
		unsigned int const GetHeight();
		unsigned int const GetNumberOfVars();
		list <KarnaughNode> GetSolutions();
		void Solve();
	private:
		void Solve2();
		unsigned int GrayEncode(unsigned int g);
		unsigned int IsJoinable(vector<unsigned short int> a, vector<unsigned short int> b);
	
		map <pair<unsigned int, unsigned int>, unsigned short int> kmap;
		map <pair<unsigned int, unsigned int>, unsigned int> kmapValues;
		map <pair<unsigned int, unsigned int>, bool> kmapDCare;
		list <KarnaughNode> blocks;
		unsigned int numberOfDCares;
		unsigned int numberOfVariables;
		unsigned int width, height;
};

#endif // KARNAUGH_MAP_H
