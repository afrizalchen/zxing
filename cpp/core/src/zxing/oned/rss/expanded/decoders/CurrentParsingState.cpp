//
//  CurrentParsingState.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/CurrentParsingState.h>

using zxing::oned::rss::expanded::decoders::CurrentParsingState;

CurrentParsingState::CurrentParsingState() {
    
}

int CurrentParsingState::getPosition() {
    return _position;
}

void CurrentParsingState::setPosition(int position) {
    _position = position;
}

void CurrentParsingState::incrementPosition(int delta) {
    _position += delta;
}

bool CurrentParsingState::isAlpha() {
    return _encoding == ALPHA;
}

bool CurrentParsingState::isNumeric() {
    return _encoding == NUMERIC;
}

bool CurrentParsingState::isIsoIec646() {
    return _encoding == ISO_IEC_646;
}

void CurrentParsingState::setNumeric() {
    _encoding = NUMERIC;
}

void CurrentParsingState::setAlpha() {
    _encoding = ALPHA;
}

void CurrentParsingState::setIsoIec646() {
    _encoding = ISO_IEC_646;
}