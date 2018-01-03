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
	//����ͬһ��cube�еĵ��density�Ĺ���
	vector< DatasetEntity>::iterator iter = p_cube.retrieveObjects().begin();
	density = calVectorDensity(p_entity , p_cube.retrieveObjects() , sigma);
	

    //����neighbour cube�еĵ��density�Ĺ���
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




//����vector�е����е�Ե�������ݶȺ�
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


//����gradient
DatasetEntity calGradient( DatasetEntity &p_entity , HyperCube &p_cube , HyperSpace *p_space) 
{

	double sigma = p_space->getSigma();
	DatasetEntity gradient(p_entity.getNumOfDimensions());

	//����ͬһ��cube�еĵ���ݶȵĹ���
	vector< DatasetEntity>::iterator iter = p_cube.retrieveObjects().begin();
	gradient = calVectorGradient(p_entity , p_cube.retrieveObjects() , sigma);
	

    //����neighbour cube�еĵ���ݶȵĹ���
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


//����ռ���������Чcube�е��density
/*
DatasetEntity gainGradient(HyperSpace *p_space ,double sigma)
{
	map< string, HyperCube> cubes = p_space->getHypercubes();
	map< string, HyperCube>::iterator iter_cube = cubes.begin();
	vector< DatasetEntity > points ;
	vector< DatasetEntity >:: iterator iter_point ;
	DatasetEntity gradient(p_space->getDimensions());
	
	for(;  iter_cube != cubes.end(); ++iter_cube)  //����ÿ����Чcube
	{
		points = (iter_cube->second).retrieveObjects(); 

		iter_point = points.begin();
		for( ; iter_point != points.end() ; ++iter_point) //����cube�е�ÿ����,����density
		{
			DatasetEntity gradient(p_space->getDimensions());
			gradient  += calDensity(*iter_point , points , sigma); //����ͬһ��cube�еĵ�
			gradient	 += getNeighbourDensity(*iter_point , iter_cube->second  , p_space ,sigma);
			//iter_point->setDensity(density);
		}
		
	}

	return 
	
}
*/

//��ɽ�������ܶ�������
DatasetEntity hillClimbing( DatasetEntity &entity ,HyperSpace *p_space)
{
	DatasetEntity current_entity = entity; //��ǰentity 
	DatasetEntity next_entity(entity.getNumOfDimensions()) ;  //��һ��entity
	double step = 1;
	int maxn = 1000; //�����ɽ����
	int reachTop = 0 ; //�Ƿ񵽴�ɽ��
	HyperCube p_cube = p_space->findCubeFromKey(entity);;
	do
	{
		if(--maxn <= 0) break; //��ɽ���
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