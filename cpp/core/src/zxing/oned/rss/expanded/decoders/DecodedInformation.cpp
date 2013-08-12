//
//  DecodedInformation.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/DecodedInformation.h>

using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::DecodedInformation;

DecodedInformation::DecodedInformation(int newPosition, Ref<String> newString)
    : DecodedObject(newPosition) {
        _newString = newString;
        _remainingValue = 0;
        _remaining = false;
}

DecodedInformation::DecodedInformation(int newPosition, Ref<String> newString, int remainingValue)
    : DecodedObject(newPosition) {
        _newString = newString;
        _remainingValue = remainingValue;
        _remaining = true;
}

Ref<String> DecodedInformation::getNewString() {
    return _newString;
}

bool DecodedInformation::isRemaining() {
    return _remaining;
}

int DecodedInformation::getRemainingValue() {
    return _remainingValue;
}