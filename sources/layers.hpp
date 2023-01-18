
#include <string.h>
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "ap_int.h"
typedef float precision_i;
typedef float precision_o;
typedef float precision_w;
typedef float precision_b; 


typedef hls::stream<precision_i> AXIstreamIN;
typedef hls::stream<precision_o> AXIstreamOUT;

typedef ap_axiu<32, 0, 0, 0> AXIS_DATA;
typedef hls::stream<AXIS_DATA> AXIstreamFINAL;

void layer0(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer1(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer2(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer3(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer4(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer5(AXIstreamIN &input0,	AXIstreamOUT &output0);
void layer6(AXIstreamIN &input0,	AXIstreamFINAL &output0);
