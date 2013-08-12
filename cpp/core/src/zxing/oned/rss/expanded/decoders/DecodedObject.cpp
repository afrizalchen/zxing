//
//  DecodedObject.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/DecodedObject.h>

using zxing::oned::rss::expanded::decoders::DecodedObject;

DecodedObject::DecodedObject(int newPosition) {
    _newPosition = newPosition;
}

int DecodedObject::getNewPosition() {
    return _newPosition;
}