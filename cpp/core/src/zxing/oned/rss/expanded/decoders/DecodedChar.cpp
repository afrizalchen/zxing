//
//  DecodedChar.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/DecodedChar.h>

using zxing::oned::rss::expanded::decoders::DecodedChar;

const char DecodedChar::FNC1 = '$';

DecodedChar::DecodedChar(int newPosition, char value)
    : DecodedObject(newPosition) {
        _value = value;
}

char DecodedChar::getValue() {
    return _value;
}

bool DecodedChar::isFNC1() {
    return _value == FNC1;
}