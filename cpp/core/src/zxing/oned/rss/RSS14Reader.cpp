//
//  RSS14Reader.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#include <sstream>
#include <zxing/NotFoundException.h>
#include <zxing/BarcodeFormat.h>
#include <zxing/common/Counted.h>
#include <zxing/common/Str.h>
#include <zxing/oned/rss/RSS14Reader.h>
#include <zxing/oned/rss/RSSUtils.h>

using zxing::Ref;
using zxing::Result;
using zxing::ResultPoint;
using zxing::String;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::FinderPattern;
using zxing::oned::rss::Pair;
using zxing::oned::rss::RSS14Reader;
using zxing::oned::rss::RSSUtils;

namespace {
    const int OUTSIDE_EVEN_TOTAL_SUBSET[5] = {1,10,34,70,126};
    const int INSIDE_ODD_TOTAL_SUBSET[4] = {4,20,48,81};
    const int OUTSIDE_GSUM[5] = {0,161,961,2015,2715};
    const int INSIDE_GSUM[4] = {0,336,1036,1516};
    const int OUTSIDE_ODD_WIDEST[5] = {8,6,4,3,1};
    const int INSIDE_ODD_WIDEST[4] = {2,4,6,8};
    
    const int FINDER_PATTERNS_[9][4] = {
        {3,8,2,1},
        {3,5,5,1},
        {3,3,7,1},
        {3,1,9,1},
        {2,7,4,1},
        {2,5,6,1},
        {2,3,8,1},
        {1,5,7,1},
        {1,3,9,1},
    };
}
#define VECTOR_INIT(v) v, v + sizeof(v)/sizeof(v[0])
const std::vector<int const*>
RSS14Reader::FINDER_PATTERNS (VECTOR_INIT(FINDER_PATTERNS_));
RSS14Reader::RSS14Reader() {
}

RSS14Reader::~RSS14Reader() {}

Ref<Result> RSS14Reader::decodeRow(int rowNumber, Ref<BitArray> row) {
    DecodeHints *hints = new DecodeHints(DecodeHints::RSS_14_HINT);
    Ref<Pair> leftPair = decodePair(row, false, rowNumber, *hints);
    addOrTally(possibleLeftPairs, leftPair);
    row->reverse();
    Ref<Pair> rightPair = decodePair(row, true, rowNumber, *hints);
    addOrTally(possibleRightPairs, rightPair);
    row->reverse();
    int lefSize = possibleLeftPairs.size();
    for (int i = 0; i < lefSize; i++) {
        Ref<Pair> left = possibleLeftPairs[i];
        if (left->getCount() > 1) {
            int rightSize = possibleRightPairs.size();
            for (int j = 0; j < rightSize; j++) {
                Ref<Pair> right = possibleRightPairs[j];
                if (right->getCount() > 1) {
                    if (checkChecksum(left, right)) {
                        return constructResult(left, right);
                    }
                }
            }
        }
    }
    throw NotFoundException();
}

void RSS14Reader::reset() {
    possibleLeftPairs.clear();
    possibleRightPairs.clear();
}

void RSS14Reader::addOrTally(std::vector< Ref<Pair> > &possiblePairs, Ref<Pair> pair) {
    if (!pair) {
        return;
    }
    boolean found = false;
    for (std::vector< Ref<Pair> >::iterator it = possiblePairs.begin(); it != possiblePairs.end(); it++) {
        Ref<Pair> other = *it;
        if (other->getValue() == pair->getValue()) {
            other->incrementCount();
            found = true;
            break;
        }
    }
    if (!found) {
        possiblePairs.insert(possiblePairs.begin(), pair);
    }
}

