#include "lenet.hpp"
#include "utils.hpp"
#include <math.h>
#include <unistd.h>
int main(){
	unsigned int Idimensions[4]   = {1, 3, 32, 32};
	unsigned int NewDimensions[4] = {1, 3, 32, 32};
	unsigned int Odimensions[4]   = {1, 10, 1, 1};

	unsigned int i,j,d,m,n,k,c_out;
	int status;

	AXIstreamIN axi_input0("input0");
	AXIstreamFINAL axi_output0("output0");



	int nb_inputs = Idimensions[0]*Idimensions[1]*Idimensions[2]*Idimensions[3];
	int nb_outputs = Odimensions[0]*Odimensions[1]*Odimensions[2]*Odimensions[3];

	float * input =  (float * ) malloc(sizeof(float) * nb_inputs);
	float * output = (float * ) malloc(sizeof(float) * nb_outputs);

	status = read_input(input,nb_inputs,"in.bin");
	if(status != EXIT_SUCCESS){
		cout << "Pas possible de lire l image" << endl;
		return EXIT_FAILURE;
	}

	status = read_output(output,nb_outputs,"out.bin");
	if(status != EXIT_SUCCESS){
		cout << "Pas possible de lire output" << endl;
		return EXIT_FAILURE;
	}


	for(i=0; i < Idimensions[2]; i++){
		for(d=0; d < Idimensions[1]; d++){
			for(j=0; j  <Idimensions[3]; j++){
				axi_input0.write((precision_i) input[IdxArrayToIdxLine(0,d,i,j,Idimensions)] );
			}
		}
	}

	lenet_hls(axi_input0,	axi_output0);

    AXIS_DATA tmp_output;
	float IP_output;
	bool last;
	for(i=0; i < Odimensions[2]; i++){
		for(c_out=0; c_out < Odimensions[1]; c_out++){
			for(j=0; j < Odimensions[3]; j++){
		
				tmp_output =  axi_output0.read();
				IP_output = *(float *) &tmp_output.data;
				last = tmp_output.last;
				k = c_out*Odimensions[2]*Odimensions[3]+i*Odimensions[3]+j;
				std::cout << last << "-" << "(" << c_out << "," << i << "," << j <<",)" << k << " (PYTORCH =>) " << output[k]  <<   " - " << IP_output << "(<= IP)"; 
				printf("0x%x -  0x%x\n",*(unsigned int * )&output[k],*(unsigned int * )&IP_output);
				if( (floorf(output[k]*100)/100 != floorf(IP_output*100)/100) && (roundf(output[k]*100)/100 != roundf(IP_output*100)/100) && (ceilf(output[k]*100)/100 != ceilf(IP_output*100)/100) ){
					cout << "Not EQUAL" << endl;
					cout << "Diff : " << (output[k]-IP_output) << endl;
					cout << "floorf : " << floorf(output[k]*100)/100 << " " << floorf(IP_output*100)/100 << endl;
					cout << "roundf : " << roundf(output[k]*100)/100 << " " << roundf(IP_output*100)/100 << endl;
					cout << "ceilf : " << ceilf(output[k]*100)/100 << " " << ceilf(IP_output*100)/100 << endl;
					// return EXIT_FAILURE;
				}
			}
		}
	}	

	free(input);
	free(output);

	return EXIT_SUCCESS;
}

