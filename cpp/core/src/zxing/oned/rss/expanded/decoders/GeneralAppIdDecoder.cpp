¬//
//  GeneralAppIdDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 6/8/13.
//
//

#include <sstream>
#include <zxing/IllegalStateException.h>
#include <zxing/oned/rss/expanded/decoders/FieldParser.h>
#include <zxing/oned/rss/expanded/decoders/GeneralAppIdDecoder.h>

using zxing::Ref;
using zxing::String;
using zxing::IllegalStateException;
using zxing::oned::rss::expanded::decoders::BlockParsedResult;
using zxing::oned::rss::expanded::decoders::DecodedChar;
using zxing::oned::rss::expanded::decoders::DecodedInformation;
using zxing::oned::rss::expanded::decoders::DecodedNumeric;
using zxing::oned::rss::expanded::decoders::FieldParser;
using zxing::oned::rss::expanded::decoders::GeneralAppIdDecoder;

GeneralAppIdDecoder::GeneralAppIdDecoder(Ref<BitArray> information) {
    _information = information;
    _current = Ref<CurrentParsingState>(new CurrentParsingState());
    _buffer = Ref<String>(new String(""));
}

Ref<String> GeneralAppIdDecoder::decodeAllCodes(Ref<String> buff, int initialPosition) {
    int currentPosition = initialPosition;
    Ref<String> remaining = Ref<String>();
    do{
        Ref<DecodedInformation> info = decodeGeneralPurposeField(currentPosition, remaining);
        Ref<String> parsedFields = FieldParser::parseFieldsInGeneralPurpose(info->getNewString());
        if (!parsedFields.empty()) {
            buff->append(parsedFields->getText());
        }
        if(info->isRemaining()) {
            std::ostringstream oss;
            oss << info->getRemainingValue();
            remaining = Ref<String>(new String(oss.str()));
        } else {
            remaining = Ref<String>();
        }
        
        if(currentPosition == info->getNewPosition()) {// No step forward!
            break;
        }
        currentPosition = info->getNewPosition();
    }while(true);
    printf("GeneralAppIdDecoder::decodeAllCodes(%d) buff=%s", initialPosition, buff->getText().c_str());
    return Ref<String>(new String(buff->getText()));
}

int GeneralAppIdDecoder::extractNumericValueFromBitArray(int pos, int bits) {
    return extractNumericValueFromBitArray(_information, pos, bits);
}

int GeneralAppIdDecoder::extractNumericValueFromBitArray(Ref<BitArray> information, int pos, int bits) {
    if(bits > 32) {
        throw new IllegalArgumentException("extractNumberValueFromBitArray can't handle more than 32 bits");
    }
    
    int value = 0;
    for (int i = 0; i < bits; ++i) {
        if (information->get(pos + i)) {
            value |= 1 << (bits - i - 1);
        }
    }
    
    return value;
}

Ref<DecodedInformation> GeneralAppIdDecoder::decodeGeneralPurposeField(int pos, Ref<String> remaining) {
    _buffer.reset(new String(""));
    
    if(!remaining.empty()) {
        _buffer->append(remaining->getText());
    }
    
    _current->setPosition(pos);
    
    Ref<DecodedInformation> lastDecoded = parseBlocks();
    if(!lastDecoded.empty() && lastDecoded->isRemaining()) {
        return Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText())), lastDecoded->getRemainingValue()));
    }
    return Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText()))));
}

bool GeneralAppIdDecoder::isStillNumeric(int pos) {
    // It's numeric if it still has 7 positions
    // and one of the first 4 bits is "1".
    if(pos + 7 > _information->getSize()){
        return pos + 4 <= _information->getSize();
    }
    
    for (int i = pos; i < pos + 3; ++i) {
        if (_information->get(i)) {
            return true;
        }
    }
    
    return _information->get(pos + 3);
}

Ref<DecodedNumeric> GeneralAppIdDecoder::decodeNumeric(int pos) {
    if(pos + 7 > _information->getSize()){
        int numeric = extractNumericValueFromBitArray(pos, 4);
        if(numeric == 0) {
            return Ref<DecodedNumeric>(new DecodedNumeric(_information->getSize(), DecodedNumeric::FNC1, DecodedNumeric::FNC1));
        }
        return Ref<DecodedNumeric>(new DecodedNumeric(_information->getSize(), numeric - 1, DecodedNumeric::FNC1));
    }
    int numeric = extractNumericValueFromBitArray(pos, 7);
    
    int digit1  = (numeric - 8) / 11;
    int digit2  = (numeric - 8) % 11;
    
    return Ref<DecodedNumeric>(new DecodedNumeric(pos + 7, digit1, digit2));
}

Ref<DecodedInformation> GeneralAppIdDecoder::parseBlocks() {
    boolean isFinished;
    Ref<BlockParsedResult> result;
    do{
        int initialPosition = _current->getPosition();
        
        if (_current->isAlpha()){
            result = parseAlphaBlock();
            isFinished = result->isFinished();
        }else if (_current->isIsoIec646()){
            result = parseIsoIec646Block();
            isFinished = result->isFinished();
        }else{ // it must be numeric
            result = parseNumericBlock();
            isFinished = result->isFinished();
        }
        
        boolean positionChanged = initialPosition != _current->getPosition();
        if(!positionChanged && !isFinished) {
            break;
        }
    } while (!isFinished);
    
    return result->getDecodedInformation();
}

