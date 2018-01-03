 
 #ifndef DENCLUE_FUNCTIONS_H
 #define DENCLUE_FUNCTIONS_H
 
 
 /* INCLUSIONS */
 #include <iostream>
 #include <cmath>
 #include "dataset.h"
 using namespace std;
 
 
 /* CLASSES */
 
 /** @class DenclueFunctions
  * 
  * @brief This class implements the functions used in the clustering step of DENCLUE.
  * These functions correspond to the influence and density of dataset entities.
  *
  * */
 class DenclueFunctions {
 
 
     public:
 
         /** Calculate the influence of an entity in another. The chosen
          * influence function was the Gaussian Influence Function, defined by:
          * I(x,y) = exp { - [distance(x,y)**2] / [2*(sigma**2)] }
          *
          *  @param entity1 First entity of the operation
          *  @param entity2 Second entity of the operation
          *  @param sigma Parameter that ponderates the influence of an entity into another
          *
          * @return The value of influence.
          * */
         static double calculateInfluence( DatasetEntity& entity1,
                 DatasetEntity& entity2, double sigma );
 
 
         /** Calculate the density in an entity. It's defined as the sum of the
          * influence of each another entity of dataset.
          *
          *  @param entity The entity to calculate density
          *  @param iter Iterator over entities of dataset.
          *  @param sigma Parameter that ponderates the influence of an entity into another
          *
          * @return The value of density in entity.
          * */
         static double calculateDensity(DatasetEntity& entity ,
                 HyperSpace::EntityIterator iter, double sigma);
 
 
         /** Calculate gradient of density functions in a given spatial point.
          *
          *  @param entity The spatial point used to calculate the gradient.
          *  @param iter Iterator over all dataset entities.
          *  @param sigma Parameter that ponderates the influence of an entity into another
          *
          * @return The vector that represents the gradient of the influence
          *  function in a given spatial point.
          * */
         static vector<double> calculateGradient( DatasetEntity& entity,
                 HyperSpace::EntityIterator iter, double sigma );
 
 
         /** Find density-attractor for an entity. The density-attractor is
          * obtained executing a hill climbing algorithm.
          *
          *  @param entity The spatial point used to calculate the gradient.
          *  @param iter Iterator over all dataset entities.
          *  @param sigma Parameter that ponderates the influence of an entity into
          *  another
          *
          * @return An entity that is the density-attractor for the given
          *  entity.
          * */
         static const DatasetEntity getDensityAttractor(  DatasetEntity&
                 entity, HyperSpace& spatial_region,
                 HyperSpace::EntityIterator iter, double sigma );
 
 
         /** Verify whether an appropriate path between two density-attractor
          * exists.  Each entity in the path MUST satisfy the minimum density
          * restriction.
          *
          *  @param attractor1 Attractor where the path must start
          *  @param attractor2 Attractor where the path must end
          *  @param xi Minimum density threshold
          *
          * @return True, if is possible to establish a path between the given
          * density-attractors.  False, otherwise.
          * */
         static bool pathBetweenExists(DatasetEntity& attractor1,
                 DatasetEntity& attractor2, HyperSpace& hs, double xi, double
                 sigma, map<string, bool>& usedEntities );
 
 
         /** Append one vector to the end of another.
          *
          *  @param dest Vector that will receive new elements.
          *  @param src Vector with elements to be appended to vector 'dest'
          *
          * */
         static void AppendVector( vector<DatasetEntity>& dest, vector<DatasetEntity>& src);
 
 
 };
 
 
 #endif
 
 

