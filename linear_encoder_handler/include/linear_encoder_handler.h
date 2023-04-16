//
// Created by main on 13.04.23.
//

#pragma once
#include "QEI.h"

class LinEnc {
public:
    LinEnc(PinName enc_a, PinName enc_b,
           PinName enc_z);
    double get_data();

private:
    QEI _encoder;
};


