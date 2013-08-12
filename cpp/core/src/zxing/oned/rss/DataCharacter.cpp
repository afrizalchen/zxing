//
//  DataCharacter.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#include <zxing/oned/rss/DataCharacter.h>

using zxing::Ref;
using zxing::oned::rss::DataCharacter;

DataCharacter::DataCharacter(int value, int checksumPortion) {
    _value = value;
    _checksumPortion = checksumPortion;
}

DataCharacter::~DataCharacter() {

}


int DataCharacter::getValue() const {
    return _value;
}

int DataCharacter::getChecksumPortion() const {
    return _checksumPortion;
}

bool DataCharacter::operator==(const DataCharacter &other) const {
    return this->equals(other);
}

bool DataCharacter::equals(const DataCharacter &other) const {
    if (_value != other._value) return false;
    if (_checksumPortion != other._checksumPortion) return false;
    return  true;
}

bool DataCharacter::equals(Ref<DataCharacter> other) const {
    if (other.empty()) return false;
    return this->equals(*(other.object_));
}