Ref<BlockParsedResult> GeneralAppIdDecoder::parseNumericBlock() {
    while (isStillNumeric(_current->getPosition())) {
        Ref<DecodedNumeric> numeric = decodeNumeric(_current->getPosition());
        _current->setPosition(numeric->getNewPosition());
        
        if(numeric->isFirstDigitFNC1()){
            Ref<DecodedInformation> information;
            if (numeric->isSecondDigitFNC1()) {
                information = Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText()))));
            } else {
                information = Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText())), numeric->getSecondDigit()));
            }
            return Ref<BlockParsedResult>(new BlockParsedResult(information, true));
        }
        std::ostringstream oss;
        oss << numeric->getFirstDigit();
        _buffer->append(oss.str());
        
        if(numeric->isSecondDigitFNC1()){
            Ref<DecodedInformation> information = Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText()))));
            return Ref<BlockParsedResult>(new BlockParsedResult(information, true));
        }
        oss.clear();
        oss.str("");
        oss << numeric->getSecondDigit();
        _buffer->append(oss.str());
    }
    
    if (isNumericToAlphaNumericLatch(_current->getPosition())) {
        _current->setAlpha();
        _current->incrementPosition(4);
    }
    return Ref<BlockParsedResult>(new BlockParsedResult(false));
}

Ref<BlockParsedResult> GeneralAppIdDecoder::parseIsoIec646Block() {
    while (isStillIsoIec646(_current->getPosition())) {
        Ref<DecodedChar> iso = decodeIsoIec646(_current->getPosition());
        _current->setPosition(iso->getNewPosition());
        
        if (iso->isFNC1()) {
            Ref<DecodedInformation> information = Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText()))));
            return Ref<BlockParsedResult>(new BlockParsedResult(information, true));
        }
        _buffer->append(iso->getValue());
    }
    
    if (isAlphaOr646ToNumericLatch(_current->getPosition())) {
        _current->incrementPosition(3);
        _current->setNumeric();
    } else if (isAlphaTo646ToAlphaLatch(_current->getPosition())) {
        if (_current->getPosition() + 5 < _information->getSize()) {
            _current->incrementPosition(5);
        } else {
            _current->setPosition(_information->getSize());
        }
        
        _current->setAlpha();
    }
    return Ref<BlockParsedResult>(new BlockParsedResult(false));
}

Ref<BlockParsedResult> GeneralAppIdDecoder::parseAlphaBlock() {
    while (isStillAlpha(_current->getPosition())) {
        Ref<DecodedChar> alpha = decodeAlphanumeric(_current->getPosition());
        _current->setPosition(alpha->getNewPosition());
        
        if(alpha->isFNC1()) {
            Ref<DecodedInformation> information = Ref<DecodedInformation>(new DecodedInformation(_current->getPosition(), Ref<String>(new String(_buffer->getText()))));
            return Ref<BlockParsedResult>(new BlockParsedResult(information, true)); //end of the char block
        }
        
        _buffer->append(alpha->getValue());
    }
    
    if (isAlphaOr646ToNumericLatch(_current->getPosition())) {
        _current->incrementPosition(3);
        _current->setNumeric();
    } else if (isAlphaTo646ToAlphaLatch(_current->getPosition())) {
        if (_current->getPosition() + 5 < _information->getSize()) {
            _current->incrementPosition(5);
        } else {
            _current->setPosition(_information->getSize());
        }
        
        _current->setIsoIec646();
    }
    return Ref<BlockParsedResult>(new BlockParsedResult(false));
}

bool GeneralAppIdDecoder::isStillIsoIec646(int pos) {
    if (pos + 5 > _information->getSize()) {
        return false;
    }
    
    int fiveBitValue = extractNumericValueFromBitArray(pos, 5);
    if (fiveBitValue >= 5 && fiveBitValue < 16) {
        return true;
    }
    
    if (pos + 7 > _information->getSize()) {
        return false;
    }
    
    int sevenBitValue = extractNumericValueFromBitArray(pos, 7);
    if(sevenBitValue >= 64 && sevenBitValue < 116) {
        return true;
    }
    
    if (pos + 8 > _information->getSize()) {
        return false;
    }
    
    int eightBitValue = extractNumericValueFromBitArray(pos, 8);
    return eightBitValue >= 232 && eightBitValue < 253;
}

