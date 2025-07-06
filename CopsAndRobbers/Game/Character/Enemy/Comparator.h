/*
	@file	Comparator.h
	@brief	ComparatorƒNƒ‰ƒX
*/
#pragma once
#ifndef COMPARATOR_DEFINED
#define COMPARATOR_DEFINED
#include "Game/Astar/AStarGraph.h"

class Comparator
{
public:
   using is_transparent = std::true_type;

   bool operator() (const CellVertex& lhs, const CellVertex& rhs) const
   {
	  if (lhs.GetScore() == rhs.GetScore())
	  {
		 return lhs.GetVertexNumber() < rhs.GetVertexNumber();
	  }
	  return lhs.GetScore() < rhs.GetScore();
   }
};
#endif // COMPARATOR_DEFINED
