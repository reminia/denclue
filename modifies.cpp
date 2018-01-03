#include "modifies.h"

typedef HyperCube space_hypercube;


double calVectorDensity(DatasetEntity& p_entity , vector< DatasetEntity> dataset, double sigma )
{
	double density  = 0 ;
	DatasetEntity gradient(p_entity.getNumOfDimensions());

	vector< DatasetEntity>::iterator iter = dataset.begin();
	for( ;iter != dataset.end() ; ++iter)
	{
         density += DenclueFunctions::calculateInfluence( p_entity, *iter, sigma );
		 /*
		 for(int i = 0 ; i < p_entity.getNumOfDimensions() ; ++i)
			 gradient[i] = (iter->attributes[i] - p_entity.attributes[i])*density;
		*/
		// gradient = gradient + (*iter - p_entity) * density ;
	}
	return density;
}

double calDensity(DatasetEntity &p_entity ,  HyperSpace *p_space)
{
	double density = 0;
	double sigma = p_space->getSigma();
	HyperCube p_cube = p_space->findCubeFromKey(p_entity);
	//计算同一个cube中的点对density的贡献
	vector< DatasetEntity>::iterator iter = p_cube.retrieveObjects().begin();
	density = calVectorDensity(p_entity , p_cube.retrieveObjects() , sigma);
	

    //计算neighbour cube中的点对density的贡献
	vector<string> neighbours = p_cube.getNeighbors();
	vector<string>::iterator iter_neighbour = neighbours.begin();
	map< string, space_hypercube> hypercubes = p_space->getHypercubes();

	for( ; iter_neighbour != neighbours.end() ; ++iter_neighbour  )
	{
		map< string, space_hypercube>::iterator iter = hypercubes.find(*iter_neighbour);
		if(iter != hypercubes.end())
		{
			if(DatasetEntity::distanceBetween( p_entity , iter->second.getMeanElement()) <= 4 * sigma)
			{
				density = density + calVectorDensity(p_entity , iter->second.retrieveObjects() ,sigma);
			}
		}
	}

	

	return density;
}




//计算vector中的所有点对单个点的梯度和
DatasetEntity  calVectorGradient( DatasetEntity& p_entity , vector< DatasetEntity> dataset, double sigma) 
{
	double density  = 0 ;
	DatasetEntity gradient(p_entity.getNumOfDimensions());

	vector< DatasetEntity>::iterator iter = dataset.begin();
	for( ;iter != dataset.end() ; ++iter)
	{
         density = DenclueFunctions::calculateInfluence( p_entity, *iter, sigma );
		 /*
		 for(int i = 0 ; i < p_entity.getNumOfDimensions() ; ++i)
			 gradient[i] = (iter->attributes[i] - p_entity.attributes[i])*density;
		*/
		 gradient = gradient + (*iter - p_entity) * density ;
	}
	return gradient;
}


//计算gradient
DatasetEntity calGradient( DatasetEntity &p_entity , HyperCube &p_cube , HyperSpace *p_space) 
{

	double sigma = p_space->getSigma();
	DatasetEntity gradient(p_entity.getNumOfDimensions());

	//计算同一个cube中的点对梯度的贡献
	vector< DatasetEntity>::iterator iter = p_cube.retrieveObjects().begin();
	gradient = calVectorGradient(p_entity , p_cube.retrieveObjects() , sigma);
	

    //计算neighbour cube中的点对梯度的贡献
	vector<string> neighbours = p_cube.getNeighbors();
	vector<string>::iterator iter_neighbour = neighbours.begin();
	map< string, space_hypercube> hypercubes = p_space->getHypercubes();

	for( ; iter_neighbour != neighbours.end() ; ++iter_neighbour  )
	{
		map< string, space_hypercube>::iterator iter = hypercubes.find(*iter_neighbour);
		if(iter != hypercubes.end())
		{
			if(DatasetEntity::distanceBetween( p_entity , iter->second.getMeanElement()) <= 4 * sigma)
			{
				gradient = gradient + calVectorGradient(p_entity , iter->second.retrieveObjects() ,sigma);
			}
		}
	}

	

	return gradient;
}


//计算空间中所有有效cube中点的density
/*
DatasetEntity gainGradient(HyperSpace *p_space ,double sigma)
{
	map< string, HyperCube> cubes = p_space->getHypercubes();
	map< string, HyperCube>::iterator iter_cube = cubes.begin();
	vector< DatasetEntity > points ;
	vector< DatasetEntity >:: iterator iter_point ;
	DatasetEntity gradient(p_space->getDimensions());
	
	for(;  iter_cube != cubes.end(); ++iter_cube)  //遍历每个有效cube
	{
		points = (iter_cube->second).retrieveObjects(); 

		iter_point = points.begin();
		for( ; iter_point != points.end() ; ++iter_point) //遍历cube中的每个点,计算density
		{
			DatasetEntity gradient(p_space->getDimensions());
			gradient  += calDensity(*iter_point , points , sigma); //计算同一个cube中的点
			gradient	 += getNeighbourDensity(*iter_point , iter_cube->second  , p_space ,sigma);
			//iter_point->setDensity(density);
		}
		
	}

	return 
	
}
*/

//爬山法计算密度吸引子
DatasetEntity hillClimbing( DatasetEntity &entity ,HyperSpace *p_space)
{
	DatasetEntity current_entity = entity; //当前entity 
	DatasetEntity next_entity(entity.getNumOfDimensions()) ;  //下一个entity
	double step = 1;
	int maxn = 1000; //最大爬山次数
	int reachTop = 0 ; //是否到达山顶
	HyperCube p_cube = p_space->findCubeFromKey(entity);;
	do
	{
		if(--maxn <= 0) break; //爬山完毕
		p_cube = p_space->findCubeFromKey(entity);
	    DatasetEntity gradient = calGradient(current_entity ,p_cube , p_space );
		next_entity = current_entity + step / gradient.getEuclideanNorm() *  gradient;
		double next_density  = calDensity(next_entity , p_space);
		next_entity.setDensity( next_density );
		reachTop = next_entity.getDensity() < current_entity.getDensity();
		if(reachTop) break;
		current_entity = next_entity;
	}while(!reachTop);

    if(maxn <= 0) return next_entity;
	return current_entity;
}