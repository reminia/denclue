 
 #ifndef DENCLUE_H
 #define DENCLUE_H
 
 
 /** INCLUSIONS **/
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <cstdlib>
 #include "hypercube.h"
 #include "hyperspace.h"
 #include "dataset.h"
 #include "denclue_functions.h"
 using namespace std;
 
 
 #define MAX_FILENAME 64
 #define MAXSIZE_LINE 1024
 
 /** STRUCTS **/
 
 /** Arguments of denclue algorithm.
  * */
 typedef struct arguments_struct {
 
     unsigned int dimension;
 
     double sigma;  // Influence of an entity in its neighborhood
     double xi;     // Minimum density level for a density-attractor to be significant
 
     FILE *input_file;  // Stream to the output file
     FILE *output_file; // Stream to the input file
 
     char input_filename[MAX_FILENAME];  // Name of the input file
     char output_filename[MAX_FILENAME]; // Name of the output file
 
 } arguments_t;
 
 
 
 
 /** METHODS **/
 
 
 // Main function of DENCLUE
 int main( int argc, char **argv);
 
 
 /** Parse command line arguments and store them in a struct
  *
  *  @param argc argc from main function
  *  @param argv argv from main function
  *  @param arguments struct to store the arguments
  *
  * @return True if all args succesfully parsed. False, otherwise.
  *
  * */
 bool parse_args( int argc, char **argv, arguments_t& arguments_t );
 
 
 /** Print usage of the program.
  *
  * */
 void usage();
 
 
 /** Print clusters to a file.
  *
  *  @param clusters Map of density attractors to entities contained in the corresponding cluster.
  *  @param output_file File to write the clusters.
  *  @param xi Minimum density threshold
  *
  * */
 void printOutput( const map< string, vector<DatasetEntity> >& clusters, FILE *output_file , double xi);
 
 
 
 #endif
 