Ref<Result> RSS14Reader::constructResult(Ref<Pair> leftPair, Ref<Pair> rightPair) {
    long long symbolValue = 4537077LL * leftPair->getValue() + rightPair->getValue();
    std::ostringstream oss;
    oss << symbolValue;
    std::string text = oss.str();
    
    Ref<String> buffer(new String(14));
    for (int i = 13 - text.length(); i > 0; i--) {
        buffer->append('0');
    }
    buffer->append(text);
    
    int checkDigit = 0;
    for (int i = 0; i < 13; i++) {
        int digit = buffer->charAt(i) - '0';
        checkDigit += (i & 0x01) == 0 ? 3 * digit : digit;
    }
    checkDigit = 10 - (checkDigit % 10);
    if (checkDigit == 10) {
        checkDigit = 0;
    }
    oss.clear();
    oss.str("");
    oss << checkDigit;
    buffer->append(oss.str());
    
    ArrayRef< Ref<ResultPoint> > leftPoints = leftPair->getFinderPattern()->getResultPoints();
    ArrayRef< Ref<ResultPoint> > rightPoints = rightPair->getFinderPattern()->getResultPoints();
    
    Array< Ref<ResultPoint> >* resultPoints = new Array< Ref<ResultPoint> >();
    std::vector< Ref<ResultPoint> >& resultPointValues (resultPoints->values());
    resultPointValues.push_back(Ref<ResultPoint>(leftPoints[0]));
    resultPointValues.push_back(Ref<ResultPoint>(leftPoints[1]));
    resultPointValues.push_back(Ref<ResultPoint>(rightPoints[0]));
    resultPointValues.push_back(Ref<ResultPoint>(rightPoints[1]));
    
    Result *result = new Result(buffer,
                                ArrayRef<char>(),
                                ArrayRef< Ref<ResultPoint> >(resultPoints),
                                BarcodeFormat::RSS_14);
    return Ref<Result>(result);
}

bool RSS14Reader::checkChecksum(Ref<Pair> leftPair, Ref<Pair> rightPair) {
    //int leftFPValue = leftPair.getFinderPattern().getValue();
    //int rightFPValue = rightPair.getFinderPattern().getValue();
    //if ((leftFPValue == 0 && rightFPValue == 8) ||
    //    (leftFPValue == 8 && rightFPValue == 0)) {
    //}
    int checkValue = (leftPair->getChecksumPortion() + 16 * rightPair->getChecksumPortion()) % 79;
    int targetCheckValue =
    9 * leftPair->getFinderPattern()->getValue() + rightPair->getFinderPattern()->getValue();
    if (targetCheckValue > 72) {
        targetCheckValue--;
    }
    if (targetCheckValue > 8) {
        targetCheckValue--;
    }
    return checkValue == targetCheckValue;
}

Ref<Pair> RSS14Reader::decodePair(Ref<BitArray> row, bool right, int rowNumber, DecodeHints hints) {
    try {
        std::vector<int> startEnd = findFinderPattern(row, 0, right);
        Ref<FinderPattern> pattern = parseFoundFinderPattern(row, rowNumber, right, startEnd);
        
        Ref<ResultPointCallback> resultPointCallback = hints.getResultPointCallback();
        
        if (!resultPointCallback.empty()) {
            float center = (startEnd[0] + startEnd[1]) / 2.0f;
            if (right) {
                // row is actually reversed
                center = row->getSize() - 1 - center;
            }
            ResultPoint *resultPoint = new ResultPoint(center, (float)rowNumber);
            resultPointCallback->foundPossibleResultPoint(*resultPoint);
        }
        
        Ref<DataCharacter> outside = decodeDataCharacter(row, pattern, true);
        Ref<DataCharacter> inside = decodeDataCharacter(row, pattern, false);
        return Ref<Pair>(new Pair(1597 * outside->getValue() + inside->getValue(),
                        outside->getChecksumPortion() + 4 * inside->getChecksumPortion(),
                        pattern));
    } catch (NotFoundException ignored) {
        return Ref<Pair>();
    }
}

