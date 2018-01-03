#ifndef _MODIFIES_H
#define _MODIFIES_H
#include "denclue.h"

//DatasetEntity gainGradient(HyperSpace *p_space ,double sigma)
double calDensity(DatasetEntity &p_entity ,  HyperSpace *p_space);
DatasetEntity calGradient( DatasetEntity &p_entity , HyperCube &p_cube , HyperSpace *p_space ); 
DatasetEntity hillClimbing(const DatasetEntity &entity , HyperCube &p_cube , HyperSpace *p_space);


#endif