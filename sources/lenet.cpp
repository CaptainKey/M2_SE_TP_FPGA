#include "lenet.hpp"

void lenet_hls(AXIstreamIN &input,AXIstreamFINAL & output){
    AXIstreamIN axi1;
    AXIstreamIN axi2;
    AXIstreamIN axi3;
    AXIstreamIN axi4;
    AXIstreamIN axi5;
    AXIstreamIN axi6;

    layer0(input,	axi1);
    layer1(axi1,	axi2);
    layer2(axi2,	axi3);
    layer3(axi3,	axi4);
    layer4(axi4,	axi5);
    layer5(axi5,	axi6);
    layer6(axi6,	output);

}