Ref<DataCharacter> RSS14Reader::decodeDataCharacter(Ref<BitArray> row, Ref<FinderPattern> pattern, bool outsideChar) {
    std::vector<int> &counters = getDataCharacterCounters();
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    counters[4] = 0;
    counters[5] = 0;
    counters[6] = 0;
    counters[7] = 0;
    
    std::vector<int> &startEnd = pattern->getStartEnd();
    if (outsideChar) {
        recordPatternInReverse(row, startEnd[0], counters);
    } else {
        recordPattern(row, startEnd[1] + 1, counters);
        // reverse it
        for (int i = 0, j = counters.size() - 1; i < j; i++, j--) {
            int temp = counters[i];
            counters[i] = counters[j];
            counters[j] = temp;
        }
    }
    
    int numModules = outsideChar ? 16 : 15;
    float elementWidth = (float) count(counters) / (float) numModules;
    
    std::vector<int> &oddCounts = this->getOddCounts();
    std::vector<int> &evenCounts = this->getEvenCounts();
    std::vector<float> &oddRoundingErrors = this->getOddRoundingErrors();
    std::vector<float> &evenRoundingErrors = this->getEvenRoundingErrors();
    
    int counterCount = counters.size();
    for (int i = 0; i < counterCount; i++) {
        float value = (float) counters[i] / elementWidth;
        int count = (int) (value + 0.5f); // Round
        if (count < 1) {
            count = 1;
        } else if (count > 8) {
            count = 8;
        }
        int offset = i >> 1;
        if ((i & 0x01) == 0) {
            oddCounts[offset] = count;
            oddRoundingErrors[offset] = value - count;
        } else {
            evenCounts[offset] = count;
            evenRoundingErrors[offset] = value - count;
        }
    }
    
    adjustOddEvenCounts(outsideChar, numModules);
    
    int oddSum = 0;
    int oddChecksumPortion = 0;
    for (int i = oddCounts.size() - 1; i >= 0; i--) {
        oddChecksumPortion *= 9;
        oddChecksumPortion += oddCounts[i];
        oddSum += oddCounts[i];
    }
    int evenChecksumPortion = 0;
    int evenSum = 0;
    for (int i = evenCounts.size() - 1; i >= 0; i--) {
        evenChecksumPortion *= 9;
        evenChecksumPortion += evenCounts[i];
        evenSum += evenCounts[i];
    }
    int checksumPortion = oddChecksumPortion + 3*evenChecksumPortion;
    
    if (outsideChar) {
        if ((oddSum & 0x01) != 0 || oddSum > 12 || oddSum < 4) {
            throw NotFoundException();
        }
        int group = (12 - oddSum) / 2;
        int oddWidest = OUTSIDE_ODD_WIDEST[group];
        int evenWidest = 9 - oddWidest;
        int vOdd = RSSUtils::getRSSvalue(oddCounts, oddWidest, false);
        int vEven = RSSUtils::getRSSvalue(evenCounts, evenWidest, true);
        int tEven = OUTSIDE_EVEN_TOTAL_SUBSET[group];
        int gSum = OUTSIDE_GSUM[group];
        return Ref<DataCharacter> (new DataCharacter(vOdd * tEven + vEven + gSum, checksumPortion));
    } else {
        if ((evenSum & 0x01) != 0 || evenSum > 10 || evenSum < 4) {
            throw NotFoundException();
        }
        int group = (10 - evenSum) / 2;
        int oddWidest = INSIDE_ODD_WIDEST[group];
        int evenWidest = 9 - oddWidest;
        int vOdd = RSSUtils::getRSSvalue(oddCounts, oddWidest, true);
        int vEven = RSSUtils::getRSSvalue(evenCounts, evenWidest, false);
        int tOdd = INSIDE_ODD_TOTAL_SUBSET[group];
        int gSum = INSIDE_GSUM[group];
        return Ref<DataCharacter>(new DataCharacter(vEven * tOdd + vOdd + gSum, checksumPortion));
    }
}

std::vector<int> RSS14Reader::findFinderPattern(Ref<BitArray> row, int rowOffset, bool rightFinderPattern) {
    std::vector<int> &counters = getDecodeFinderCounters();
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    
    int width = row->getSize();
    boolean isWhite = false;
    while (rowOffset < width) {
        isWhite = !row->get(rowOffset);
        if (rightFinderPattern == isWhite) {
            // Will encounter white first when searching for right finder pattern
            break;
        }
        rowOffset++;
    }
    
    int counterPosition = 0;
    int patternStart = rowOffset;
    for (int x = rowOffset; x < width; x++) {
        if (row->get(x) ^ isWhite) {
            counters[counterPosition]++;
        } else {
            if (counterPosition == 3) {
                if (isFinderPattern(counters)) {
                    int pattern[] = {patternStart, x};
                    return std::vector<int>(pattern, pattern + sizeof(pattern) / sizeof(int));
                }
                patternStart += counters[0] + counters[1];
                counters[0] = counters[2];
                counters[1] = counters[3];
                counters[2] = 0;
                counters[3] = 0;
                counterPosition--;
            } else {
                counterPosition++;
            }
            counters[counterPosition] = 1;
            isWhite = !isWhite;
        }
    }
    throw NotFoundException();
}

