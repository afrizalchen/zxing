//
//  DecodedNumeric.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/DecodedNumeric.h>

using zxing::oned::rss::expanded::decoders::DecodedNumeric;

const int DecodedNumeric::FNC1 = 10;

DecodedNumeric::DecodedNumeric(int newPosition, int firstDigit, int secondDigit)
    :DecodedObject(newPosition) {
        _firstDigit = firstDigit;
        _secondDigit = secondDigit;
}

int DecodedNumeric::getFirstDigit() {
    return  _firstDigit;
}

int DecodedNumeric::getSecondDigit() {
    return _secondDigit;
}

int DecodedNumeric::getValue() {
    return _firstDigit * 10 + _secondDigit;
}

bool DecodedNumeric::isFirstDigitFNC1() {
    return _firstDigit == FNC1;
}

bool DecodedNumeric::isSecondDigitFNC1() {
    return _secondDigit == FNC1;
}

bool DecodedNumeric::isAnyFNC1() {
    return _firstDigit == FNC1 || _secondDigit == FNC1;
}