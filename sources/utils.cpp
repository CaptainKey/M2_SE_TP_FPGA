#include "utils.hpp"

unsigned int IdxArrayToIdxLine(unsigned int i,unsigned int j,unsigned int k,unsigned int w,unsigned int * dimensions){
    unsigned int idx = i*dimensions[1]*dimensions[2]*dimensions[3]+j*dimensions[2]*dimensions[3]+k*dimensions[3]+w;
    return idx;
}

int read_input(float* tmp, int nb_inputs,char * name){
	FILE* file = fopen(name, "rb");
	if (!file){
		return EXIT_FAILURE;
	}
	size_t read = fread(tmp, sizeof(float), nb_inputs, file);
    if (read != nb_inputs){
        cout << "Did not read all data" << endl;
        return EXIT_FAILURE;
    }else{
        cout << "Nb input : " << read << endl;
    }

    return EXIT_SUCCESS;
}

int read_output(float* tmp, int nb_output,char * name){
	FILE* file = fopen(name, "rb");
	if (!file){
		return EXIT_FAILURE;
	}
	size_t read = fread(tmp, sizeof(float), nb_output, file);
    if (read != nb_output){
        cout << "Did not read all data" << endl;
        return EXIT_FAILURE;
    }else{
        cout << "Nb output : " << read << endl;
    }

    return EXIT_SUCCESS;
}

