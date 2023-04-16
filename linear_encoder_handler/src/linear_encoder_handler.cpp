//
// Created by main on 13.04.23.
//

#include "linear_encoder_handler.h"

LinEnc::LinEnc(PinName enc_a, PinName enc_b, PinName enc_z) : _encoder(enc_a, enc_b, enc_z,
                                                                       30000, QEI::X4_ENCODING)
                                                                       {
                                                                            _encoder.reset();
                                                                       }

double LinEnc::get_data() {
    auto data = _encoder.getPulses() * 0.0001;
    return data;
}