Ref<FinderPattern> RSS14Reader::parseFoundFinderPattern(Ref<BitArray> row, int rowNumber, bool right, std::vector<int> &startEnd) {
    // Actually we found elements 2-5
    boolean firstIsBlack = row->get(startEnd[0]);
    int firstElementStart = startEnd[0] - 1;
    // Locate element 1
    while (firstElementStart >= 0 && firstIsBlack ^ row->get(firstElementStart)) {
        firstElementStart--;
    }
    firstElementStart++;
    int firstCounter = startEnd[0] - firstElementStart;
    // Make 'counters' hold 1-4
    std::vector<int> &counters = getDecodeFinderCounters();
    std::vector<int> countersCopy(counters.begin(),counters.end());
    int counterCount = counters.size();
    
    for (int y = 0; y < counterCount - 1; y++) {
        counters[y + 1] = countersCopy[y];
    }
    
    counters[0] = firstCounter;
    int value = parseFinderValue(counters, FINDER_PATTERNS);
    int start = firstElementStart;
    int end = startEnd[1];
    if (right) {
        // row is actually reversed
        start = row->getSize() - 1 - start;
        end = row->getSize() - 1 - end;
    }
    int rangeArr[] = {firstElementStart, startEnd[1]};
    std::vector<int> range (rangeArr, rangeArr + sizeof(rangeArr) / sizeof(rangeArr[0]));
    return Ref<FinderPattern>(new FinderPattern(value, range, start, end, rowNumber));
}

void RSS14Reader::adjustOddEvenCounts(bool outsideChar, int numModules) {
    int oddSum = AbstractRSSReader::count(getOddCounts());
    int evenSum = AbstractRSSReader::count(getEvenCounts());
    int mismatch = oddSum + evenSum - numModules;
    boolean oddParityBad = (oddSum & 0x01) == (outsideChar ? 1 : 0);
    boolean evenParityBad = (evenSum & 0x01) == 1;
    
    boolean incrementOdd = false;
    boolean decrementOdd = false;
    boolean incrementEven = false;
    boolean decrementEven = false;
    
    if (outsideChar) {
        if (oddSum > 12) {
            decrementOdd = true;
        } else if (oddSum < 4) {
            incrementOdd = true;
        }
        if (evenSum > 12) {
            decrementEven = true;
        } else if (evenSum < 4) {
            incrementEven = true;
        }
    } else {
        if (oddSum > 11) {
            decrementOdd = true;
        } else if (oddSum < 5) {
            incrementOdd = true;
        }
        if (evenSum > 10) {
            decrementEven = true;
        } else if (evenSum < 4) {
            incrementEven = true;
        }
    }
    
    /*if (mismatch == 2) {
     if (!(oddParityBad && evenParityBad)) {
     throw ReaderException.getInstance();
     }
     decrementOdd = true;
     decrementEven = true;
     } else if (mismatch == -2) {
     if (!(oddParityBad && evenParityBad)) {
     throw ReaderException.getInstance();
     }
     incrementOdd = true;
     incrementEven = true;
     } else */if (mismatch == 1) {
         if (oddParityBad) {
             if (evenParityBad) {
                 throw NotFoundException();
             }
             decrementOdd = true;
         } else {
             if (!evenParityBad) {
                 throw NotFoundException();
             }
             decrementEven = true;
         }
     } else if (mismatch == -1) {
         if (oddParityBad) {
             if (evenParityBad) {
                 throw NotFoundException();
             }
             incrementOdd = true;
         } else {
             if (!evenParityBad) {
                 throw NotFoundException();
             }
             incrementEven = true;
         }
     } else if (mismatch == 0) {
         if (oddParityBad) {
             if (!evenParityBad) {
                 throw NotFoundException();
             }
             // Both bad
             if (oddSum < evenSum) {
                 incrementOdd = true;
                 decrementEven = true;
             } else {
                 decrementOdd = true;
                 incrementEven = true;
             }
         } else {
             if (evenParityBad) {
                 throw NotFoundException();
             }
             // Nothing to do!
         }
     } else {
         throw NotFoundException();
     }
    
    if (incrementOdd) {
        if (decrementOdd) {
            throw NotFoundException();
        }
        std::vector<int> &oddCounts = getOddCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        increment(oddCounts, oddRoundingErrors);
    }
    if (decrementOdd) {
        std::vector<int> &oddCounts = getOddCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        decrement(oddCounts, oddRoundingErrors);
    }
    if (incrementEven) {
        if (decrementEven) {
            throw NotFoundException();
        }
        std::vector<int> &evenCounts = getEvenCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        increment(evenCounts, oddRoundingErrors);
    }
    if (decrementEven) {
        std::vector<int> &evenCounts = getEvenCounts();
        std::vector<float> &evenRoundingErrors = getEvenRoundingErrors();
        decrement(evenCounts, evenRoundingErrors);
    }
}