Ref<DecodedChar> GeneralAppIdDecoder::decodeIsoIec646(int pos) {
    int fiveBitValue = extractNumericValueFromBitArray(pos, 5);
    if (fiveBitValue == 15) {
        return Ref<DecodedChar>(new DecodedChar(pos + 5, DecodedChar::FNC1));
    }
    
    if (fiveBitValue >= 5 && fiveBitValue < 15) {
        return Ref<DecodedChar>(new DecodedChar(pos + 5, (char) ('0' + fiveBitValue - 5)));
    }
    
    int sevenBitValue = extractNumericValueFromBitArray(pos, 7);
    
    if (sevenBitValue >= 64 && sevenBitValue < 90) {
        return Ref<DecodedChar>(new DecodedChar(pos + 7, (char) (sevenBitValue + 1)));
    }
    
    if (sevenBitValue >= 90 && sevenBitValue < 116) {
        return Ref<DecodedChar>(new DecodedChar(pos + 7, (char) (sevenBitValue + 7)));
    }
    
    int eightBitValue = extractNumericValueFromBitArray(pos, 8);
    char c;
    switch (eightBitValue) {
        case 232:
            c = '!';
            break;
        case 233:
            c = '"';
            break;
        case 234:
            c = '%';
            break;
        case 235:
            c = '&';
            break;
        case 236:
            c = '\'';
            break;
        case 237:
            c = '(';
            break;
        case 238:
            c = ')';
            break;
        case 239:
            c = '*';
            break;
        case 240:
            c = '+';
            break;
        case 241:
            c = ',';
            break;
        case 242:
            c = '-';
            break;
        case 243:
            c = '.';
            break;
        case 244:
            c = '/';
            break;
        case 245:
            c = ':';
            break;
        case 246:
            c = ';';
            break;
        case 247:
            c = '<';
            break;
        case 248:
            c = '=';
            break;
        case 249:
            c = '>';
            break;
        case 250:
            c = '?';
            break;
        case 251:
            c = '_';
            break;
        case 252:
            c = ' ';
            break;
        default:
            std::ostringstream oss;
            oss << "Decoding invalid ISO/IEC 646 value: " << eightBitValue;
            throw new IllegalArgumentException(oss.str().c_str());
    }
    return Ref<DecodedChar>(new DecodedChar(pos + 8, c));
}

bool GeneralAppIdDecoder::isStillAlpha(int pos) {
    if(pos + 5 > _information->getSize()) {
        return false;
    }
    
    // We now check if it's a valid 5-bit value (0..9 and FNC1)
    int fiveBitValue = extractNumericValueFromBitArray(pos, 5);
    if (fiveBitValue >= 5 && fiveBitValue < 16) {
        return true;
    }
    
    if (pos + 6 > _information->getSize()) {
        return false;
    }
    
    int sixBitValue =  extractNumericValueFromBitArray(pos, 6);
    return sixBitValue >= 16 && sixBitValue < 63; // 63 not included
}

Ref<DecodedChar> GeneralAppIdDecoder::decodeAlphanumeric(int pos) {
    int fiveBitValue = extractNumericValueFromBitArray(pos, 5);
    if (fiveBitValue == 15) {
        return Ref<DecodedChar>(new DecodedChar(pos + 5, DecodedChar::FNC1));
    }
    
    if (fiveBitValue >= 5 && fiveBitValue < 15) {
        return Ref<DecodedChar>(new DecodedChar(pos + 5, (char) ('0' + fiveBitValue - 5)));
    }
    
    int sixBitValue =  extractNumericValueFromBitArray(pos, 6);
    
    if (sixBitValue >= 32 && sixBitValue < 58) {
        return Ref<DecodedChar>(new DecodedChar(pos + 6, (char) (sixBitValue + 33)));
    }
    
    char c;
    switch (sixBitValue){
        case 58:
            c = '*';
            break;
        case 59:
            c = ',';
            break;
        case 60:
            c = '-';
            break;
        case 61:
            c = '.';
            break;
        case 62:
            c = '/';
            break;
        default:
            std::ostringstream oss;
            oss << "Decoding invalid alphanumeric value: " << sixBitValue;
            throw new IllegalStateException(oss.str().c_str());
    }
    return Ref<DecodedChar>(new DecodedChar(pos + 6, c));
}

bool GeneralAppIdDecoder::isAlphaTo646ToAlphaLatch(int pos) {
    if (pos + 1 > _information->getSize()) {
        return false;
    }
    
    for (int i = 0; i < 5 && i + pos < _information->getSize(); ++i) {
        if(i == 2){
            if (!_information->get(pos + 2)) {
                return false;
            }
        } else if (_information->get(pos + i)) {
            return false;
        }
    }
    
    return true;
}

bool GeneralAppIdDecoder::isAlphaOr646ToNumericLatch(int pos) {
    // Next is alphanumeric if there are 3 positions and they are all zeros
    if (pos + 3 > _information->getSize()) {
        return false;
    }
    
    for (int i = pos; i < pos + 3; ++i) {
        if (_information->get(i)) {
            return false;
        }
    }
    return true;
}

bool GeneralAppIdDecoder::isNumericToAlphaNumericLatch(int pos) {
    // Next is alphanumeric if there are 4 positions and they are all zeros, or
    // if there is a subset of this just before the end of the symbol
    if (pos + 1 > _information->getSize()) {
        return false;
    }
    
    for (int i = 0; i < 4 && i + pos < _information->getSize(); ++i) {
        if (_information->get(pos + i)) {
            return false;
        }
    }
    return true;
}

