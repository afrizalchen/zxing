//
//  Pair.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#include <zxing/oned/rss/Pair.h>

using zxing::Ref;
using zxing::oned::rss::Pair;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::FinderPattern;

Pair::Pair(int value, int checksumPortion, Ref<FinderPattern> finderPattern)
 : DataCharacter(value, checksumPortion) {
     _finderPattern = finderPattern;
}

Pair::~Pair() {
    
}

Ref<FinderPattern> Pair::getFinderPattern() const {
    return _finderPattern;
}

int Pair::getCount() const {
    return _count;
}

void Pair::incrementCount() {
    _count++;
}
