//
//  ExpandedPair.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#include <zxing/oned/rss/expanded/ExpandedPair.h>

using zxing::Ref;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::FinderPattern;
using zxing::oned::rss::expanded::ExpandedPair;

ExpandedPair::ExpandedPair(Ref<DataCharacter> leftChar, Ref<DataCharacter> rightChar, Ref<FinderPattern> finderPattern, bool mayBeLast) {
    _leftChar = leftChar;
    _rightChar = rightChar;
    _finderPattern = finderPattern;
    _mayBeLast = mayBeLast;
}

bool ExpandedPair::mayBeLast(){
    return _mayBeLast;
}

Ref<DataCharacter> ExpandedPair::getLeftChar(){
    return _leftChar;
}

Ref<DataCharacter> ExpandedPair::getRightChar(){
    return _rightChar;
}

Ref<FinderPattern> ExpandedPair::getFinderPattern(){
    return _finderPattern;
}

bool ExpandedPair::mustBeLast(){
    return _rightChar.empty();
}

bool ExpandedPair::operator==(const ExpandedPair &other) const {
    if (_leftChar != other._leftChar) return false;
    if (_rightChar != other._rightChar) return false;
    if (_finderPattern != other._finderPattern) return false;
    return true;
}